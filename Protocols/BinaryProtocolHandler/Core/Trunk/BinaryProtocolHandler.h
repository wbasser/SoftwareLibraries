/******************************************************************************
 * @file BinaryProtocol.h
 *
 * @brief Binary protocol definitions 
 *
 * This file provides the definitions for the binary protocol handler
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup BinaryProtocol
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYPROTOCOL_H
#define _BINARYPROTOCOL_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinaryProtocolHandler/BinaryProtocolHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the broadcast address
#define BINPROT_BROADCAST_ADDR                ( 0 )

// enumerations ---------------------------------------------------------------
/// enumerate the message status
typedef enum _BINPROTMSGSTS
{
  BINPROT_MSGSTS_OKAY = 0,      ///< message ok
  BINPROT_MSGSTS_RCVOKAY,       ///< message transmit ok, message receive ok
  BINPROT_MSGSTS_TIMEOUT = -1,  ///< timeout
  BINPROT_MSGSTS_ERRXMT = -2,   ///< transmit error
  BINPROT_MSGSTS_ILLDEF = -2,   ///< illegal device
} BINPROTMSGSTS;

// structures -----------------------------------------------------------------
/// define the message structure
typedef struct _BINPROTMSG
{
  U8      nDstAddr;             ///< destination address
  U8      nSrcAddr;             ///< source address
  U8      nSequence;            ///< sequence number
  U8      nCommand;             ///< command
  U8      nOption;              ///< option
  PU8     pnMessage;            ///< pointer to the message
  U16     wLength;              ///< length of message
} BINPROTMSG, *PBINPROTMSG;
#define BINPROTMSG_SIZE           sizeof( BINPROTMSG )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          BinaryProtocol_Initialize( void ); 
extern  BINPROTMSGSTS BinaryProtocol_Close( BINPROTENUM eProtEnum );
extern  void          BinaryProtocol_ProcessCharCallback( BINPROTENUM eProtEnum, U8 nData );
extern  BINPROTMSGSTS BinaryProtocol_SendRcvMessage( BINPROTENUM eProtEnum, PBINPROTMSG ptXmtMessage, PBINPROTMSG ptRcvMessage, S16 sTimeoutMsec );

/**@} EOF BinaryProtocol.h */

#endif  // _BINARYPROTOCOL_H