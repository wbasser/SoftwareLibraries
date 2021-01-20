/******************************************************************************
 * @file QueueScheduler.h
 *
 * @brief quueue manager declarations
 *
 * This file declares the API's for the queue manager
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
 * $Rev: $
 * 
 *
 * \addtogroup 
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _QUEUESCHEDULER_H
#define _QUEUESCHEDULER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "QueueScheduler/QueueScheduler_prm.h"

// library includes -----------------------------------------------------------
#include "TaskScheduler/TaskScheduler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the queue errors
typedef enum _QUEUESTATUS
{
  QUEUE_STATUS_NONE = 0,     // no error
  QUEUE_STATUS_ILLQUE,       // illegal queue
  QUEUE_STATUS_QUEFUL,       // queue full
  QUEUE_STATUS_QUEEMP,       // queue empty
} QUEUESTATUS;

/// enumerate the queue events
typedef enum _QUEUEEVENTS
{
  QUEUE_EVENT_GETEMPTY = 1, // get  - queue is now empty event
  QUEUE_EVENT_GET,          // get event
  QUEUE_EVENT_PUT,          // put event
  QUEUE_EVENT_PUTFULL,      // put - queue is now full event
} QUEUEEVENTS;

// structures -----------------------------------------------------------------
/// define the task handle
typedef PVOID   PQUEUESCHEDULERHANDLE;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void									QueueScheduler_Initialize( void );
extern  PQUEUESCHEDULERHANDLE	QueueScheduler_Create( PTASKSCHEDULERHANDLE ptTask, U8 nNumEntries, U16 wEntrySize, BOOL bEmptyEnb, BOOL bPutEnb, BOOL bGetEnb, BOOL bFullEnb );
extern  QUEUESTATUS 					QueueScheduler_Get( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry );
extern  QUEUESTATUS 					QueueScheduler_PutTail( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry );
#if ( QUEUESCHEDULER_INCLUDE_PUTHEAD == TRUE )
  extern  QUEUESTATUS 					QueueScheduler_PutHead( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry );
#endif
#if ( QUEUESCHEDULER_INCLUDE_PEEK == TRUE )
  extern  QUEUESTATUS 					QueueScheduler_Peek( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry );
#endif
#if ( QUEUESCHEDULER_INCLUDE_POP == TRUE )
  extern  QUEUESTATUS 					QueueScheduler_Pop( PQUEUESCHEDULERHANDLE ptQueue );
#endif
#if ( QUEUESCHEDULER_INCLUDE_FLUSH == TRUE )
  extern  QUEUESTATUS 					QueueScheduler_Flush( PQUEUESCHEDULERHANDLE ptQueue );
#endif
#if ( QUEUESCHEDULER_INCLUDE_GETREMAINING == TRUE )
  extern  QUEUESTATUS 					QueueScheduler_GetRemaining( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnRemaining );
#endif
#if ( QUEUESCHEDULER_INCLUDE_GETSTATUS == TRUE )
  extern  QUEUESTATUS 					QueueScheduler_GetStatus( PQUEUESCHEDULERHANDLE ptQueue );  
#endif

/**@} EOF QueueScheduler.h */

#endif  // _QUEUESCHEDULER_H