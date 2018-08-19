/******************************************************************************
 * @file DALIBusMaster.c
 *
 * @brief DALI Bus Master implementation 
 *
 * This file provides the implementation for the DALI bus master
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
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusMaster/DALIBusMaster.h"
#include "DALIBusMaster/DALIBusMaster_prm.h"

// library includes -----------------------------------------------------------
#include "StateExecutionEngine/StateExecutionEngine.h"
#if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
#include "DebugManager/DebugManager.h"
#endif // DALIBUSMASTER_ENABLE_DEBUG

// Macros and Defines ---------------------------------------------------------
/// define the bus error threshhold count
#define BUS_ERROR_THRESHOLD                 ( 8 )

/// define the events for transmit start/done
#define BUS_TRANSMIT_START_EVENT            ( 0x3E3E )
#define BUS_TRANSMIT_DONE_EVENT             ( 0xE3E3 )

/// define the wait times for receive
#define XMIT_WAIT_TIME_ECHO                 ( TASK_TIME_MSECS( 22 ))
#define INTERFRAME_WAIT_TIME                ( TASK_TIME_MSECS( 12 ))
#define RECV_WAIT_TIME                      ( TASK_TIME_MSECS( 16 ))
#define BACKFRM_WAIT_TIME                   ( TASK_TIME_MSECS(  9 ))

/// define the DALI response size
#define DALI_RESPONSE_SIZE                  ( sizeof( U8 ))

/// define the put event for the incoming queue
#define DALIBUSMASTER_XMTMSG_QUEUEEVENT     ( QUEUEPUT_EVENT( DALIBUSMASTER_XMTMSG_QUEUE_ENUM ))

/// define the macros for generating byte index, shift count, device status mask
#define COMPUTE_BYTE_INDEX( idx )           ( idx / DALIBUSMASTER_DEVSTS_MAX )
#define COMPUTE_SHIFT_COUNT( idx )          (( idx % DALIBUSMASTER_DEVSTS_MAX ) * DALI_DEV_SIZE_NUMBITS )
#define COMPUTE_DEVSTS_MASK( idx )          ( BITS( DALIBUSMASTER_DEVSTS_BAD, ( idx % DALIBUSMASTER_DEVSTS_MAX ) * DALI_DEV_SIZE_NUMBITS ))

// enumerations ---------------------------------------------------------------
/// enumerate the bus states
typedef enum _BUSSTATE
{
  BUS_STATE_IDLE = 0,           ///< idle state
  BUS_STATE_WAIT,               ///< wait for message to complete
  BUS_STATE_MAX
} BUSSTATE;

/// enumerate the transmit states
typedef enum _XMTSTATE
{
  XMT_STATE_IDLE          = 0,  ///< idle state
  XMT_STATE_WAITXMTDONE,        ///< wait for transmit to be done
  XMT_STATE_WAITXMTECHO,        ///< wait for the transmit echo
  XMT_STATE_WAITRCVDONE,        ///< wait for receive done
  XMT_STATE_WAITFRMTIME,        ///< wait for the expiration of the frame time
  XMT_STATE_WAITBFMTIME,        ///< wait for the expiration of the backward frame time 
  XMT_STATE_MAX
} XMTSTATE;

/// enumerate the response state
typedef enum _RSPTYPE
{
  RSP_TYPE_NONE       = 0,      ///< no response
  RSP_TYPE_MAYBE,               ///< maybe a response is required if a yes
  RSP_TYPE_REQUIRED,            ///< response required
  RSP_TYPE_ERROR      = 0xE0,   ///< group bit set on query command
} RSPTYPE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  STATEEXECENGCONTROL     tXmtStateCtl;     ///< transmit state control
static  STATEEXECENGCONTROL     tBusStateCtl;     ///< bus state control
static  BOOL                    bFlushEvent;      ///< flush the event
static  BOOL                    bEdgeEvent;       ///< edge event received
static  BOOL                    bBusError;        ///< bus error
static  U8                      nBusErrorCount;   ///< bus error count
static  RSPTYPE                 eAnswerExp;       ///< answer expected
static  BOOL                    bRepeatCmd;       ///< repeat command
static  BOOL                    bEdgeEvent;       ///< edge event received
static  DALIXMTMSG              tLclRcvMsg;       ///< local transmit message
static  DALIBUSMASTERMSG        tCurMessage;      ///< current message
static  DALIBUSMASTERDEVSTATUS  tDevStatus;       ///< device status

// local function prototypes --------------------------------------------------
static  RSPTYPE CheckForAnswer( PDALIXMTMSG ptMsg );
static  BOOL    CheckForRepeat( PDALIXMTMSG ptMsg );

//******************************************************************************
// bus state functions
//******************************************************************************
// BUS_STATE_WAIT - bus wait for message state function
static  void  BusStateWaitEnty( void );
static  U8    BusStateWaitExec( TASKARG xArg );

//******************************************************************************
// transmit state functions
//******************************************************************************
/// XMT_STATE_IDLE functions
static  U8    XmtStateIdleExc( TASKARG xArg );
static  void  XmtStateIdleExt( void );

/// XMT_STATE_ANY  exit function
static  void  XmtStateAnyExt( void );

/// XMT_STATE_WAITXMTDONE function
static  void  XmtStateWaitXmtDoneEnt( void );
static  U8    XmtStateWaitXmtDoneExc( TASKARG xArg );

/// XMT_STATE_WAITXMTECHO function
static  void  XmtStateWaitXmtEchoEnt( void );
static  U8    XmtStateWaitXmtEchoExc( TASKARG xArg );

/// XMT_STATE_WAITRCVDONE function
static  void  XmtStateWaitRcvDoneEnt( void );
static  U8    XmtStateWaitRcvDoneExc( TASKARG xArg );

/// XMT_STATE_WAITFRMTIME functions
static  void  XmtStateWaitFrmTimeEnt( void );
static  U8    XmtStateWaitFrmTimeExc( TASKARG xArg );

/// XMT_STATE_WAITFRMTIME functions
static  void  XmtStateWaitBfmTimeEnt( void );
static  U8    XmtStateWaitBfmTimeExc( TASKARG xArg );

// constant parameter initializations -----------------------------------------
// declare the status description
static  const C8  szStsNone[ ]        = { "XMTOK" };
static  const C8  szStsXmtOKRcvOK[ ]  = { "XMTOKRCVOK" };
static  const C8  szStsXmtOKRcvErr[ ] = { "XMTOKRCVERR" }; 
static  const C8  szStsXmtTmo[ ]      = { "XMTTMO" };
static  const C8  szStsBusErr[ ]      = { "BUSERR" };
static  const C8  szStsCmpErr[ ]      = { "CMPERR" };
static  const C8  szStsTmoRcv[ ]      = { "RCVTMO" };
static  const C8  szStsBusCol[ ]      = { "BUSCOL" };
static  const C8  szStsIllOpr[ ]      = { "ILLOPR" };
static  const C8  szStsBusMon[ ]      = { "BADBUS" };

/// define the table
static  const PC8 pszStatuses[ ] =
{
  ( PC8 )szStsNone, 
  ( PC8 )szStsXmtOKRcvOK,
  ( PC8 )szStsXmtOKRcvErr,
  ( PC8 )szStsXmtTmo,
  ( PC8 )szStsBusErr,
  ( PC8 )szStsCmpErr,
  ( PC8 )szStsTmoRcv,
  ( PC8 )szStsBusCol,
  ( PC8 )szStsIllOpr,
  ( PC8 )szStsBusMon
};

//******************************************************************************
// top level state transition table and state table
//******************************************************************************
static  const STATEEXECENGEVENT atBusIdleEvents[ ] =
{
 STATEEXECENGEVENT_ENTRY( DALIBUSMASTER_XMTMSG_QUEUEEVENT,     BUS_STATE_WAIT, FALSE ),
 STATEEXECENGEVENT_END( )
};

static  const STATEEXECENGTABLE  atBusStates[ BUS_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( BUS_STATE_IDLE, NULL,             NULL,             NULL,             atBusIdleEvents ),
  STATEXECENGETABLE_ENTRY( BUS_STATE_WAIT, BusStateWaitEnty, BusStateWaitExec, NULL,             NULL            ),
};

//******************************************************************************
// transmit state transition table and state table
//******************************************************************************
/// 
static  const CODE STATEEXECENGEVENT  atWaitTmoEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( TASK_TIMEOUT_EVENT, XMT_STATE_IDLE, TRUE  ),
  STATEEXECENGEVENT_END( )
};

/// initialize the main state table
const CODE STATEEXECENGTABLE  atTransmitHandlerStates[ XMT_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( XMT_STATE_IDLE,        NULL,                   XmtStateIdleExc,        XmtStateIdleExt,  NULL                ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_WAITXMTDONE, XmtStateWaitXmtDoneEnt, XmtStateWaitXmtDoneExc, XmtStateAnyExt,   atWaitTmoEvents     ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_WAITXMTECHO, XmtStateWaitXmtEchoEnt, XmtStateWaitXmtEchoExc, XmtStateAnyExt,   atWaitTmoEvents     ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_WAITRCVDONE, XmtStateWaitRcvDoneEnt, XmtStateWaitRcvDoneExc, XmtStateAnyExt,   NULL                ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_WAITFRMTIME, XmtStateWaitFrmTimeEnt, XmtStateWaitFrmTimeExc, XmtStateAnyExt,   NULL                ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_WAITBFMTIME, XmtStateWaitBfmTimeEnt, XmtStateWaitBfmTimeExc, XmtStateAnyExt,   NULL                ),
};

/******************************************************************************
 * @function DALIBusMaster_Initialize
 *
 * @brief initialization
 *
 * This function will perform any necessary initialization
 *
 *****************************************************************************/
