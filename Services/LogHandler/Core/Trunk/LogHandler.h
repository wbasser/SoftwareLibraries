/******************************************************************************
 * @file LogHandler.h
 *
 * @brief Log Handler implementation declarations
 *
 * This file provides the declarations for the implementation of the log handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LOGHANDLER_H
#define _LOGHANDLER_H

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler_cfg.h"

// library includes -----------------------------------------------------------
#if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  #include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS

// enumerations ---------------------------------------------------------------
/// enumerate the log position type
typedef enum _LOGPOS
{
  LOG_POS_OLDEST = 0,     ///< oldest position
  LOG_POS_NEXT,           ///< next position from current
  LOG_POS_PREV,           ///< previous position from current
  LOG_POS_NEWEST,         ///< newest position
  LOG_POS_MAX
} LOGPOS;

/// enumerate the log display modes
typedef enum _LOGDISPMODE
{
  LOG_DISPMODE_NEWEST_ONLY = 0, ///< display the newest only
  LOG_DISPMODE_NEWEST_LAST16,   ///< newest - last 16
  LOG_DISPMODE_OLDEST_ONLY,     ///< display the oldest only
  LOG_DISPMODE_OLDEST_NEXT16,   ///< display the oldest - next 16
  LOG_DISPMODE_ALL,             ///< display all entries
  LOG_DISPMODE_MAX
} LOGDISPMODE;

// global parameter declarations -----------------------------------------------
#if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  extern  const CODE ASCCMDENTRY g_atLogHandlerCmdHandlerTable[ ];
#endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS

// global function prototypes --------------------------------------------------
extern  void  LogHandler_Initialize( void );
extern  BOOL  LogHandler_AddEntry( LOGTYPE eType, U32 uArg1, U32 uArg2 );
extern  BOOL  LogHandler_GetEntry( LOGPOS ePos, PLOGTYPE peType, PU32 puArg1, PU32 puArg2, PU64 phTime ); 
extern  U16   LogHandler_GetCount( void );
extern  PC8   LogHandler_GetDescription( LOGTYPE eType );
extern  void  LogHandler_ClearAllEntries( void );

/**@} EOF LogHandler.h */

#endif  // _LOGHANDLER_H