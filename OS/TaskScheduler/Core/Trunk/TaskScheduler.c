/******************************************************************************
 * @file TaskScheduler.c
 *
 * @brief Task scheduler implementation 
 *
 * This file provides the implementation for the task scheduler
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include  "TaskScheduler/TaskScheduler.h"

// library includes -----------------------------------------------------------
#include  "Interrupt/Interrupt.h"
#include  "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PTASKCTL )handle )

/// define the minimum priority events
#define MIN_PRIORITY_EVENTS               ( 2 )

// structures -----------------------------------------------------------------
/// define the task scheduler control structure
typedef struct _TASKCTL
{
  struct _TASKCTL*        ptSignature;      ///< my signature
  struct _TASKCTL*        ptNextTask;       ///< pointer to the next structure
  U32                     uDelayTime;       ///< current delay time
  U32                     uDelayCount;      ///< delay count
  TASKSCHEDULEQUESIZEARG  xNrmRdIdx;        ///< event buffer read index
  TASKSCHEDULEQUESIZEARG  xNrmWrIdx;        ///< event buffer write index
  TASKSCHEDULEQUESIZEARG  xNrmCount;        ///< event count
  TASKSCHEDULEQUESIZEARG  xNrmMaxEvents;    ///< normal event size
  U8                      nPriRdIdx;        ///< priority event buffer read index
  U8                      nPriWrIdx;        ///< priority event buffer write index
  U8                      nPriCount;        ///< priority event count
  U8                      nPriMaxEvents;    ///< priority event size
  U8                      nPriority;        ///< task priority
  BOOL                    bDelayInProgress; ///< delay in progress
  BOOL                    bEnabled;         ///< task enabled
  TASKSCHEDULERTYPE       eType;            ///< task type
  PTASKSCHEDULERFUNC      pvHandler;        ///< pointer to the handler
  PTASKSCHEDULEREVENT     pxNrmEvents;      ///< pointer to the normal events
  PTASKSCHEDULEREVENT     pxPriEvents;      ///< pointer to the priority events
} TASKCTL, *PTASKCTL;
#define TASKCTL_SIZE          sizeof( TASKCTL )

// local parameter declarations -----------------------------------------------
static  PTASKCTL    ptFirstTask;      ///< pointer to the first task control structure
static  U32         uSystemTickRate;  ///< execution rate of the tick handler

// local function prototypes --------------------------------------------------
static  BOOL  PutPriorityEvent( PTASKCTL ptTask, TASKSCHEDULEREVENT xEvent );

/******************************************************************************
 * @function TaskScheduler_Initialize
 *
 * @brief initialization
 *
 * This function will reset the first pointer to null
 *
 *****************************************************************************/
void TaskScheduler_Initialize( void )
{
  // set the first to null
  ptFirstTask = NULL;

  // set the tick rate
  uSystemTickRate = SystemTick_GetTickRateUsec( );
}

/******************************************************************************
 * @function TaskScheduler_IdleProcess
 *
 * @brief idle process
 *
 * This function will check each task for active and events scheduled, and call
 * the event handler
 *
 *****************************************************************************/
