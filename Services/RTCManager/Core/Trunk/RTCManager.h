/******************************************************************************
 * @file RTCManager.h
 *
 * @brief RTC Manager delcarations 
 *
 * This file provides the declarations for the implementation of the RTC 
 * manager
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
 * \addtogroup RTCManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTCMANAGER_H
#define _RTCMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RTCManager/RTCManager_prm.h"
#include "RTCmanager/RTCmanager_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "TaskManager/TaskManager.h"
#if ( RTCMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif  // RTCMANAGER_ENABLE_DEBUG_COMMANDS

// Macros and Defines ---------------------------------------------------------
/// define the number of events/execution rate for the fast time handler
#define RTCMANAGER_NUM_EVENTS                   ( 1 )
#define RTCMANAGER_EXEC_RATE                    ( TASK_TIME_SECS( 1 ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
#if ( RTCMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
extern  const CODE ASCCMDENTRY atRtcManagerCmdHandlerTable[ ];
#endif  // RTCMANAGER_ENABLE_DEBUG_COMMANDS

// global function prototypes --------------------------------------------------
extern  void  RTCManager_Initialize( void );
extern  void  RTCManager_EnableDisableFastTime( BOOL bState );
extern  void  RTCManager_GetDateTime( PDATETIME ptDateTime );
extern  void  RTCManager_SetDateTime( PDATETIME ptDateTime );
extern  BOOL  RTCManager_ProcessEvent( TASKARG xArg );

/**@} EOF RTCManager.h */

#endif  // _RTCMANAGER_H