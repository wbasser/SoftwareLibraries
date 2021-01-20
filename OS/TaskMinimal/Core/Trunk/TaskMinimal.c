/******************************************************************************
 * @file TaskMinimal.c
 *
 * @brief minimal task scheduler implementation
 *
 * This file provides the implementation for the minimal task scheduler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TaskMinimal/TaskMinimal.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PTASKCTL )handle )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the task scheduler control structure
typedef struct _TASKCTL
{
  struct _TASKCTL*        ptSignature;      ///< my signature
  struct _TASKCTL*        ptNextTask;       ///< pointer to the next structure
  U16                     wExecRate;        ///< current execute rate
  U16                     wDelayCount;      ///< delay count
  U8                      nNrmRdIdx;        ///< event buffer read index
  U8                      nNrmWrIdx;        ///< event buffer write index
  U8                      nNrmCount;        ///< event count
  U8                      nNrmEvents;       ///< normal event size
  BOOL                    bDelayInProgress; ///< delay in progress
  BOOL                    bEnabled;         ///< task enabled
  BOOL                    bTimeElapsed;     ///< time elapsed
  PVCALLBACK              pvHandler;        ///< pointer to the handler
  PTASKMINIMALEVENT       pxNrmEvents;      ///< pointer to the normal events
} TASKCTL, *PTASKCTL;
#define TASKCTL_SIZE          sizeof( TASKCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PTASKCTL    ptFirstTask;      ///< pointer to the first task control structure

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function TaskMinimal_Initialize
 *
 * @brief initialization
 *
 * This function will reset the first pointer to null
 *
 *****************************************************************************/
void TaskMinimal_Initialize( void )
{
  // set the first to null
  ptFirstTask = NULL;
}

/******************************************************************************
 * @function TaskMinimal_Create
 *
 * @brief create a task
 *
 * This function will create a task
 *
 * @param[in]   pvHandler       pointer ot the handler
 * @param[in]   wExecRateMsecs  execution reate in msecs
 * @param[in]   nNumEvents      number of events
 * @param[in]   bInitialOn      is the task evnable
 *
 * @return      task handle or NULL on error
 *
 *****************************************************************************/
PTASKMINIMALHANDLE TaskMinimal_Create( PVCALLBACK pvHandler, U16 wExecRateMsecs, U8 nNumEvents, BOOL bInitialOn )
{
  PTASKCTL  ptNewTask, ptCurTask, ptLstTask;
  
  // allocate space for new task
  if (( ptNewTask = malloc( TASKCTL_SIZE )) != NULL )
  {
    // now allocate space for the normal event queue
    if (( ptNewTask->pxNrmEvents = ( PTASKMINIMALEVENT )malloc( sizeof( TASKMINIMALEVENT ) * nNumEvents )) != NULL )
    {
      // now initialize the control structure
      ptNewTask->ptSignature      = ptNewTask;
      ptNewTask->ptNextTask       = NULL;
      ptNewTask->wExecRate        = wExecRateMsecs;
      ptNewTask->wDelayCount      = 0;
      ptNewTask->nNrmRdIdx        = 0;
      ptNewTask->nNrmWrIdx        = 0;
      ptNewTask->nNrmCount        = 0;
      ptNewTask->nNrmEvents       = nNumEvents;
      ptNewTask->bDelayInProgress = ( wExecRateMsecs != 0 );
      ptNewTask->bEnabled         = bInitialOn;
      ptNewTask->pvHandler        = pvHandler;
      ptNewTask->bTimeElapsed     = FALSE;
        
      // now insert in list
      if ( ptFirstTask == NULL )
      {
        // add it
        ptFirstTask = ptNewTask;
      }
      else
      {
        // search for the entry point
        for ( ptCurTask = ptFirstTask, ptLstTask = NULL; ( ptCurTask != NULL ); ptLstTask = ptCurTask, ptCurTask = ptCurTask->ptNextTask );

        // check for null
        if ( ptCurTask == NULL )
        {
          // add at end
          ptLstTask->ptNextTask = ptNewTask;
        }
      }
    }
    else
    {
      // error - not enough room
      free( ptNewTask );
      ptNewTask = NULL;
    }
  }
  
  // return the handle
  return(( PTASKMINIMALHANDLE )ptNewTask );
}