void DALIBusMaster_Initialize( void )
{
  // set the state engine
  tXmtStateCtl.ptStates = atTransmitHandlerStates;
  tBusStateCtl.ptStates = atBusStates;

  // initialize the state at the top level table
  StateExecutionEngine_Initialize( &tBusStateCtl, BUS_STATE_IDLE );
  StateExecutionEngine_Initialize( &tXmtStateCtl, XMT_STATE_IDLE );
  
  // clear the bus error
  bBusError = FALSE;
  nBusErrorCount = 0;

  // get the bus configuration from storage
  DALIBusMaster_GetDeviceStatus( &tDevStatus );
}

/******************************************************************************
 * @function DALIBusMaster_GetStatusDescription
 *
 * @brief get the status description
 *
 * This function will return a pointer to a string of the error description
 *
 * @param[in]   eStatus     current status to translate
 *
 * @return      pointer to the string
 *
 *****************************************************************************/
PC8 DALIBusMaster_GetStatusDescription( DALIBUSMASTERSTS eStatus )
{
  PC8 pszDescription;

  // check for valid status
  if ( eStatus < DALIBUSMASTER_STS_MAX )
  {
    // get the description string
    pszDescription =  pszStatuses[ eStatus ];
  }
  else
  {
    pszDescription = pszStatuses[ DALIBUSMASTER_STS_MAX ];
  }

  // return the status
  return( pszDescription );
}

