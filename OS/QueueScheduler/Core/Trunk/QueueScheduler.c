/******************************************************************************
 * @file  QueueScheduler.c 
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
 * \addtogroup QueueScheduler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "string.h"

// local includes -------------------------------------------------------------
#include "QueueScheduler/QueueScheduler.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PQUEUECTL )handle )

/// define the macro foe creating events
#define CREATE_QUEUE_EVENT( event, queue )    (( event << QUEUE_EVENT_SHIFT ) | queue )

/// define the queue event shift value
#if ( TASKSCHEDULER_EVENT_SIZE_BYTES == 1 )
  #define QUEUE_EVENT_SHIFT               ( 2 )
#elif ( TASKSCHEDULER_EVENT_SIZE_BYTES == 2 )
  #define QUEUE_EVENT_SHIFT               ( 8 )
#elif ( TASKSCHEDULER_EVENT_SIZE_BYTES == 4 )
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

// structures -----------------------------------------------------------------
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

/// define the queue control structure
typedef struct _QUEUECTL
{
  struct _QUEUECTL*   	ptSignature;   	///< my signature
  struct _QUEUECTL*   	ptNextQueue;   	///< pointer to the next structure
	PTASKSCHEDULERHANDLE 	ptTask;					///< task handler
	QUEUEEVNFLAGS					tEventFlags;		///< event flags
	U8										nNumEntries;		///< entry size
	U16										wEntrySize;			///< entry size
  U8										nRdIdx;					///< queue read index
  U8										nWrIdx;					///< queue write index
  U8										nCount;					///< count	
	PU8										pnStorage;			///< pointer to the queue storage
	U8										nEnum;					///< enumeration
} QUEUECTL, *PQUEUECTL;
#define	QUEUECTL_SIZE	sizeof( QUEUECTL )

// local parameter declarations -----------------------------------------------
static  PQUEUECTL    	ptFirstQueue;      ///< pointer to the first queue control structure

// local function prototypes --------------------------------------------------
static	void	PostEvent( PQUEUECTL ptQueue, BOOL bForceGet );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function QueueScheduler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any initialization required
 *
 *****************************************************************************/
void QueueScheduler_Initialize( void )
{
	// clear the first queue ointer
	ptFirstQueue = NULL;
}

/******************************************************************************
 * @function QueueScheduler_Create
 *
 * @brief queue creation
 *
 * This function will create a queue
 *
 * @param[in]   ptTask				task pointer
 * @param[in]		nNumEntreis		number of entries for this queue
 * @param[in]		WEntrySize		size of the entry
 * @param[in]		bEmptyEnb			empty event enable
 * @param[in]		bPutEnb				put event enable
 * @param[in]		bGetEnb				get event enable
 * @param[in]		bFullEnb			full event enable
 *
 * @return			returns the pointer to the queue or null if no room
 *
 *****************************************************************************/
PQUEUESCHEDULERHANDLE	QueueScheduler_Create( PTASKSCHEDULERHANDLE ptTask, U8 nNumEntries, U16 wEntrySize, BOOL bEmptyEnb, BOOL bPutEnb, BOOL bGetEnb, BOOL bFullEnb )
{
  PQUEUECTL  ptNewQueue, ptCurQueue, ptLstQueue;
  
  // allocate space for new task
  if (( ptNewQueue = malloc( QUEUECTL_SIZE )) != NULL )
  {
    // now allocate space for the number of entries
    if (( ptNewQueue->pnStorage = ( PU8 )malloc( nNumEntries * wEntrySize )) != NULL )
    {
			// now initialize the control structure
			ptNewQueue->ptSignature               = ptNewQueue;
			ptNewQueue->ptNextQueue               = NULL;
			ptNewQueue->ptTask                    = ptTask;
			ptNewQueue->tEventFlags.tBits.bEmpty  = bEmptyEnb;
			ptNewQueue->tEventFlags.tBits.bPut		= bPutEnb;
			ptNewQueue->tEventFlags.tBits.bGet		= bGetEnb;
			ptNewQueue->tEventFlags.tBits.bFull		= bFullEnb;
			ptNewQueue->nNumEntries               = nNumEntries;
			ptNewQueue->wEntrySize                = wEntrySize;
			ptNewQueue->nRdIdx                    = 0;
			ptNewQueue->nWrIdx                    = 0;
			ptNewQueue->nCount                    = 0;
			
			// now insert in list
			if ( ptFirstQueue == NULL )
			{
				// add it
				ptFirstQueue = ptNewQueue;
				ptNewQueue->nEnum = 1;
			}
			else
			{
				// search for the entry point
				for ( ptCurQueue = ptFirstQueue, ptLstQueue = NULL; ptCurQueue != NULL; ptLstQueue = ptCurQueue, ptCurQueue = ptCurQueue->ptNextQueue );

				// check for null
				if ( ptCurQueue == NULL )
				{
					// add at end
					ptLstQueue->ptNextQueue = ptNewQueue;
					ptNewQueue->nEnum = ptLstQueue->nEnum + 1;
				}
			}
		}
    else
    {
      // error - not enough room
      free( ptNewQueue );
      ptNewQueue = NULL;
    }
  }
  
  // return the handle
  return(( PTASKSCHEDULERHANDLE )ptNewQueue );
}

