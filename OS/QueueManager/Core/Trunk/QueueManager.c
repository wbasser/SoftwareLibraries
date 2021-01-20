/******************************************************************************
 * @file  QueueManager.c 
 *
 * @brief queue mananger implementation
 *
 * This file provides the implementation of the queue manager
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
 * \addtogroup QueueManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "string.h"

// local includes -------------------------------------------------------------
#include "QueueManager/QueueManager.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro foe creating events
#define CREATE_QUEUE_EVENT( event, queue )    (( event << QUEUE_EVENT_SHIFT ) | queue )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the queue control structure
typedef struct _QUEUECTL
{
  XQUEUENUMENTRIES  xRdIdx;				///< queue read index
  XQUEUENUMENTRIES  xWrIdx;				///< queue write index
  XQUEUENUMENTRIES  xCount;				///< count	
} QUEUECTL, *PQUEUECTL;
#define	QUEUECTL_SIZE	sizeof( QUEUECTL )

// local parameter declarations -----------------------------------------------
static	QUEUECTL	atQueueCtls[ QUEUE_ENUM_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function QueueManager_Initialize
 *
 * @brief queue initialization
 *
 * This function will initialize all queues in the configuration list
 *
 *****************************************************************************/
void QueueManager_Initialize( void )
{
  QUEUEENUM eQueue;
  PQUEUECTL ptCtl;
  
  // for each queue in the list
  for ( eQueue = 0; eQueue < QUEUE_ENUM_MAX; eQueue++ )
  {
    // get a pointer to the control
    ptCtl = &atQueueCtls[ eQueue ];
    
    // reset the indices/count
    ptCtl->xRdIdx = 0;
    ptCtl->xWrIdx = 0;
    ptCtl->xCount = 0;
  }
}

