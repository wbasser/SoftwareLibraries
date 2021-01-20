/******************************************************************************
 * @file TaskManager.h
 *
 * @brief task manager declarations
 *
 * This file provides the declarations for the task manager
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
 * \addtogroup TaskManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _TASKMANAGER_H
#define _TASKMANAGER_H

// local includes -------------------------------------------------------------
#include "TaskManager/TaskManager_cfg.h"

// Macros and Defines ---------------------------------------------------------

// global function prototypes --------------------------------------------------
extern 	void  TaskManager_Initialize( void );
extern	void  TaskManager_IdleProcess( void );
extern	void  TaskManager_TickProcess( void );
extern  BOOL  TaskManager_EnableDisable( TASKSCHDENUMS eTask, BOOL bState );
extern	BOOL  TaskManager_PostEvent( TASKSCHDENUMS eTask, TASKARG xArg );
extern  BOOL  TaskManager_PostPriorityEvent( TASKSCHDENUMS eTask, TASKARG xArg );
extern	BOOL  TaskManager_PostEventIrq( TASKSCHDENUMS eTask, TASKARG xArg );
extern  BOOL  TaskManager_PostPriorityEventIrq( TASKSCHDENUMS eTask, TASKARG xArg );
extern	BOOL  TaskManager_FlushEvents( TASKSCHDENUMS eTask );
extern	BOOL  TaskManager_StartTimer( TASKSCHDENUMS eTask, U32 uTime );
extern	BOOL  TaskManager_StopTimer( TASKSCHDENUMS eTask );
extern  BOOL  TaskManager_CheckTasksPending( TASKSCHDENUMS eTask );
#if ( TASK_TICK_ENABLE == 1 )
extern  BOOL  TaskManager_TickEnableDisable( TASKTICKENUMS eTask, BOOL bState );
#endif  // TASK_TICK_ENABLE

/**@} EOF TaskManager.h */

#endif  // _TASKMANAGER_H