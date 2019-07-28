/******************************************************************************
 * @file QueueManager_def
 *
 * @brief queue manager control/definitions structures definitions
 *
 * This file provides the structure and macros that allow the creation of a
 * queue.
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
 * \addtogroup QueueManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _QUEUEMANAGER_DEF_H
#define _QUEUEMANAGER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "QueueManager/QueueManager_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro to create the queue storage buffer
#define QUEUE_BUFFER( name, entry_size, num_entries ) \
  static U8 an ## name ## Queue[ entry_size * num_entries ] 

/// define the macro to create a queue entry
#define QUEUE_ENTRY( task, entry_size, num_entries, name, empflg, putflg, getflg, fullflg ) \
  { .eTaskEnum = task, \
    .xEntrySize = entry_size, \
    .xNumEntries = num_entries, \
    .pnQueue = ( PU8 )&an ## name ## Queue, \
    .tEventFlags.tBits = \
    { .bEmpty = empflg, \
      .bPut = putflg, \
      .bGet = getflg, \
      .bFull = fullflg, \
    }, \
  }  

/// define the queue entry type
#if ( QUEUEMAMAGER_ENABLE_LARGE_ENTRY_SIZE )
typedef U16   XQUEUEENTRYSIZE;
#else
typedef U8    XQUEUEENTRYSIZE;
#endif

/// define the queue number of entries type
#if ( QUEUEMANAGER_ENABLE_LARGE_NUMBER_ENTRIES )
typedef U16   XQUEUENUMENTRIES;
#else
typedef U8    XQUEUENUMENTRIES;
#endif

/// define the queue event shift value
#if ( TASK_TSKARG_SIZE_BYTES == 1 )
#define QUEUE_EVENT_SHIFT               ( 2 )
#elif ( TASK_TSKARG_SIZE_BYTES == 2 )
#define QUEUE_EVENT_SHIFT               ( 8 )
#elif ( TASK_TSKARG_SIZE_BYTES == 4 )
#define QUEUE_EVENT_SHIFT               ( 16 )
#endif

/// define a macro to generate a QUEUEGETEMPTY event
#define QUEUEGETEMPTY_EVENT( queueenum ) \
  (( QUEUE_EVENT_GETEMPTY << QUEUE_EVENT_SHIFT ) | queueenum )

/// define a macro to generate a QUEUEGET event
#define QUEUEGET_EVENT( queueenum ) \
  (( QUEUE_EVENT_GET << QUEUE_EVENT_SHIFT ) | queueenum )

/// define a macro to generate a QUEUEPUT event
#define QUEUEPUT_EVENT( queueenum ) \
  (( QUEUE_EVENT_PUT << QUEUE_EVENT_SHIFT ) | queueenum )

/// define a macro to generate a QUEUEPUTFULL event
#define QUEUEPUTFULL_EVENT( queueenum ) \
  (( QUEUE_EVENT_PUTFULL << QUEUE_EVENT_SHIFT ) | queueenum )

// enumerations ---------------------------------------------------------------
/// define the queue event flag structure
typedef union	_QUEUEEVNFLAGS
{
  struct  
  {
    BOOL	bEmpty  : 1;	///< empty event
    BOOL	bPut		: 1;	///< put event
    BOOL	bGet		: 1;	///< get event
    BOOL	bFull		: 1;	///< full event
  } tBits;
  U8	nByte;
} QUEUEEVNFLAGS;

/// enumerate the queue events
typedef enum _QUEUEEVENTS
{
  QUEUE_EVENT_GETEMPTY = 1, // get  - queue is now empty event
  QUEUE_EVENT_GET,          // get event
  QUEUE_EVENT_PUT,          // put event
  QUEUE_EVENT_PUTFULL,      // put - queue is now full event
} QUEUEEVENTS;

// structures -----------------------------------------------------------------
/// define the Queue definition structure
typedef struct 	_QUEUEDEF
{
  TASKSCHDENUMS	    eTaskEnum;		///< task number for events
  XQUEUENUMENTRIES  xNumEntries;	///< number of entries in queue
  XQUEUEENTRYSIZE   xEntrySize;		///< size of each entry
  QUEUEEVNFLAGS     tEventFlags;	///< event flags
  PU8			          pnQueue;		  ///< pointer to the buffer
} QUEUEDEF, *PQUEUEDEF;
#define QUEUEDEF_SIZE sizeof( QUEUEDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF QueueManager_def.h */

#endif  // _QUEUEMANAGER_DEF_H