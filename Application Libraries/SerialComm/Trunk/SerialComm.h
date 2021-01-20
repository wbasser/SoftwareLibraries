/*****************************************************************************
//
//   $Workfile: SerialComm.h $
// 
//    Function: Serial Communication Class Declarations
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
//   $History: SerialComm.h $
 * 
 * *****************  Version 1  *****************
 * User: Wbasser      Date: 6/12/07    Time: 8:50a
 * Created in $/SfwLibraries/C++
 * INITIAL CHECKIN
 * 
 ******************************************************************************/

#if !defined(AFX_SERIALCOMM_H__0832E179_09C3_4E68_BBA8_6F0F1D1EACF9__INCLUDED_)
#define AFX_SERIALCOMM_H__0832E179_09C3_4E68_BBA8_6F0F1D1EACF9__INCLUDED_

#pragma once

// local includes
#include "../MsgBuffer/MsgBuffer.h"

// define the number of unsolicitied message
#define	NUM_UNS_MSGS	            ( 8 )

// define the unsolicited message event
#define SERCOM_UNS_MSG_EVENT    ( 0x5C5C )

class CSerialComm  
{
public:
// construction/destruction
	CSerialComm();
	virtual ~CSerialComm();

// attributes
public:
	CMsgBuffer		  m_mbufLclMsg;
	CMsgBuffer		  m_mbufUnsMsg[ NUM_UNS_MSGS ];
	HANDLE			    m_hRcvMsgEvent;

//private:
	bool			      m_fMsgPending;
	bool			      m_fWaitingOnResponse;
	HANDLE			    m_hComm;
	HANDLE			    m_hThreadExitEvent;
	CWinThread*	  m_pReadThread;
	DCB				    m_dcbOld;
	COMMTIMEOUTS	  m_toOld;
	HANDLE			m_hCommMutex;
	HANDLE			m_hDataMutex;
	CMsgBuffer*		m_pmbufUserMsg;
	DWORD			m_dwUnsMsg;
	WORD			m_wUnsRdIdx;
	WORD			m_wUnsWrIdx;
	bool			m_fUnsEmpty;
	bool			m_fUnsFull;
	HWND			m_hWnd;

// implementation
public:
	virtual	bool		Open( int iPort, DWORD dwBaud, DWORD dwParity = NOPARITY, DWORD dwLength = 8, DWORD dwStop = ONESTOPBIT, HWND hWnd = NULL, DWORD dwUnsMsg = 0 );
	virtual	bool		Close( void );
	virtual	void		SendXmtBuffer( CMsgBuffer* pBuffer, DWORD dwTimeOut );
	virtual void		GetRcvBuffer( CMsgBuffer* pBuffer, DWORD dwTimeOut );
	virtual	bool		ProcessRxByte( BYTE nChar );
	virtual	void		FormatTxBuffer( CMsgBuffer* pBuffer );
	virtual	bool		CheckCmdPending( void );
	virtual	bool		GetUnsMsg( CMsgBuffer* pBuffer );
	virtual	bool		IsUnsMsgEmpty( );
//	friend	static		UINT ReadThread( LPVOID lpParam );

//private:
	void	ProcessBytes( BYTE* pBuffer, DWORD dwNumBytes );
};

#endif // !defined(AFX_SERIALCOMM_H__0832E179_09C3_4E68_BBA8_6F0F1D1EACF9__INCLUDED_)
