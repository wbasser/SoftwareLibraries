/******************************************************************************
 * @file StateExecutionEngine.c
 *
 * @brief state execution engine implementation
 *
 * This file implements a finite state machine execution engine
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

// local includes -------------------------------------------------------------
#include "StateExecutionEngine/StateExecutionEngine.h"

// local function prototypes --------------------------------------------------
static  void            ChangeStates( PSTATEEXECENGCONTROL ptControl, U8 nNewState );
static  STATEEXECENGARG GetEventFromTable( STATEEXECENGEVENT const* ptEvents, U8 nEventIndex );

/******************************************************************************
 * @function StateExecutionEngine_Initialize
 *
 * @brief state execution engine initialization
 *
 * This function initializes the control structure for a given state engine
 * instantiation
 *
 * @param[in]   ptControl   pointer to the control structure
 *
 *****************************************************************************/
void StateExecutionEngine_Initialize( PSTATEEXECENGCONTROL ptControl, U8 nDefaultState )
{
  PVSTATEEXECENGENTFUNC pvEntryFunc;
  
  // clear the last state/event
  ptControl->nLastState = STATEEXECENG_STATE_NONE;
  ptControl->xLastEvent = 0;
  ptControl->nCurState = nDefaultState;

  // get the entry function
  pvEntryFunc = ( PVSTATEEXECENGENTFUNC )PGM_RDWORD( ptControl->ptStates[ ptControl->nCurState ].tFuncs.pvEntryFunc );
  if ( pvEntryFunc != NULL )
  {
    // execute it
    pvEntryFunc( );
  }
}

/******************************************************************************
 * @function StateExecutionEngine_Process
 *
 * @brief state execution engine process an event
 *
 * This function processes an event and based on given information, changes 
 * states appropriatealy
 *
 * @param[in]   ptControl   pointer to the control structure
 * @param[in]   xEvent      event to process
 *
 *****************************************************************************/
void StateExecutionEngine_Process( PSTATEEXECENGCONTROL ptControl, STATEEXECENGARG xEvent )
{
  BOOL                      bStateChangeReq;
  U8                        nEventIndex, nNextState;
  STATEEXECENGARG           xLocalEvent;
  PVSTATEEXECENGEXCFUNC		  pvExecFunc;
  STATEEXECENGEVENT const*  ptEvents;
  STATEEXECENGTABLE const*  ptStates;
  
  // get the pointer to the current state table
  ptStates = &ptControl->ptStates[ ptControl->nCurState ];
  
  // get the pointer to the current event table
  ptEvents = ( PSTATEEXECENGEVENT )PGM_RDWORD( ptStates->ptEventTable );
  
  // set the last event
  ptControl->xLastEvent = xEvent;
  ptControl->bExecExit = TRUE;

  // clear the state change flag
  bStateChangeReq = FALSE;

  // do we have an event table
  if ( ptEvents != NULL )
  {
    // search for the event
    nEventIndex = 0;
    
    // while we haven't found a matching entry
    while(( xLocalEvent = GetEventFromTable( ptEvents, nEventIndex )) != 0 )
    {
      // event match
      if ( xLocalEvent == xEvent )
      {
        // set the found flag
        bStateChangeReq = TRUE;

        // set the next state/exit flag
        nNextState = PGM_RDBYTE( ptEvents[ nEventIndex ].nNextState );
        ptControl->bExecExit = PGM_RDBYTE( ptEvents[ nEventIndex ].bExecuteExit );

        // break out of loop
        break;
      }

      // increment the index/get the local event
      nEventIndex++;
    }
  }
  
  // did we find an event
  if ( bStateChangeReq == FALSE ) 
  {
    // is there an execute function
    if (( pvExecFunc = ( PVSTATEEXECENGEXCFUNC )PGM_RDWORD( ptStates->tFuncs.pvExecuteFunc )) != NULL )
    {
      // execute it/check for change state
      if (( nNextState = pvExecFunc( xEvent )) != STATEEXECENG_STATE_NONE )
      {
        // set the change state flag
        bStateChangeReq = TRUE;
      }
    }
  }

  // was a change state requested
  if ( bStateChangeReq == TRUE )
  {
    // change states/update the pointer to the state table
    ChangeStates( ptControl, nNextState );
  }
}

/******************************************************************************
 * @function ChangeStates
 *
 * @brief change states
 *
 * This function will execute an exit function if appropriate, change the state
 * execute the entry function 
 *
 * @param[in]   ptControl   pointer to the control structure
 * @param[in]   nNewState   new state
 *
 *****************************************************************************/
static void ChangeStates( PSTATEEXECENGCONTROL ptControl, U8 nNewState )
{
  PVSTATEEXECENGENTFUNC   pvEntryFunc;
  PVSTATEEXECENGEXTFUNC   pvExitFunc;
  
  // set the next state
  ptControl->nNextState = nNewState;
  
  // check for an exit function/execute if not null
  if (( pvExitFunc = ( PVSTATEEXECENGEXTFUNC )PGM_RDWORD( ptControl->ptStates[ ptControl->nCurState ].tFuncs.pvExitFunc )) != NULL )
  {
    // check flag
    if ( ptControl->bExecExit )
    {
      // execute it
      pvExitFunc( );
//      pvExitFunc( ptControl->pvArg );
    }
  }

  // update the state information in the control structure
  ptControl->nLastState = ptControl->nCurState;
  ptControl->nCurState = ptControl->nNextState;
  ptControl->nNextState = STATEEXECENG_STATE_NONE;
  
  // now execute a entry function if not null
  if (( pvEntryFunc = ( PVSTATEEXECENGENTFUNC )PGM_RDWORD( ptControl->ptStates[ ptControl->nCurState ].tFuncs.pvEntryFunc )) != NULL )
  {
    // execute it
    pvEntryFunc( );
    // pvEntryFunc( ptControl->pvArg );
  }
}

/******************************************************************************
 * @function GetEventFromTable
 *
 * @brief gets an event from the table
 *
 * This function will get an event from the table
 *
 * @param[in]   ptEvents    pointer to the event table
 * @param[in]   nEventIndex table index
 *
 * @return      event at index
 *
 *****************************************************************************/
static STATEEXECENGARG GetEventFromTable( STATEEXECENGEVENT const* ptEvents, U8 nEventIndex )
{
  STATEEXECENGARG xEvent;

  #if ( STATEEXECENG_ARG_SIZE_BYTES == 1 )
    xEvent = PGM_RDBYTE( ptEvents[ nEventIndex ].xEvent );
  #elif ( STATEEXECENG_ARG_SIZE_BYTES == 2 )
    xEvent = PGM_RDWORD( ptEvents[ nEventIndex ].xEvent );
  #elif ( STATEEXECENG_ARG_SIZE_BYTES == 4 )
    xEvent = PGM_RDDWRD( ptEvents[ nEventIndex ].xEvent );
  #endif  // STATEEXECENG_ARG_SIZE_BYTES

  // return the event
  return( xEvent );
}

/**@} EOF StateExecutionEngine.c */
