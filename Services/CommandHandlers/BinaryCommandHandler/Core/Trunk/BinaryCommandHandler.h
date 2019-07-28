/******************************************************************************
 * @file BinaryCommandHandler.h
 *
 * @brief Binary protocol handler declarations 
 *
 * This file provices the declarations for the binary protocol handler
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
 * $Log: $
 * 
 *
 * \addtogroup BinaryCommandHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYCOMMANDHANDLER_H
#define _BINARYCOMMANDHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the command ack
#define BINCMD_RESPONSE_ACK                 ( 0x06 )

/// define the macro to disable special commands
#define BINCMD_DISABLE_SPECIAL_CMD          ( -1 )

// enumerations ---------------------------------------------------------------
/// enumerate the return status for the protocol handler
typedef enum _BINCMDSTS
{
  BINCMD_STS_IDLE = 0,            ///< idle state
  BINCMD_STS_MSG_INPROG,          ///< message decoding is in progress
  BINCMD_STS_MSGRCVD_ERR,         ///< message received and error detected
  BINCMD_STS_MSGRCVD_PROC,        ///< message received and processed
  BINCMD_STS_MSGRCVD_NOTPROC,     ///< message received but no handler for it
  BINCMD_STS_ILLPROTENUM = 0xE8,  ///< illegal protocol enumeration
  BINCMD_STS_BUFFULL,             ///< transmit buffer full
  BINCMD_STS_BUFEMPTY,            ///< buffer empty
  BINCMD_STS_ILLBUFIDX,           ///< illegal bus index
  BINCMD_STS_NOTMASTERDEV,        ///< not a master device
  BINCMD_STS_ILLMSTCMD,           ///< illegal master command
  BINCMD_STS_CRCERR,              ///< CRC error
  BINCMD_STS_BUSBUSY,             ///< bus busy
} BINCMDSTS;

// global function prototypes --------------------------------------------------
extern  void      BinaryCommandHandler_Initialize( void );
#if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
extern  BINCMDSTS BinaryCommandHandler_SendMstMessage( BINCMDENUM eProtEnum, U8 nMstCmdEnum, U8 nOption, U8 nDstAddr, S16 sSpecialCmd );
#endif // BINCMDHAND_ENABLE_MASTERMODE
extern  BINCMDSTS BinaryCommandHandler_ProcessChar( BINCMDENUM eProtEnum, U8 nRcvChar, U8 nCompareValue );
extern  BINCMDSTS BinaryCommandHandler_ResetProtocol( BINCMDENUM eProtEnum );
extern  BINCMDSTS BinaryCommandHandler_ResetXmtLength( BINCMDENUM eProtEnum );
extern  BINCMDSTS BinaryCommandHandler_BeginMessage( BINCMDENUM eProtEnum, U8 nCommand, U8 nOption );
extern  BINCMDSTS BinaryCommandHandler_SendMessage( BINCMDENUM eProtEnum );
extern  BINCMDSTS BinaryCommandHandler_SetOption( BINCMDENUM eProtEnum, U8 nOption );
extern  BINCMDSTS BinaryCommandHandler_SetSequence( BINCMDENUM eProtEnum, U8 nSequence );
extern  BINCMDSTS BinaryCommandHandler_SetDstAddr( BINCMDENUM eProtEnum, U8 nDstAddr );
extern  BINCMDSTS BinaryCommandHandler_SetLclAddr( BINCMDENUM eProtEnum, U8 nLclAddr );
extern  BINCMDSTS BinaryCommandHandler_GetLclAddr( BINCMDENUM eProtEnum, PU8 pnLclAddr );
extern  BINCMDSTS BinaryCommandHandler_GetCommand( BINCMDENUM eProtEnum, PU8 pnCommand );
extern  BINCMDSTS BinaryCommandHandler_GetOption( BINCMDENUM eProtEnum, PU8 pnOption );
extern  BINCMDSTS BinaryCommandHandler_GetSequence( BINCMDENUM eProtEnum, PU8 pnSequence );
extern  BINCMDSTS BinaryCommandHandler_GetSrcAddr( BINCMDENUM eProtEnum, PU8 pnSrcAddr );
extern  BINCMDSTS BinaryCommandHandler_GetRcvLength( BINCMDENUM eProtEnum, PU16 pwRcvLength );
extern  BINCMDSTS BinaryCommandHandler_SetXmtLength( BINCMDENUM eProtEnum, U16 wXmtLength );
extern  BINCMDSTS BinaryCommandHandler_SetMessageByte( BINCMDENUM eProtEnum, U8 nXmtChar );
extern  BINCMDSTS BinaryCommandHandler_GetMessageByte( BINCMDENUM eProtEnum, U16 wIndex, PU8 pnRcvChar );
extern	BINCMDSTS BinaryCommandHandler_SetMessageBlock( BINCMDENUM eProtEnum, PU8 pnData, U16 wLength );
extern  BINCMDSTS BinaryCommandHandler_GetRcvBufferPointer( BINCMDENUM eProtEnum, PU8* ppnBuffer );
extern  BINCMDSTS BinaryCommandHandler_GetXmtBufferPointer( BINCMDENUM eProtEnum, PU8* ppnBuffer );
extern  BINCMDSTS BinaryCommandHandler_GetRcvBufferSize( BINCMDENUM eProtEnum, PU16 pwBufSize );
extern  BINCMDSTS BinaryCommandHandler_GetXmtBufferSize( BINCMDENUM eProtEnum, PU16 pwBufSize );
#ifdef __ATMEL_AVR__
extern	BINCMDSTS BinaryCommandHandler_SetMessageBlockP( BINCMDENUM eProtEnum, PFU8 pnData, U16 wLength );
#endif

/**@} EOF BinaryCommandHandler.h */

#endif  // _BINARYCOMMANDHANDLER_H