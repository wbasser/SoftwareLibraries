/******************************************************************************
 * @file TaskManager.c
 *
 * @brief task manager implementation
 *
 * This file provides the code for the implementation of the task manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of Cyber 
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
 * \addtogroup TaskManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TaskManager/TaskManager.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the scheduled task control structure
typedef struct  _SCHDTASKCTL
{
  U32         uDelayTime;       ///< current delay time
  U32         uDelayCount;      ///< delay count
  QUESIZEARG  xRdIdx;           ///< event buffer read index
  QUESIZEARG  xWrIdx;           ///< event buffer write index
  QUESIZEARG  xEvnCount;        ///< event count
  U8          nPriRdIdx;        ///< priority event buffer read index
  U8          nPriWrIdx;        ///< priority event buffer write index
  U8          nPriEvnCount;     ///< priority event count
  BOOL        bDelayInProgress; ///< delay in progress
  BOOL        bEnabled;         ///< task enabled
  TASKARG axPriEvents[ TASK_MAXNUM_PRI_EVENTS ];
} SCHDTASKCTL, *PSCHDTASKCTL;
#define SCHDTASKCTL_SIZE  sizeof( SCHDTASKCTL );

#if ( TASK_TICK_ENABLE == 1 )
/// define the tick task control structure
typedef struct  _TICKTASKCTL
{
  U32		uDelayTime;			  ///< current delay time
  U32		uDelayCount;		  ///< delay count
  BOOL  bEnabled;         ///< task enabled
} TICKTASKCTL, *PTICKTASKCTL;
#define TICKTASKCTL_SIZE  sizeof( TICKTASKCTL );
#endif  // TASK_TICK_ENABLE

// local parameter declarations -----------------------------------------------
static	U32			    uExecutionRate;
static	SCHDTASKCTL	atSchdCtls[ TASK_SCHD_MAX ];
#if ( TASK_TICK_ENABLE == 1 )
static	TICKTASKCTL	atTickCtls[ TASK_TICK_MAX ];
#endif  // TASK_TICK_ENABLE
#if ( TASK_DISABLE_TICK_PROCESSING == 1 )
static  VBOOL       bProcessTickFlag;
#endif // TASK_DISABLE_TICK_PROCESSING
static  U16         wNrmEventCount;
static  U16         wPriEventCount;

// local function prototypes --------------------------------------------------
static  BOOL  PutPriorityEvent( PSCHDTASKCTL ptCtl, TASKARG xArg );
static  void  ProcessTick( void );

/******************************************************************************
 * @function TaskManager_Initialize
 *
 * @brief task manager initialization
 *
 * This function initializes the tasks manager and any tasks that have 
 * initialization function
 *
 *****************************************************************************/
