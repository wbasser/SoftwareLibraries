/*****************************************************************************
//   $Workfile: $
//    Function: Cyber Protocol Class Implementation
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
//  Copyright (C) 2004-2010 Cyber Integration, LLC. All Rights Reserved
//
//   $History: $
 * 
 ******************************************************************************/

#include "../stdafx.h"
#include "CybProtocol.h"
#include "../Crc16Tabl/Crc16Tabl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// define the error offset
#define	MSG_ERROR_OFFSET	1

// define the remote error codes
#define	ERR_MSG_CKSM	0xFF

// define the command errors
#define	ERR_CMD_ILCMD	0xFE
#define	ERR_CMD_ILLEN	0xFD

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCybProtocol::CCybProtocol( bool fMultiMode, bool fCrcMode, bool fSeqMode )
: m_pCrc( NULL )
{
	// reset the decode state
	m_eDecState = MSG_IDLE;
	m_fMultiMode = fMultiMode;
	m_fCrcMode = fCrcMode;
	m_fSeqMode = fSeqMode;

	// if crc mode create crc
	if ( fCrcMode )
		m_pCrc = new CCrc16Tabl;

	// clear the sequence number
	m_nSequence = 0;
}

CCybProtocol::~CCybProtocol()
{
	if ( m_pCrc != NULL )
		delete( m_pCrc );
}

void CCybProtocol::SetMultiMode( bool fMode )
{
	m_fMultiMode = fMode;
}

bool CCybProtocol::GetMultiMode( void )
{
	return( m_fMultiMode );
}

void CCybProtocol::SetCrcMode( bool fCrc )
{
	m_fCrcMode = fCrc;
	if ( m_pCrc == NULL )
		m_pCrc = new CCrc16Tabl;
}

bool CCybProtocol::GetCrcMode( void )
{
	return( m_fCrcMode );
}

void CCybProtocol::SetSequenceMode( bool fMode )
{
	m_fSeqMode = fMode;
}

bool CCybProtocol::GetSequenceMode( void )
{
	return( m_fSeqMode );
}

BYTE CCybProtocol::GetSequence( CMsgBuffer* pBuffer )
{
	return( pBuffer->m_anRxMsgBuf[ ( m_fMultiMode ) ? 4 : 0 ] );
}

int CCybProtocol::XmitBuffer( short iTimeout )
{
	CString strError;

	// send the buffer
	int iReturn = 0;
	SendXmtBuffer( &m_msgBuf, abs( iTimeout ));

	// check for a protocol error
	if ( m_msgBuf.GetMsgStatus( ) != MSG_DATA_VALID )
	{
		iReturn = m_msgBuf.GetMsgStatus( );
	}
	else if ( iTimeout > 0 )
	{
    // check for a nak
    if ( m_msgBuf.GetOption1( ) == CH_NAK )
    {
		  iReturn = m_msgBuf.GetOption2( );
    }
	}

	// return the status
	return( iReturn );
}

int CCybProtocol::RecvBuffer( short iTimeout )
{
	// send the buffer
	int iReturn = 0;
	GetRcvBuffer( &m_msgBuf, abs( iTimeout ));

	// check for a protocol error
	if ( m_msgBuf.GetMsgStatus( ) != MSG_DATA_VALID )
	{
		iReturn = m_msgBuf.GetMsgStatus( );
	}

	// return the status
	return( iReturn );
}


CString CCybProtocol::ProcessSysError( int iError )
{
	CString strError;

	// determine the error
	switch( iError )
	{
	case MSG_PROTOCOL_ERROR :
		strError = _T( "Receive protocol error!" );
		break;

	case MSG_CKSUM_ERR :
		strError = _T( "Receive checksum error!" );
		break;

	case MSG_TIMEOUT :
		strError = _T( "Receive timeout error!" );
		break;

	case MSG_UNKNOWN :
		strError = _T( "Receive unknown error!" );
		break;
		
	case MSG_NOT_SENT :
		strError = _T( "Message not sent!" );
		break;

	default :
		strError = _T( "Unknown protocol error!" );
		break;
	}

	// return
	return( strError );
}

