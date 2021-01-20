/*****************************************************************************
//   $Workfile: $
//    Function: Message Buffer Implementation
//      Author: Bill Basser
//   $JustDate: $
//   $Revision: 1.2 $
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
#include "MsgBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgBuffer::CMsgBuffer( bool fEndian )
{
	// reset the buffers
	ResetBuffers( );
	m_fBigEndian = fEndian;
}

CMsgBuffer::~CMsgBuffer()
{

}

void CMsgBuffer::SetBigEndian( bool fEndian )
{
	m_fBigEndian = fEndian;
}

//////////////////////////////////////////////////////////////////////
// buffer manipulations
//////////////////////////////////////////////////////////////////////
void CMsgBuffer::PutMsgByte( BYTE nValue )
{
	// add the byte to the message tx buffer
	AddToMsgTxBuffer( nValue );
}

void CMsgBuffer::PutMsgWord( WORD wValue )
{
	tWord.wValue = wValue;

	PutMsgByte(( m_fBigEndian ) ? tWord.anValue[ 1 ] : tWord.anValue[ 0 ]);
	PutMsgByte(( m_fBigEndian ) ? tWord.anValue[ 0 ] : tWord.anValue[ 1 ]);
}

void CMsgBuffer::PutMsgLong( DWORD dwValue )
{
	tDblWord.dwValue = dwValue;

	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 3 ] : tDblWord.anValue[ 0 ]);
	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 2 ] : tDblWord.anValue[ 1 ]);
	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 1 ] : tDblWord.anValue[ 2 ]);
	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 0 ] : tDblWord.anValue[ 3 ]);
}

void CMsgBuffer::PutMsgFloat( float fltValue )
{
	tDblWord.fltValue = fltValue;

	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 3 ] : tDblWord.anValue[ 0 ]);
	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 2 ] : tDblWord.anValue[ 1 ]);
	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 1 ] : tDblWord.anValue[ 2 ]);
	PutMsgByte(( m_fBigEndian ) ? tDblWord.anValue[ 0 ] : tDblWord.anValue[ 3 ]);
}

void CMsgBuffer::PutMsgHuge( DWORD64 hValue )
{
	tHugeWord.hValue = hValue;

	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 7 ] : tHugeWord.anValue[ 0 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 6 ] : tHugeWord.anValue[ 1 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 5 ] : tHugeWord.anValue[ 2 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 4 ] : tHugeWord.anValue[ 3 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 3 ] : tHugeWord.anValue[ 4 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 2 ] : tHugeWord.anValue[ 5 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 1 ] : tHugeWord.anValue[ 6 ]);
	PutMsgByte(( m_fBigEndian ) ? tHugeWord.anValue[ 0 ] : tHugeWord.anValue[ 7 ]);
}

void CMsgBuffer::PutMsgString( CString strValue )
{
	// stuff each character/delimit with a zero
	for ( short i = 0; i < strValue.GetLength( ); i++ )
		PutMsgByte(( BYTE )strValue.GetAt( i ));
	PutMsgByte( '\0' );
}

BYTE CMsgBuffer::GetMsgByte( short iIndex )
{
	// return the byte at index
	if ( iIndex >= m_iRxMsgLen )
	{
		// return error
		return( 0xEE );
	}
	else
	{
		// return the value
		return( m_anRxMsgBuf[ iIndex ] );
	}
}

WORD CMsgBuffer::GetMsgWord( short iIndex )
{
	tWord.anValue[0] = GetMsgByte(( m_fBigEndian ) ? iIndex + 1 : iIndex + 0 );
	tWord.anValue[1] = GetMsgByte(( m_fBigEndian ) ? iIndex + 0 : iIndex + 1 );
	return( tWord.wValue );
}

DWORD CMsgBuffer::GetMsgLong( short iIndex )
{
	tDblWord.anValue[0] = GetMsgByte(( m_fBigEndian ) ? iIndex + 3 : iIndex + 0 );
	tDblWord.anValue[1] = GetMsgByte(( m_fBigEndian ) ? iIndex + 2 : iIndex + 1 );
	tDblWord.anValue[2] = GetMsgByte(( m_fBigEndian ) ? iIndex + 1 : iIndex + 2 );
	tDblWord.anValue[3] = GetMsgByte(( m_fBigEndian ) ? iIndex + 0 : iIndex + 3 );
	return( tDblWord.dwValue );
}

DWORD64 CMsgBuffer::GetMsgHuge( short iIndex )
{
	tHugeWord.anValue[0] = GetMsgByte(( m_fBigEndian ) ? iIndex + 7 : iIndex + 0 );
	tHugeWord.anValue[1] = GetMsgByte(( m_fBigEndian ) ? iIndex + 6 : iIndex + 1 );
	tHugeWord.anValue[2] = GetMsgByte(( m_fBigEndian ) ? iIndex + 5 : iIndex + 2 );
	tHugeWord.anValue[3] = GetMsgByte(( m_fBigEndian ) ? iIndex + 4 : iIndex + 3 );
	tHugeWord.anValue[4] = GetMsgByte(( m_fBigEndian ) ? iIndex + 3 : iIndex + 4 );
	tHugeWord.anValue[5] = GetMsgByte(( m_fBigEndian ) ? iIndex + 2 : iIndex + 5 );
	tHugeWord.anValue[6] = GetMsgByte(( m_fBigEndian ) ? iIndex + 1 : iIndex + 6 );
	tHugeWord.anValue[7] = GetMsgByte(( m_fBigEndian ) ? iIndex + 0 : iIndex + 7 );
	return( tHugeWord.hValue );
}

float CMsgBuffer::GetMsgFloat( short iIndex )
{
	tDblWord.anValue[0] = GetMsgByte(( m_fBigEndian ) ? iIndex + 3 : iIndex + 0 );
	tDblWord.anValue[1] = GetMsgByte(( m_fBigEndian ) ? iIndex + 2 : iIndex + 1 );
	tDblWord.anValue[2] = GetMsgByte(( m_fBigEndian ) ? iIndex + 1 : iIndex + 2 );
	tDblWord.anValue[3] = GetMsgByte(( m_fBigEndian ) ? iIndex + 0 : iIndex + 3 );
	return( tDblWord.fltValue );
}

CString CMsgBuffer::GetMsgString( short iIndex )
{
	// build and return a string
	CString strTemp;
	short	iLength = 0;
	while (( iIndex < m_iRxMsgLen ) && ( m_anRxMsgBuf[ iIndex ] != '\0' ))
		strTemp.Insert( iLength++, m_anRxMsgBuf[ iIndex++ ] );
	return( strTemp );
}

CString CMsgBuffer::GetMsgString( short iIndex, short iExpLen )
{
	// build and return a string
	CString strTemp;
	short	iLength = 0;
	while (( iIndex < m_iRxMsgLen ) && ( iLength < iExpLen ))
		strTemp.Insert( iLength++, m_anRxMsgBuf[ iIndex++ ] );
	return( strTemp );
}

//////////////////////////////////////////////////////////////////////
// add data to buffers
//////////////////////////////////////////////////////////////////////
void CMsgBuffer::AddToMsgRxBuffer( BYTE nValue )
{
	// if no overflow, add
	if ( m_iRxMsgLen < MAX_BUF_SIZE )
		m_anRxMsgBuf[ m_iRxMsgLen++ ] = nValue;
}

void CMsgBuffer::AddToMsgTxBuffer( BYTE nValue )
{
	// if no overflow, add
	if ( m_iTxMsgLen < MAX_BUF_SIZE )
		m_anTxMsgBuf[ m_iTxMsgLen++ ] = nValue;
}

void CMsgBuffer::AddToRawRxBuffer( BYTE nValue )
{
	// if no overflow, add
	if ( m_iRxRawLen < MAX_BUF_SIZE )
		m_anRxRawBuf[ m_iRxRawLen++ ] = nValue;
}

void CMsgBuffer::AddToRawTxBuffer( BYTE nValue )
{
	// if no overflow, add
	if ( m_iTxRawLen < MAX_BUF_SIZE )
		m_anTxRawBuf[ m_iTxRawLen++ ] = nValue;
}

//////////////////////////////////////////////////////////////////////
// reset buffers
//////////////////////////////////////////////////////////////////////
void CMsgBuffer::ResetBuffers( void )
{
	ResetRxBuffer( );
	ResetTxBuffer( );
}

void CMsgBuffer::ResetRxBuffer( void )
{
	// reset indices
	m_iRxRawLen = m_iRxMsgLen = 0;
}

void CMsgBuffer::ResetTxBuffer( void )
{
	// reset indices/state
	m_iTxRawLen = m_iTxMsgLen = 0;
	m_eStatus = MSG_NOT_SENT;
}

void CMsgBuffer::SetSrcAddr( BYTE nAddr )
{
	// set the destination
	m_nSrcAddr = nAddr;
}

void CMsgBuffer::SetDstAddr( BYTE nAddr )
{
	// set the destination
	m_nDstAddr = nAddr;
}

void CMsgBuffer::SetCommand(BYTE nValue)
{
  // set the command option 1
  m_nCommand = nValue;
}

void CMsgBuffer::SetOption1( BYTE nValue )
{
	// set the command option 1
	m_nOption1 = nValue;
}

void CMsgBuffer::SetOption2( BYTE nValue)
{
	// set the command option 2
	m_nOption2 = nValue;
}

void CMsgBuffer::SetSequence( BYTE nSequence )
{
	// set the sequence
	m_nSequence = nSequence;
}

BYTE CMsgBuffer::GetSrcAddr( void )
{
	// return the source address
	return( m_nSrcAddr );
}

BYTE CMsgBuffer::GetDstAddr( void )
{
	// return the destination address
	return( m_nDstAddr );
}

BYTE CMsgBuffer::GetCommand( void )
{
	// return the command option 1
	return( m_nCommand );
}

BYTE CMsgBuffer::GetOption1( void )
{
	// return the command option 2
	return( m_nOption1 );
}

BYTE CMsgBuffer::GetOption2(void)
{
  // return the command option 2
  return(m_nOption2);
}

BYTE CMsgBuffer::GetSequence( void )
{
	// return the sequence
	return( m_nSequence );
}

//////////////////////////////////////////////////////////////////////
// set/get message/device status
//////////////////////////////////////////////////////////////////////
void CMsgBuffer::SetMsgStatus( MSGSTATUS eStatus )
{
	// set the status
	m_eStatus = eStatus;
}

MSGSTATUS CMsgBuffer::GetMsgStatus( void )
{
	// return the status
	return ( m_eStatus );
}

void CMsgBuffer::CopyMessage( CMsgBuffer* pSrc )
{
	// copy the variables
	m_iRxRawLen	= pSrc->m_iRxRawLen;
	m_iTxRawLen = pSrc->m_iTxRawLen;
	m_iRxMsgLen = pSrc->m_iRxMsgLen;
	m_iTxMsgLen = pSrc->m_iTxMsgLen;
	m_eStatus	  = pSrc->m_eStatus;
	m_nDstAddr	  = pSrc->m_nDstAddr;
	m_nSrcAddr	  = pSrc->m_nSrcAddr;
  m_nCommand  = pSrc->m_nCommand;
	m_nOption1	  = pSrc->m_nOption1;
	m_nOption2	  = pSrc->m_nOption2;
	m_nSequence = pSrc->m_nSequence;

	// for each buffer
	int i;
	for ( i = 0; i < m_iRxRawLen; i++ )
		m_anRxRawBuf[ i ] = pSrc->m_anRxRawBuf[ i ];
	for ( i = 0; i < m_iRxMsgLen; i++ )
		m_anRxMsgBuf[ i ] = pSrc->m_anRxMsgBuf[ i ];
	for ( i = 0; i < m_iTxRawLen; i++ )
		m_anTxRawBuf[ i ] = pSrc->m_anTxRawBuf[ i ];
	for ( i = 0; i < m_iTxMsgLen; i++ )
		m_anTxMsgBuf[ i ] = pSrc->m_anTxMsgBuf[ i ];
}

void CMsgBuffer::CopyRxMessage( CMsgBuffer* pSrc )
{
	// copy the variables
	m_iRxRawLen	= pSrc->m_iRxRawLen;
	m_iRxMsgLen = pSrc->m_iRxMsgLen;
	m_eStatus	  = pSrc->m_eStatus;
  m_nCommand  = pSrc->m_nCommand;
  m_nOption1  = pSrc->m_nOption1;
  m_nOption2  = pSrc->m_nOption2;

	// for each buffer
	int i;
	for ( i = 0; i < m_iRxRawLen; i++ )
		m_anRxRawBuf[ i ] = pSrc->m_anRxRawBuf[ i ];
	for ( i = 0; i < m_iRxMsgLen; i++ )
		m_anRxMsgBuf[ i ] = pSrc->m_anRxMsgBuf[ i ];
}
