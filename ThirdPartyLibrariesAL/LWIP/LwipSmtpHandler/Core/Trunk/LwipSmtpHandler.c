/******************************************************************************
 * @file LwipSmtpHandler.c
 *
 * @brief LWIP Simple Mail Transport Protocol (SMTP) implemen tation 
 *
 * This file provides the implementation for the SMTP protocol
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LwipSmtpHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "utils/lwiplib.h"
#include "lwip/dns.h"
#include "QueueManager/QueueManager.h"
#include "StateExecutionEngine/StateExecutionEngine.h"

// local includes -------------------------------------------------------------
#include "LwipSmptHandler/LwipSmtpHandler.h"

// Macros and Defines ---------------------------------------------------------
// define the SMTP port
#define SMTP_CON_PORT         ( 25 )

// define the size of the local buffer
#define	LCL_BUF_SIZE	        ( 200 )

// define the maximum number of arguments
#define	SMTP_NUM_ARGS	        ( 8 )

// enumerations ---------------------------------------------------------------
// enumerate the STMP events
typedef enum	_SMTPEVENTS
{
  SMTP_EVENT_RDY		  = 220,
  SMTP_EVENT_CLOSE,
  SMTP_EVENT_OK		    = 250,
  SMTP_EVENT_DATA		  = 354
} SMTPEVENTS;

// enumerate the SMTP states
typedef enum	_SMTPSTATES
{
  SMTP_STATE_IDLE = 0,
  SMTP_STATE_ECHO,
  SMTP_STATE_MHDR,
  SMTP_STATE_MRCV,
  SMTP_STATE_MDAT,
  SMTP_STATE_MCNT,
  SMTP_STATE_COMP,
  SMTP_STATE_QUIT,
  SMTP_STATE_MAX
} SMTPSTATES;

/// enumerate the top level process states
typedef enum _SMTPTOPLVL
{
  SMTP_TOPLVL_IDLE = 0,     ///< idle state
  SMTP_TOPLVL_BUSY          ///< busy
} SMTPTOPLVL;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	struct tcp_pcb	      *ptConnCtl;
static	struct ip_addr	      tIpAddr;
static  BOOL                  bRequestClose;
static	BOOL			            bConnectInProgress;
static  U32                   uXmtCount;
static	VU32			            uXmtBytesSent;
static	U32				            uSmtpServerIpa;
static	struct ip_addr	      tSmtpServerIpa;
static  STATEEXECENGCONTROL   tStateCtl;
static	PC8				            apszArgs[ SMTP_NUM_ARGS ];
static	U32				            auArgs[ SMTP_NUM_ARGS ];
static	U8				            anLclBuffer[ LCL_BUF_SIZE ];
static	U8				            nBufIdx;
static	SMTMSG			          tLclMsg;
static  LWIPSMTPDEF           tSmtpConfig;
static  SMTPTOPLVL            eSmtpTopLvl;
static  LWIPSMTPMSG           tLclMsg;

// local function prototypes --------------------------------------------------
static	void  HostNameFoundCallback( const PC8 pszName, struct ip_addr* ptIpAddr, PVOID pvArg );
static	S8		ConnectedCallback( PVOID pvArg, struct tcp_pcb *ptPcb, S8 cErr );
static  S8    DataSentCallback( PVOID pvArg, struct tcp_pcb *ptPcb, U16 wSentLen );
static  S8    DataReceivedCallback( PVOID pvArg, struct tcp_pcb *ptPcb, struct pbuf *ptBuf, S8 cErr );
static  void  ErrorDetectedCallback( PVOID pvArg, S8 cErr );
static  void  CloseConnection( struct tcp_pcb *ptPcb );

// SMTP_STATE_ECHO functions
static	void	SmtpStateEchoEntry( void );

// SMTP_STATE_MHDR functions
static	void	SmtpStateMhdrEntry( void );

// SMTP_STATE_MRCV functions
static	void	SmtpStateMrcvEntry( void );

// SMTP_STATE_MDAT functions
static	void	SmtpStateMdatEntry( void );

// SMTP_STATE_MCNT functions
static	void	SmtpStateMcntEntry( void );

// SMTP_STATE_COMP functions
static	void	SmtpStateCompEntry( void );
static	U8	  SmtpStateCompExec( TASKARG xArg );

// SMTP_STATE_QUI functions
static	void	SmtpStateQuitEntry( void );

// SMTP_ALL other states
static	U8	  SmtpStateAllsExec( TASKARG xArg );

// constant parameter initializations -----------------------------------------
// configuration defaults
const CODE LWIPSMTPDEF  tLwipSmtpDflts =
{
  .acHostName = "smtp.gmail.com",
  .wPort      = 465
  .acFromName = "wbasser@gmail.com"
};

// string constant initializations
static	const C8	szSmtpServer[ ]     = { "smtpint.electrolux-na.com" };
static	const C8	szSmtpEhlo[ ]	      = { "HELO %s\r\n" };
static	const C8	szSmtpData[ ]	      = { "DATA\r\n" };
static	const C8	szSmtpQuit[ ]	      = { "QUIT\r\n\r\n" };
static	const C8	szSmtpDend[ ]	      = { "\r\n.\r\n" };
static	const C8	szSmtpMlfr[ ]	      = { "MAIL FROM: %s\r\n" };
static	const C8	szSmtpMlto[ ]	      = { "RCPT TO: %s\r\n" };
static	const C8	szSmtpCsbj[ ]	      = { "Subject: %s\r\n" };
static	const C8	szSmtpCfrm[ ]	      = { "FROM: \"%s-%s\"<%s>\r\n" };
static	const C8	szSmtpCnto[ ]	      = { "TO: \"%s\"<%s>\r\n\r\n" };
static	const C8	szDataLog[ ]	      = { "MACS Data Logger" };
static	const C8	szDelims[ ]		      = { " \t\r" };
static	const C8	szFmtStr[ ]		      = { "%s\r\n" };
static  const C8  acErrConnected[ ]   = { 0x15, 0xEF };

// SMTP_STATE_IDLE events
static	const CODE STATEEXECENGEVENT	atSmtpStateIdleEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_RDY,	SMTP_STATE_ECHO, FALSE ),
  STATEEXECENGEVENT_END( )
};

// SMTP_STATE_ECHO events
static	const CODE STATEEXECENGEVENT	atSmtpStateEchoEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_OK,	SMTP_STATE_MHDR, FALSE ),
  STATEEXECENGEVENT_END( )
};

// SMTP_STATE_MHDR events
static	const CODE STATEEXECENGEVENT	atSmtpStateMhdrEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_OK,	SMTP_STATE_MRCV, FALSE ),
  STATEEXECENGEVENT_END( )
};

// SMTP_STATE_MRCV events
static	const CODE STATEEXECENGEVENT	atSmtpStateMrcvEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_OK,	SMTP_STATE_MDAT, FALSE ),
  STATEEXECENGEVENT_END( )
};

// SMTP_STATE_MDAT events
static	const CODE STATEEXECENGEVENT	atSmtpStateMdatEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_DATA,	SMTP_STATE_MCNT, FALSE ),
  STATEEXECENGEVENT_END( )
};

// SMTP_STATE_MCNT events
static	const CODE STATEEXECENGEVENT	atSmtpStateMcntEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_OK,	SMTP_STATE_COMP, FALSE ),
  STATEEXECENGEVENT_END( )
};

// SMTP_STATE_COMP events
static	const CODE STATEEXECENGEVENT	atSmtpStateCompEvents[ ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_EVENT_CLOSE,	SMTP_STATE_IDLE, FALSE ),
  STATEEXECENGEVENT_END( )
};

// states table
static	const CODE STATEEXECENGTABLE	atSmtpStateTable[ SMTP_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( SMTP_STATE_IDLE,	              NULL,	             NULL,	NULL,	atSmtpStateIdleEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_ECHO,	SmtpStateEchoEntry,	SmtpStateAllsExec,	NULL,	atSmtpStateEchoEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_MHDR,	SmtpStateMhdrEntry,	SmtpStateAllsExec,	NULL,	atSmtpStateMhdrEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_MRCV,	SmtpStateMrcvEntry,	SmtpStateAllsExec,	NULL,	atSmtpStateMrcvEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_MDAT,	SmtpStateMdatEntry,	SmtpStateAllsExec,	NULL,	atSmtpStateMdatEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_MCNT,	SmtpStateMcntEntry,	SmtpStateAllsExec,	NULL,	atSmtpStateMcntEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_COMP,	              NULL,	SmtpStateCompExec,	NULL,	atSmtpStateCompEvents	),
  STATEXECENGETABLE_ENTRY( SMTP_STATE_QUIT, SmtpStateQuitEntry,	             NULL,  NULL,	atSmtpStateQuitEvents ),
};

/******************************************************************************
 * @function LwipSmtpHandler_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the state machine, get the IP address for
 * the SMTP server
 *
 *****************************************************************************/