CString CCybProtocol::ProcessDevError( BYTE nError )
{
	CString strError;

	// determine the error
	switch( nError )
	{
	case ERR_MSG_CKSM :
		strError = _T( "Checksum error!" );
		break;

	case ERR_CMD_ILCMD :
		strError = _T( "Illegal command!" );
		break;

	case ERR_CMD_ILLEN :
		strError = _T( "Illegal message length!" );
		break;

	default :
		strError = _T( "" );
		break;
	}

	// return
	return( strError );
}

//////////////////////////////////////////////////////////////////////
// process receive message bytes
//////////////////////////////////////////////////////////////////////
bool CCybProtocol::ProcessRxByte( BYTE nData )
{
	bool fMessageComplete = false;

	if ( m_eDecState < MSG_CRCMSB && m_fCrcMode )
	{
		m_wRxCrc = m_pCrc->CrcCalcByte( m_wRxCrc, nData );
	}
	else
	{
		// add the character to the checksum
		m_nRxChecksum += nData;
	}

  // save the current state
  m_ePrvState = m_eDecState;

	// process the stat
	switch( m_eDecState )
	{
	case MSG_IDLE :
		// process DLE's only
		if ( nData == CH_DLE )
			m_eDecState = MSG_HEADCMD;
		break;

	case MSG_HEADCMD :
		// only allow SOH
		if ( nData == CH_SOH )
		{
			// re-set the checksum and length/set message flag
			if ( m_fCrcMode )
			{
				m_wRxCrc = m_pCrc->GetInitialValue( );
				m_wRxCrc = m_pCrc->CrcCalcByte( m_wRxCrc, CH_DLE );
				m_wRxCrc = m_pCrc->CrcCalcByte( m_wRxCrc, CH_SOH );
			}
			else
			{
				m_nRxChecksum = CH_DLE + CH_SOH;
			}
			m_mbufLclMsg.m_iRxMsgLen = 0;
			m_eDecState = ( m_fMultiMode ) ? MSG_SRCADR : MSG_COMMAND;
		}
		else
		{
			// back to idle
			m_eDecState = MSG_IDLE;
			m_mbufLclMsg.SetMsgStatus( MSG_PROTOCOL_ERROR ); 
			fMessageComplete = true;
		}
		break;

	case MSG_SRCADR :
    if (nData == CH_DLE)
    {
      m_eDecState = MSG_CONTROL;
    }
    else
    {
      m_mbufLclMsg.SetSrcAddr(nData);
      m_eDecState = MSG_DSTADR;
    }
		break;

	case MSG_DSTADR :
    if (nData == CH_DLE)
    {
      m_eDecState = MSG_CONTROL;
    }
    else
    {
      m_mbufLclMsg.SetDstAddr( nData );
		  m_eDecState = MSG_COMMAND;
    }
    break;

	case MSG_COMMAND :
    if (nData == CH_DLE)
    {
      m_eDecState = MSG_CONTROL;
    }
    else
    {
      m_mbufLclMsg.SetCommand(nData);
      m_eDecState = MSG_OPTION1;
    }
		break;

	case MSG_OPTION1 :
    if (nData == CH_DLE)
    {
      m_eDecState = MSG_CONTROL;
    }
    else
    {
      // stuff the response
      m_mbufLclMsg.SetOption1(nData);
      m_eDecState = MSG_OPTION2;
    }
		break;

  case MSG_OPTION2:
    if (nData == CH_DLE)
    {
      m_eDecState = MSG_CONTROL;
    }
    else
    {
      // stuff the response
      m_mbufLclMsg.SetOption2(nData);
      m_eDecState = (m_fSeqMode) ? MSG_SEQUENCE : MSG_HEADDAT;
    }
    break;

  case MSG_SEQUENCE :
    if (nData == CH_DLE)
    {
      m_eDecState = MSG_CONTROL;
    }
    else
    {
      m_mbufLclMsg.SetSequence(nData);
      m_eDecState = MSG_HEADDAT;
    }
		break;

	case MSG_HEADDAT :
		// only allow dle
		if ( nData == CH_DLE )
			m_eDecState = MSG_CONTROL;
		else
		{
			m_eDecState = MSG_IDLE;
			m_mbufLclMsg.SetMsgStatus( MSG_PROTOCOL_ERROR ); 
			fMessageComplete = true;
		}
		break;

	case MSG_CONTROL :
		// process the character
		switch( nData )
		{
		case CH_EOT :
			// process the checksum
			m_eDecState = ( m_fCrcMode ) ? MSG_CRCMSB : MSG_CHECKSUM;
			break;

		case CH_STX :
			m_eDecState = MSG_DATA;
			break;

		case CH_SPC :
      // process the previous state
      switch ( m_ePrvState )
      {
      case MSG_SRCADR :
        m_mbufLclMsg.SetSrcAddr(nData);
        m_eDecState = MSG_DSTADR;
        break;

      case MSG_DSTADR :
        m_mbufLclMsg.SetDstAddr(nData);
        m_eDecState = MSG_COMMAND;
        break;

      case MSG_COMMAND :
        m_mbufLclMsg.SetCommand(nData);
        m_eDecState = MSG_OPTION1;
        break;

      case MSG_OPTION1 :
        m_mbufLclMsg.SetOption1(nData);
        m_eDecState = MSG_OPTION2;
        break;

      case MSG_OPTION2 :
        m_mbufLclMsg.SetOption2(nData);
        m_eDecState = (m_fSeqMode) ? MSG_SEQUENCE : MSG_HEADDAT;
        break;

      case MSG_SEQUENCE :
        m_mbufLclMsg.SetSequence(nData);
        m_eDecState = MSG_HEADDAT;
        break;

      default :
        // stuff the data byte
        m_mbufLclMsg.AddToMsgRxBuffer(CH_DLE);
        m_eDecState = MSG_DATA;
        break;
      }
			break;

		default :
			// error/clear in progress/back to idle
			m_eDecState = MSG_IDLE;
			m_mbufLclMsg.SetMsgStatus( MSG_PROTOCOL_ERROR ); 
			fMessageComplete = true;
			break;
		}
		break;

	case MSG_DATA :
		// if DLE goto control else stuff data
		if ( nData == CH_DLE )
			m_eDecState = MSG_CONTROL;
		else
			m_mbufLclMsg.AddToMsgRxBuffer( nData );
		break;

	case MSG_CHECKSUM :
		// check for a zero checksum/set message complete
		m_mbufLclMsg.SetMsgStatus( ( m_nRxChecksum == 0 ) ? MSG_DATA_VALID : MSG_CKSUM_ERR );
		fMessageComplete = true;

		// clear the message in progress flag/go back to idle
		m_eDecState = MSG_IDLE;
		break;

	case MSG_CRCMSB :
		m_wRxCrcRcv = ( nData << 8 );
		m_eDecState = MSG_CRCLSB;
		break;

	case MSG_CRCLSB :
		// set the lsb of the checksum/goto msb
		m_wRxCrcRcv |= nData;
		m_mbufLclMsg.SetMsgStatus( ( m_wRxCrcRcv == m_wRxCrc ) ? MSG_DATA_VALID : MSG_CKSUM_ERR );
		fMessageComplete = true;

		// clear the message in progress flag/go back to idle
		m_eDecState = MSG_IDLE;
		break;

	default :
		m_eDecState = MSG_IDLE;
		fMessageComplete = true;
		break;
	}

	// return the message complete status
	return fMessageComplete;
}

