/*****************************************************************************
//
//   $Workfile: MsgBuffer.h $
// 
//    Function: Message Buffer Class Declarations
//
//      Author: Bill Basser
//
//   $JustDate:  6/12/07 $
//
//   $Revision: 1 $
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
//   $History: MsgBuffer.h $
 * 
 * *****************  Version 1  *****************
 * User: Wbasser      Date: 6/12/07    Time: 8:50a
 * Created in $/SfwLibraries/C++
 * INITIAL CHECKIN
 * 
 ******************************************************************************/

#pragma once

#include "../stdafx.h"

// define the buffer size
#define	MAX_BUF_SIZE	  256

// define the msg status
typedef enum tagMSGSTATUS
{
	MSG_DATA_VALID		    = 0,
	MSG_PROTOCOL_ERROR	  = -1,
	MSG_CKSUM_ERR		    = -2,
	MSG_TIMEOUT			    = -3,
	MSG_UNKNOWN			    = -4,
	MSG_NOT_SENT		      = -5
} MSGSTATUS;

class CMsgBuffer  
{
// construction/destruction
public:
	CMsgBuffer( bool fBigEndian = false );
	virtual ~CMsgBuffer();

// attributes
public:
	BYTE    	m_anRxRawBuf[ MAX_BUF_SIZE ];
	BYTE	    m_anTxRawBuf[ MAX_BUF_SIZE ];
	BYTE	    m_anRxMsgBuf[ MAX_BUF_SIZE ];
	BYTE	    m_anTxMsgBuf[ MAX_BUF_SIZE ];
	short	  m_iRxRawLen;
	short	  m_iTxRawLen;
	short	  m_iRxMsgLen;
	short	  m_iTxMsgLen;

protected:
	MSGSTATUS	m_eStatus;
	bool		  m_fBigEndian;	
	BYTE	    m_nDstAddr;
	BYTE	    m_nSrcAddr;
	BYTE	    m_nCommand;
  BYTE	    m_nOption1;
  BYTE	    m_nOption2;
	BYTE	    m_nSequence;
	union
	{
		WORD  	  wValue;
		BYTE	    anValue[ 2 ];
	} tWord;
	union
	{
		float	  fltValue;
		DWORD	  dwValue;
		WORD	    awValue[ 2 ];
		BYTE	    anValue[ 4 ];
	} tDblWord;
  union
  {
    DWORD64 hValue;
    DWORD   dwValue[ 2 ];
    WORD    awValue[ 4 ];
    BYTE    anValue[ 8 ];
  } tHugeWord;

// operations
public:
	void		    PutMsgByte( BYTE nValue );
	void		    PutMsgWord( WORD wValue );
	void		    PutMsgLong( DWORD dwValue );
  void      PutMsgHuge( DWORD64 hValue );
	void		    PutMsgFloat( float fValue );
	void		    PutMsgString( CString strValue );
	BYTE		    GetMsgByte( short iIndex );
	WORD		    GetMsgWord( short iIndex );
	DWORD		  GetMsgLong( short iIndex );
  DWORD64   GetMsgHuge( short iIndex );
	float		  GetMsgFloat( short iIndex );
	CString		GetMsgString( short iIndex );
	CString		GetMsgString( short iIndex, short iExpLen );
	void		    ResetBuffers( void );
	void		    ResetTxBuffer( void );
	void		    ResetRxBuffer( void );
	void		    AddToRawRxBuffer( BYTE nValue );
	void		    AddToRawTxBuffer( BYTE nValue );
	void		    AddToMsgRxBuffer( BYTE nValue );
	void		    AddToMsgTxBuffer( BYTE nValue );
	void		    SetMsgStatus( MSGSTATUS eStatus );
	MSGSTATUS	GetMsgStatus( void );
	void		    SetDstAddr( BYTE nAddr );
	void		    SetSrcAddr( BYTE nAddr );
  void      SetCommand( BYTE nValue );
	void		    SetOption1( BYTE nValue );
	void		    SetOption2( BYTE nValue );
	void		    SetSequence( BYTE nSequence );
	BYTE		    GetDstAddr( void );
	BYTE		    GetSrcAddr( void );
  BYTE      GetCommand( void );
	BYTE		    GetOption1( void );
	BYTE		    GetOption2( void );
	BYTE		    GetSequence( void );
	void		    CopyMessage( CMsgBuffer* pSrc );
  void      CopyRxMessage( CMsgBuffer* pSrc );


// overridables
	virtual	void	SetBigEndian( bool fEndian );
};
