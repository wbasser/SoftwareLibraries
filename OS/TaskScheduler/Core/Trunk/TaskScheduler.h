/******************************************************************************
 * @file TaskScheduler.h
 *
 * @brief task scheduler declarations
 *
 * This file provides the declarations for the task scheduler
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup TaskScheduler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TASKSCHEDULER_H
#define _TASKSCHEDULER_H

// local includes -------------------------------------------------------------
#include "TaskScheduler/TaskScheduler_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macros for defining task execution times
#define TASKSCHEDULER_TIME_MSEC( rate )     ( rate * 1000l )
#define TASKSCHEDULER_TIME_SECS( rate )     (( U32 )( TASKSCHEDULER_TIME_MSEC( rate ) * 1000l ))
#define TASKSCHDDULER_RATE_HZ( rate )       (( 1000000l / rate ))

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _TASKSCHEDULERERR
{
  TASKSCHEDULER_ERR_NONE = 0,         ///< no error
  TASKSCHEDULER_ERR_ILLTASKHANDLE,    ///< illegal task handle
  TASKSCHEDULER_ERR_EVENTQUEUEFULL,   ///< event queue full
} TASKSCHEDULERERR;

/// enumerate the task types
typedef enum _TASKSCHEDULERTYPE
{
  TASKSCHEDULER_TYPE_EVENT = 0,           ///< event task
  TASKSCHEDULER_TYPE_TIMED_ONESHOT,       ///< timed one shot
  TASKSCHEDULER_TYPE_TIMED_CONTINUOUS,    ///< timed continuous 
  TASKSCHEDULER_TYPE_MAX  
} TASKSCHEDULERTYPE;

// structures -----------------------------------------------------------------
/// define the task handle
typedef PVOID   PTASKSCHEDULERHANDLE;

/// determine the event argument size
#if ( TASKSCHEDULER_EVENT_SIZE_BYTES == 1 )
    typedef U8                              TASKSCHEDULEREVENT;
    typedef PU8                             PTASKSCHEDULEREVENT;
  #define TASKSCHEDULER_TIMEOUT_EVENT       ( 0xFF )
  #define TASKSCHEDULER_INITIALIZE_EVENT    ( 0xFE )
#elif ( TASKSCHEDULER_EVENT_SIZE_BYTES == 2 )
    typedef U16                             TASKSCHEDULEREVENT;
    typedef PU16                            PTASKSCHEDULEREVENT;
  #define TASKSCHEDULER_TIMEOUT_EVENT       ( 0XFFFF )
  #define TASKSCHEDULER_INITIALIZE_EVENT    ( 0xFFFE )
#elif ( TASKSCHEDULER_EVENT_SIZE_BYTES == 4 )
    typedef U32                             TASKSCHEDULEREVENT;
    typedef PU32                            PTASKSCHEDULEREVENT;
  #define TASKSCHEDULER_TIMEOUT_EVENT       ( 0XFFFFFFFF )
  #define TASKSCHEDULER_INITIALIZE_EVENT    ( 0xFFFFFFFE )
#else
  #error You must select a task argument size int TaskManager_prm.h
#endif

/// define the queue argument size
#if ( TASKSCHEDULER_ENABLE_LARGE_EVENT_QUEUES == 1 )
  typedef U16                             TASKSCHEDULEQUESIZEARG;
#else
  typedef U8                              TASKSCHEDULEQUESIZEARG;
#endif

/// define the task handler function
typedef BOOL    ( *PTASKSCHEDULERFUNC )( TASKSCHEDULEREVENT );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void                  TaskScheduler_Initialize( void );
extern	void                  TaskScheduler_IdleProcess( void );
extern	void                  TaskScheduler_TickProcess( void );
extern  PTASKSCHEDULERHANDLE  TaskScheduler_Create( TASKSCHEDULERTYPE eType, PTASKSCHEDULERFUNC pvHandler, TASKSCHEDULEQUESIZEARG xNumOfNrmEvents, U8 nNumOfPriEvents, U32 uExecutionRateUsec, U8 nPriority, BOOL bInitialOn, BOOL bRunOnInit );
extern  TASKSCHEDULERERR      TaskScheduler_Delete( PTASKSCHEDULERHANDLE ptTask );
extern  TASKSCHEDULERERR      TaskScheduler_PostEvent( PTASKSCHEDULERHANDLE ptTask, TASKSCHEDULEREVENT xEvent );
extern  TASKSCHEDULERERR      TaskScheduler_PostPriorityEvent( PTASKSCHEDULERHANDLE ptTask, TASKSCHEDULEREVENT xEvent );
extern  TASKSCHEDULERERR      TaskScheduler_DisableEnable( PTASKSCHEDULERHANDLE ptTask, BOOL bState );
extern  TASKSCHEDULERERR      TaskScheduler_FlushEvents( PTASKSCHEDULERHANDLE ptTask );
extern	TASKSCHEDULERERR      TaskScheduler_TimerControl( PTASKSCHEDULERHANDLE ptTask, U32 uTime, BOOL bState );
extern  BOOL                  TaskScheduler_CheckTasksPending( void );

/**@} EOF TaskScheduler.h */

#endif  // _TASKSCHEDULER_H