void TaskScheduler_IdleProcess( void )
{
  PTASKCTL            ptCurTask;
  BOOL                bPriorityEvent = FALSE;
  BOOL                bEventFound = FALSE;
  TASKSCHEDULEREVENT  xEvent;
  
  // for each item in list
  ptCurTask = ptFirstTask;
  while( ptCurTask != NULL )
  {
    // is this task enabled
    if ( ptCurTask->bEnabled )
    {
      // check for priority event
      if ( ptCurTask->nPriCount != 0 )
      {
        // get the event/set priority event flag
        xEvent = *( ptCurTask->pxPriEvents + ptCurTask->nPriRdIdx );
        bPriorityEvent = TRUE;
        bEventFound = TRUE;
      }
      else if ( ptCurTask->xNrmCount != 0 )
      {
        // get a normal event
        xEvent = *( ptCurTask->pxNrmEvents + ptCurTask->xNrmRdIdx );
        bEventFound = TRUE;
      }
      else
      {
        // clear flags
        bPriorityEvent = bEventFound = FALSE;
      }
      
      // now check for event execution
      if ( bEventFound )
      {
        // call the event handler
        if ( ptCurTask->pvHandler( xEvent ))
        {
          // event was handled
          Interrupt_Disable( );
          
          // check for type of event
          if ( bPriorityEvent )
          {
            // decrement count/adjust pointers/check for rollover
            ptCurTask->nPriCount--;
            ptCurTask->nPriRdIdx++;
            ptCurTask->nPriRdIdx %= ptCurTask->nPriMaxEvents;
          }
          else
          {
            // decrement count/adjust pointers/check for rollover
            ptCurTask->xNrmCount--;
            ptCurTask->xNrmRdIdx++;
            ptCurTask->xNrmRdIdx %= ptCurTask->xNrmMaxEvents;
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
 * @function TaskScheduler_TickProcess
 *
 * @brief tick process
 *
 * This function will check each task for timer active, decrement the time and
 * if time has expired, post a priority event
 *
 *****************************************************************************/
void TaskScheduler_TickProcess( void )
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
        if ( --ptCurTask->uDelayCount == 0 )
        {
          // post a timeout event to the priority queue
          PutPriorityEvent( ptCurTask, TASKSCHEDULER_TIMEOUT_EVENT );
          
          // adjust the time
          if ( ptCurTask->eType == TASKSCHEDULER_TYPE_TIMED_CONTINUOUS )
          {
            // reset the time
            ptCurTask->uDelayCount = ptCurTask->uDelayTime;
          }
          else
          {
            // clear the delay in progress
            ptCurTask->bDelayInProgress = FALSE;
          }
        }
      }
    }
    
    // next task
    ptCurTask = ptCurTask->ptNextTask;
  }
}

/******************************************************************************
 * @function TaskScheduler_Create
 *
 * @brief create a task
 *
 * This function will create a task by allocating memory for both the task control
 * structure, the normal and high priority queues and return the handle to the
 * newly crated task
 *
 * @param[in]   eType               task type
 * @param[in]   pvHandler           task handler
 * @param[in]   xNumOfNrmEvents     number of normal events
 * @param[in]   nNumOfPriEvents     number of priority events
 * @param[in]   uExecutionRateUsec  execution rate if a timed task in microseconds
 * @param[in]   nPriority           task priority 
 * @param[in]   bInitialOn          enable task on initialization
 * @param[in]   bRunOnInit          run the task when created
 *
 * @return      handle to the newly created task or NULL if failure
 *
 *****************************************************************************/
PTASKSCHEDULERHANDLE TaskScheduler_Create( TASKSCHEDULERTYPE eType, PTASKSCHEDULERFUNC pvHandler, TASKSCHEDULEQUESIZEARG xNumOfNrmEvents, U8 nNumOfPriEvents, U32 uExecutionRateUsec, U8 nPriority, BOOL bInitialOn, BOOL bRunOnInit )
{
  PTASKCTL  ptNewTask, ptCurTask, ptLstTask;
  
  // allocate space for new task
  if (( ptNewTask = malloc( TASKCTL_SIZE )) != NULL )
  {
    // now allocate space for the normal event queue
    if (( ptNewTask->pxNrmEvents = ( PTASKSCHEDULEREVENT )malloc( sizeof( TASKSCHEDULEREVENT ) * xNumOfNrmEvents )) != NULL )
    {
      // now allocate space for the priority events
      if (( ptNewTask->pxPriEvents = ( PTASKSCHEDULEREVENT )malloc(( sizeof( TASKSCHEDULEREVENT ) * ( MIN_PRIORITY_EVENTS + nNumOfPriEvents )))) != NULL )
      {
        // now initialize the control structure
        ptNewTask->ptSignature      = ptNewTask;
        ptNewTask->ptNextTask       = NULL;
        ptNewTask->uDelayTime       = uExecutionRateUsec / uSystemTickRate;
        ptNewTask->uDelayCount      = ptNewTask->uDelayTime;
        ptNewTask->xNrmRdIdx        = 0;
        ptNewTask->xNrmWrIdx        = 0;
        ptNewTask->xNrmCount        = 0;
        ptNewTask->xNrmMaxEvents    = xNumOfNrmEvents;
        ptNewTask->nPriRdIdx        = 0;
        ptNewTask->nPriWrIdx        = 0;
        ptNewTask->nPriCount        = 0;
        ptNewTask->nPriMaxEvents    = MIN_PRIORITY_EVENTS + nNumOfPriEvents;
        ptNewTask->nPriority        = nPriority;
        ptNewTask->bDelayInProgress = ( eType == TASKSCHEDULER_TYPE_EVENT ) ? FALSE : TRUE;
        ptNewTask->bEnabled         = bInitialOn;
        ptNewTask->pvHandler        = pvHandler;
        ptNewTask->eType            = eType;
        
        // check for run on init
        if ( bRunOnInit )
        {
          // shove an initialize event on the priority queue
          PutPriorityEvent( ptNewTask, TASKSCHEDULER_INITIALIZE_EVENT );
        }
        
        // now insert in list
        if ( ptFirstTask == NULL )
        {
          // add it
          ptFirstTask = ptNewTask;
        }
        else
        {
          // search for the entry point
          for ( ptCurTask = ptFirstTask, ptLstTask = NULL; ( ptCurTask != NULL ) && ( ptCurTask->nPriority < nPriority ); ptLstTask = ptCurTask, ptCurTask = ptCurTask->ptNextTask );

          // check for null
          if ( ptCurTask == NULL )
          {
            // add at end
            ptLstTask->ptNextTask = ptNewTask;
          }
          else if ( ptCurTask == ptFirstTask )
          {
            // add in front
            ptFirstTask = ptNewTask;
            ptNewTask->ptNextTask = ptCurTask;
          }
          else
          {
            // add in middle
            ptLstTask->ptNextTask = ptNewTask;
            ptNewTask->ptNextTask = ptCurTask;
          }
        }
      }
      else
      {
        // error not enough room
        free( ptNewTask->pxNrmEvents );
        free( ptNewTask );
        ptNewTask = NULL;
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
  return(( PTASKSCHEDULERHANDLE )ptNewTask );
}

/******************************************************************************
 * @function TaskScheduler_Delete
 *
 * @brief delete a task from the list
 *
 * This function will delete and free the memory for a given task
 *
 * @param[in]   ptTask    task handle
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKSCHEDULERERR TaskScheduler_Delete( PTASKSCHEDULERHANDLE ptTask )
{
  TASKSCHEDULERERR  eError;
  PTASKCTL          ptCurTask, ptLstTask, ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( ptTask );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // search for the task
    for ( ptCurTask = ptFirstTask, ptLstTask = NULL; ( ptCurTask->ptNextTask != NULL ) && ( ptCurTask != ptSelTask ); ptLstTask = ptCurTask, ptCurTask = ptCurTask->ptNextTask )

    // check for event found
    if ( ptCurTask != NULL )
    {
        // check for root
        if ( ptCurTask == ptFirstTask )
        {
            // first - set first to next
            ptFirstTask = ptCurTask->ptNextTask;
        }
        else if ( ptCurTask->ptNextTask == NULL )
        {
            // last, clear the previous next
            ptLstTask->ptNextTask = NULL;
        }
        else
        {
            // middle - set last next to next
            ptLstTask->ptNextTask = ptCurTask->ptNextTask;
        }

        // free the event buffer/event
        free( ptCurTask->pxPriEvents );
        free( ptCurTask->pxNrmEvents );
        free( ptCurTask );
    }
  }
  else
  {
    // set the error
    eError = TASKSCHEDULER_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskScheduler_PostEvent
 *
 * @brief post an event to the task
 *
 * This function will check for space on the queue and will post the event
 * to the event queue
 *
 * @param[in]   ptTask    task handle
 * @param[in]   xEvent    event to post
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKSCHEDULERERR TaskScheduler_PostEvent( PTASKSCHEDULERHANDLE ptTask, TASKSCHEDULEREVENT xEvent )
{
  TASKSCHEDULERERR  eError = TASKSCHEDULER_ERR_NONE;
  PTASKCTL          ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( ptTask );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // check for room
    if ( ptSelTask->xNrmCount < ptSelTask->xNrmMaxEvents )
    {
      // add the event/adjust pointer/increment count
      Interrupt_Disable( );
      *( ptSelTask->pxNrmEvents + ptSelTask->xNrmWrIdx++ ) = xEvent;
      ptSelTask->xNrmWrIdx %= ptSelTask->xNrmMaxEvents;
      ptSelTask->xNrmCount++;
      Interrupt_Enable( );
    }
    else
    {
      // queue - full error
      eError = TASKSCHEDULER_ERR_EVENTQUEUEFULL;
    }
  }
  else
  {
    // set the error
    eError = TASKSCHEDULER_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskScheduler_PostPriorityEvent
 *
 * @brief post a priority event
 *
 * This function will check for space on the queue and will post the event
 * to the event queue
 *
 * @param[in]   ptTask    task handle
 * @param[in]   xEvent    event to post
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKSCHEDULERERR TaskScheduler_PostPriorityEvent( PTASKSCHEDULERHANDLE ptTask, TASKSCHEDULEREVENT xEvent )
{
  TASKSCHEDULERERR  eError = TASKSCHEDULER_ERR_NONE;
  PTASKCTL          ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( ptTask );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // add a priority event
    Interrupt_Disable( );
    eError = ( PutPriorityEvent( ptTask, xEvent )) ? TASKSCHEDULER_ERR_NONE : TASKSCHEDULER_ERR_EVENTQUEUEFULL;
    Interrupt_Enable( );
  }
  else
  {
    // set the error
    eError = TASKSCHEDULER_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskScheduler_DisableEnable
 *
 * @brief task enable/disable
 *
 * This function will enable disable the task
 *
 * @param[in]   ptTask    task handle
 * @param[in]   bState    desired task state
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKSCHEDULERERR TaskScheduler_DisableEnable( PTASKSCHEDULERHANDLE ptTask, BOOL bState )
{
  TASKSCHEDULERERR  eError = TASKSCHEDULER_ERR_NONE;
  PTASKCTL          ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( ptTask );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // now set the task state
    ptSelTask->bEnabled = bState;
  }
  else
  {
    // set the error
    eError = TASKSCHEDULER_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskScheduler_FlushEvents
 *
 * @brief flush events
 *
 * This function will empty all event queues for the given task
 *
 * @param[in]   ptTask    task handle
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKSCHEDULERERR TaskScheduler_FlushEvents( PTASKSCHEDULERHANDLE ptTask )
{
  TASKSCHEDULERERR  eError = TASKSCHEDULER_ERR_NONE;
  PTASKCTL          ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( ptTask );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // disable interrupts
    Interrupt_Disable( );
    
    // clear all queues
    ptSelTask->xNrmCount = 0;
    ptSelTask->xNrmRdIdx = 0;
    ptSelTask->xNrmWrIdx = 0;
    ptSelTask->nPriCount = 0;
    ptSelTask->nPriRdIdx = 0;
    ptSelTask->nPriWrIdx = 0;
    
    // reenable interrupts
    Interrupt_Enable( );
  }
  else
  {
    // set the error
    eError = TASKSCHEDULER_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskScheduler_TimerControl
 *
 * @brief timer control
 *
 * This function will start/stop a timer, if a time value is given it will
 * be applied
 *
 * @param[in]   ptTask    task handle
 * @param[in]   uTime     desired time( if 0, use existing time )
 * @param[in]   bState    timer state 
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TASKSCHEDULERERR TaskScheduler_TimerControl( PTASKSCHEDULERHANDLE ptTask, U32 uTime, BOOL bState )
{
  TASKSCHEDULERERR  eError = TASKSCHEDULER_ERR_NONE;
  PTASKCTL          ptSelTask;
  
  // map the pointer
  ptSelTask = MAP_HANDLE_TO_POINTER( ptTask );
  
  // first validate that the task is valid
  if ( ptSelTask == ptSelTask->ptSignature )
  {
    // disable interrupts
    Interrupt_Disable( );
    
    // set the state
    ptSelTask->bDelayInProgress = bState;
    
    // set the state
    if ( bState )
    {
      // check for a new time
      if ( uTime != 0 )
      {
        // calculate the new time
        ptSelTask->uDelayTime = uTime / uSystemTickRate;
      }
      
      // update the time
      ptSelTask->uDelayCount = ptSelTask->uDelayTime;
    }
    
    // re-enable interrupts
    Interrupt_Enable( );
  }
  else
  {
    // set the error
    eError = TASKSCHEDULER_ERR_ILLTASKHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function TaskScheduler_CheckTasksPending
 *
 * @brief check for tasks pending
 *
 * This function will check for any tasks pending
 *
 * @return      TRUE if tasks pending
 *
 *****************************************************************************/
BOOL TaskScheduler_CheckTasksPending( void )
{
  PTASKCTL  ptCurTask;
  BOOL      bTasksPending = FALSE;
  
  // for each item in list
  ptCurTask = ptFirstTask;
  while(( ptCurTask ) != NULL && !bTasksPending )
  {
    // is this task enabled
    if ( ptCurTask->bEnabled )
    {
      // is this task have any pending events
      if (( ptCurTask->xNrmCount != 0 ) || ( ptCurTask->nPriCount != 0 ))
      {
        // set status true/exit
        bTasksPending = TRUE;
        break;
      }
    }
    
    // next task
    ptCurTask = ptCurTask->ptNextTask;
  }
  
  // return status
  return( bTasksPending );
}
 
/******************************************************************************
 * @function PutPriorityEvent
 *
 * @brief put a priority event into the queue
 *
 * This function will put a priority event into the queue
 *
 * @param[in]   ptTask    task handle
 *
 * @return      TRUE if okay, FALSE if queue full
 *
 *****************************************************************************/
static BOOL PutPriorityEvent( PTASKCTL ptTask, TASKSCHEDULEREVENT xEvent )
{
  BOOL      bStatus = FALSE;
  PTASKCTL  ptSelTask;
  
  // check for room
  if ( ptTask->nPriCount < ptTask->nPriMaxEvents )
  {
    // add the event/adjust pointer/increment count
    *( ptTask->pxPriEvents + ptTask->nPriWrIdx++ ) = xEvent;
    ptTask->nPriWrIdx %= ptTask->nPriMaxEvents;
    ptTask->nPriCount++;
    
    // set the status to true
    bStatus = TRUE;
  }
  
  // return status
  return( bStatus );
}

/**@} EOF TaskScheduler.c */
