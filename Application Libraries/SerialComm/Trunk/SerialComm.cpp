/*****************************************************************************
//   $Workfile: $
//    Function: Serial Communication Implementation
//      Author: Bill Basser
//   $JustDate: $
//   $Revision: $
//
//	This document contains proprietary data and information of Cyber Integration
//  LLC.  It is the exclusive property of Cyber Integration, LLC and
//  will not be disclosed in any form to any party without prior written 
//  permission of Cyber Integration, LLC.	This document may not be reproduced 
//  or further used without the prior written permission of Cyber Integration
//  LLC.
//  
//  Copyright (C) 2004 Cyber Integration, LLC. All Rights Reserved
//
//   $History: $
 * 
 ******************************************************************************/

//#include "../stdafx.h"
#include "SerialComm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// local declarations
#define	NUM_READ_HANDLES	2

static		UINT ReadThread(LPVOID lpParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialComm::CSerialComm()
{
	// initialization
	m_pReadThread = NULL;
	m_fMsgPending = false;
	m_pmbufUserMsg = NULL;
}

CSerialComm::~CSerialComm()
{
	// close upon destruction
	Close( );
}

//////////////////////////////////////////////////////////////////////
// implementation
bool CSerialComm::Open( int iPort, DWORD dwBaud, DWORD dwParity, DWORD dwLength, DWORD dwStop, HWND hWnd, DWORD dwUnsMsg )
{
	// build the port string
	CString strPort;
	strPort.Format( _T( "\\\\.\\COM%d" ), iPort );

	// open the comm port
	m_hComm = CreateFile( strPort,
		                  GENERIC_READ | GENERIC_WRITE,
						  0,
						  NULL,
						  OPEN_EXISTING,
						  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
						  NULL );

	// check for valid open
	if ( m_hComm == INVALID_HANDLE_VALUE )
		return false;

	// now get the current comm state
	if ( !GetCommState( m_hComm, &m_dcbOld ))
		return false;

	// copy the unsolicited message number and window
	m_dwUnsMsg = dwUnsMsg;
	m_hWnd = hWnd;

	// copy the current parameters
	DCB dcbNew = m_dcbOld;

	// change the baud rate, size, parity and stop bits
	dcbNew.BaudRate = dwBaud;
	dcbNew.ByteSize = ( BYTE )dwLength;
	dcbNew.Parity   = ( BYTE )dwParity;
	dcbNew.StopBits = ( BYTE )dwStop;

	// disable any flow control
	dcbNew.fOutxCtsFlow = false;
	dcbNew.fOutxDsrFlow = false;
	dcbNew.fDtrControl  = DTR_CONTROL_DISABLE;
	dcbNew.fOutX        = false;
	dcbNew.fInX         = false;
	dcbNew.fRtsControl  = RTS_CONTROL_DISABLE;

	// set the new parameters
	if ( !SetCommState( m_hComm, &dcbNew ))
	{
		// report the error/restore DCB/close/return
		SetCommState( m_hComm, &m_dcbOld );
		CloseHandle( m_hComm );
		return false;
	}

	// set the timeouts
	GetCommTimeouts( m_hComm, &m_toOld );
	COMMTIMEOUTS to;
	to.ReadIntervalTimeout = 10;
	to.ReadTotalTimeoutMultiplier = 0;
	to.ReadTotalTimeoutConstant = 0;
	to.WriteTotalTimeoutMultiplier = 10000;
	to.WriteTotalTimeoutConstant = 10;

	// set the timeouts
	if ( !SetCommTimeouts( m_hComm, &to ))
	{
		// report the error/restore DCB/close/return
		SetCommState( m_hComm, &m_dcbOld );
		CloseHandle( m_hComm );
		return false;
	}

	// set the indices to 0
	m_wUnsRdIdx = m_wUnsWrIdx = 0;
	m_fUnsEmpty = true;
	m_fUnsFull = false;

	// purge the communications device
	PurgeComm( m_hComm, ( PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ));

	// create the read thread
	m_pReadThread = AfxBeginThread( ReadThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
	m_pReadThread->m_bAutoDelete = FALSE;
	m_pReadThread->ResumeThread( );

	// create the exit/message event
	m_hThreadExitEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hRcvMsgEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	// create the data/com mutexes
	m_hCommMutex = CreateMutex( NULL, false, NULL );
	m_hDataMutex = CreateMutex( NULL, false, NULL );

	// turn on DTR/RTS to power converter
	EscapeCommFunction( m_hComm, SETDTR );
	EscapeCommFunction( m_hComm, SETRTS );

	// if we got here - return OK
	return true;
}

bool CSerialComm::Close( )
{
	if ( m_pReadThread != NULL )
	{
		TRACE0( "Setting Read Thread Exit.\n" );
		// set the thread exit event
		SetEvent( m_hThreadExitEvent );

		// wait for thread to exit
		if ( WaitForSingleObject( m_pReadThread->m_hThread, 1000 ) == WAIT_TIMEOUT )
		{
			TRACE0 ( "Read thread did not shut down!\n" );
		}

		// reset thread exit event
		ResetEvent( m_hThreadExitEvent );

		// close the mutex
		CloseHandle( m_hCommMutex );
		CloseHandle( m_hDataMutex );

		// close the exit and read event handles
		CloseHandle( m_hThreadExitEvent );

		// restore the old parameters
		SetCommTimeouts( m_hComm, &m_toOld );
		SetCommState( m_hComm, &m_dcbOld );

		// turn off DTR/RTS to power converter
//		EscapeCommFunction( m_hComm, CLRDTR );
//		EscapeCommFunction( m_hComm, CLRRTS );

		// close the handle
		PurgeComm( m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR );
		CloseHandle( m_hComm );

		// clean up read thread
		delete m_pReadThread;
		m_pReadThread = NULL;
	}

	TRACE0( "Communications closed.\n" );
	// return false
	return false;
}

//////////////////////////////////////////////////////////////////////
//  message transmit routines
//////////////////////////////////////////////////////////////////////
void CSerialComm::SendXmtBuffer( CMsgBuffer* pBuffer, DWORD dwTimeOut )
{
	bool fReturn = false;
	OVERLAPPED osWrite =  { 0 };

	// format the message buffer
	m_fMsgPending = true;
	FormatTxBuffer( pBuffer );

  #ifdef SERIAL_COMM_DEBUG_ENABLE
  for ( int i = 0; i < pBuffer->m_iTxRawLen; i++ )
  {
    TRACE1( "{%02X}", pBuffer->m_anTxRawBuf[ i ] );
  }
  TRACE( _T("\n" ));
  #endif

	// obtain COMM mutex
	WaitForSingleObject( m_hCommMutex, INFINITE );

	// create the overlapped event for read
	osWrite.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	// get the data mutex
	WaitForSingleObject( m_hDataMutex, INFINITE );
	m_pmbufUserMsg = ( dwTimeOut == 0 ) ? NULL : pBuffer;
	ReleaseMutex( m_hDataMutex );

	// clear the receive count/write it
	DWORD dwResult;
	WriteFile( m_hComm, pBuffer->m_anTxRawBuf, pBuffer->m_iTxRawLen, &dwResult, &osWrite );

	// wait for an event/close evebnt
	dwResult = WaitForSingleObject( osWrite.hEvent, dwTimeOut );
	CloseHandle( osWrite.hEvent );

	// if timeout is not zero
	if ( dwTimeOut != 0 )
	{
		// reset the receive event/wait for it/get data buffer
		ResetEvent( m_hRcvMsgEvent );
		dwResult = WaitForSingleObject( m_hRcvMsgEvent, dwTimeOut );
		WaitForSingleObject( m_hDataMutex, INFINITE );

		// process the result
		switch ( dwResult )
		{
		case WAIT_OBJECT_0 :
			break;

		case WAIT_TIMEOUT :
			m_pmbufUserMsg->SetMsgStatus( MSG_TIMEOUT );
			break;

		default :
			m_pmbufUserMsg->SetMsgStatus( MSG_UNKNOWN );
			break;
		}

		// clear the buffer pointer/relase data mutex
		m_pmbufUserMsg = NULL;
		ReleaseMutex( m_hDataMutex );
	}

	// release the COMM mutex/clear the message pending flag
	ReleaseMutex( m_hCommMutex );
	m_fMsgPending = false;
}

//////////////////////////////////////////////////////////////////////
//  message receive routines
//////////////////////////////////////////////////////////////////////
void CSerialComm::GetRcvBuffer( CMsgBuffer* pBuffer, DWORD dwTimeOut )
{
	DWORD dwResult;

	// obtain COMM mutex
	WaitForSingleObject( m_hCommMutex, INFINITE );

	// get the data mutex
	WaitForSingleObject( m_hDataMutex, INFINITE );
	m_pmbufUserMsg = ( dwTimeOut == 0 ) ? NULL : pBuffer;
	ReleaseMutex( m_hDataMutex );

	// reset the receive event/wait for it/get data buffer
	ResetEvent( m_hRcvMsgEvent );
	dwResult = WaitForSingleObject( m_hRcvMsgEvent, dwTimeOut );
	WaitForSingleObject( m_hDataMutex, INFINITE );

	// process the result
	switch ( dwResult )
	{
	case WAIT_OBJECT_0 :
		break;

	case WAIT_TIMEOUT :
		m_pmbufUserMsg->SetMsgStatus( MSG_TIMEOUT );
		break;

	default :
		m_pmbufUserMsg->SetMsgStatus( MSG_UNKNOWN );
		break;
	}

	// clear the buffer pointer/relase data mutex
	m_pmbufUserMsg = NULL;
	ReleaseMutex( m_hDataMutex );

	// release the COMM mutex/clear the message pending flag
	ReleaseMutex( m_hCommMutex );
	m_fMsgPending = false;
}

void CSerialComm::FormatTxBuffer( CMsgBuffer* pBuffer )
{
	for ( int i = 0; i < pBuffer->m_iTxMsgLen; i++ )
		pBuffer->AddToRawTxBuffer( pBuffer->m_anTxMsgBuf[ i ] );
}

bool CSerialComm::CheckCmdPending( )
{
	// just return the state
	return( m_fMsgPending );
}

bool CSerialComm::IsUnsMsgEmpty( )
{
	// return the state
	return( m_fUnsEmpty );
}

bool CSerialComm::GetUnsMsg( CMsgBuffer* pBuffer )
{
	bool	fReturn;
	
	// check for empty
	if ( m_fUnsEmpty )
		fReturn = false;
	else
	{
		// not empty
		fReturn = true;

		// get the message
		WaitForSingleObject( m_hDataMutex, INFINITE );
		pBuffer->ResetRxBuffer( );
		pBuffer->CopyMessage( &m_mbufUnsMsg[ m_wUnsRdIdx ] );
		
		// adjust the pointer/set flags
		m_wUnsRdIdx++;
		m_wUnsRdIdx %= NUM_UNS_MSGS;
		m_fUnsFull = false;
		if ( m_wUnsRdIdx == m_wUnsWrIdx )
			m_fUnsEmpty = true;

		// release the mutex		
		ReleaseMutex( m_hDataMutex );
	}

	// return
	return ( fReturn );
}

//////////////////////////////////////////////////////////////////////
// process the received bytes
//////////////////////////////////////////////////////////////////////
void CSerialComm::ProcessBytes( BYTE* pBuffer, DWORD dwNumBytes )
{
	BYTE	nChar;

	// for each byte in the buffer
	for ( DWORD dwX = 0; dwX < dwNumBytes; dwX++ )
	{
		// copy data to the raw buffer
		nChar = *( pBuffer + dwX );
		m_mbufLclMsg.AddToRawRxBuffer( nChar );
    #ifdef SERIAL_COMM_DEBUG_ENABLE
    TRACE1("[%02X]\n", nChar);
    #endif

		// process the character
		if ( ProcessRxByte( nChar ))
		{
      TRACE( _T("\n" ));

			// coppy command data to buffer
			WaitForSingleObject( m_hDataMutex, 200 );
			if ( m_pmbufUserMsg != NULL )
			{
				// reset the user buffers rx data/copy msg/raw to user
				m_pmbufUserMsg->ResetRxBuffer( );
				m_pmbufUserMsg->CopyRxMessage( &m_mbufLclMsg );

				// set the message finished event
				SetEvent( m_hRcvMsgEvent );
			}
			else
			{
				// if unsolicited message is not 0
				if (( m_dwUnsMsg != 0 ) && ( m_hWnd != NULL ))
				{
					// if not full
					if ( !m_fUnsFull )
					{
						// reset the user buffers rx data/copy msg/raw to user
						m_mbufUnsMsg[ m_wUnsWrIdx ].ResetRxBuffer( );
						m_mbufUnsMsg[ m_wUnsWrIdx ].CopyMessage( &m_mbufLclMsg );

						// adjust the write pointer
						m_wUnsWrIdx++;
						m_wUnsWrIdx %= NUM_UNS_MSGS;

						// clear the empty flag
						m_fUnsEmpty = false;
						if ( m_wUnsWrIdx == m_wUnsRdIdx )
							m_fUnsFull = true;

						// send a message
						PostMessage( m_hWnd, m_dwUnsMsg, SERCOM_UNS_MSG_EVENT, 0 );
					}
				}
			}

			// reset the local rx data
			m_mbufLclMsg.ResetRxBuffer( );
			ReleaseMutex( m_hDataMutex );
		}
	}
}

bool CSerialComm::ProcessRxByte( BYTE nChar )
{
	// add to buffer and return true
	m_mbufLclMsg.AddToMsgRxBuffer( nChar );
	m_mbufLclMsg.SetMsgStatus( MSG_DATA_VALID );
	return true ;
}

//////////////////////////////////////////////////////////////////////
// local functions and threads
//////////////////////////////////////////////////////////////////////
static UINT ReadThread( LPVOID lpParam )
{
	OVERLAPPED	osRead =  { 0 };
	BYTE		nLclBuffer[ MAX_BUF_SIZE ];
	DWORD		dwBytesRead, dwResult;
	HANDLE		hArray[ NUM_READ_HANDLES ];
	bool		fWaitingOnRead = FALSE;
	bool		fThreadDone = FALSE;

	// get a pointer to the parameters
	CSerialComm* pComm = ( CSerialComm* )lpParam;

	// create the overlapped event for read
	osRead.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	// fill the event array
	hArray[ 0 ] = osRead.hEvent;
	hArray[ 1 ] = pComm->m_hThreadExitEvent;

	// stay in the read thread
	while ( !fThreadDone )
	{
		// check for no outstanding read
		if ( !fWaitingOnRead )
		{
			// issue one
			if ( !ReadFile( pComm->m_hComm, nLclBuffer, MAX_BUF_SIZE, &dwBytesRead, &osRead ))
			{
				// process error
				if ( GetLastError( ) != ERROR_IO_PENDING )
				{
					TRACE0( "Error in process read thread!\n" );
				}

				// set the waiting on read
				fWaitingOnRead = true;
			}
			else
			{
				// characters were already there - process them
				pComm->ProcessBytes( nLclBuffer, dwBytesRead );
			}
		}

		// check for a pending read
		if ( fWaitingOnRead )
		{
			// wait for an event
			dwResult = WaitForMultipleObjects( NUM_READ_HANDLES, hArray, FALSE, 100 );

			// process the result
			switch ( dwResult )
			{
			// thread exit event
			case WAIT_OBJECT_0 :
				// get last overlapped result
				if ( !GetOverlappedResult( pComm->m_hComm, &osRead, &dwBytesRead, FALSE ))
				{
					// clear the error
					dwResult = GetLastError( );
					TRACE1( "Error on get overlapped - %l\n", dwResult );
				}
				else
				{
					// process them
					pComm->ProcessBytes( nLclBuffer, dwBytesRead );
				}

				// clear the waiting for read flag
				fWaitingOnRead = false;
				break;

			// read completed
			case WAIT_OBJECT_0 + 1:
				TRACE0( "Setting Read Thread Inactive.\n" );
				// clear the thread active flag
				fThreadDone = true;
				break;

				// timeout event
			case WAIT_TIMEOUT :
				// clear the waiting for read flag
				fWaitingOnRead = false;
				break;

			// unknown
			default :
//				TRACE1( "Unknown event - %u:%08lX!\n", dwResult );
				break;
			}
		}
	}

	// close event handle
	CloseHandle( osRead.hEvent );

	// return 1
	return 1;
}


