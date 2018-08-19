/****************************************7**************************************
 * @file  DALIProtocolHandler_cfg.h
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
 * \addtogroup  DALIProtocolHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIPROTOCOLHANDLER_CFG_H
#define _DALIPROTOCOLHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DALIProtocolHandler_SetLedLevel( U8 nDeviceIndex, U16 wLevel );
extern  BOOL  DALIProtocolHandler_GetLedStatus( void );
extern  void  DALIProtocolHandler_SeedRandom( void);
extern  U32   DALIProtocolHandler_GenerateRandom( void );
extern  BOOL  DALIProtocolHandler_WriteMemoryBlock( U8 nDeviceIndex, U8 nMemoryBlock, U8 nBlockAddress, U8 nValue, BOOL bBypassLockByte );
extern  BOOL  DALIProtocolHandler_ReadMemoryBlock( U8 nDeviceIndex, U8 nMemoryBlock, U8 nBlockAddress, PU8 pnValue );
extern  BOOL  DALIProtocolHandler_WriteMBParameter( U8 nDeviceIndex, U8 nParam, U8 nValue, BOOL bBypassLockByte );
extern  BOOL  DALIProtocolHandler_GetDefaultValue( U8 nDeviceIndex, U8 nParam, PU8 pnValue );
extern  BOOL  DALIProtocolHandler_PutValue( U8 nDeviceIndex, U8 nParam, U8 nValue );
extern  BOOL  DALIProtocolHandler_GetValue( U8 nDeviceIndex, U8 nParam, PU8 pnValue );
extern  void  DALIProtocolHandler_ResetUserDefaults( void );

/**@} EOF DALIProtocolHandler_cfg.h */
#endif  // _DALIPROTOCOLHANDLER_CFG_H