/******************************************************************************
 * @function QueueScheduler_Get
 *
 * @brief gets an entry from the queue
 *
 * This function will get a entry from the queue if one is available
 *
 * @param[in]   ptQueue  	pointer to the queue
 * @param[io]   pnEntry the pointer to store the queue entry in
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueScheduler_Get( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry )
{
  QUEUESTATUS   eError = QUEUE_STATUS_NONE;
  PQUEUECTL     ptSelQueue;
  
  // map the pointer
  ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
  // first validate that the task is valid
  if ( ptSelQueue == ptSelQueue->ptSignature )
  {
    // is there anything in the queue
    if ( ptSelQueue->nCount != 0 )
    {
      // disable interrupts
      Interrupt_Disable( );
      
      // now copy the entry to the pointer
      memcpy( pnEntry, ( ptSelQueue->pnStorage + ( ptSelQueue->nRdIdx * ptSelQueue->wEntrySize )), ptSelQueue->wEntrySize );
      
      // adjust the read index
      ptSelQueue->nRdIdx++;
      if ( ptSelQueue->nRdIdx >= ptSelQueue->nNumEntries )
      {
        // reset back to zero
        ptSelQueue->nRdIdx = 0;
      }
      
      // decrement  the count
      ptSelQueue->nCount--;
      
      // re-enable interrupts
      Interrupt_Enable( );
			
			// now post event
			PostEvent( ptSelQueue, TRUE );
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
 * @function QueueScheduler_PutTail
 *
 * @brief puts a value onto the tail of the quueue
 *
 * This function checks for empty space if available, puts the event at the end
 * of the queue.  It will also post an appropriate event to the desired task
 *
 * @param[in]   ptQueue  	pointer to the queue
 * @param[in]   pnEntry the pointer to the queue entry
 *
 * @return      a QUEUESTATUS value based on results
 *
 *****************************************************************************/
