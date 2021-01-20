/******************************************************************************
 * @file TaskMinimal.h
 *
 * @brief Minimal task scheduler 
 *
 * This file provides the declarations for the task handler
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
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
 * \addtogroup TaskMinimal
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TASKMINIMAL_H
#define _TASKMINIMAL_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TaskMinimal/TaskMinimal_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _TASKMINIMALERR
{
  TASKMINIMAL_ERR_NONE = 0,         ///< no error
  TASKMINIMAL_ERR_ILLTASKHANDLE,    ///< illegal task handle
  TASKMINIMAL_ERR_EVENTQUEUEFULL,   ///< event queue full
} TASKMINIMALERR;

// structures -----------------------------------------------------------------
/// define the task enumerator type
typedef PVOID   PTASKMINIMALHANDLE;

/// determine the event argument size
#if ( TASKMINIMAL_EVENT_SIZE_BYTES == 1 )
	typedef U8		                          TASKMINIMALEVENT;
	typedef	PU8		                          PTASKMINIMALEVENT;
  #define	TASKMINIMAL_TIMEOUT_EVENT	    0xFF
#elif ( TASKMINIMAL_EVENT_SIZE_BYTES == 2 )
	typedef U16		                          TASKMINIMALEVENT;
	typedef PU16	                          PTASKMINIMALEVENT;
  #define	TASKMINIMAL_TIMEOUT_EVENT	    0xFFFF
#elif ( TASKMINIMAL_EVENT_SIZE_BYTES == 4 )
	typedef U32		                          TASKMINIMALEVENT;
	typedef	PU32	                          PTASKMINIMALEVENT;
  #define	TASKMINIMAL_TIMEOUT_EVENT	    0xFFFFFFFF
#else
  #error You must select a task argument size int TaskManager_prm.h
#endif

/// define the task callback
typedef BOOL  ( *PVCALLBACK )( TASKMINIMALEVENT xEvent );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void                TaskMinimal_Initialize( void );
extern  PTASKMINIMALHANDLE  TaskMinimal_Create( PVCALLBACK pvHandler, U16 nExecRateMsecs, U8 nNumEvents, BOOL bInitialOn );
extern  TASKMINIMALERR      TaskMinimal_PostEvent( PTASKMINIMALHANDLE pvHandle, TASKMINIMALEVENT xEvent );
extern  TASKMINIMALERR      TaskMinimal_EnableDisable( PTASKMINIMALHANDLE pvHandle, BOOL bState );
extern  TASKMINIMALERR      TaskMinimal_SetExecutionRate( PTASKMINIMALHANDLE pvHandle, U16 nExecRateMsecs );
extern  void                TaskMinimal_IdleProcess( void );
extern  void                TaskMinimal_TickProcess( U16 wTickTime );

/**@} EOF TaskMinimal.h */

#endif  // _TASKMINIMAL_H