void TaskManager_Initialize( void )
{
  U8			      nIdx;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  
  // store the execution rate
  uExecutionRate = SystemTick_GetTickRateUsec( );
  
  // clear the  evente count
  wNrmEventCount = wPriEventCount = 0;

  #if ( TASK_DISABLE_TICK_PROCESSING == 1 )
  // just clear tick flag
  bProcessTickFlag = FALSE;
  #endif // TASK_DISABLE_TICK_PROCESSING
  
  // for each entry in the scheduled task list
  for ( nIdx = 0; nIdx < TASK_SCHD_MAX; nIdx++ )
  {
    // get a pointer to the task control/definition
    ptSchdCtl = &atSchdCtls[ nIdx ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ nIdx ];
    
    // set the initial values
    ptSchdCtl->uDelayTime   = PGM_RDDWRD( ptSchdDef->uDelayTime ) / uExecutionRate;
    ptSchdCtl->uDelayCount  = ptSchdCtl->uDelayTime;
    ptSchdCtl->xEvnCount    = 0;
    ptSchdCtl->xRdIdx       = 0;
    ptSchdCtl->xWrIdx       = 0;
    ptSchdCtl->nPriEvnCount = 0;
    ptSchdCtl->nPriRdIdx    = 0;
    ptSchdCtl->nPriWrIdx    = 0;
    ptSchdCtl->bEnabled     = PGM_RDBYTE( ptSchdDef->bEnabled );
    switch ( PGM_RDBYTE( ptSchdDef->eType ))
    {
      case TASK_TYPE_EVENT :
        ptSchdCtl->bDelayInProgress = FALSE;
        break;

      default :
        ptSchdCtl->bDelayInProgress = TRUE;
        break;
    }
    
    // check for init
    if ( PGM_RDBYTE( ptSchdDef->bRunOnInit ))
    {
      // put a prioirty event
      PutPriorityEvent( ptSchdCtl, TASK_INITIALIZE_EVENT );
    }
  }

  #if ( TASK_TICK_ENABLE == 1 )
  PTASKTICKDEF  ptTickDef;
  PTICKTASKCTL	ptTickCtl;
  
  // for each entry in the tick task list
  for ( nIdx = 0; nIdx < TASK_TICK_MAX; nIdx++ )
  {
    // get a pointer to the task control/definition
    ptTickCtl = &atTickCtls[ nIdx ];
    ptTickDef = ( PTASKTICKDEF )&atTaskTickDefs[ nIdx ];
    
    // set the initial values
    ptTickCtl->uDelayTime	= PGM_RDDWRD( ptTickDef->uDelayTime ) / uExecutionRate;
    ptTickCtl->uDelayCount	= ptTickCtl->uDelayTime;
    ptTickCtl->bEnabled     = PGM_RDBYTE( ptTickDef->bEnabled );
  }
  #endif  // TASK_TICK_ENABLE
}

/******************************************************************************
 * @function TaskManager_IdleProcess
 *
 * @brief this the idle loop for processing all scheduled tasks
 *
 * This function loops through all tasks and well execute those that have
 * and event count not equal to 0
 *
 *****************************************************************************/
void TaskManager_IdleProcess( void )
{
  U8            nIdx;
  PVEXECFUNC    pvExec;
  PTASKARG	pxEvents;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  TASKARG	xArg;
  BOOL          bPriEvent;
  
  #if ( TASK_DISABLE_TICK_PROCESSING == 1 )
  // normal process of the tick task
  if ( bProcessTickFlag == TRUE )
  {
    // process the tick functionality
    ProcessTick( );
  }
  #endif // TASK_DISABLE_TICK_PROCESSING
  
  // for each entry in the task list
  for ( nIdx = 0; nIdx < TASK_SCHD_MAX; nIdx++ )
  {
    // get the pointers
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ nIdx ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ nIdx ];

    // determine if this task is enabled
    if ( ptSchdCtl->bEnabled )
    {
      // get the task execution function
      pvExec = ( PVOID )PGM_RDWORD( ptSchdDef->pvExec );
      
      if (( ptSchdCtl->xEvnCount != 0 ) || ( ptSchdCtl->nPriEvnCount != 0 ))
      {
        // determine if we have any priority events
        if ( ptSchdCtl->nPriEvnCount != 0 )
        {
          // get tht priority event
          xArg = ptSchdCtl->axPriEvents[ ptSchdCtl->nPriRdIdx ];
          
          // set the priority event flag
          bPriEvent = TRUE;
        }
        else
        {
          // get the event          
          pxEvents = ( PTASKARG )PGM_RDWORD( ptSchdDef->pxEvents );
          xArg = *( pxEvents + ptSchdCtl->xRdIdx );
          
          // clear the priority event flag
          bPriEvent = FALSE;
        }
      
        // execute the task
        if ( pvExec( xArg ))
        {
          // disable interrupts
          Interrupt_Disable( );
          
          // determine if this was a priority event
          if ( bPriEvent == TRUE )
          {
            // decrement the priority event count/adjust read index
            ptSchdCtl->nPriEvnCount--;
            ptSchdCtl->nPriRdIdx++;
            ptSchdCtl->nPriRdIdx %= TASK_MAXNUM_PRI_EVENTS;
            wPriEventCount--;
          }
          else
          {
            // decrement the event count/adjust read index
            ptSchdCtl->xEvnCount--;
            ptSchdCtl->xRdIdx++;
            ptSchdCtl->xRdIdx %= GETQUEUESIZE( ptSchdDef->xNumEvents );
            wNrmEventCount--;
          }
          
          // re-enable interrupts
          Interrupt_Enable( );
        }
      }
    }
  }
}

