/******************************************************************************
 * @file StateExecutionEngine.h
 *
 * @brief state execution engine declarations
 *
 * This file provides the declarations of the state execution engine.  This
 * implements a finite state machine engine that processes ta
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
 * \addtogroup StateExecutionEngine
 * @{
 *****************************************************************************/
   
// ensure only one instantiation
#ifndef _STATEEXECUTIONENGINE_H
#define _STATEEXECUTIONENGINE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "StateExecutionEngine/StateExecutionEngine_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the "None" state.
#define	STATEEXECENG_STATE_NONE		( 0xFF)

/// define the helper macro for creating event table entries
#define STATEEXECENGEVENT_ENTRY( event, state, execflag ) \
  { \
    .xEvent = event, \
    .nNextState = state, \
    .bExecuteExit = execflag \
  }

/// define the helper macro for the end event table entry
#define STATEEXECENGEVENT_END( ) \
  { \
    .xEvent = 0, \
    .nNextState = STATEEXECENG_STATE_NONE, \
    .bExecuteExit = FALSE \
  }

/// define the helper macro for creating state table entries
#define STATEXECENGETABLE_ENTRY( state, enter, exec, exit, event ) \
  { \
    .nState = state, \
    .tFuncs = \
    { \
      .pvEntryFunc = ( PVSTATEEXECENGENTFUNC )enter, \
      .pvExecuteFunc = ( PVSTATEEXECENGEXCFUNC )exec, \
      .pvExitFunc = ( PVSTATEEXECENGEXTFUNC )exit \
    }, \
    .ptEventTable = ( PSTATEEXECENGEVENT )event \
  }

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
#if ( STATEEXECENG_ARG_SIZE_BYTES == 1 )
  typedef U8		    STATEEXECENGARG;
  typedef	PU8		    PSTATEEXECENGARG;
#elif ( STATEEXECENG_ARG_SIZE_BYTES == 2 )
  typedef U16		    STATEEXECENGARG;
  typedef PU16	    PSTATEEXECENGARG;
#elif ( STATEEXECENG_ARG_SIZE_BYTES == 4 )
  typedef U32		    STATEEXECENGARG;
  typedef	PU32	    PSTATEEXECENGARG;
#else
  #error You must select a task argument size in StateExecutionEngine_prm.h
#endif  // STATEEXECENG_ARG_SIZE_BYTES

/// Function pointer type definitions for entry, exec, and exit functions
typedef void ( *PVSTATEEXECENGENTFUNC )( void );
typedef U8   ( *PVSTATEEXECENGEXCFUNC )( STATEEXECENGARG );
typedef void ( *PVSTATEEXECENGEXTFUNC )( void );

/// define the event structure
typedef struct _STATEEXECENGEVENT
{
  STATEEXECENGARG xEvent;             ///< event argument
  U8              nNextState;         ///< next state
  BOOL            bExecuteExit;       ///< execute exit function flag
} STATEEXECENGEVENT, *PSTATEEXECENGEVENT;
#define STATEEXECENGEVENT_SIZE    sizeof( STATEEXECENGEVENT )

/// define the state table structure
typedef struct _STATEEXECENGTABLE
{
  U8                        nState;         ///< state name
  struct
  {
    PVSTATEEXECENGENTFUNC   pvEntryFunc;    ///< entry function
    PVSTATEEXECENGEXCFUNC   pvExecuteFunc;  ///< execute function
    PVSTATEEXECENGEXTFUNC   pvExitFunc;     ///< exit function
  } tFuncs;
  STATEEXECENGEVENT const*  ptEventTable;   ///< pointer to the event table
} STATEEXECENGTABLE, *PSTATEEXECENGTABLE;
#define STATEEXECENGTABLE_SIZE    sizeof( STATEEXECENGTABLE )

/// define the control structure
typedef struct _STATEEXECENGCONTROL
{
  U8                        nLastState;   ///< last state
  U8                        nCurState;    ///< current state
  U8                        nNextState;   ///< next state
  STATEEXECENGARG           xLastEvent;   ///< last event
  BOOL                      bExecExit;    ///< execute the exit function for this event
  BOOL                      bFlushEvent;  ///< flush event flag
  STATEEXECENGTABLE const*  ptStates;     ///< pointer to the states for this instance
  //  PVOID                     pvArg;        ///< pointer to an user supplied argument
} STATEEXECENGCONTROL, *PSTATEEXECENGCONTROL;
#define STATEEXECENGCONTROL_SIZE  sizeof( STATEEXECENGCONTROL )

// global function prototypes --------------------------------------------------
extern  void  StateExecutionEngine_Initialize( PSTATEEXECENGCONTROL ptControl, U8 nDefaultState );
extern  void  StateExecutionEngine_Process( PSTATEEXECENGCONTROL ptControl, STATEEXECENGARG xEvent );

/**@} EOF StateExecutionEngine.h */

#endif  // _STATEEXECUTIONENGINE.H