void LwipSmtpHandler_Initialize( void )
{
  // initialize the state table
  tStateCtl = atSmptStateTable;
  StateExecutionEngine_Initialize( &tStateCtl, SMPT_STATE_IDLE );
  
  // reset the TCP connection control
  ptConnCtl = NULL;
  
  // clear the flags
  bRequestClose = bConnectInProgress = FALSE;
  
  // set the top level state
  eSmtpTopLvl = SMTP_TOPLVL_IDLE;
  
  // get the configuration
  ConfigManager_GetLwipLocDef( &tSmtpConfig );

  // initiate a host name lookup
  tSmtpServerIpa.addr = 0;
  dns_gethostbyname( tSmtpConfig.acHostName, &tSmtpServerIpa, ( dns_found_callback )HostNameFoundCallback, NULL );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void LwipSmtHandler_SendMessage( PC8 pszRcpCont, PC8 pszRcvMesg )
{
}

/******************************************************************************
 * @function LwipSmtpHandler_SocketProcess
 *
 * @brief socket process
 *
 * This function is called by the LWIP timer processor
 *
 *****************************************************************************/
void LwipSmtpHandler_SocketProcess( void )
{
  // are we connected and is there data to send
  if (( ptConnCtl != NULL ) && ( uXmtCount != 0 ))
  {
    // write the data
    tcp_write( ptConnCtl, anLclBuffer, uXmtCount, 1 );
    
    // increment the count of the bytes sent
    uXmtBytesSent += uXmtCount;
    
    // output the data
    tcp_output( ptConnCtl );
    
    // reset the transmit count
    uXmtCount = 0;
  }
  
  // check for a close
  if ( ptConnCtl != NULL ) && ( uXmtBytesSent == 0 ) && ( bRequestClose ))
  {
    // close the connection
    CLoseConnection( ptConnCtl );
  }
}

