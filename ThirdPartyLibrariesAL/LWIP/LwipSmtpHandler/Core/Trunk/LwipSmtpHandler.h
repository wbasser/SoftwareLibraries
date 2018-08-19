/******************************************************************************
 * @file LwipSmtpHandler.h
 *
 * @brief LWIP Simple Mail Transport Protocol (SMTP) handler declarations 
 *
 * This file provides the declarations for the SMTP handler
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
 
// ensure only one instantiation
#ifndef _LWIPSMTPHANDLER_H
#define _LWIPSMTPHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the number of messages
#define	LWIPSMT_NUM_MSGS		    ( 4 )
#define	LWIPSMT_NUM_EVENTS		  ( LWIPSMT_NUM_MSGS )

/// define the SMTP server host name maximum length
#defene SMTP_HOST_NAME_LEN      ( 80 )
#defene SMTP_FROM_NAME_LEN      ( 80 )

/// define the SMTP message lengths
#define LWIPSMTP_MSGADDR_LEN    ( 80 )
#define LWIPSMTP_MSGDATA_LEN    ( 160 )
#define LWIPSMTP_MSGSUBJ_LEN    ( 40 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct PACKED _LWIPSMTPDEF
{
  C8  acHostName[ SMTP_HOST_NAME_LEN ];
  C8  acFromName[ SMTP_FROM_NAME_LEN ];
  U16 wPort;
} LWIPSMTPDEF, *PLWIPSMTPDEF;
#define LWIPSMTPDEF_SIZE   sizeof( LWIPSMTPDEF )

/// define the SMTP message structure
typedef	struct	_LWIPSMTPMSG
{
	C8	acAddr[ LWIPSMTP_MSGADDR_LEN ];
	C8	acData[ LWIPSMTP_MSGDATA_LEN ];
  C8  acSubj[ LWIPSMTP_MSGSUBJ_LEN ];
} LWIPSMTPMSG, *PLWIPSMTPMSG;
#define	LWIPSMTPMSG_SIZE	sizeof( LWIPSMTPMSG )

// global parameter declarations -----------------------------------------------
extern  const CODE LWIPSMTPDEF  tLwipSmtpDflts;

// global function prototypes --------------------------------------------------
extern	void	LwipSmtpHandler_Initialize( void );
extern	void	LwipSmtpHandler_SendMessage( PC8 pszRcpCont, PC8 pszRcvMesg );
extern  void  LwipSmtpHandler_SocketProcess( void );
extern	BOOL	LwipSmtpHandler_ProcessEvents( TSKARG xArg );
extern  void  LwipSmtpHandler_UpdateConfiguration( void );


/**@} EOF LwipSmtpHandler.h */

#endif  // _LWIPSMTPHANDLER_H