/******************************************************************************
 * @function TaskManager_TickProcess
 *
 * @brief task tick process 
 *
 * This function loops through each task, checks for timer running, decrements
 * the timer and if zero, posts an event.
 *
 *****************************************************************************/
void TaskManager_TickProcess( void )
{
  #if ( TASK_TICK_ENABLE == 1 )
  PTASKTICKDEF  ptTickDef;
  PTICKTASKCTL	ptTickCtl;
  PVEXECFUNC    pvExec;
  U8            nIdx;

  // for each entry in the tick task list
  for ( nIdx = 0; nIdx < TASK_TICK_MAX; nIdx++ )
  {
    // get the pointers
    ptTickCtl = ( PTICKTASKCTL )&g_atTickCtls[ nIdx ];
    ptTickDef = ( PTASKTICKDEF )&g_atTaskTickDefs[ nIdx ];

    // check for enabled
    if ( ptTickCtl->bEnabled )
    {
      // decrement the delay
      if ( --ptTickCtl->uDelayCount == 0 )
      {
        // execute the task
        pvExec = ( PVEXECFUNC )PGM_RDWORD( ptTickDef->pvExec );
        pvExec( TASK_TIMEOUT_EVENT );

        // reset the time
        ptTickCtl->uDelayCount = ptTickCtl->uDelayTime;
      }
    }
  }
  #endif  // TASK_TICK_ENABLE

  #if ( TASK_DISABLE_TICK_PROCESSING == 0 )
  // normal process of the tick task
  ProcessTick( );
  #else
  // just set tick flag
  bProcessTickFlag = TRUE;
  #endif // TASK_DISABLE_TICK_PROCESSING
}

/******************************************************************************
 * @function TaskManager_EnableDisable
 *
 * @brief enables/disables a task to run
 *
 * This function sets the enable flag in the control structure
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   bState  state of the task
 *
 * @return      TRUE if okay, FALSE if illegal task enumeration
 *
 *****************************************************************************/
BOOL TaskManager_EnableDisable( TASKSCHDENUMS eTask, BOOL bState )
{
  BOOL bStatus = FALSE;
  
  // valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // set the enable flag/indicate good status
    atSchdCtls[ eTask ].bEnabled = bState;
    bStatus = TRUE;
  }
  
  // return the status
  return( bStatus );
}

#if ( TASK_TICK_ENABLE == 1 )
/******************************************************************************
 * @function TaskManager_TickEnableDisable
 *
 * @brief enables/disables a tick task to run
 *
 * This function sets the enable flag in the control structure
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   bState  state of the task
 *
 * @return      TRUE if okay, FALSE if illegal task enumeration
 *
 *****************************************************************************/
BOOL TaskManager_TickEnableDisable( TASKTICKENUMS eTask, BOOL bState )
{
  BOOL bStatus = FALSE;
  
  // valid task
  if ( eTask < TASK_TICK_MAX )
  {
    // set the enable flag/indicate good status
    atTickCtls[ eTask ].bEnabled = bState;
    bStatus = TRUE;
  }
  
  // return the status
  return( bStatus );
}
#endif  // TASK_TICK_ENABLE