/******************************************************************************
 * @function LwipSmtHandler_ProcessEvents
 *
 * @brief process task manager events
 *
 * This function will process the task manager events, if the event is handler
 * the event will be flushed
 *
 * @param[in]   xArg      event to be processed
 *
 * @return    TRUE if event is handled, FALSE if deferered
 *
 *****************************************************************************/
BOOL LwipSmtHandler_ProcessEvents( TASSKARG xArg )
{
  BOOL    bFlushEvents = TRUE;
  PVOID   pvPcb;
  err_t   eError;
  
  // determine the state
  switch( eSmtpTopLvl )
  {
    case SMTP_TOPLVL_IDLE :
      // is this an queue put event
      if ( xArg == QUEUE_EVENT_PUT )
      {
        // new message ready to send
        QueueManager_Get( LWIPSMTP_INCOMING_MSG_ENUM, *( PU8 )&tLclMsg );
        
        // check to see if a connection is in progress
        if ( bConnectInProgress == FALSE )
        {
          // set connect in progress
          bConnectInProgress = TRUE;
          
          // ensure that we have a valid server IPA
          if ( tSmtpServerIpa.addr != 0  )
          {
            // create a new socket/connect to server
            pvPcb = tcp_new( );
            eError = tcp_Connect( pvPcb, &tSmtpServerIpa, SMTP_CON_PORT, ConnectedCallback );
            
            // now go to busy state
            eSmtpTopLvl = SMTP_TOPLVL_BUSY;
          }
        }
      }
      break;
      
    case SMTP_TOPLVL_BUSY :
      break;
      
    default :
      // should never happen - reset back to idle
      eSmtpTopLvl = SMPT_TOPLVL_IDLE;
      break;
  }

  // return the flush events
  return( bFlushEvents );
}