/******************************************************************************
 * @function  DALIBusMaster_GetDevPresentCnt
 *
 * @brief get the number of present devices
 *
 * This function will determine and return the number of active devices
 *
 * @return      the number of active devices
 *
 *****************************************************************************/
U8 DALIBusMaster_GetDevPresentCnt( void )
{
  U8 nIdx, nBytIdx, nBitShf, nBitMsk;
  U8 nDevPresentCnt = 0;
  
  // loop through device table  
  for ( nIdx = 0; nIdx < DALI_MAX_NUM_OF_DEVICES; nIdx++ )
  {
    // compute byte/bit index
    nBytIdx = COMPUTE_BYTE_INDEX( nIdx );
    nBitShf = COMPUTE_SHIFT_COUNT( nIdx );
    nBitMsk = COMPUTE_DEVSTS_MASK( nIdx );
    
    // check to see if device is present
    if ((( tDevStatus.tValues.anValues[ nBytIdx ] & nBitMsk ) >> nBitShf ) == DALIBUSMASTER_DEVSTS_PRESENT )
    {
      // increment device present count
      nDevPresentCnt++;
    }
  }

  // return the number of devices
  return ( nDevPresentCnt );
}

/******************************************************************************
 * @function DALIBuseMaster_GetDeviceStatusHuge
 *
 * @brief  devices in the status table 
 *
 * This function will return the a huge value with each device at a given status
 *
 * @param[in]   eOption     device count option
 *
 * @return      
 *
 *****************************************************************************/
U64 DALIBusMaster_GetDeviceStatusHuge( DALIBUSMASTERDEVSTS eStatus )
{
  U64   hDevSts;
  U8    nDevIdx, nBytIdx, nBitShf, nBitMsk;

  // clear the status value
  hDevSts = 0ll;
  
  // for each device
  for ( nDevIdx = 0; nDevIdx < DALI_MAX_NUM_OF_DEVICES; nDevIdx++ )
  {
    // compute byte/bit index
    nBytIdx = COMPUTE_BYTE_INDEX( nDevIdx );
    nBitShf = COMPUTE_SHIFT_COUNT( nDevIdx );
    nBitMsk = COMPUTE_DEVSTS_MASK( nDevIdx );

    // generate the membership set
    hDevSts !=((( tDevStatus.tValues.anValues[ nBytIdx ] & nBitMsk ) >> nBitShf ) == eStatus ) ? BIT( nDevIdx ) : 0;
  }

  // return the value
  return( hDevSts );
}

/******************************************************************************
 * @function DALIBusMaster_GetDeviceTableEntry
 *
 * @brief get a device table entry
 *
 * This function will return a device entry table
 *
 * @param[in]   nIndex    device table index
 *
 * @return      current device status
 *
 *****************************************************************************/
