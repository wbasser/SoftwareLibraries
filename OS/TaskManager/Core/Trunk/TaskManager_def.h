/******************************************************************************
 * @file TaskManager_def.h
 *
 * @brief definitions for the task managers
 *
 * This file defines the macros, structures and parameters for the task manager
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
#ifndef _TASKMANAGER_DEF_H
#define _TASKMANAGER_DEF_H

// local includes -------------------------------------------------------------
#include "TaskManager/TaskManager_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the scheduled task creation macro
#define	TASKSCHD( type, taskhandler, numevents, bufname, executionrate, enabled, runoninit )	\
  { .eType = type, \
    .pvExec = ( PVEXECFUNC )taskhandler, \
    .xNumEvents = numevents, \
    .pxEvents = ( PTASKARG )&at ## bufname ## Events, \
    .uDelayTime = executionrate, \
    .bEnabled = enabled, \
    .bRunOnInit = runoninit \
  }
                
/// define the tick task creation macro
#define	TASKTICK( taskhandler, executionrate, enabled ) \
  { .pvExec = ( PVEXECFUNC )taskhandler, \
    .uDelayTime = executionrate, \
    .bEnabled = enabled, \
  }

/// define the task event queue 
#define TASKEVENTSTORAGE( bufname, size ) \
  static TASKARG at ## bufname ## Events[ size ]
  
/// define the time macros
#define TASK_TIME_USECS( a )  _UL_( a )
#define TASK_TIME_MSECS( a )  _UL_( a * 1000 )
#define TASK_TIME_SECS( a )   _UL_( a * 1000 * 1000 )
#define TASK_TIME_MINS( a )   _UL_( a * 1000 * 1000 * 60 )

/// determine the event argument size
#if ( TASK_TSKARG_SIZE_BYTES == 1 )
	typedef U8		                TASKARG;
	typedef	PU8		                PTASKARG;
  #define PGM_RDTASKARG         PGM_RDBYTE
  #define	TASK_TIMEOUT_EVENT	  0xFF
  #define TASK_INITIALIZE_EVENT 0xFE
#elif ( TASK_TSKARG_SIZE_BYTES == 2 )
	typedef U16		                TASKARG;
	typedef PU16	                PTASKARG;
  #define PGM_RDTASKARG         PGM_RDWORD
  #define	TASK_TIMEOUT_EVENT	  0xFFFF
  #define TASK_INITIALIZE_EVENT 0xFFFE
#elif ( TASK_TSKARG_SIZE_BYTES == 4 )
	typedef U32		                TASKARG;
	typedef	PU32	                PTASKARG;
  #define PGM_RDTASKARG         PGM_RDDWRD
  #define	TASK_TIMEOUT_EVENT	  0xFFFFFFFF
  #define TASK_INITIALIZE_EVENT 0xFFFFFFFE
#else
  #error You must select a task argument size int TaskManager_prm.h
#endif

#if ( TASK_ENABLE_LARGE_EVENT_QUEUES == 1 )
  typedef U16                   QUESIZEARG;
  #define GETQUEUESIZE(a)       PGM_RDWORD(a)
#else
  typedef U8                    QUESIZEARG;
  #define GETQUEUESIZE(a)       PGM_RDBYTE(a)
#endif

/// define the function pointer format for the different structures
typedef BOOL  ( *PVEXECFUNC )( TASKARG arg );

// enumerations ---------------------------------------------------------------
/// enumerate the task type
typedef enum	_TASKTYPE
{
	TASK_TYPE_EVENT = 0,
	TASK_TYPE_TIMED_ONESHOT,
	TASK_TYPE_TIMED_CONTINUOUS,
} TASKTYPE;

// structures -----------------------------------------------------------------
/// define the scheduled task definition structure
typedef struct 	_TASKSCHDDEF
{
	TASKTYPE	  eType;        ///< task type
	PVEXECFUNC  pvExec;	      ///< pointer to the task handler
	QUESIZEARG	xNumEvents;	  ///< number of events
	PTASKARG	  pxEvents;	    ///< pointer to the event buffer
	U32		      uDelayTime;   ///< delay time in microseconds
  BOOL        bEnabled;     ///< task enabled
  BOOL        bRunOnInit;   ///< run on initialization
} TASKSCHDDEF, *PTASKSCHDDEF;
#define	TASKSCHDDEF_SIZE	sizeof( TASKSCHDDEF )

/// define the task tick task definition structure
typedef struct	_TSKTICKDEF
{
	PVEXECFUNC  pvExec;	      ///< pointer to the task handler
	U32		      uDelayTime;		///< delay time in microseconds
  BOOL        bEnabled;     ///< task enabled
} TASKTICKDEF, *PTASKTICKDEF;
#define	TASKTICKDEF_SIZE	sizeof( TASKTICKDEF )

/**@} EOF TaskManager_def.h */

#endif  // _TASKMANAGER_DEF_H