/******************************************************************************
 * @function LwipSmtpHandler_UpdateConfiguration
 *
 * @brief update configuration
 *
 * This function will handle an update to the configuration
 *
 *****************************************************************************/
void LwipSmtpHandler_UpdateConfiguration( void )
{
  // re-initiate a host name lookup
  tSmtpServerIpa.addr = 0;
  dns_gethostbyname( tSmtpConfig.acHostName, &tSmtpServerIpa, ( dns_found_callback )HostNameFoundCallback, NULL );
}


/******************************************************************************
 * @function HostNameFoundCallback
 *
 * @brief host name callback function
 *
 * This function processes the callback from a host name lookup
 *
 * @param[in]   pszName     host name
 * @param[in]   ipAddr      pointer to the ip address
 * @param[in]   pvArg       pointer to an argument
 *
 *****************************************************************************/
static void HostNameFound( const PC8 pszName, struct ip_addr* ptIpAddr, PVOID pvArg )
{
  // set the IPA address for SMTP server
  tSmtpServerIpa.addr = ptIpAddr->addr;
}

/******************************************************************************
 * @function ConnectedCallback
 *
 * @brief connected callback function
 *
 * This function processes the callback from a connection
 *
 * @param[in]   pvArg       pointer to an argument
 * @param[in]   tcp_pcb     pointer to the tcp control structure
 * @param[in]   cErr        error
 *
 * @return      ERR_OK
 *
 *****************************************************************************/
static S8 ConnectedCallback( PVOID pvArg, struct tcp_pcb *ptPcb, S8 cErr )
{
  // set up the handler for data received
  tcp_arg( ptPcb, ptPcb );
  tcp_recv( ptPcb, DataReceivedCallback );
  tcp_err( ptPcb, ErrorDetectedCallback );
  tcp_sent( ptPcb, DataSentCallback );

  // set the current connection
  ptConnCtl = ptPcb;

  // force idle state
  tSmtpControl.eCurState	= SMTP_STATE_IDLE;
  
  // return no error
  return( ERR_OK );
}

/******************************************************************************
 * @function DataSentCallback
 *
 * @brief data sent callback function
 *
 * This function processes the callback from a data being sent
 *
 * @param[in]   pvArg       pointer to an argument
 * @param[in]   tcp_pcb     pointer to the tcp control structure
 * @param[in]   wSentLen    number of bytes sent
 *
 * @return      ERR_OK
 *
 *****************************************************************************/
static S8 DataSentCallback( PVOID pvArg, struct tcp_pcb *ptPcb, U16 wSentLen )
{
  // is this our main connection
  if ( pvArg != NULL )
  {
    // decrement the outstanding bytes
    uXmtBytesSent -= wSentLen;
  }
  else
  {
    // check to see if this was our error message
    if ( wSentLen == sizeof( acErrConnected ))
    {
      // close the temp connection
      tcp_sent( ptPcb, NULL );
      tcp_close( ptPcb );
    }
  }

  // return no error
  return( ERR_OK );
}

/******************************************************************************
 * @function DataReceivedCallback
 *
 * @brief data received callback function
 *
 * This function processes the callback from data being received
 *
 * @param[in]   pvArg       pointer to an argument
 * @param[in]   tcp_pcb     pointer to the tcp control structure
 * @param[in]   ptBuf       pointer to the recieved buffer structure
 * @param[in]   cErr        error
 *
 * @return      ERR_OK
 *
 *****************************************************************************/