DALIBUSMASTERDEVSTS DALIBusMaster_GetDeviceTableEntry( U8 nIndex )
{
  DALIBUSMASTERDEVSTS eStatus = DALIBUSMASTER_DEVSTS_NOTPRESENT;
  U8                  nBytIdx, nBitShf, nBitMsk;

  // check for valid index
  if ( nIndex < DALI_MAX_NUM_OF_DEVICES )
  {
    // compute byte/bit index
    nBytIdx = COMPUTE_BYTE_INDEX( nIndex );
    nBitShf = COMPUTE_SHIFT_COUNT( nIndex );
    nBitMsk = COMPUTE_DEVSTS_MASK( nIndex );
      
    // set the device status
    eStatus = ( tDevStatus.tValues.anValues[ nBytIdx ] & nBitMsk ) >> nBitShf;
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function DALIBusMaster_SetDeviceTableEntry
 *
 * @brief set a device table entry
 *
 * This function will se the device entry status at the given index
 *
 * @param[in]   nIndex      device index
 * @param[in]   eStatus     device statsus
 *
 *****************************************************************************/
void DALIBusMaster_SetDeviceTableEntry( U8 nIndex, DALIBUSMASTERDEVSTS eStatus )
{
  U8  nBytIdx, nBitShf, nBitMsk;

  // check for valid index
  if ( nIndex < DALI_MAX_NUM_OF_DEVICES )
  {
    // compute byte/bit index
    nBytIdx = COMPUTE_BYTE_INDEX( nIndex );
    nBitShf = COMPUTE_SHIFT_COUNT( nIndex );
    nBitMsk = COMPUTE_DEVSTS_MASK( nIndex );
   
    // set the status
    tDevStatus.tValues.anValues[ nBytIdx ] &= ~nBitMsk;
    tDevStatus.tValues.anValues[ nBytIdx ] |= ( eStatus << nBitShf );
  }
}

/******************************************************************************
 * @function DALIBusMaster_ProcessBusEvent
 *
 * @brief process the bus event
 *
 * This function will process the bus event
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE if event handler, FALSE if not
 *
 *****************************************************************************/
BOOL DALIBusMaster_ProcessBusEvent( TASKARG xArg )
{
  BOOL  bFlushEvent = TRUE;
  
  // check for debug
  #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( DALIBUSMASTER_BUS_DEBUG_BASE | tBusStateCtl.nCurState, xArg );
  #endif
  
  // execute the state engine
  StateExecutionEngine_Process( &tBusStateCtl, xArg );
  
  // return the flush event flag
  return( bFlushEvent );
}

/******************************************************************************
 * @function DALIBusMaster_ProcessXmtEvent
 *
 * @brief process the transmit event
 *
 * This function will process the transmit event
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE 
 *
 *****************************************************************************/
BOOL DALIBusMaster_ProcessXmtEvent( TASKARG xArg )
{
  // check for debug
  #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE | tXmtStateCtl.nCurState, xArg );
  #endif
  
  // execute the state engine
  StateExecutionEngine_Process( &tXmtStateCtl, xArg );
  
  // return true to always flush event
  return( TRUE );
}

/******************************************************************************
 * @function DALIBusMaster_ProcessMonEvent
 *
 * @brief process the bus monitor
 *
 * This function will check the status of the bus and set the appropriate error
 * flags
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE
 *
 *****************************************************************************/
BOOL DALIBusMaster_ProcessMonEvent( TASKARG xArg )
{
  BOOL  bBusState;

  // check for enable
  if ( DALIBUSMASTER_BUSMON_GPIO_ENUM != GPIO_PIN_ENUM_ILLEGAL )
  {
    // determine the state of the pin
    Gpio_Get( DALIBUSMASTER_BUSMON_GPIO_ENUM, &bBusState );
  
    // now determine what to do
    if ( bBusState )
    {
      // increment the bus error count
      if (( ++nBusErrorCount == BUS_ERROR_THRESHOLD ) && ( bBusError == FALSE ))
      {
        // set the bus error flag
        bBusError = TRUE;
     }
    }
    else
    {
      // clear the count
      nBusErrorCount = 0;
      bBusError = FALSE;
    }
  }

  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function DALIBusMaster_StoreDevStatus
 *
 * @brief store the device status
 *
 * This function will call the abstracted function to store the device status
 *
 *****************************************************************************/
void DALIBusMaster_StoreDevStatus( void )
{
  // call the abstracted function
  //DALIBusMaster_StoreDevice( phStatus );
}

/******************************************************************************
 * @function CheckForAnswer
 *
 * @brief check and message for a answer
 *
 * This function checks to see if this message needs to be answer
 *
 * @param[in]   tLclMsg    DALI message
 *
 * @return      response type
 *
 *****************************************************************************/
static RSPTYPE CheckForAnswer(  PDALIXMTMSG ptMsg )
{
  RSPTYPE eAnswer = RSP_TYPE_NONE;

  // check for command
  if ( ptMsg->tFields.tAddr.bLvlCmd == TRUE )
  {
    // check for a compare or write memory location commands (extended) also check for application extended query commands
    if ((( ptMsg->anBuffer[ 0 ] == DALI_CMD_COMPARE ) && ( ptMsg->anBuffer[ 1 ] == 0x00 )) ||
         ( ptMsg->anBuffer[ 0 ] == DALI_CMD_WRITEMEMLOC ))
    {
      eAnswer = RSP_TYPE_MAYBE;
    }
    // check for a query short address command (special)
    else if ((( ptMsg->anBuffer[ 0 ] == DALI_CMD_QUERYSHORTADDR ) && ( ptMsg->anBuffer[ 1 ] == 0x00 )) ||
              ( ptMsg->anBuffer[ 0 ] == DALI_CMD_VERIFYSHORTADDR ))
    {
      // set response is required
      eAnswer = RSP_TYPE_REQUIRED;
    }
    // check for extended/special commands
    else if (( ptMsg->anBuffer[ 0 ] == DALI_CMD_INITIALIZE ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_RANDOMIZE ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_WITHDRAW ) ||
             ( ptMsg->anBuffer[ 0 ] == DALI_CMD_SEARCHADDRH ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_SEARCHADDRM ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_SEARCHADDRL ) ||
             ( ptMsg->anBuffer[ 0 ] == DALI_CMD_PROGRAMSHORTADDR ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_PROGRAMSHORTADDR ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_ENABLEDEVTYPE ) ||
             ( ptMsg->anBuffer[ 0 ] == DALI_CMD_DTR ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_DTR1 ) || ( ptMsg->anBuffer[ 0 ] == DALI_CMD_DTR2 ))
    {
      // set answer type to none
      eAnswer = RSP_TYPE_NONE;
    }          
    // check for query commands
    else if (( ptMsg->tFields.nDataCmd == DALI_CMD_QUERYSTATUS ) || 
       (( ptMsg->tFields.nDataCmd >= DALI_CMD_QUERYVERSIONNUM ) && ( ptMsg->tFields.nDataCmd <= DALI_CMD_QUERYPHYMINLVL )) ||
       (( ptMsg->tFields.nDataCmd >= DALI_CMD_QUERYCONTENTDTR1) && ( ptMsg->tFields.nDataCmd <= DALI_CMD_QUERYCONTENTDTR2 )) ||
       (( ptMsg->tFields.nDataCmd >= DALI_CMD_QUERYACTUALLVL ) && ( ptMsg->tFields.nDataCmd <= DALI_CMD_QUERYFADETIMERATE )) ||
       (( ptMsg->tFields.nDataCmd >= DALI_CMD_QUERYSCENELVL00 ) && ( ptMsg->tFields.nDataCmd <= DALI_CMD_READMEMLOCATION )) ||
       (( ptMsg->tFields.nDataCmd >= DALI_CMD_QUERYGEARTYPE ) && ( ptMsg->tFields.nDataCmd <= DALI_CMD_QUERYEXTVERNUM )))
        
    {
      // check for broadcast
      if ( ptMsg->tFields.tAddr.bDirGrp == FALSE )
      {
        // set response is required
        eAnswer = RSP_TYPE_REQUIRED;
      }
//      else
//      {
//        // check for DALI Debug mode
//        eAnswer = (( SYSCTRLMGRLCLMODE )SystemControlManager_GetMode( ) == DALIMASTERHANDLER_DALIDEBUG_MODE ) ? RSP_TYPE_REQUIRED : RSP_TYPE_ERROR;
//      }
    }
    else if ((( ptMsg->tFields.nDataCmd >= DALI_CMD_QUERYCONTROLGEAR ) && ( ptMsg->tFields.nDataCmd <= DALI_CMD_QUERYMISSINGSHORTADDR )) ||
             ( ptMsg->tFields.nDataCmd == DALI_CMD_QUERYPOWERFAILURE ))
    {
      // check for broadcast
      if ( ptMsg->tFields.tAddr.bDirGrp == FALSE )
      {
        // set response is required
        eAnswer = RSP_TYPE_MAYBE;
      }
//      else
//      {
//        // check for DALI Debug mode
//        eAnswer = (( SYSCTRLMGRLCLMODE )SystemControlManager_GetMode( ) == DALIMASTERHANDLER_DALIDEBUG_MODE ) ? RSP_TYPE_MAYBE : RSP_TYPE_ERROR;
//      }
    }
  }

  // return status
  return( eAnswer );
}

/******************************************************************************
 * @function CheckForRepeat
 *
 * @brief check message for repeat
 *
 * This function checks to see if this message needs to be sent twice
 *
 * @param[in]   tLclMsg    DALI message
 *
 * @return      TRUE if message needs to repeated
 *
 *****************************************************************************/
static BOOL CheckForRepeat( PDALIXMTMSG ptMsg )
{
  BOOL  bRepeat = FALSE;

  // check for initialize
  if ( ptMsg->anBuffer[ 0 ] == DALI_CMD_INITIALIZE )
  {
    // set auto repeat
    bRepeat = TRUE;
  }
  // check for auto repeat
  else if (( ptMsg->anBuffer[ 1 ] == 0 ) && ( ptMsg->anBuffer[ 0 ] == DALI_CMD_RANDOMIZE ))
  {
    // set auto repeat
    bRepeat = TRUE;
  }
  // check for config commands
  else if (( ptMsg->anBuffer[ 0 ] == DALI_ADDR_BROADCAST_COMMAND ) || (( ptMsg->tFields.tAddr.bLvlCmd == TRUE ) && ( ptMsg->tFields.tAddr.bDirGrp == FALSE )) )
  {
    if ((( ptMsg->anBuffer[ 1 ] >= DALI_CMD_RESET ) && ( ptMsg->anBuffer[ 1 ] <= DALI_CMD_ENABLEWRTMEM )) ||
        (( ptMsg->anBuffer[ 1 ] >= DALI_CMD_REFERENCESYSPOWER ) && ( ptMsg->anBuffer[ 1 ] <= DALI_CMD_SELECTDIMCURVE )))
    {
      // set the repeat
      bRepeat = TRUE;
    }
  }

  // return status
  return( bRepeat );
}

//******************************************************************************
// bus wait state functions
//******************************************************************************
static void BusStateWaitEnty( void )
{
  // Get the response to the DALI message
  QueueManager_Get( DALIBUSMASTER_XMTMSG_QUEUE_ENUM, ( PU8 )&tCurMessage );
  tCurMessage.tDaliXmtRcvMsg.nRcvMsg = 0;

  // now send it
  TaskManager_PostEvent( DALIBUSMASTER_XMT_TASK_ENUM, BUS_TRANSMIT_START_EVENT );
}

static U8 BusStateWaitExec( TASKARG xArg )
{
  U8              nNextState = 	STATEEXECENG_STATE_NONE;
  PVDALIBUSMSTRCB pvRcvCallback;

  // process the argument
  switch( xArg )
  {
    case BUS_TRANSMIT_DONE_EVENT :
      // get the callback function
      pvRcvCallback = tCurMessage.pvCallbackFunc;

      // if not null - execute it
      if ( pvRcvCallback != NULL )
      {
        // execuite it
        pvRcvCallback( &tCurMessage );
      }
      else
      {
        // post done
        QueueManager_PutTail( tCurMessage.nOption, ( PU8 )&tCurMessage );
      }

      // set next state back to idle
      nNextState = BUS_STATE_IDLE;
      break;

    default :
      break;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * XMT_STATE_IDLE functions
 *****************************************************************************/
static U8 XmtStateIdleExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // check the argument
  if ( xArg == BUS_TRANSMIT_START_EVENT )
  {
    // test for bus error
    if ( bBusError )
    {
      // set the error state
      tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_BUSMONITORBAD;
      
      // post the error event
      #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
      DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE | 0x0021, tCurMessage.tDaliXmtRcvMsg.eStatus );
      #endif // DALIBUSMASTER_ENABLE_DEBUG
      
      // set the done event in the bus state machine
      TaskManager_PostEvent( DALIBUSMASTER_BUS_TASK_ENUM, BUS_TRANSMIT_DONE_EVENT );
      
      // do not execute the exit function
      tXmtStateCtl.bExecExit = FALSE;
    }
    else
    {
      // clear the local/receive messages
      memset( &tLclRcvMsg, 0, DALIXMTMSG_SIZE );
  
      // check for a answer
      eAnswerExp = CheckForAnswer( &tCurMessage.tDaliXmtRcvMsg.tXmtMsg );

      // check for a repeat
      bRepeatCmd = CheckForRepeat( &tCurMessage.tDaliXmtRcvMsg.tXmtMsg );

      #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
      DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE | 0x0010, bRepeatCmd << 4 | eAnswerExp );
      DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE | 0x0080, (( tCurMessage.tDaliXmtRcvMsg.tXmtMsg.anBuffer[ 0 ] << 8 ) | tCurMessage.tDaliXmtRcvMsg.tXmtMsg.anBuffer[ 1 ] ));
      #endif // DALIBUSMASTER_ENABLE_DEBUG

      // now check for an illegal query operation
      if ( eAnswerExp == RSP_TYPE_ERROR )
      {
        // set the error state
        tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_ILLOPERATION;
    
        // post the error event
        #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
        DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE | 0x0021, tCurMessage.tDaliXmtRcvMsg.eStatus );
        #endif // DALIBUSMASTER_ENABLE_DEBUG
    
        // set the done event in the bus state machine
        TaskManager_PostEvent( DALIBUSMASTER_BUS_TASK_ENUM, BUS_TRANSMIT_DONE_EVENT );
      
        // do not execute the exit function
        tXmtStateCtl.bExecExit = FALSE;
      }
      else
      {
        // set the state to wait exit
        nNextState = XMT_STATE_WAITXMTDONE;
      }
    }
  }
 
  // return the next state
  return( nNextState );
}