/******************************************************************************
 * @function TaskManager_PostEvent
 *
 * @brief post an event to a task
 *
 * This function will post an event to a task if there is room
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   xArg    argument to post to task
 *
 * @return  TRUE if okay, false if illegal task enumeration or no room
 *
 *****************************************************************************/
BOOL TaskManager_PostEvent( TASKSCHDENUMS eTask, TASKARG xArg )
{
  BOOL          bStatus = FALSE;
  PTASKARG		pxEvents;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  TASKTYPE		eType;
  QUESIZEARG    xNumEvents;
  
  // valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ eTask ];
    
    // test for a valid entry
    eType = PGM_RDBYTE( ptSchdDef->eType );
    if (( eType != TASK_TYPE_TIMED_ONESHOT ) && ( ptSchdCtl->bEnabled == TRUE ))
    {
      // disable interrupts
      Interrupt_Disable( );
      
      // get the number of events
      xNumEvents = PGM_RDBYTE( ptSchdDef->xNumEvents );
      
      // is there room
      if ( ptSchdCtl->xEvnCount < xNumEvents )
      {
        // get the event pointer
        pxEvents = ( PTASKARG )PGM_RDWORD( ptSchdDef->pxEvents );

        // stuff it/increment count/adjust write index
        *( pxEvents + ptSchdCtl->xWrIdx++ ) = xArg;
        ptSchdCtl->xEvnCount++;
        ptSchdCtl->xWrIdx %= xNumEvents;
        
        // increment event count
        wNrmEventCount++;
        
        // set good status
        bStatus = TRUE;
      }
      
      // re-enable interrupts
      Interrupt_Enable( );
    }
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function TaskManager_PostPriorityEvent
 *
 * @brief post an event to a task
 *
 * This function will post an event to a task if there is room
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   xArg    argument to post to task
 *
 * @return  TRUE if okay, false if illegal task enumeration or no room
 *
 *****************************************************************************/
BOOL TaskManager_PostPriorityEvent( TASKSCHDENUMS eTask, TASKARG xArg )
{
  BOOL          bStatus = FALSE;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  TASKTYPE		  eType;
  
  // valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ eTask ];
    
    // test for a valid entry
    eType = PGM_RDBYTE( ptSchdDef->eType );
    if (( eType != TASK_TYPE_TIMED_ONESHOT ) && ( ptSchdCtl->bEnabled == TRUE ))
    {
      // disable interrupts
      Interrupt_Disable( );
      
      // put a priority event
      bStatus = PutPriorityEvent( ptSchdCtl, xArg );
      
      // re-enable interrupts
      Interrupt_Enable( );
    }
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function TaskManager_PostEventIrq
 *
 * @brief post an event to a task
 *
 * This function will post an event to a task if there is room
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   xArg    argument to post to task
 *
 * @return  TRUE if okay, false if illegal task enumeration or no room
 *
 *****************************************************************************/
BOOL TaskManager_PostEventIrq( TASKSCHDENUMS eTask, TASKARG xArg )
{
  BOOL          bStatus = FALSE;
  PTASKARG		  pxEvents;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  TASKTYPE		  eType;
  QUESIZEARG    xNumEvents;
  
  // valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ eTask ];
    
    // test for a valid entry
    eType = PGM_RDBYTE( ptSchdDef->eType );
    if (( eType != TASK_TYPE_TIMED_ONESHOT ) && ( ptSchdCtl->bEnabled == TRUE ))
    {
      // get the number of events
      xNumEvents = PGM_RDBYTE( ptSchdDef->xNumEvents );
      
      // is there room
      if ( ptSchdCtl->xEvnCount < xNumEvents )
      {
        // get the event pointer
        pxEvents = ( PTASKARG )PGM_RDWORD( ptSchdDef->pxEvents );

        // stuff it/increment count/adjust write index
        *( pxEvents + ptSchdCtl->xWrIdx++ ) = xArg;
        ptSchdCtl->xEvnCount++;
        ptSchdCtl->xWrIdx %= xNumEvents;
        
        // increment event count
        wNrmEventCount++;
        
        // set good status
        bStatus = TRUE;
      }
    }
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function TaskManager_PostPriorityEventIrq
 *
 * @brief post an event to a task
 *
 * This function will post an event to a task if there is room
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   xArg    argument to post to task
 *
 * @return  TRUE if okay, false if illegal task enumeration or no room
 *
 *****************************************************************************/
BOOL TaskManager_PostPriorityEventIrq( TASKSCHDENUMS eTask, TASKARG xArg )
{
  BOOL          bStatus = FALSE;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  TASKTYPE		  eType;
  
  // valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ eTask ];
    
    // test for a valid entry
    eType = PGM_RDBYTE( ptSchdDef->eType );
    if (( eType != TASK_TYPE_TIMED_ONESHOT ) && ( ptSchdCtl->bEnabled == TRUE ))
    {
      // put a priority event
      bStatus = PutPriorityEvent( ptSchdCtl, xArg );
    }
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function TaskManager_FlushEvents
 *
 * @brief flush all events 
 *
 * This function flushes all events from a given task, resets the event count
 * to zero and resets the indices
 *
 * @param[in]   eTask task enumeration
 *
 * @return      TRUE if no errors, FALSE if illegal task enumration
 *
 *****************************************************************************/
BOOL TaskManager_FlushEvents( TASKSCHDENUMS eTask )
{
  BOOL 		      bStatus = FALSE;
  PSCHDTASKCTL  ptSchdCtl;
  
  // test for a valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];

    // disable interrupts
    Interrupt_Disable( );
    
    // adjust the event counts
    wNrmEventCount -= ptSchdCtl->xEvnCount;
    wPriEventCount -= ptSchdCtl->nPriEvnCount;
    
    // stuff it/reset count/reset indices
    ptSchdCtl->xEvnCount = 0;
    ptSchdCtl->xWrIdx = 0;
    ptSchdCtl->xRdIdx = 0;
    ptSchdCtl->nPriEvnCount = 0;
    ptSchdCtl->nPriWrIdx = 0;
    ptSchdCtl->nPriRdIdx = 0;
    
    // re-enable interrupts
    Interrupt_Enable( );
      
    // set good status
    bStatus = TRUE;
  }
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function TaskManager_StartTimer 
 *
 * @brief start a timer
 *
 * This function will set the timers time, and start the timer
 *
 * @param[in]   eTask   task enumeration
 * @param[in]   uTime   time to set in usecs
 *
 * @return      TRUE if no errors, FALSE if illegal enumeration
 *
 *****************************************************************************/
BOOL TaskManager_StartTimer( TASKSCHDENUMS eTask, U32 uTime )
{
  BOOL 		      bStatus = FALSE;
  PSCHDTASKCTL  ptSchdCtl;
  
  // test for a valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];

    // disable interrupts
    Interrupt_Disable( );
    
    // stuff it/reset count/reset indices
    if ( uTime != 0 )
    {
      // set the new time
      ptSchdCtl->uDelayTime = uTime / uExecutionRate;
    }
    ptSchdCtl->uDelayCount = ptSchdCtl->uDelayTime;
    ptSchdCtl->bDelayInProgress = TRUE;
    
    // re-enable interrupts
    Interrupt_Enable( );
      
    // set good status
    bStatus = TRUE;
  }
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function TaskManager_StopTimer
 *
 * @brief stop a tasks timer
 *
 * This function stops a tasks timer
 *
 * @param[in]   eTask   task enumeration
 *
 * @return      TRUE if ok, FALSE if illegal task enumeration
 *
 *****************************************************************************/
BOOL TaskManager_StopTimer( TASKSCHDENUMS eTask )
{
  BOOL 		      bStatus = FALSE;
  PSCHDTASKCTL  ptSchdCtl;
  
  // test for a valid task
  if ( eTask < TASK_SCHD_MAX )
  {
    // get a pointer to the event control
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ eTask ];

    // disable interrupts
    Interrupt_Disable( );
 
    // clear the delay in progress
    ptSchdCtl->bDelayInProgress = FALSE;
    
    // re-enable interrupts
    Interrupt_Enable( );
      
    // set good status
    bStatus = TRUE;
  }
  
  // return status
  return( bStatus );
}
 
/******************************************************************************
 * @function TaskManager_CheckTasksPending
 *
 * @brief checks for pending tasks
 *
 * This function will check for both normal and priority tasks pending 
 *
 * @return      TRUE if tasks pending, FALSE if not
 *
 *****************************************************************************/
BOOL TaskManager_CheckTasksPending( void )
{
  BOOL bPending;
  
  // check the event counts
  bPending = (( wPriEventCount == 0 ) && ( wNrmEventCount == 0 )) ? TRUE : FALSE;
  
  // return the pending status
  return( bPending );
}

/******************************************************************************
 * @function PutPriorityEvent
 *
 * @brief put a priority event on the event queue
 *
 * This function puts a priority event in front of the queue
 *
 * @param[in]   ptCtl   pointer to the control structure
 * @param[in]   tArg    event to put on the queue
 *
 * @return      TRUE if ok
 *
 *****************************************************************************/
static BOOL PutPriorityEvent( PSCHDTASKCTL ptCtl, TASKARG xArg )
{
  BOOL      bStatus = FALSE;

  // is there room
  if ( ptCtl->nPriEvnCount < TASK_MAXNUM_PRI_EVENTS )
  {
    // stuff it/increment write index/adjust
    ptCtl->axPriEvents[ ptCtl->nPriWrIdx++ ] = xArg;
    ptCtl->nPriWrIdx %= TASK_MAXNUM_PRI_EVENTS;
    ptCtl->nPriEvnCount++;
    
    // increment priority event count
    wPriEventCount++;
    
    // set good status
    bStatus = TRUE;
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ProcessTick
 *
 * @brief process a tick task
 *
 * This function performs the processing of every task for time change
 *
 *****************************************************************************/
static void ProcessTick( void )
{
  U8			      nIdx;
  PSCHDTASKCTL	ptSchdCtl;
  PTASKSCHDDEF	ptSchdDef;
  TASKTYPE		  eType;
  
  #if (TASK_DISABLE_TICK_PROCESSING == 1 )
  // clear the tick flag
  bProcessTickFlag = FALSE;
  #endif // TASK_DISABLE_TICK_PROCESSING
  
  // for each entry in the scheduled task list
  for ( nIdx = 0; nIdx < TASK_SCHD_MAX; nIdx++ )
  {
    // get the pointers
    ptSchdCtl = ( PSCHDTASKCTL )&atSchdCtls[ nIdx ];
    ptSchdDef = ( PTASKSCHDDEF )&g_atTaskSchdDefs[ nIdx ];
    
    // determine if there is a pending task
    if (( ptSchdCtl->bDelayInProgress ) && ( ptSchdCtl->bEnabled ))
    {
      // only decrement the delay if not zero
      if ( ptSchdCtl->uDelayCount != 0 )
      {
        // decrement the delay
        if ( --ptSchdCtl->uDelayCount == 0 )
        {
          // put a prioirty event
          PutPriorityEvent( ptSchdCtl, TASK_TIMEOUT_EVENT );
          
          // get the task type
          eType = PGM_RDBYTE( ptSchdDef->eType );
      
          // determine if this is a one shot or continuous timer
          if ( eType == TASK_TYPE_TIMED_CONTINUOUS )
          {
            // reload the time
            ptSchdCtl->uDelayCount = ptSchdCtl->uDelayTime;
          }
          else
          {
            // clear the delay in progress flag
            ptSchdCtl->bDelayInProgress = FALSE;
          }
        }
      }
    }
  }
}

/**@} EOF .c */