/******************************************************************************
 * @function TaskMinimal_PostEvent
 *
 * @brief post an event
 *
 * This function will post an event
 *
 * @param[in]   pvHandle    task handle
 * @param[in]   xEvent      event to post
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKMINIMALERR TaskMinimal_PostEvent( PTASKMINIMALHANDLE pvHandle, TASKMINIMALEVENT xEvent )
{
  TASKMINIMALERR  eError = TASKMINIMAL_ERR_NONE;
  PTASKCTL        ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // check for room
    if ( ptSelTask->nNrmCount < ptSelTask->nNrmEvents )
    {
      // add the event/adjust pointer/increment count
      Interrupt_Disable( );
      *( ptSelTask->pxNrmEvents + ptSelTask->nNrmWrIdx++ ) = xEvent;
      ptSelTask->nNrmWrIdx %= ptSelTask->nNrmEvents;
      ptSelTask->nNrmCount++;
      Interrupt_Enable( );
    }
    else
    {
      // queue - full error
      eError = TASKMINIMAL_ERR_EVENTQUEUEFULL;
    }
  }
  else
  {
    // set the error
    eError = TASKMINIMAL_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskMinimal_EnableDisable
 *
 * @brief enable disable a task
 *
 * This function will enable or disable the given task
 *
 * @param[in]   pvHandle  task handler
 * @param[in]   bSTate    state of the 
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKMINIMALERR TaskMinimal_EnableDisable( PTASKMINIMALHANDLE pvHandle, BOOL bState )
{
  TASKMINIMALERR  eError = TASKMINIMAL_ERR_NONE;
  PTASKCTL        ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // set the state
    ptSelTask->bEnabled = bState;
  }
  else
  {
    // set the error
    eError = TASKMINIMAL_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskMinimal_SetExecutionRate
 *
 * @brief change the execution rate
 *
 * This function will change the execution rate
 *
 * @param[in]   pvHandle  task handler
 * @param[in]   wExecRateMsecs    execution rate in milliseconds
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKMINIMALERR TaskMinimal_SetExecutionRate( PTASKMINIMALHANDLE pvHandle, U16 wExecRateMsecs )
{
  TASKMINIMALERR  eError = TASKMINIMAL_ERR_NONE;
  PTASKCTL        ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // add the event/change time
    Interrupt_Disable( );
    ptSelTask->wExecRate        = wExecRateMsecs;
    ptSelTask->wDelayCount      = 0;
    ptSelTask->bDelayInProgress = ( wExecRateMsecs != 0 );
    Interrupt_Enable( );
  }
  else
  {
    // set the error
    eError = TASKMINIMAL_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskMinimal_IdleProcess
 *
 * @brief idle process
 *
 * This function will scan through the task list and anything is slated to be 
 * executed, it will call the handler
 *
 *****************************************************************************/
void TaskMinimal_IdleProcess( void )
{
  PTASKCTL            ptCurTask;
  BOOL                bEventFound;
  TASKMINIMALEVENT    xEvent;
  
  // for each item in list
  ptCurTask = ptFirstTask;
  while( ptCurTask != NULL )
  {
    // is this task enabled
    if ( ptCurTask->bEnabled )
    {
      // clear the event found flag
      bEventFound = FALSE;
      
      // check to see if time elapsed
      if ( ptCurTask->bTimeElapsed )
      {
        // set the timeout event
        xEvent = TASKMINIMAL_TIMEOUT_EVENT;
        
        // set the event found flag
        bEventFound = TRUE;
      }
      else if ( ptCurTask->nNrmCount != 0 )
      {
        // get a normal event
        xEvent = *( ptCurTask->pxNrmEvents + ptCurTask->nNrmRdIdx );

        // set the event found flag
        bEventFound = TRUE;
      }
      
      // check for event found
      if ( bEventFound )
      {
        // call the event handler
        if ( ptCurTask->pvHandler( xEvent ))
        {
          // event was handled
          Interrupt_Disable( );
          
          // check for an time elapsed event
          if ( ptCurTask->bTimeElapsed )
          {
            // clear the flag
            ptCurTask->bTimeElapsed = FALSE;
          }
          else
          {
            // decrement count/adjust pointers/check for rollover
            ptCurTask->nNrmCount--;
            ptCurTask->nNrmRdIdx++;
            ptCurTask->nNrmRdIdx %= ptCurTask->nNrmEvents;
          }
          
          // re-enable interrupts
          Interrupt_Enable( );
        }
      }
    }
    
    // next task
    ptCurTask = ptCurTask->ptNextTask;
  }
}

/******************************************************************************
 * @function TaskMinimal_TickProcess
 *
 * @brief tick process
 *
 * This function will process the tick
 *
 * @param[in]   wTickTime     tick time
 *
 *****************************************************************************/
void TaskMinimal_TickProcess( U16 wTickTime )
{
  PTASKCTL  ptCurTask;
  
  // for each item in list
  ptCurTask = ptFirstTask;
  while( ptCurTask != NULL )
  {
    // is this task enabled
    if ( ptCurTask->bEnabled )
    {
      // is delay in progress
      if ( ptCurTask->bDelayInProgress )
      {
        // decrement the time
        ptCurTask->wDelayCount += wTickTime;
        if ( ptCurTask->wDelayCount >= ptCurTask->wExecRate )
        {
          // reset the delay count
          ptCurTask->wDelayCount = 0;
          
          // post a timeout event to the priority queue
          ptCurTask->bTimeElapsed = TRUE;
        }
      }
    }
    
    // next task
    ptCurTask = ptCurTask->ptNextTask;
  }
}

/**@} EOF TaskMinimal.c */