static void XmtStateIdleExt( void )
{
  // now send the message
  DALIBusMaster_Receive(( PU8 )&tLclRcvMsg.anBuffer[ 0 ], DALIXMTMSG_SIZE );
  DALIBusMaster_Transmit(( PU8 )&tCurMessage.tDaliXmtRcvMsg.tXmtMsg.anBuffer[ 0 ], DALIXMTMSG_SIZE );

  // start a timer
  TaskManager_StartTimer( DALIBUSMASTER_XMT_TASK_ENUM, XMIT_WAIT_TIME_ECHO );
}

/******************************************************************************
 * XMT_STATE_WAITXMTDONE functions
 *****************************************************************************/
static void XmtStateWaitXmtDoneEnt( void )
{
  // set the transmit timeout flag
  tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_TMOXMT;
}

static BOOL XmtStateWaitXmtDoneExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // now determine the event
  switch( xArg )
  {
    case DALIBUSMASTER_TRANSMIT_DONE_EVENT :
      nNextState = XMT_STATE_WAITXMTECHO; 
      tXmtStateCtl.bExecExit = FALSE;
      break;
      
    case DALIBUSMASTER_RECEIVE_ERROR_EVENT :
      nNextState = XMT_STATE_IDLE;
      tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_BUSERR;
      break;
    
    default :
      break;
  }
  
  // now return the next state
  return( nNextState );
}

