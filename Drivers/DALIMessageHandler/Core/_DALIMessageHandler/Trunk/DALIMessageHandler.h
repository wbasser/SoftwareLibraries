/****************************************7**************************************
 * @file  DALIMessageHandler.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_H
#define _DALIMESSAGEHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of event
#define	DALIMESSAGE_EVENT_NUM_EVENTS    ( 4 )
#define	DALIMESSAGE_TIMER_NUM_EVENTS    ( 1 )
#define DALIMESSAGE_OUTPUT_NUM_EVENTS   ( 1 )
#define DALIMESSAGE_BUSFAIL_NUM_EVENTS  ( 1 )

/// define the timer execution rate
#define	DALIMESSAGE_TIMER_EXEC_RATE		( TASK_TIME_MINS( 15 ))
#define DALIMESSAGE_OUTPUT_EXEC_RATE  ( TASK_TIME_MSECS( 1 ))
#define DALIMESSAGE_BUSFAIL_EXEC_RATE ( TASK_TIME_MSECS( 5 ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  PDALICTL  g_ptDaliCtl;  ///< pointer to the control structure

// global function prototypes --------------------------------------------------
extern  void      DALIMessageHandler_Initialize( void );
extern	void      DALIMessageHandler_Control( BOOL bState );
extern  BOOL      DALIMessageHandler_ProcessEvent( TASKARG xArg );
extern  BOOL      DALIMessageHandler_ProcessTimer( TASKARG xArg );
extern	BOOL      DALIMessageHandler_ProcessOutput( TASKARG xArg );
extern  BOOL      DALIMessageHandler_ProcessBusFail( TASKARG xArg );
extern  void      DALIMessageHandler_ForceMessage( U8 nAddr, U8 nDataCmd );

/**@} EOF DALIMessageHandler.h */

#endif  // _DALIMESSAGEHANDLER_H
