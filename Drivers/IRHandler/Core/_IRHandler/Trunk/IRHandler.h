/******************************************************************************
 * @file IRHandler.h
 *
 * @brief IR Handler
 *
 * This file provides the declarations for the IR handler
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
 * $Log: $
 * 
 *
 * \addtogroup IRHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IRHANDLER_H
#define _IRHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IRhandler/IRhandler_cfg.h"
#include "IRHandler/IRCapture/IRCapture.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the IR events
#if ( TASK_TSKARG_SIZE_BYTES == 1 )
#define IRHANDLER_CAPDONE_EVENT			  ( 0x57 )
#elif ( TASK_TSKARG_SIZE_BYTES == 2 )
#define IRHANDLER_CAPDONE_EVENT			  ( 0x5757 )
#elif ( TASK_TSKARG_SIZE_BYTES == 4 )
#define IRHANDLER_CAPDONE_EVENT			  ( 0x57575757 )
#endif

/// define the maximum number of bits
#define IRHANDLER_NUM_BITS            ( 32 )

/// define the number of task arguments
#define IRHANDLER_NUM_TASK_EVENTS     ( IRHANDLER_NUM_BITS + 2 )

/// define the queue entry size
#define IRHANDLER_QUEUE_ENTRY_SIZE    ( sizeof ( U32UN ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      IRHandler_Initialize( void );
extern  BOOL      IRHandler_ProcessEvent( TASKARG xArg );
extern  U8        IRHandler_GetCaptureCount( void );
extern  U16UN     IRHandler_GetCaptureValue( U8 nIndex );

/**@} EOF IRHandler.h */

#endif  // _IRHANDLER_H