/******************************************************************************
 * XMT_STATE_WAITXMTECHO functions
 *****************************************************************************/
static void XmtStateWaitXmtEchoEnt( void )
{
  // set the transmit timeout flag
  tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_BUSERR;
}

static BOOL XmtStateWaitXmtEchoExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // now determine the event
  switch( xArg )
  {
    case DALIBUSMASTER_RECEIVE_DONE_EVENT :
      // stop the timer
      TaskManager_StopTimer( DALIBUSMASTER_XMT_TASK_ENUM );

      // clear the edge event
      bEdgeEvent = FALSE;

      // compare for equal
      if ( memcmp( &tLclRcvMsg, &tCurMessage.tDaliXmtRcvMsg.tXmtMsg, DALIXMTMSG_SIZE ) == 0 )
      {
        // good compare - determine if we are to wait for an echo
        if ( eAnswerExp != RSP_TYPE_NONE )
        {
          // next state is rcv done
          nNextState = XMT_STATE_WAITRCVDONE;
        }
        else
        {
          // next state is wait frm time/clear error
          nNextState = XMT_STATE_WAITFRMTIME;
          tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_NOERROR;
        }
        tXmtStateCtl.bExecExit = FALSE;
      }
      else
      {
        // set the compare error
        tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_CMPERR;
        
        // set the idle state
        nNextState = XMT_STATE_IDLE;
      }
      break;
      
    default :
      break;
  }
  
  // now return the next state
  return( nNextState );
}