static S8 DataReceivedCallback( PVOID pvArg, struct tcp_pcb *ptPcb, struct pbuf *ptBuf, S8 cErr )
{
  struct pbuf *ptLcl;
  U8			    nIndex;
  U8			    nArgs;
  PC8			    pcBuffer;

  // check for a packet
  if (( cErr == ERR_OK ) && ( ptBuf != NULL ))
  {
    // accept the packet
    tcp_recved( ptPcb, ptBuf->tot_len );

    // loop through the buffers in this packet
    for ( ptLcl = ptBuf, pcBuffer = ptLcl->payload; ptLcl != NULL; ptLcl = ptLcl->next )
    {
      // parse the buffer/execut the state
      nArgs = StrParseLine( pcBuffer, ( PC8 )szDelims, &apszArgs[ 0 ], auArgs, SMTP_NUM_ARGS );
      
      // process the event through the state engine
      StateExecutionEngine_Process( &tSmtpControl, ( TASKARG )auArgs[ 0 ] );
    }

    // free the buffer
    pbuf_free( ptBuf );
  }
  else
  {
    // reset the transmitted count/null packet, close connection
    uXmtCount = 0;
    CloseConnection( ptPcb );
  }

  // return ok
  return( ERR_OK );
}

/******************************************************************************
 * @function ErrorDetectedCallback
 *
 * @brief error detected callback function
 *
 * This function processes the callback upon an error detection
 *
 * @param[in]   pvArg       pointer to an argument
 * @param[in]   cErr        error
 *
 *****************************************************************************/
static void ErrorDetectedCallback( PVOID pvArg, S8 cErr )
{
  // reset the connection
  ptConnCtl = NULL;
}

/******************************************************************************
 * @function CloseConnection
 *
 * @brief connection close
 *
 * This function will close the connection
 *
 * @param[in]   tcp_pcb     pointer to the tcp control structure
 *
 *****************************************************************************/
static void CloseConnection( struct tcp_pcb *ptPcb )
{
  // clear all callbacks
  tcp_sent( ptPcb, NULL );
  tcp_recv( ptPcb, NULL );
  tcp_err( ptPcb, NULL );
  
  // reset the connection
  ptConnCtl = NULL;
  
  // set the connection flag
  bRequestClose = bConnectInProgress = FALSE;

  // close the connection
  tcp_close( ptPcb );
}

/******************************************************************************
 * SMTP_STATE_ECHO functions
 *****************************************************************************/
static void SmtpStateEchoEntry( void )
{
  // send the echo string
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpEhlo, apszArgs[ 1 ] );
}

/******************************************************************************
 * SMTP_STATE_MHDR functions
 *****************************************************************************/
static void SmtpStateMhdrEntry( void )
{
  // send the from string
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpMlfr, szSmtpFrom );
}

/******************************************************************************
 * SMTP_STATE_MRCV functions
 *****************************************************************************/
static void	SmtpStateMrcvEntry( void )
{
  // send the to string
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpMlto, tLclMsg.szAddr );
}

/******************************************************************************
 * SMTP_STATE_MDAT functions
 *****************************************************************************/
static void	SmtpStateMdatEntry( void )
{
  // send the data command
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpData );
}

/******************************************************************************
 * SMTP_STATE_MCNT functions
 *****************************************************************************/
static void	SmtpStateMcntEntry( void )
{
  // send the content
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpCsbj, szSDlogSubj );
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpCfrm, szDataLog, CfgGetNam( ), szSmtpFrom );
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpCnto, tLclMsg.szAddr, tLclMsg.szAddr );
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szFmtStr, tLclMsg.szData );
  uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpDend );
}

/******************************************************************************
 * SMTP_STATE_COMP functions
 *****************************************************************************/
static void	SmtpStateCompEntry( void )
{
  U8  nNewState;

  // call the default handler
  if (( nNewState = SmtpStateAllsExec( xArg )) == STE_STATE_NONE )
  {
      // set the state back to mail header
      eNewState = SMTP_STATE_MHDR;
    }
    else
    {
      // send quit
      uXmtCount += sprintf(( PC8 )&anLclBuffer[ uXmtCount ], szSmtpQuit );
      eNewState = STE_STATE_NONE;
    }
  }

  // return the new state
  return( nNewState );
}

static U8 SmtpStateCompExec( TASKARG xArg )
{
  // set request close flag
  bRequestClose = TRUE;
}

/******************************************************************************
 * SMTP_STATE_QUI functions
 *****************************************************************************/
static	void	SmtpStateQuitEntry( void )
{
}

/******************************************************************************
 * SMTP_ALL other states
 *****************************************************************************/
static	U8	  SmtpStateAllsExec( TASKARG xArg )
{
}

/**@} EOF LwipSmtpHandler.c */
