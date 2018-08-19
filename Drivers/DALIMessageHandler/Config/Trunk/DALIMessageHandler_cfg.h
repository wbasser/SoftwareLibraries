/****************************************7**************************************
 * @file  DALIMessageHandler_cfg.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_CFG_H
#define _DALIMESSAGEHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DALIMessageHandler_OutputXmtMsg( U8 nResponse );
extern  void  DALIMessageHandler_OutputRcvMsg( PDALIMSG ptMsg );
extern  void  DALIMessageHandler_StartReceive( PU8 pnData, U8 nLength );
extern  void  DALIMessageHandler_StopReceive( void );
extern  void  DALIMessageHandler_StartTransmit( PU8 pnData, U8 nLength );
extern  void  DALIMessageHandler_SetLedLevel( U16 wLevel );
extern  BOOL  DALIMessageHandler_GetLedStatus( void );
extern  void  DALIMessageHandler_SeedRandom( void);
extern  U32   DALIMessageHandler_GenerateRandom( void );
extern  BOOL  DALIMessageHandler_WriteMemoryBlock( U8 nMemoryBlock, U8 nBlockAddress, U8 nValue, BOOL bBypassLockByte );
extern  BOOL  DALIMessageHandler_ReadMemoryBlock( U8 nMemoryBlock, U8 nBlockAddress, PU8 pnValue );
extern  BOOL  DALIMessageHandler_WriteMBParameter( U8 nParam, U8 nValue, BOOL bBypassLockByte );
extern  BOOL  DALIMessageHandler_GetDefaultValue( U8 nParam, PU8 pnValue );
extern  BOOL  DALIMessageHandler_PutValue( U8 nParam, U8 nValue );
extern  BOOL  DALIMessageHandler_GetValue( U8 nParam, PU8 pnValue );
extern  void  DALIMessageHandler_ResetUserDefaults( void );

/**@} EOF DALIMessageHandler_cfg.h */
#endif  // _DALIMESSAGEHANDLER_CFG_H