/******************************************************************************
 * XMT_STATE_WAITRCVDONE functions
 *****************************************************************************/
static void XmtStateWaitRcvDoneEnt( void )
{
  // start a receive
  DALIBusMaster_Receive( &tCurMessage.tDaliXmtRcvMsg.nRcvMsg, DALI_RESPONSE_SIZE );
  tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_TMORCV;

  // start a timer
  TaskManager_StartTimer( DALIBUSMASTER_XMT_TASK_ENUM, RECV_WAIT_TIME );
}

static BOOL XmtStateWaitRcvDoneExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // now determine the event
  switch( xArg )
  {
    case DALIBUSMASTER_RECEIVE_EDGE_EVENT :
      // set the edge event
	    bEdgeEvent = TRUE;
      break;
    
    case DALIBUSMASTER_RECEIVE_DONE_EVENT :
      // stop the timer/set the answer rcvd flag/go to idle/
      TaskManager_StopTimer( DALIBUSMASTER_XMT_TASK_ENUM );
      nNextState = XMT_STATE_WAITBFMTIME;
      tCurMessage.tDaliXmtRcvMsg.eStatus = DALIBUSMASTER_STS_NOERRRCV;
      tXmtStateCtl.bExecExit = FALSE;
      break;

    case DALIBUSMASTER_RECEIVE_ERROR_EVENT :
      // go to wait frametime/set the error status/clear the repeat command
      nNextState = XMT_STATE_WAITFRMTIME;
      tCurMessage.tDaliXmtRcvMsg.eStatus = ( bEdgeEvent ) ? DALIBUSMASTER_STS_COLLISION : DALIBUSMASTER_STS_TMORCV;
      bRepeatCmd = FALSE;
      tXmtStateCtl.bExecExit = FALSE;
      break;
      
    case TASK_TIMEOUT_EVENT :
      // stop the receive/go to wait frametime/set the error status/clear the repeat command/
      DALIBusMaster_StopReceive( );
      nNextState = XMT_STATE_IDLE;
      tCurMessage.tDaliXmtRcvMsg.eStatus = ( eAnswerExp == RSP_TYPE_REQUIRED ) ? DALIBUSMASTER_STS_TMORCV : DALIBUSMASTER_STS_NOERRNORCV;
      break;
    
    default :
      break;
  }
  
  // now return the next state
  return( nNextState );
}