//////////////////////////////////////////////////////////////////////
// transmit buffer format routines
//////////////////////////////////////////////////////////////////////
void CCybProtocol::FormatTxBuffer( CMsgBuffer* pBuffer )
{
	int	iIdx = 0;

	// clear the checksum
	pBuffer->m_iTxRawLen = 0;
	if ( m_fCrcMode )
		m_wTxCrc = m_pCrc->GetInitialValue( );
	else
		m_nTxChecksum = 0;

	// send the header
	StuffXmtBuffer( pBuffer, CH_DLE, false );
	StuffXmtBuffer( pBuffer, CH_SOH, false );

	// check for multi mode
	if ( m_fMultiMode )
	{
		// send the source/destination address
		StuffXmtBuffer( pBuffer, pBuffer->GetDstAddr( ), false );
		StuffXmtBuffer( pBuffer, pBuffer->GetSrcAddr( ), false );
	}

	// send the command/option
	StuffXmtBuffer( pBuffer, pBuffer->GetCommand( ), true );
	StuffXmtBuffer( pBuffer, pBuffer->GetOption1( ), true );
  StuffXmtBuffer( pBuffer, pBuffer->GetOption2( ), true );

	// check for sequence
	if ( m_fSeqMode )
	{
		// send the sequence number
		StuffXmtBuffer( pBuffer, pBuffer->GetSequence( ), true );
	}

	// if data length is not zero
	if ( iIdx < pBuffer->m_iTxMsgLen )
	{
		// stuff the data header
		StuffXmtBuffer( pBuffer, CH_DLE, false );
		StuffXmtBuffer( pBuffer, CH_STX, false );
		
		// stuff the characters
		while ( iIdx < pBuffer->m_iTxMsgLen )
			StuffXmtBuffer( pBuffer, pBuffer->m_anTxMsgBuf[ iIdx++ ], true );
	}

	// stuff the trailer
	StuffXmtBuffer( pBuffer, CH_DLE, false );
	StuffXmtBuffer( pBuffer, CH_EOT, false );
	
	// stuff the checksum
	if ( m_fCrcMode )
	{
		StuffXmtBuffer( pBuffer, m_wTxCrc >> 8, false, false );
		StuffXmtBuffer( pBuffer, m_wTxCrc & 0xFF, false, false );
	}
	else
	{
		m_nTxChecksum = ~m_nTxChecksum;
		m_nTxChecksum++;
		StuffXmtBuffer( pBuffer, m_nTxChecksum, false );
	}
}

