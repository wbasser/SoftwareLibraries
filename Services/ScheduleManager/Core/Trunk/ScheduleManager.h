/**************************************************************************//**
 * @file ScheduleManager.h
 *
 * @brief  light control manager implementation declarations 
 *
 * This file provides the declarations for the light control manager
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
 * \addtogroup schedulerManager 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SCHEDULEMANAGER_H
#define _SCHEDULEMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ScheduleManager/ScheduleManager_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events for this task
#define SCHDMNGR_NUM_EVENTS          ( 8 )

/// define the execution rate
#define SCHDMNGR_EXEC_RATE           ( TASK_TIME_MINS( 1 ))

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _SCHDMNGRERROR
{
  SCHDMNGR_ERROR_NONE         = 0,      ///< no error
  SCHDMNGR_ERROR_ILLRULEINDEX = 0xE0,   ///< illegal rule index
} SCHDMNGRERROR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void                ScheduleManager_Initialize( void );
extern  void                ScheduleManager_Control( BOOL bState );
extern  PSCHDMNGRDEFTBL     ScheduleManager_GetActlTable( void );
extern  PSCHDMNGRDEFTBL     ScheduleManager_GetDfltTable( void );
extern  SCHDMNGRERROR       ScheduleManager_SetSchdRule( SCHDMNGRDEFENUM eRuleIndex, PSCHDMNGRDEFENTRY ptRule, BOOL bUpdateNVRom );
extern  SCHDMNGRERROR       ScheduleManager_GetSchdRule( SCHDMNGRDEFENUM eRuleIndex, PSCHDMNGRDEFENTRY* pptRule );
extern  SCHDMNGRERROR       ScheduleManager_SetScheduleIndex( SCHDMNGRDEFENUM eScheduleIndex );
extern  U8                  ScheduleManager_GetScheduleIndex( void );
extern  void                ScheduleManager_ForceNVRomUpdate( void );
extern  BOOL                ScheduleManager_ProcessEvent( TASKARG xArg );

/**@} EOF ScheduleManager.h */

#endif  // _ScheduleManager_H