/******************************************************************************
 * @function QueueManager_PutHead
 *
 * @brief puts a value onto the head of the quueue
 *
 * This function checks for empty space if available, puts the event at the head
 * of the queue.  It will also post an appropriate event to the desired task
 *
 * @param[in]   eQueue  the queue enumeration
 * @param[in]   pnEntry the pointer to the queue entry
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_PutHead( QUEUEENUM eQueue, PU8 pnEntry )
{
  QUEUESTATUS       eError = QUEUE_STATUS_NONE;
  PQUEUECTL		      ptCtl;
  PQUEUEDEF		      ptDef;
  QUEUEEVNFLAGS	    tEvents;
  TASKSCHDENUMS	    eTaskEnum;
  XQUEUEENTRYSIZE   wSize;
  XQUEUENUMENTRIES  xNumEntries;
  PU8               pnQueue;
  U8                nEvent = 0;


  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    ptDef = ( PQUEUEDEF )&g_atQueueDefs[ eQueue ];
    
    // get the number of events
    xNumEntries = PGM_RDBYTE( ptDef->xNumEntries );
    
    // is there room
    if ( ptCtl->xCount < xNumEntries )
    {
      // get the size/event flags enables/task enum
      wSize = PGM_RDBYTE( ptDef->xEntrySize );
      tEvents.nByte = ( U8 )PGM_RDBYTE( ptDef->tEventFlags.nByte );
      eTaskEnum = PGM_RDBYTE( ptDef->eTaskEnum );
      
      // get the pointer to the queue
      pnQueue = ( PU8 )PGM_RDWORD( ptDef->pnQueue );
      
      // disable interrupts
      Interrupt_Disable( );
      
      // adjust the read pointer
      if ( ptCtl->xRdIdx == 0 )
      {
        // reset the index to the end
        ptCtl->xRdIdx = wSize - 1;
      }
      else
      {
        // increment the read index
        ptCtl->xRdIdx++;
      }
      
      // now copy the into the queue
      memcpy(( pnQueue + ( ptCtl->xRdIdx * wSize )), pnEntry, wSize );
      
      // increment the count
      ptCtl->xCount++;
      
      // re-enable interrupts
      Interrupt_Enable( );
      
      // check get event
      if ( tEvents.tBits.bPut )
      {
        // set the event
        nEvent = QUEUE_EVENT_PUT;
      }
      
      // check for full
      if (( ptCtl->xCount == xNumEntries ) && ( tEvents.tBits.bFull ))
      {
        // or the empty event
        nEvent = QUEUE_EVENT_PUTFULL;
      }
      
      // check if event is present
      if ( nEvent != 0 )
      {
        // post the event
        TaskManager_PostEvent( eTaskEnum, ( TASKARG )CREATE_QUEUE_EVENT( nEvent, eQueue ));
      }
    }
    else
    {
      // return the queue full error
      eError = QUEUE_STATUS_QUEFUL;
    }
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_PutTail
 *
 * @brief puts a value onto the tail of the quueue
 *
 * This function checks for empty space if available, puts the event at the end
 * of the queue.  It will also post an appropriate event to the desired task
 *
 * @param[in]   eQueue  the queue enumeration
 * @param[in]   pnEntry the pointer to the queue entry
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_PutTail( QUEUEENUM eQueue, PU8 pnEntry )
{
  QUEUESTATUS       eError = QUEUE_STATUS_NONE;
  PQUEUECTL		      ptCtl;
  PQUEUEDEF		      ptDef;
  QUEUEEVNFLAGS	    tEvents;
  TASKSCHDENUMS	    eTaskEnum;
  XQUEUEENTRYSIZE   wSize;
  XQUEUENUMENTRIES  xNumEntries;
  U8                nEvent = 0;
  PU8               pnQueue;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    ptDef = ( PQUEUEDEF )&g_atQueueDefs[ eQueue ];
    
    // get the number of events
    xNumEntries = PGM_RDBYTE( ptDef->xNumEntries );
    
    // is there room
    if ( ptCtl->xCount < xNumEntries )
    {
      // get the size/event flags enables/task enum
      wSize = PGM_RDBYTE( ptDef->xEntrySize );
      tEvents.nByte = ( U8 )PGM_RDBYTE( ptDef->tEventFlags.nByte );
      eTaskEnum = PGM_RDBYTE( ptDef->eTaskEnum );
      
      // get the pointer to the queue
      pnQueue = ( PU8 )PGM_RDWORD( ptDef->pnQueue );
      
      // disable interrupts
      Interrupt_Disable( );
      
      // now copy the into the queue
      memcpy(( pnQueue + ( ptCtl->xWrIdx * wSize )), pnEntry, wSize );
      
      // adjust the write index
      ptCtl->xWrIdx++;
      if ( ptCtl->xWrIdx >= xNumEntries )
      {
        // reset back to zero
        ptCtl->xWrIdx = 0;
      }
      
      // increment the count
      ptCtl->xCount++;
      
      // re-enable interrupts
      Interrupt_Enable( );
      
      // check get event
      if ( tEvents.tBits.bPut )
      {
        // set the event
        nEvent = QUEUE_EVENT_PUT;
      }
      
      // check for full
      if (( ptCtl->xCount == xNumEntries ) && ( tEvents.tBits.bFull ))
      {
        // or the empty event
        nEvent |= QUEUE_EVENT_PUTFULL;
      }
      
      // check if event is present
      if ( nEvent != 0 )
      {
        // post the event
        TaskManager_PostEvent( eTaskEnum, ( TASKARG )CREATE_QUEUE_EVENT( nEvent, eQueue ));
      }
    }
    else
    {
      // return the queue full error
      eError = QUEUE_STATUS_QUEFUL;
    }
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_Get
 *
 * @brief gets an entry from the queue
 *
 * This function will get a entry from the queue if one is available
 *
 * @param[in]   eQueue  the queue enumeration
 * @param[io]   pnEntry the pointer to store the queue entry in
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_Get( QUEUEENUM eQueue, PU8 pnEntry )
{
  QUEUESTATUS       eError = QUEUE_STATUS_NONE;
  PQUEUECTL		      ptCtl;
  PQUEUEDEF		      ptDef;
  QUEUEEVNFLAGS	    tEvents;
  TASKSCHDENUMS	    eTaskEnum;
  XQUEUEENTRYSIZE   wSize;
  XQUEUENUMENTRIES  xNumEntries;
  U8                nEvent = 0;
  PU8               pnQueue;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    ptDef = ( PQUEUEDEF )&g_atQueueDefs[ eQueue ];
    
    // get the number of events
    xNumEntries = PGM_RDBYTE( ptDef->xNumEntries );
    
    // is there anything in the queue
    if ( ptCtl->xCount != 0 )
    {
      // get the size/event flags enables/task enum
      wSize = PGM_RDBYTE( ptDef->xEntrySize );
      tEvents.nByte = ( U8 )PGM_RDBYTE( ptDef->tEventFlags.nByte );
      eTaskEnum = PGM_RDBYTE( ptDef->eTaskEnum );
      
      // get the pointer to the queue
      pnQueue = ( PU8 )PGM_RDWORD( ptDef->pnQueue );
      
      // disable interrupts
      Interrupt_Disable( );
      
      // now copy the entry to the pointer
      memcpy( pnEntry, ( pnQueue + ( ptCtl->xRdIdx * wSize )), wSize );
      
      // adjust the read index
      ptCtl->xRdIdx++;
      if ( ptCtl->xRdIdx >= xNumEntries )
      {
        // reset back to zero
        ptCtl->xRdIdx = 0;
      }
      
      // decrement  the count
      ptCtl->xCount--;
      
      // re-enable interrupts
      Interrupt_Enable( );
      
      // check get event
      if ( tEvents.tBits.bGet )
      {
        // set the event
        nEvent = QUEUE_EVENT_GET;
      }
      
      // check for empty
      if (( ptCtl->xCount == 0 ) && ( tEvents.tBits.bEmpty ))
      {
        // or the empty event
        nEvent |= QUEUE_EVENT_GETEMPTY;
      }
      
      // check if event is present
      if ( nEvent != 0 )
      {
        // post the event
        TaskManager_PostEvent( eTaskEnum, ( TASKARG )CREATE_QUEUE_EVENT( nEvent, eQueue ));
      }
    }
    else
    {
      // return the queue empty error
      eError = QUEUE_STATUS_QUEEMP;
    }
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_Peek
 *
 * @brief gets an entry from the queue
 *
 * This function will get a entry from the queue if one is available but not 
 * remove from the queue
 *
 * @param[in]   eQueue  the queue enumeration
 * @param[io]   pnEntry the pointer to store the queue entry in
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_Peek( QUEUEENUM eQueue, PU8 pnEntry )
{
  QUEUESTATUS     eError = QUEUE_STATUS_NONE;
  PQUEUECTL		    ptCtl;
  PQUEUEDEF		    ptDef;
  XQUEUEENTRYSIZE wSize;
  PU8             pnQueue;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    ptDef = ( PQUEUEDEF )&g_atQueueDefs[ eQueue ];
    
    // is there anything in the queue
    if ( ptCtl->xCount != 0 )
    {
      // get the size/event flags enables/task enum
      wSize = PGM_RDBYTE( ptDef->xEntrySize );
      
      // get the pointer to the queue
      pnQueue = ( PU8 )PGM_RDWORD( ptDef->pnQueue );
      
      // disable interrupts
      Interrupt_Disable( );
      
      // now copy the entry to the pointer
      memcpy( pnEntry, ( pnQueue + ( ptCtl->xRdIdx * wSize )), wSize );
      
      // re-enable interrupts
      Interrupt_Enable( );
    }
    else
    {
      // return the queue empty error
      eError = QUEUE_STATUS_QUEEMP;
    }
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_Pop
 *
 * @brief removes an entry from the queue
 *
 * This function will pop an entry from the queue if one is there
 *
 * @param[in]   eQueue  the queue enumeration
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_Pop( QUEUEENUM eQueue )
{
  QUEUESTATUS       eError = QUEUE_STATUS_NONE;
  PQUEUECTL		      ptCtl;
  PQUEUEDEF		      ptDef;
  QUEUEEVNFLAGS	    tEvents;
  TASKSCHDENUMS	    eTaskEnum;
  XQUEUENUMENTRIES  xNumEntries;
  U8                nEvent = 0;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    ptDef = ( PQUEUEDEF )&g_atQueueDefs[ eQueue ];
    
    // get the number of events
    xNumEntries = PGM_RDBYTE( ptDef->xNumEntries );
    
    // is there anything in the queue
    if ( ptCtl->xCount != 0 )
    {
      // get the size/event flags enables/task enum
      tEvents.nByte = ( U8 )PGM_RDBYTE( ptDef->tEventFlags.nByte );
      eTaskEnum = PGM_RDBYTE( ptDef->eTaskEnum );
      
      // disable interrupts
      Interrupt_Disable( );
      
      // adjust the read index
      ptCtl->xRdIdx++;
      if ( ptCtl->xRdIdx >= xNumEntries )
      {
        // reset back to zero
        ptCtl->xRdIdx = 0;
      }
      
      // decrement  the count
      ptCtl->xCount--;
      
      // re-enable interrupts
      Interrupt_Enable( );
      
      // check for empty
      if (( ptCtl->xCount == 0 ) && ( tEvents.tBits.bEmpty ))
      {
        // or the empty event
        nEvent |= QUEUE_EVENT_GETEMPTY;
      }
      
      // check if event is present
      if ( nEvent != 0 )
      {
        // post the event
        TaskManager_PostEvent( eTaskEnum, ( TASKARG )CREATE_QUEUE_EVENT( nEvent, eQueue ));
      }
    }
    else
    {
      // return the queue empty error
      eError = QUEUE_STATUS_QUEEMP;
    }
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_Flush
 *
 * @brief flush all entries
 *
 * This function will flush all entries in the given queue and clear the indices
 *
 * @param[in]   eQueue  the queue enumeration
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_Flush( QUEUEENUM eQueue )
{
  QUEUESTATUS   eError = QUEUE_STATUS_NONE;
  PQUEUECTL		  ptCtl;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    
    // disable interrupts
    Interrupt_Disable( );
    
    // clear the indices
    ptCtl->xWrIdx = 0;
    ptCtl->xRdIdx = 0;
    ptCtl->xCount = 0;
    
    // re-enable interrupts
    Interrupt_Enable( );
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_GetRemaining
 *
 * @brief get the number of entries in the queue
 *
 * This function will return the number of entries still in the queue
 *
 * @param[in]   eQueue  the queue enumeration
 * @param[io]   pnRemaining the pointer to store the number of entries remaining
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_GetRemaining( QUEUEENUM eQueue, PU8 pnRemaining )
{
  QUEUESTATUS  eError = QUEUE_STATUS_NONE;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // set the remiaining equal to the count
    *( pnRemaining ) = atQueueCtls[ eQueue ].xCount;
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function QueueManager_GetStatus
 *
 * @brief get the queue status
 *
 * This function will return the current status as to full/empty/illegal
 *
 * @param[in]   eQueue  the queue enumeration
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueManager_GetStatus( QUEUEENUM eQueue )
{
  QUEUESTATUS       eError = QUEUE_STATUS_NONE;
  PQUEUECTL		      ptCtl;
  PQUEUEDEF		      ptDef;
  XQUEUENUMENTRIES  xNumEntries;
  
  // check for a valid queue
  if ( eQueue < QUEUE_ENUM_MAX )
  {
    // get a pointer to the control/definition structures
    ptCtl = &atQueueCtls[ eQueue ];
    ptDef = ( PQUEUEDEF )&g_atQueueDefs[ eQueue ];
    
    // get the number of entries
    xNumEntries = PGM_RDBYTE( ptDef->xNumEntries );
    
    // is the queue empty
    if ( ptCtl->xCount == 0 )
    {
      // set the queue empty status
      eError = QUEUE_STATUS_QUEEMP;
    }
    else if ( ptCtl->xCount == xNumEntries )
    {
      // set the full error
      eError = QUEUE_STATUS_QUEFUL;
    }
  }
  else
  {
    // return the error
    eError = QUEUE_STATUS_ILLQUE;
  }
  
  // return the status
  return( eError );
}

/**@} EOF QueueManager.c */