QUEUESTATUS QueueScheduler_PutTail( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry )
{
  QUEUESTATUS   eError = QUEUE_STATUS_NONE;
  PQUEUECTL     ptSelQueue;
  
  // map the pointer
  ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
  // first validate that the task is valid
  if ( ptSelQueue == ptSelQueue->ptSignature )
  {
		// check for room
		if ( ptSelQueue->nCount < ptSelQueue->nNumEntries )
		{
			// disable interrupts
			Interrupt_Disable( );
			
			// now copy the data to the queue
			memcpy( ptSelQueue->pnStorage + ( ptSelQueue->nWrIdx * ptSelQueue->wEntrySize ), pnEntry, ptSelQueue->wEntrySize );
			
      // adjust the write index
			ptSelQueue->nWrIdx++;
      if ( ptSelQueue->nWrIdx >= ptSelQueue->nNumEntries )
      {
        // reset the index to the end
        ptSelQueue->nWrIdx = 0;
      }
			
			// increment the count
			ptSelQueue->nCount++;
			
			// re-enable interrupts
			Interrupt_Enable( );
			
			// now post event
			PostEvent( ptSelQueue, FALSE );
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

#if ( QUEUESCHEDULER_INCLUDE_PUTHEAD == TRUE )
  /******************************************************************************
   * @function QueueScheduler_PutHead
   *
   * @brief puts a value onto the head of the quueue
   *
   * This function checks for empty space if available, puts the event at the head
   * of the queue.  It will also post an appropriate event to the desired task
   *
   * @param[in]   ptQueue  	pointer to the queue
   * @param[in]   pnEntry 	the pointer to the queue entry
   *
   * @return      a QUEUESTATUS value based on results
   *
   *****************************************************************************/
  QUEUESTATUS QueueScheduler_PutHead( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry )
  {
    QUEUESTATUS   eError = QUEUE_STATUS_NONE;
    PQUEUECTL     ptSelQueue;
  
    // map the pointer
    ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
    // first validate that the task is valid
    if ( ptSelQueue == ptSelQueue->ptSignature )
    {
      // check for room
      if ( ptSelQueue->nCount < ptSelQueue->nNumEntries )
      {
        // disable interrupts
        Interrupt_Disable( );
			
        // adjust the read pointer
        if ( ptSelQueue->nRdIdx == 0 )
        {
          // reset the index to the end
          ptSelQueue->nRdIdx = ptSelQueue->nNumEntries - 1;
        }
        else
        {
          // increment the read index
          ptSelQueue->nRdIdx++;
        }
			
        // now copy the data to the queue
        memcpy( ptSelQueue->pnStorage + ( ptSelQueue->nRdIdx * ptSelQueue->wEntrySize ), pnEntry, ptSelQueue->wEntrySize );
			
        // increment the count
        ptSelQueue->nCount++;
			
        // re-enable interrupts
        Interrupt_Enable( );
			
        // now post event
        PostEvent( ptSelQueue, FALSE );
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
#endif

#if ( QUEUESCHEDULER_INCLUDE_PEEK == TRUE )
  /******************************************************************************
   * @function QueueScheduler_Peek
   *
   * @brief gets an entry from the queue
   *
   * This function will get a entry from the queue if one is available but not 
   * remove from the queue
   *
   * @param[in]   ptQueue  	pointer to the queue
   * @param[io]   pnEntry the pointer to store the queue entry in
   *
   * @return      a QUEUESTATUS value based on results
   *
   *****************************************************************************/
  QUEUESTATUS QueueScheduler_Peek( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnEntry )
  {
    QUEUESTATUS   eError = QUEUE_STATUS_NONE;
    PQUEUECTL     ptSelQueue;
  
    // map the pointer
    ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
    // first validate that the task is valid
    if ( ptSelQueue == ptSelQueue->ptSignature )
    {
      // is there anything in the queue
      if ( ptSelQueue->nCount != 0 )
      {
        // disable interrupts
        Interrupt_Disable( );
      
        // now copy the entry to the pointer
        memcpy( pnEntry, ( ptSelQueue->pnStorage + ( ptSelQueue->nRdIdx * ptSelQueue->wEntrySize )), ptSelQueue->wEntrySize );
      
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
#endif

#if ( QUEUESCHEDULER_INCLUDE_POP == TRUE )
  /******************************************************************************
   * @function QueueScheduler_Pop
   *
   * @brief removes an entry from the queue
   *
   * This function will pop an entry from the queue if one is there
   *
   * @param[in]   ptQueue  	pointer to the queue
   *
   * @return      a QUEUESTATUS value based on results
   *
   *****************************************************************************/
  QUEUESTATUS QueueScheduler_Pop( PQUEUESCHEDULERHANDLE ptQueue )
  {
    QUEUESTATUS   eError = QUEUE_STATUS_NONE;
    PQUEUECTL     ptSelQueue;
  
    // map the pointer
    ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
    // first validate that the task is valid
    if ( ptSelQueue == ptSelQueue->ptSignature )
    {
      // is there anything in the queue
      if ( ptSelQueue->nCount != 0 )
      {
        // disable interrupts
        Interrupt_Disable( );
      
        // adjust the read index
        ptSelQueue->nRdIdx++;
        if ( ptSelQueue->nRdIdx >= ptSelQueue->nNumEntries )
        {
          // reset back to zero
          ptSelQueue->nRdIdx = 0;
        }
      
        // decrement  the count
        ptSelQueue->nCount--;
      
        // re-enable interrupts
        Interrupt_Enable( );
			
        // now post event
        PostEvent( ptSelQueue, TRUE );
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
#endif

#if ( QUEUESCHEDULER_INCLUDE_FLUSH == TRUE )
  /******************************************************************************
   * @function QueueScheduler_Flush
   *
   * @brief flush all entries
   *
   * This function will flush all entries in the given queue and clear the indices
   *
   * @param[in]   ptQueue  	pointer to the queue
   *
   * @return      a QUEUESTATUS value based on results
   *
   *****************************************************************************/
  QUEUESTATUS QueueScheduler_Flush( PQUEUESCHEDULERHANDLE ptQueue )
  {
    QUEUESTATUS   eError = QUEUE_STATUS_NONE;
    PQUEUECTL     ptSelQueue;
  
    // map the pointer
    ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
    // first validate that the task is valid
    if ( ptSelQueue == ptSelQueue->ptSignature )
    {
      // disable interrupts
      Interrupt_Disable( );
    
      // clear the indices
      ptSelQueue->nWrIdx = 0;
      ptSelQueue->nRdIdx = 0;
      ptSelQueue->nCount = 0;
    
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
#endif

#if ( QUEUESCHEDULER_INCLUDE_GETREMAINING == TRUE )
  /******************************************************************************
   * @function QueueScheduler_GetRemaining
   *
   * @brief get the number of entries in the queue
   *
   * This function will return the number of entries still in the queue
   *
   * @param[in]   ptQueue  	pointer to the queue
   * @param[io]   pnRemaining the pointer to store the number of entries remaining
   *
   * @return      a QUEUESTATUS value based on results
   *
   *****************************************************************************/
  QUEUESTATUS QueueScheduler_GetRemaining( PQUEUESCHEDULERHANDLE ptQueue, PU8 pnRemaining )
  {
    QUEUESTATUS   eError = QUEUE_STATUS_NONE;
    PQUEUECTL     ptSelQueue;
  
    // map the pointer
    ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
    // first validate that the task is valid
    if ( ptSelQueue == ptSelQueue->ptSignature )
    {
      // set the remiaining equal to the count
      *( pnRemaining ) = ptSelQueue->nCount;
    }
    else
    {
      // return the error
      eError = QUEUE_STATUS_ILLQUE;
    }
  
    // return the status
    return( eError );
  }
#endif

#if ( QUEUESCHEDULER_INCLUDE_GETSTATUS == TRUE )
  /******************************************************************************
   * @function QueueScheduler_GetStatus
   *
   * @brief get the queue status
   *
   * This function will return the current status as to full/empty/illegal
   *
   * @param[in]   ptQueue  	pointer to the queue
   *
   * @return      a QUEUESTATUS value based on results
   *
   *****************************************************************************/
  QUEUESTATUS QueueScheduler_GetStatus( PQUEUESCHEDULERHANDLE ptQueue )
  {
    QUEUESTATUS   eError = QUEUE_STATUS_NONE;
    PQUEUECTL     ptSelQueue;
  
    // map the pointer
    ptSelQueue = MAP_HANDLE_TO_POINTER( ptQueue );
  
    // first validate that the task is valid
    if ( ptSelQueue == ptSelQueue->ptSignature )
    {
      // is the queue empty
      if ( ptSelQueue->nCount == 0 )
      {
        // set the queue empty status
        eError = QUEUE_STATUS_QUEEMP;
      }
      else if ( ptSelQueue->nCount == ptSelQueue->nNumEntries )
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
#endif

/******************************************************************************
 * @function PostEvent
 *
 * @brief poset event
 *
 * This function will post an event
 *
 * @param[in]   ptQueue  	pointer to the queue
 * @param[in]		bForceGet	force a get event 
 *
 *****************************************************************************/
static void	PostEvent( PQUEUECTL ptQueue, BOOL bForceGet )
{
	U8	nEvent = 0;
	
	// check for get
	if ( bForceGet )
	{
		// is it enabled
		if ( ptQueue->tEventFlags.tBits.bGet )
		{
			// set the event
			nEvent = QUEUE_EVENT_GET;
		}
		
		// check for empty
		if (( ptQueue->nCount == 0 ) && ( ptQueue->tEventFlags.tBits.bEmpty ))
		{
			// set the evemt
			nEvent = QUEUE_EVENT_GETEMPTY;
		}
	}
	else
	{
		// check put event
		if ( ptQueue->tEventFlags.tBits.bPut )
		{
			// set the event
			nEvent = QUEUE_EVENT_PUT;
		}
		
		// check for full
		if (( ptQueue->nCount == ptQueue->nNumEntries ) && ( ptQueue->tEventFlags.tBits.bFull ))
		{
			// or the empty event
			nEvent = QUEUE_EVENT_PUTFULL;
		}
		
		// check if event is present
		if ( nEvent != 0 )
		{
			// post the event
			TaskScheduler_PostEvent( ptQueue->ptTask, ( TASKSCHEDULEREVENT )CREATE_QUEUE_EVENT( nEvent, ptQueue->nEnum ));
		}
	}
}

/**@} EOF QueueScheduler.c */
