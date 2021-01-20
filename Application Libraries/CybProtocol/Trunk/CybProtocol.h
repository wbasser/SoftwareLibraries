/*****************************************************************************
//   $Workfile: CybProtocol.h $
//    Function: Cyber Protocol Class Declarations
//      Author: Bill Basser
//   $JustDate:  6/12/07 $
//   $Revision: 1 $
//
//	This document contains proprietary data and information of Cyber Integration
//  LLC.  It is the exclusive property of Cyber Integration, LLC and
//  will not be disclosed in any form to any party without prior written 
//  permission of Cyber Integration, LLC.	This document may not be reproduced 
//  or further used without the prior written permission of Cyber Integration
//  LLC.
//  Copyright (C) 2004-2010 Cyber Integration, LLC. All Rights Reserved
//
//   $History: CybProtocol.h $
 * 
 ******************************************************************************/


#if !defined(AFX_CYBPROTOCOL_H__E1D7E41C_3C36_407C_99E9_552256AA9C4E__INCLUDED_)
#define AFX_CYBPROTOCOL_H__E1D7E41C_3C36_407C_99E9_552256AA9C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../SerialComm/SerialComm.h"
#include "../Crc16Tabl/Crc16Tabl.h"

// define the timeout macro
#define	TMO( a, b )	(( a == 0 ) ? 0 : b)

// special characters
#define	CH_SOH  	        ( 0x01 )
#define	CH_STX	          ( 0x02 )
#define	CH_EOT	          ( 0x04 )
#define CH_ACK          ( 0x06 )
#define	CH_DLE	          ( 0x10 )
#define CH_NAK          ( 0x15 )
#define	CH_SPC	          ( 0xEF )

class CCybProtocol : public CSerialComm
{
public:
// construction/destruction
	CCybProtocol( bool fMultiMode = false, bool fCrcMode = false, bool fSeqMode = false );
	virtual ~CCybProtocol( void );

// attributes
public:
  enum  CYBCMDS
  {
    CYBCMDS_PING_DEV = 0x80,    ///< ping and resond
    CYBCMDS_GET_REV,            ///< get the software revision
    CYBCMDS_RST_DFL,            ///< reset defaults
    CYBCMDS_LOG_NUM,            ///< get the number of log entries
    CYBCMDS_LOG_OLD,            ///< get the oldest log entry
    CYBCMDS_LOG_NXT,            ///< get the next log entry
    CYBCMDS_LOG_PRV,            ///< get the previous log entry
    CYBCMDS_LOG_NEW,            ///< get the newest log entry
    CYBCMDS_LOG_RST,            ///< reset the log entries
    CYBCMDS_DATA_ECHO,          ///< data echo
    CYBCMDS_GET_SER,            ///< get the serial number
    CYBCMDS_JMP_BOOT = 0x90,    ///< jump to boot
    CYBCMDS_REQ_PARAMS,         ///< request the parameters
    CYBCMDS_REQ_ERASE,          ///< erase application area
    CYBCMDS_WRT_PAGE,           ///< write page
    CYBCMDS_RD_PAGE,            ///< read page
    CYBCMDS_REQ_SIGS,           ///< request the signatures
    CYBCMDS_JMP_APP,            ///< jump to application
    CYBCMDS_CRC_BLOCK,          ///< CRC block test
  };
  CMsgBuffer	  m_msgBuf;

protected:
	enum DECSTATES
	{
		MSG_IDLE = 0,
		MSG_HEADCMD,
		MSG_HEADDAT,
		MSG_DSTADR,
		MSG_SRCADR,
		MSG_SEQUENCE,
		MSG_COMMAND,
		MSG_OPTION1,
    MSG_OPTION2,
    MSG_CONTROL,
		MSG_DATA,
		MSG_CHECKSUM,
		MSG_CRCMSB,
		MSG_CRCLSB
	} m_eDecState, m_ePrvState;
  bool	        m_fMultiMode;
  bool	        m_fCrcMode;
  bool	        m_fSeqMode;
  BYTE		      m_nRxChecksum;
	BYTE	    	    m_nTxChecksum;
	WORD		      m_wRxCrc;
	WORD		      m_wTxCrc;
	WORD		      m_wRxCrcRcv;
	CCrc16Tabl*	m_pCrc;
	BYTE		      m_nSequence;

// operations
public:
	void  	  SetMultiMode( bool fMode );
	bool	    GetMultiMode( void );
	void	    SetCrcMode( bool fCrc );
	bool	    GetCrcMode( void );
	void	    SetSequenceMode( bool fMode );
	bool	    GetSequenceMode( void );
	int		  XmitBuffer( short iTimeout );
	int		  RecvBuffer( short iTimeout );
	CString ProcessSysError( int iError );
	CString ProcessDevError( BYTE nError );
	BYTE	    GetSequence( CMsgBuffer* pBuffer );

// implementation
public:
	bool	  ProcessRxByte( BYTE nChar );
	void	  FormatTxBuffer( CMsgBuffer* pBuffer );
  
// virtual implementations
  virtual bool  SendMessage(BYTE nCommand, BYTE nOption1, BYTE nOption2, BYTE* pnData, int nLength, DWORD dwTimeout);
  virtual bool  SendMessage(BYTE nCommand, BYTE nOption1, BYTE nOption2, DWORD dwTimeout);
  virtual bool  SendMessage(CMsgBuffer* pMsgBuffer, DWORD dwTimeout);

protected:
	void	  StuffXmtBuffer( CMsgBuffer* pBuffer, BYTE nChar, bool fChkDel, bool fApplyCrc = true );
};

#endif // !defined(AFX_CYBPROTOCOL_H__E1D7E41C_3C36_407C_99E9_552256AA9C4E__INCLUDED_)