void CCybProtocol::StuffXmtBuffer( CMsgBuffer* pBuffer, BYTE nChar, bool fChkDel, bool fApplyCrc )
{
	// stuff the character/add to checksum
	if ( m_fCrcMode )
	{
		if ( fApplyCrc )
			m_wTxCrc = m_pCrc->CrcCalcByte( m_wTxCrc, nChar );
	}
	else
	{
		m_nTxChecksum += nChar;
	}
	pBuffer->AddToRawTxBuffer( nChar );

	// check for delete
	if (( nChar == CH_DLE ) && fChkDel )
		StuffXmtBuffer( pBuffer, CH_SPC, false, fApplyCrc );
}

bool CCybProtocol::SendMessage( BYTE nCommand, BYTE nOption1, BYTE nOption2, BYTE* pnData, int nLength, DWORD dwTimeout )
{
	bool  fReturn = false;

	// reset the tx buffer/fill destination,api.opcode
	 m_msgBuf.ResetTxBuffer( );
	 m_msgBuf.SetCommand( nCommand );
   m_msgBuf.SetOption1( nOption1 );
   m_msgBuf.SetOption2( nOption2 );

  // now copy the data
  while( nLength-- )
  {
    m_msgBuf.PutMsgByte( *( pnData++ ));
  }

	// send it
	int iResult;
	if (( iResult = XmitBuffer((short ) dwTimeout )) != 0 )
	{
		// error - process it
		fReturn  = true;
	}

  // ensure that we got an ACK
  if (( dwTimeout > 0 ) && ( m_msgBuf.GetOption1( ) != CH_ACK ))
  {
    // indicate an error
    fReturn = true;
  }

	// return the status
	return( fReturn );
}

bool CCybProtocol::SendMessage( BYTE nCommand, BYTE nOption1, BYTE nOption2, DWORD dwTimeout )
{
	bool  fReturn = false;

	// fill destination,api.opcode
  m_msgBuf.SetCommand(nCommand);
  m_msgBuf.SetOption1(nOption1);
  m_msgBuf.SetOption2(nOption2);

	// send it
	int iResult;
	if (( iResult = XmitBuffer((short ) dwTimeout )) != 0 )
	{
		// error - process it
		fReturn  = true;
	}

  // ensure that we got an ACK
  if (( dwTimeout > 0 ) && ( m_msgBuf.GetOption1( ) != CH_ACK ))
  {
    // indicate an error
    fReturn = true;
  }

	// return the status
	return( fReturn );
}

bool CCybProtocol::SendMessage(CMsgBuffer* pMsgBuffer, DWORD dwTimeout)
{
  bool  fReturn = false;

  // copy the message buffer to the local
  m_msgBuf.CopyMessage(pMsgBuffer);

  // send it
  int iResult;
  if ((iResult = XmitBuffer((short)dwTimeout)) != 0)
  {
    // error - process it
    fReturn = true;
  }

  // ensure that we got an ACK
  if ((dwTimeout > 0) && (m_msgBuf.GetOption1() != CH_ACK))
  {
    // indicate an error
    fReturn = true;
  }

  // now copy the local back to
  pMsgBuffer->CopyMessage(&m_msgBuf);

  // return the status
  return(fReturn);

}