/******************************************************************************
* XMT_STATE_WAITFRMTIME functions
 *****************************************************************************/
static void XmtStateWaitFrmTimeEnt( void )
{
  // start a timer
  TaskManager_StartTimer( DALIBUSMASTER_XMT_TASK_ENUM, INTERFRAME_WAIT_TIME );
}

static U8 XmtStateWaitFrmTimeExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // process the event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // check for auto repeat
      if ( bRepeatCmd )
      {
        #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
        DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE, 0 );
        #endif // DALIBUSMASTER_ENABLE_DEBUG
        // clear the command
        bRepeatCmd = FALSE;

        // clear the local/receive messages
        memset( &tLclRcvMsg, 0, DALIXMTMSG_SIZE );
      
        // now send the message
        DALIBusMaster_Receive(( PU8 )&tLclRcvMsg.anBuffer[ 0 ], DALIXMTMSG_SIZE );
        DALIBusMaster_Transmit(( PU8 )&tCurMessage.tDaliXmtRcvMsg.tXmtMsg.anBuffer[ 0 ], DALIXMTMSG_SIZE );

        // start a timer
        TaskManager_StartTimer( DALIBUSMASTER_XMT_TASK_ENUM, XMIT_WAIT_TIME_ECHO );

        // set state back to wait transmit
        nNextState = XMT_STATE_WAITXMTDONE;

        // do not execute exit
        tXmtStateCtl.bExecExit = FALSE;
      }
      else
      {
        // set state back to idle/ execute the exit
        nNextState = XMT_STATE_IDLE;
        tXmtStateCtl.bExecExit = TRUE;
      }

      break;

    default :
      break;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
* XMT_STATE_WAITBFMTIME functions
 *****************************************************************************/
static void XmtStateWaitBfmTimeEnt( void )
{
  // start a timer
  TaskManager_StartTimer( DALIBUSMASTER_XMT_TASK_ENUM, BACKFRM_WAIT_TIME );
}

static U8 XmtStateWaitBfmTimeExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // process the event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // set state back to idle/execute the exit
      nNextState = XMT_STATE_IDLE;
      tXmtStateCtl.bExecExit = TRUE;
      break;

    default :
      break;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * XMT_STATE_ANY_EXIT functions
 *****************************************************************************/
static void XmtStateAnyExt( void )
{
  #if ( DALIBUSMASTER_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( DALIBUSMASTER_XMT_DEBUG_BASE | 0x0020, tCurMessage.tDaliXmtRcvMsg.eStatus );
  #endif // DALIBUSMASTER_ENABLE_DEBUG
  // set the done event in the bus state machine
  TaskManager_PostEvent( DALIBUSMASTER_BUS_TASK_ENUM, BUS_TRANSMIT_DONE_EVENT );
}

/**@} EOF DALIBusMaster.c */
