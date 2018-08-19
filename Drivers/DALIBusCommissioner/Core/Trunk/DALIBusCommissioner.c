/******************************************************************************
 * @file DALIBusCommissioner.c
 *
 * @brief DALI Bus commissioner implementation 
 *
 * This file provides the DALI Bus commissioner implementation
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusCommissioner
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusCommissioner/DALIBusCommissioner.h"

// library includes -----------------------------------------------------------
#include "SystemControlManager/SystemControlManager.h"
#include "StateExecutionEngine/StateExecutionEngine.h"
#if ( DALIBUSCOMMISSIONER_DEBUG_MODE != DALIBUSCOMMISSIONER_DEBUGMODE_NONE )
#include "DebugManager/DebugManager.h"
#endif // DALIBUSCOMMISSIONER_DEBUG_MODE

// Macros and Defines ---------------------------------------------------------
#define QUEUE_EVENT_PUT_RESPONSE  ( QUEUEPUT_EVENT( DALIBUSCOMMSSIONER_RCV_QUEUE ))

/// define the all device address
#define ALL_DEV_ADDRESS           ( 0xFF )

/// define the HI/MID/LO indices
#define SEARCH_ADDR_HI            ( LE_U32_MS2_IDX )
#define SEARCH_ADDR_MD            ( LE_U32_MS1_IDX )
#define SEARCH_ADDR_LO            ( LE_U32_LSB_IDX )

/// define the randomize wait time
#define RANDOMIZE_WAIT_TIME       ( TASK_TIME_MSECS( 100 ))

/// define the max/min address
#define SEARCH_ADDR_MIN           ( 0x00000000 )
#define SEARCH_ADDR_MAX           ( 0x01000000 )

/// define the number of devices per line
#define NUM_DEVICES_LINE          ( 8 )

// enumerations ---------------------------------------------------------------
/// determine the event argument size
#if ( TASK_TSKARG_SIZE_BYTES == 1 )
#define DALIBUSCOM_EXEC_EVENT		    ( 0XC0 )
#define DALIBUSCOM_ABORT_COMMISSION         ( 0xC1 )
#define DALIBUSCOM_START_COMMISSION_PARTIAL ( 0xC2 )
#define DALIBUSCOM_START_COMMISSION_FULL    ( 0xC3 )
#define DALIBUSCOM_EXIT_COMMISSION          ( 0xC4 )
#elif ( TASK_TSKARG_SIZE_BYTES == 2 )
#define DALIBUSCOM_EXEC_EVENT               ( 0xC0C0 )
#define DALIBUSCOM_ABORT_COMMISSION         ( 0xC1C1 )
#define DALIBUSCOM_START_COMMISSION_PARTIAL ( 0xC2C2 )
#define DALIBUSCOM_START_COMMISSION_FULL    ( 0xC3C3 )
#define DALIBUSCOM_EXIT_COMMISSION          ( 0xC4C4 )
#elif ( TASK_TSKARG_SIZE_BYTES == 4 )
#define DALIBUSCOM_EXEC_EVENT               ( 0xC0C0C0C0 )
#define DALIBUSCOM_ABORT_COMMISSION         ( 0xC1C1C1C1 )
#define DALIBUSCOM_START_COMMISSION_PARTIAL ( 0xC2C2C2C2 )
#define DALIBUSCOM_START_COMMISSION_FULL    ( 0xC3C3C3C3 )
#define DALIBUSCOM_EXIT_COMMISSION          ( 0xC4C4C4C4 )
#endif

/// enumerate the configuration bus states
typedef enum _CFBSTATE
{
  CFB_STATE_IDLE = 0,             ///< 0 - idle state
  CFB_STATE_FIND_SHORTADDR,       ///< 1 - find a device with this current address
  CFB_STATE_WAIT_SHORTADDR,       ///< 2 - wait for the response
  CFB_STATE_SEND_INITIALIZE,      ///< 3 - initialize the bus
  CFB_STATE_WAIT_INITIALIZE,      ///< 4 - wait for initialization
  CFB_STATE_SEND_RANDOMIZE,       ///< 5 - initiate a randomize command 
  CFB_STATE_WAIT_RANDOMIZE,       ///< 6 - wait for randomize delay to complete
  CFB_STATE_SEND_SEARCHADDRH,     ///< 7 - send the search address High byte
  CFB_STATE_SEND_SEARCHADDRM,     ///< 8 - send the search address middle byte
  CFB_STATE_SEND_SEARCHADDRL,     ///< 9 - send the search address low byte
  CFB_STATE_SEND_COMPARE,         ///< 10 - send a compare command
  CFB_STATE_SEND_PROGADDR,        ///< 11 - send a program address
  CFB_STATE_WAIT_PROGADDR,        ///< 12 - wait for program address delay to complete
  CFB_STATE_SEND_WITHDRAW,        ///< 13 - tell that unit to withdraw
  CFB_STATE_SEND_TERMINATE,       ///< 14 - send a terminate command
  CFB_STATE_ERROR,                ///< 15 - error state
  CFB_STATE_EXIT,                 ///< 16 - exit state
  CFB_STATE_MAX
} CFBSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  STATEEXECENGCONTROL tStateCtl;        ///< the state control structure
static  U8                  nCurDevIdx;       ///< current device index
static  U32UN               tPrvSearchAddr;   ///< previous search address
static  U32UN               tCurSearchAddr;   ///< current search address
static  U32UN               tMinAddr;         ///< minimum search address
static  U32UN               tMaxAddr;         ///< maximum search address
static  CFBSTATE            eNextState;       ///< next state
static  U8                  nCurFound;        ///< number of current found devices
static  BOOL                bFlushEvent;      ///< flush the event
static  BOOL                bSpecialTest;     ///< special test
static  BOOL                bFullCommission;  ///< full commission
static  U8                  nLastCmdEnum;     ///< last command enum

// local function prototypes --------------------------------------------------
static  U32   CalculateMidPoint( U32 uMinValue, U32 uMaxValue );
static  void  SendLocalMessage( U8 nAddress, U8 nData );
static  void  GetLocalResponse( PU8 nStatus, PU8 pnData );

/// command handlers
#if ( DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS == 1 )
static  ASCCMDSTS CmdQryCnt( U8 nCmdEnum );
static  ASCCMDSTS CmdQryDev( U8 nCmdEnum );
static  ASCCMDSTS CmdBusCom( U8 nCmdEnum );
#endif  // DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS

// CFB_STATE_IDLE functions
static  U8    CfbStateIdleExc( TASKARG xArg );
static  void  CfbStateIdleExt( void );

// CFB_STATE_FIND_SHORTADDR functions
static  void  CfbStateFindShortAddrEnt( void );
static  U8    CfbStateFindShortAddrExc( TASKARG xArg );

// CFB_STATE_WAIT_SHORTADDR functions
static  U8    CfbStateWaitShortAddrExc( TASKARG xArg );

// CFB_STATE_SEND_INITIALIZE functions
static  void  CfbStateSendInitializeEnt( void );
static  U8    CfbStateSendInitializeExc( TASKARG xArg );

// CFB_STATE_WAIT_INITIALIZE functions
static  U8    CfbStateWaitInitializeExc( TASKARG xArg );

// CFB_STATE_SEND_RANDOMIZE functions
static  void  CfbStateSendRandomizeEnt( void );
static  U8    CfbStateSendRandomizeExc( TASKARG xArg );

// CFB_STATE_WAIT_RANDOMIZE functions
static  void  CfbStateWaitRandomizeEnt( void );
static  U8    CfbStateWaitRandomizeExc( TASKARG xArg );

// CFB_STATE_SEND_SEARCHADDRH functions
static  void  CfbStateSendSearchAddrHEnt( void );

// CFB_STATE_SEND_SEARCHADDRM functions
static  void  CfbStateSendSearchAddrMEnt( void );

// CFB_STATE_SEND_SEARCHADDRL functions
static  void  CfbStateSendSearchAddrLEnt( void );

// CFB_STATE_SEND_COMPARE functions
static  void  CfbStateSendCompareEnt( void );
static  U8    CfbStateSendCompareExc( TASKARG xArg );

// CFB_STATE_SEND_PROGADDR functions
static  void  CfbStateSendProgAddrEnt( void );

// CFB_STATE_WAIT_PROGADDR functions
static  void  CfbStateWaitProgAddrEnt( void );
static  BOOL  CfbStateWaitProgAddrExc( TASKARG xArg );

// CFB_STATE_SEND_WITHDRAW functions
static  void  CfbStateSendWithdrawEnt( void );
static  void  CfbStateSendWithdrawExt( void );

// CFB_STATE_SEND_TERMINATE functions
static  void  CfbStateSendTerminateEnt( void );
static  void  CfbStateSendTerminateExt( void );

// CFB_STATE_ERROR functions
static  void  CfbStateErrorEnt( void );

// CFB_STATE_EXIT functions
static  void  CfbStateExitEnt( void );

// CFB_STATE_ANY functions
static  U8    CfbStateAnyExc( TASKARG xArg );

// constant parameter initializations -----------------------------------------
//******************************************************************************
// exit events
//******************************************************************************
static  const STATEEXECENGEVENT atCfbAbortEvents[ ] =
{
 STATEEXECENGEVENT_ENTRY( DALIBUSCOM_ABORT_COMMISSION, CFB_STATE_SEND_TERMINATE, FALSE ),
 STATEEXECENGEVENT_END( )
};

static  const STATEEXECENGEVENT atCfbExitEvents[ ] =
{
 STATEEXECENGEVENT_ENTRY( DALIBUSCOM_EXIT_COMMISSION, CFB_STATE_EXIT, FALSE ),
 STATEEXECENGEVENT_END( )
};

/// initialize the main state table
static  const STATEEXECENGTABLE  atCfbStates[ CFB_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( CFB_STATE_IDLE,             NULL,                       CfbStateIdleExc,           CfbStateIdleExt,          atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_FIND_SHORTADDR,   CfbStateFindShortAddrEnt,   CfbStateFindShortAddrExc,  NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_WAIT_SHORTADDR,   NULL,                       CfbStateWaitShortAddrExc,  NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_INITIALIZE,  CfbStateSendInitializeEnt,  CfbStateSendInitializeExc, NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_WAIT_INITIALIZE,  NULL,                       CfbStateWaitInitializeExc, NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_RANDOMIZE,   CfbStateSendRandomizeEnt,   CfbStateSendRandomizeExc,  NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_WAIT_RANDOMIZE,   CfbStateWaitRandomizeEnt,   CfbStateWaitRandomizeExc,  NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_SEARCHADDRH, CfbStateSendSearchAddrHEnt, CfbStateAnyExc,            NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_SEARCHADDRM, CfbStateSendSearchAddrMEnt, CfbStateAnyExc,            NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_SEARCHADDRL, CfbStateSendSearchAddrLEnt, CfbStateAnyExc,            NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_COMPARE,     CfbStateSendCompareEnt,     CfbStateSendCompareExc,    NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_PROGADDR,    CfbStateSendProgAddrEnt,    CfbStateAnyExc,            NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_WAIT_PROGADDR,    CfbStateWaitProgAddrEnt,    CfbStateWaitProgAddrExc,   NULL,                     atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_WITHDRAW,    CfbStateSendWithdrawEnt,    CfbStateAnyExc,            CfbStateSendWithdrawExt,  atCfbAbortEvents ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_SEND_TERMINATE,   CfbStateSendTerminateEnt,   CfbStateAnyExc,            CfbStateSendTerminateExt, atCfbExitEvents  ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_ERROR,            CfbStateErrorEnt,           NULL,                      NULL,                     atCfbExitEvents  ),
  STATEXECENGETABLE_ENTRY( CFB_STATE_EXIT,             CfbStateExitEnt,            NULL,                      NULL,                     NULL             ),
};

#if ( DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS == 1 )
/// define the string constants
static  const CODE C8 szHeader[ ]   = { "    0   1   2   3   4   5   6   7\n\r" };
static  const CODE C8 szRspCnt[ ]   = { "RCNT: %3d:%02X\n\r" };
static  const CODE C8 szModErr[ ]   = { "Unable to switch system to commission mode!\n\r" };
static  const CODE C8 szComMod[ ]   = { "Illegal bus commission mode!\n\r" };

/// declare the command strings
static  const CODE C8 szQryCnt[ ]   = { "QCNT" };  
static  const CODE C8 szQryDev[ ]   = { "QDEV" };
static  const CODE C8 szBusCom[ ]   = { "BCOM" };

/// define the device status strings
static  const CODE C8 szDevBad[ ]   = { "BAD " };
static  const CODE C8 szDevNpr[ ]   = { "--- " };
static  const CODE C8 szDevCol[ ]   = { "COL " };
static  const CODE C8 szDevUnk[ ]   = { "UNK " };
static  const CODE C8 szDevOK[ ]    = { "OK  " };

/// define the bus commission type string
static  const CODE C8 szComFull[ ]  = { "FULL" };
static  const CODE C8 szComPart[ ]  = { "PART" };
static  const CODE C8 szComStop[ ]  = { "STOP" };

/// initialize the command table
const CODE ASCCMDENTRY atDaliBusCommissionerCmdHandlerTable[ ] =
{
  ASCCMD_ENTRY( szQryCnt, 4, 0, ASCFLAG_COMPARE_NONE, 0, CmdQryCnt ),
  ASCCMD_ENTRY( szQryDev, 4, 0, ASCFLAG_COMPARE_NONE, 0, CmdQryDev ),
  ASCCMD_ENTRY( szBusCom, 4, 0, ASCFLAG_COMPARE_NONE, 0, CmdBusCom ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};
#endif  // DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS


/******************************************************************************
 * @function DALIBusCommissioner_Initialize
 *
 * @brief initialize the bus commisision
 *
 * This function will perform all initialization required for the bus commissioner
 *
 *****************************************************************************/
void DALIBusCommissioner_Initialize( void )
{
  // set the state engine
  tStateCtl.ptStates = atCfbStates;
    
  // initialize the state
  StateExecutionEngine_Initialize( &tStateCtl, CFB_STATE_IDLE );
  
  // clear the full commission mode
  bFullCommission = FALSE;
}

/******************************************************************************
 * @function DALIBusCommissioner_ProcessEvent
 *
 * @brief process the event
 *
 * This function willl process the event posted to this task
 *
 * @param[in]   xARg          task argument
 *
 * @return      TRUE if event is to be flushed, FALSE if not
 *
 *****************************************************************************/
BOOL DALIBusCommissioner_ProcessEvent( TASKARG xArg )
{
  // force the flush event flag
  bFlushEvent = TRUE;
  #if ( DALIBUSCOMMISSIONER_DEBUG_MODE > DALIBUSCOMMISSIONER_DEBUGMODE_NONE )
  DebugManager_AddElement( DALIBUSCOMMISSIONER_DEBUG_BASE | tStateCtl.nCurState, xArg  );
  #endif // DALIBUSCOMMISSIONER_DEBUG_MODE

  // process the event through the state engine
  StateExecutionEngine_Process( &tStateCtl, xArg );

  // return the status
  return( bFlushEvent );
}

/******************************************************************************
 * @function DALIBusCommissioner_Control
 *
 * @brief control the bus commissiioner
 *
 * This function will process the request for a bus commmissioner function
 *
 * @param[in]   xARg          task argument
 *
 *****************************************************************************/
void DALIBusCommissioner_Control( TASKARG tState )
{
  // test for non zero
  if ( tState != 0 )
  {
    // post the event
    TaskManager_PostEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, tState );

    // determine the abstracted start/stop function
    DALIBusCommissioner_CommissionStartStop(( tState == DALIBUSCOM_ABORT_COMMISSION ) ? FALSE : TRUE, nLastCmdEnum );
  }
}

/******************************************************************************
 * CFB_STATE_IDLE functions
 *****************************************************************************/
static U8 CfbStateIdleExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // determine mode
  switch( xArg )
  {
    case DALIBUSCOM_START_COMMISSION_PARTIAL :
      // set next state to find short address's/clear the full flag
      nNextState = CFB_STATE_FIND_SHORTADDR;
      bFullCommission = FALSE;
      break;

    case DALIBUSCOM_START_COMMISSION_FULL :
      // set next state to send initialize/set the full flag
      nNextState = CFB_STATE_SEND_INITIALIZE;
      bFullCommission = TRUE;
      break;

    default :
      break;
  }

  // return the next state
  return( nNextState );
}

static void CfbStateIdleExt( void )
{
  // for each device
  for( nCurDevIdx = 0; nCurDevIdx < DALI_MAX_NUM_OF_DEVICES; nCurDevIdx++ )
  {
    // reset the presence
    DALIBusCommissioner_SetDeviceTableEntry( nCurDevIdx, DALIBUSCOMMISSIONER_DEVSTS_NOTPRESENT );
  }

  // reset the device index
  nCurDevIdx = 0;
}

/******************************************************************************
 * CFB_STATE_FIND_SHORTADDRESS functions
 *****************************************************************************/
static void CfbStateFindShortAddrEnt( void )
{
  // post an event to cause execution to the next state
  TaskManager_PostEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXEC_EVENT );
}

static U8 CfbStateFindShortAddrExc( TASKARG xArg )
{
  // send the message
  SendLocalMessage((( nCurDevIdx << 1 ) | DALI_CMD_COMMAND_MASK ), DALI_CMD_QUERYSTATUS );

  // switch states
  return( CFB_STATE_WAIT_SHORTADDR );
}

/******************************************************************************
 * CFB_STATE_WAIT_SHORTADDRESS functions
 *****************************************************************************/
static U8 CfbStateWaitShortAddrExc( TASKARG xArg )
{
  U8  nNextState = CFB_STATE_FIND_SHORTADDR;
  U8  nStatus, nData;

  // make sure this is hour event
  if ( xArg == QUEUE_EVENT_PUT_RESPONSE )
  {
    // get the response
    GetLocalResponse( &nStatus, &nData );

    // now process the response
    switch( nStatus )
    {
      case DALIBUSCOMMISSIONER_MSGSTS_NOERRRCV :
        // mark as present
        DALIBusCommissioner_SetDeviceTableEntry( nCurDevIdx, DALIBUSCOMMISSIONER_DEVSTS_PRESENT  );
        break;

      case DALIBUSCOMMISSIONER_MSGSTS_COLLISION :
        // mark as present
        DALIBusCommissioner_SetDeviceTableEntry( nCurDevIdx, DALIBUSCOMMISSIONER_DEVSTS_COLLISION );
        break;

      default :
        nNextState = CFB_STATE_ERROR;
        break;
    } 

    // check for continuation
    if ( nNextState != CFB_STATE_ERROR )
    {
      // increment the index
      nCurDevIdx++;

      // now set the next state/do not execute exit
      if ( nCurDevIdx <  DALI_MAX_NUM_OF_DEVICES )
      {
        // set next state based on full/partial
        nNextState = ( bFullCommission == TRUE ) ? CFB_STATE_SEND_INITIALIZE : CFB_STATE_EXIT;
      }
    }
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * CFB_STATE_SEND_INITIALIZE functions
 *****************************************************************************/
static void CfbStateSendInitializeEnt( void )
{
  // reset the device index
  nCurDevIdx = 0;

  // post an event to cause execution to the next state
  TaskManager_PostEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXEC_EVENT );
}

static U8 CfbStateSendInitializeExc( TASKARG xArg )
{
  BOOL  bDeviceFound = FALSE;
  U8    nAddress;
  
  // is this a full initialization
  if ( bFullCommission == TRUE )
  {
    // set the address to full
    nAddress = 0;

    // clear the device found flag
    bDeviceFound = FALSE;
  }
  else
  {
    // find any device that was marked as collision
    while( !bDeviceFound && ( nCurDevIdx <  DALI_MAX_NUM_OF_DEVICES ))
    {
      // is this device collided
      if ( DALIBusCommissioner_GetDeviceTableEntry( nCurDevIdx ) == DALIBUSCOMMISSIONER_MSGSTS_COLLISION )
      {
        // set device found
        bDeviceFound = TRUE;
      }
      else
      {
        // increment the device index
        nCurDevIdx++;
      }
    }

    // set the address
    nAddress = ( bDeviceFound ) ? (( nCurDevIdx << 1 ) | 0x01 ) : ALL_DEV_ADDRESS ;
  }

  // set the command/address/send the message
  SendLocalMessage( DALI_CMD_INITIALIZE, nAddress );

  // switch states
  return( CFB_STATE_WAIT_INITIALIZE );
}

/******************************************************************************
 * CFB_STATE_WAIT_INITIALIZE functions
 *****************************************************************************/
static U8 CfbStateWaitInitializeExc( TASKARG xArg )
{
  U8  nNextState, nStatus, nData;

  // get the response
  GetLocalResponse( &nStatus, &nData );

  // check for ok
  switch( nStatus )
  {
    case DALIBUSCOMMISSIONER_MSGSTS_NOERROR :
      // if we are in full mode
      if ( bFullCommission == TRUE )
      {
        // set the next state to randomize
        nNextState = CFB_STATE_SEND_RANDOMIZE;
      }
      else
      {
        // now set the next state
        nNextState = ( nCurDevIdx <  DALI_MAX_NUM_OF_DEVICES ) ? CFB_STATE_SEND_INITIALIZE : CFB_STATE_SEND_RANDOMIZE;
        tStateCtl.bExecExit = FALSE;
      }
      break;

    default :
      nNextState = CFB_STATE_ERROR;
      break;
  }

  // return the next state
  return( nNextState );
}


/******************************************************************************
 * CFB_STATE_SEND_RANDOMIZE functions
 *****************************************************************************/
static void CfbStateSendRandomizeEnt( void )
{
  // send a randomize message
  SendLocalMessage( DALI_CMD_RANDOMIZE, 0 );

  // set the min/max addresses
  tMinAddr.uValue = SEARCH_ADDR_MIN;
  tMaxAddr.uValue = SEARCH_ADDR_MAX;
        
  // calculate the search address/force previous to a mismatch
  tCurSearchAddr.uValue = CalculateMidPoint( tMinAddr.uValue, tMaxAddr.uValue );
  tPrvSearchAddr.uValue = ~tCurSearchAddr.uValue;
        
  // clear the found count
  nCurFound = 0;
}

static U8 CfbStateSendRandomizeExc( TASKARG xArg )
{
  U8  nNextState, nStatus, nData;

  // get the response
  GetLocalResponse( &nStatus, &nData );

  // check for ok
  switch( nStatus )
  {
    case DALIBUSCOMMISSIONER_MSGSTS_NOERROR :
      // now set the next state
      nNextState = CFB_STATE_WAIT_RANDOMIZE;
      tStateCtl.bExecExit = FALSE;
      break;

    default :
      nNextState = CFB_STATE_ERROR;
      break;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * CFB_STATE_WAIT_RANDOMIZE functions
 *****************************************************************************/
static void CfbStateWaitRandomizeEnt( void )
{
  // clear the special test mode
  bSpecialTest = FALSE;

  // start a 100  MSEC timer
  TaskManager_StartTimer( DALIBUSCOMMISSIONER_PROCESS_TASK, RANDOMIZE_WAIT_TIME );
}

static U8 CfbStateWaitRandomizeExc( TASKARG xArg )
{
  U8  nNextState = CFB_STATE_ERROR;

  // process the event
  if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // set next state to send search address high
    nNextState = CFB_STATE_SEND_SEARCHADDRH;
  }

  // return next state
  return( nNextState );
}

/******************************************************************************
 * CFB_STATE_SEND_SEARCHADDRH functions
 *****************************************************************************/
static void CfbStateSendSearchAddrHEnt( void )
{
  // go to send search address medium
  eNextState = CFB_STATE_SEND_SEARCHADDRM;

  // check for identical search address
  if ( tCurSearchAddr.anValue[ SEARCH_ADDR_HI ] != tPrvSearchAddr.anValue[ SEARCH_ADDR_HI ] )
  {
    // set the previous
    tPrvSearchAddr.anValue[ SEARCH_ADDR_HI ] = tCurSearchAddr.anValue[ SEARCH_ADDR_HI ];
        
    // send the address high byte
    SendLocalMessage( DALI_CMD_SEARCHADDRH, tCurSearchAddr.anValue[ SEARCH_ADDR_HI ] );
  }
  else
  {
    // post an event to cause execution to the next state
    TaskManager_PostEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXEC_EVENT );
  }
}

/******************************************************************************
 * CFB_STATE_SEND_SEARCHADDRM functions
 *****************************************************************************/
static void CfbStateSendSearchAddrMEnt( void )
{
  // go to send search address medium
  eNextState = CFB_STATE_SEND_SEARCHADDRL;

  // check for identical search address
  if ( tCurSearchAddr.anValue[ SEARCH_ADDR_MD ] != tPrvSearchAddr.anValue[ SEARCH_ADDR_MD ] )
  {
    // set the previous
    tPrvSearchAddr.anValue[ SEARCH_ADDR_MD ] = tCurSearchAddr.anValue[ SEARCH_ADDR_MD ];
        
    // send the address high byte
    SendLocalMessage( DALI_CMD_SEARCHADDRM, tCurSearchAddr.anValue[ SEARCH_ADDR_MD ] );
  }
  else
  {
    // post an event to cause execution to the next state
    TaskManager_PostEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXEC_EVENT );
  }
}

/******************************************************************************
 * CFB_STATE_SEND_SEARCHADDRL functions
 *****************************************************************************/
static void CfbStateSendSearchAddrLEnt( void )
{
  // go to send search address medium
  eNextState = CFB_STATE_SEND_COMPARE;

  // check for identical search address
  if ( tCurSearchAddr.anValue[ SEARCH_ADDR_LO ] != tPrvSearchAddr.anValue[ SEARCH_ADDR_LO ] )
  {
    // set the previous
    tPrvSearchAddr.anValue[ SEARCH_ADDR_LO ] = tCurSearchAddr.anValue[ SEARCH_ADDR_LO ];
        
    // send the address high byte
    SendLocalMessage( DALI_CMD_SEARCHADDRL, tCurSearchAddr.anValue[ SEARCH_ADDR_LO ] );
  }
  else
  {
    // post an event to cause execution to the next state
    TaskManager_PostEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXEC_EVENT );
  }
}

/******************************************************************************
 * CFB_STATE_SEND_COMPARE functions
 *****************************************************************************/
static void CfbStateSendCompareEnt( void )
{
  // send a compare 
  SendLocalMessage( DALI_CMD_COMPARE, 0 );

  // clear the found
  nCurFound = 0;
}

static U8 CfbStateSendCompareExc( TASKARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  U8  nStatus, nData;

  if ( xArg == QUEUE_EVENT_PUT_RESPONSE )
  {
    // get the response
    GetLocalResponse( &nStatus, &nData );

    // check for ok
    switch( nStatus )
    {
      case DALIBUSCOMMISSIONER_MSGSTS_NOERROR :
      case DALIBUSCOMMISSIONER_MSGSTS_NOERRRCV :
      case DALIBUSCOMMISSIONER_MSGSTS_COLLISION :
        // increment the found count
        nCurFound++;

        // test for special test        
        if ( bSpecialTest == FALSE )
        {
          // set the maximum to the current search address
          tMaxAddr.uValue = tCurSearchAddr.uValue;
        }
        break;

      case DALIBUSCOMMISSIONER_MSGSTS_NOERRNORCV :
        // test for special test
        if ( bSpecialTest == FALSE )
        {
          // set the minimum to the current search address
          tMinAddr.uValue = tCurSearchAddr.uValue + 1;
        }
        break;

      default :
        nNextState = CFB_STATE_ERROR;
        break;
    }

    // check for no error
    if ( nNextState != CFB_STATE_ERROR )
    {
      // check for done
      if ( tMinAddr.uValue == tMaxAddr.uValue )
      {
        // check for special case
        if ( bSpecialTest == TRUE )
        {
          // check for found
          if ( nCurFound == 1 )
          {
            // set the next state to program address
            nNextState = CFB_STATE_SEND_PROGADDR;
          }
          else
          {
            // terminate
            nNextState = CFB_STATE_SEND_TERMINATE;
          }
        }
        else
        {
          // now see if there were any found
          if ( nCurFound == 0 )
          {
            // adjust the search address
            tCurSearchAddr.uValue++;

            // set the special test mode
            bSpecialTest = TRUE;

            // set the state back to send search address H
            nNextState = CFB_STATE_SEND_SEARCHADDRH;
          }
          else
          {
            // set next state to program address
            nNextState = CFB_STATE_SEND_PROGADDR;
          }
        }
      }
      else
      {
        // calculate a new search address
        tCurSearchAddr.uValue = CalculateMidPoint( tMinAddr.uValue, tMaxAddr.uValue );
      
        // set the state back to send search address H
        nNextState = CFB_STATE_SEND_SEARCHADDRH;
      }
    }
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * CFB_STATE_SEND_PROGADDR functions
 *****************************************************************************/
static void CfbStateSendProgAddrEnt( void )
{
  // find an empty short address
  for ( nCurDevIdx = 0; nCurDevIdx <  DALI_MAX_NUM_OF_DEVICES; nCurDevIdx++ )
  {
    // find out if this device is not present
    if ( DALIBusCommissioner_GetDeviceTableEntry( nCurDevIdx ) == DALIBUSCOMMISSIONER_DEVSTS_NOTPRESENT )
    {
      // set as present
      DALIBusCommissioner_SetDeviceTableEntry( nCurDevIdx, DALIBUSCOMMISSIONER_DEVSTS_PRESENT  );

      // exit the loop
      break;
    }
  }
  
  // check for full
  if ( nCurDevIdx >=  DALI_MAX_NUM_OF_DEVICES )
  {
    // table is full - set next state to terminate
    eNextState = CFB_STATE_SEND_TERMINATE;
    
    // force an entry
    TaskManager_PostPriorityEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXEC_EVENT );
  }
  else
  {
    // send a program short address message
    SendLocalMessage( DALI_CMD_PROGRAMSHORTADDR, ( nCurDevIdx << 1 ) | 0x01 );

    #if ( DALIBUSCOMMISSIONER_PROGWITH_DELAY != 0 )
    // next state is delay
    eNextState = CFB_STATE_WAIT_PROGADDR;
    #else
    // set the next state to send the the withdraw command
    eNextState = CFB_STATE_SEND_WITHDRAW;
    #endif
  }

  // clear the special test
  bSpecialTest = FALSE;
}

/******************************************************************************
 * CFB_STATE_WAIT_PROGADDR functions
 *****************************************************************************/
static void CfbStateWaitProgAddrEnt( void )
{
  // start a timer based on parameter
  TaskManager_StartTimer( DALIBUSCOMMISSIONER_PROCESS_TASK, TASK_TIME_MSECS( DALIBUSCOMMISSIONER_PROGWITH_DELAY ));
}

static U8 CfbStateWaitProgAddrExc( TASKARG xArg )
{
  U8  nNextState = CFB_STATE_ERROR;

  // process the event
  if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // set next state to send search address high
    nNextState = CFB_STATE_SEND_WITHDRAW;
  }

  // return next state
  return( nNextState );
}

/******************************************************************************
 * CFB_STATE_SEND_WITHDRAW functions
 *****************************************************************************/
static void CfbStateSendWithdrawEnt( void )
{
  // send a randomize message
  SendLocalMessage( DALI_CMD_WITHDRAW, 0 );
}

static void CfbStateSendWithdrawExt( void )
{
  // set the min/max addresses
  tMinAddr.uValue = tCurSearchAddr.uValue;
  tMaxAddr.uValue = SEARCH_ADDR_MAX;
  
  // calculate the search address/force previous to a mismatch
  tCurSearchAddr.uValue = CalculateMidPoint( tMinAddr.uValue, tMaxAddr.uValue );
  
  // clear the found count
  nCurFound = 0;
  bSpecialTest = 0;
}

/******************************************************************************
 * CFB_STATE_SEND_TERMINATE functions
 *****************************************************************************/
static void CfbStateSendTerminateEnt( void )
{
  // send a terminate message
  SendLocalMessage( DALI_CMD_TERMINATE, 0 );

  // force an exit
  TaskManager_PostPriorityEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXIT_COMMISSION );
}

static void CfbStateSendTerminateExt( void )
{
}

/******************************************************************************
 * CFB_STATE_ERROR functions
 *****************************************************************************/
static void CfbStateErrorEnt( void )
{
  // force an exit
  TaskManager_PostPriorityEvent( DALIBUSCOMMISSIONER_PROCESS_TASK, DALIBUSCOM_EXIT_COMMISSION );
}

/******************************************************************************
 * CFB_STATE_EXIT functions
 *****************************************************************************/
static void CfbStateExitEnt( void )
{
  // determine the abstracted start/stop function
  DALIBusCommissioner_CommissionStartStop( FALSE, nLastCmdEnum );

  // force an exit
}


/******************************************************************************
 * CFB_STATE_ANY functions
 *****************************************************************************/
static U8 CfbStateAnyExc( TASKARG xArg )
{
  U8  nNextState, nStatus, nData;
   
  if ( xArg == QUEUE_EVENT_PUT_RESPONSE )
  {
    // get the response
    GetLocalResponse( &nStatus, &nData );

    // check for ok
    switch( nStatus )
    {
      case DALIBUSCOMMISSIONER_MSGSTS_NOERROR :
        // now set the next state
        nNextState = eNextState;
        break;

      default :
        nNextState = CFB_STATE_ERROR;
        break;
    }
  }
  else if ( xArg == DALIBUSCOM_EXEC_EVENT )
  {
    // change states
    nNextState = eNextState;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * @function CalculateMidPoint
 *
 * @brief calculate a midpoint 
 *
 * This function will calculate a midpoint based on a minimum/maximum passed
 * values
 *
 * @param[in]   uMinValue     minimum value
 * @param[in]   uMaxValue     maximum value   
 *
 * @return      the calculated mid point value
 *
 *****************************************************************************/
static U32 CalculateMidPoint( U32 uMinValue, U32 uMaxValue )
{
  U32 uMidPoint;

  uMidPoint = ( uMaxValue + uMinValue ) >> 1;

  // add the element to debug
  #if ( DALIBUSCOMMISSIONER_DEBUG_MODE == DALIBUSCOMMISSIONER_DEBUGMODE_MAXIMUM  )
  DebugManager_AddElement( DALIBUSCOMMISSIONER_DEBUG_BASE | 0x90, (( uMidPoint >> 16 ) & 0xFFFF ));
  DebugManager_AddElement( DALIBUSCOMMISSIONER_DEBUG_BASE | 0x91, ( uMidPoint & 0xFFFF ));
  #endif // DALIBUSCOMMISSIONER_DEBUG_MODE

  // return the midpoint
  return( uMidPoint );
}

/******************************************************************************
 * @function SendLocalMessage
 *
 * @brief send a local message
 *
 * This function will send a local message to the DALI master
 *
 * @param[in]   nAddress        address
 * @param[in]   nData           
 *
 *****************************************************************************/
static void SendLocalMessage( U8 nAddress, U8 nData )
{
  // add the element to debug
  #if ( DALIBUSCOMMISSIONER_DEBUG_MODE >= DALIBUSCOMMISSIONER_DEBUGMODE_TXRXDATA )
  DebugManager_AddElement( DALIBUSCOMMISSIONER_DEBUG_BASE | 0xF0, ( nAddress << 8 ) | nData );
  #endif // DALIBUSCOMMISSIONER_DEBUG_MODE

  // send the message
  DALIBusCommissioner_PutMessage( nAddress, nData );
}

/******************************************************************************
 * @function GetLocalResponse
 *
 * @brief get a local response
 *
 * This function will get a response from the DALI Master response queue
 *
 * @param[io]   pnStatus      pointer to store the status
 * @param[io]   pnData        pointer to store the data
 *
 *****************************************************************************/
static void GetLocalResponse( PU8 pnStatus, PU8 pnData )
{
  // get the response
  DALIBusCommissioner_GetMessage( pnStatus, pnData );

  #if ( DALIBUSCOMMISSIONER_DEBUG_MODE >= DALIBUSCOMMISSIONER_DEBUGMODE_TXRXDATA )
  DebugManager_AddElement( DALIBUSCOMMISSIONER_DEBUG_BASE | 0x0080, ( *pnStatus << 8 ) | *pnData );
  #endif // DALIBUSCOMMISSIONER_DEBUG_MODE
}

#if ( DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS == 1 )
/******************************************************************************
 * @function CmdQryCnt
 *
 * @brief displays the numbers of devices found on bus
 *
 * This command will display to the terminal the current number of devices 
 * listed as present in the DALI device table
 *
 * @return  ASCCMD_STS_OUTPUTBUFFER
 *****************************************************************************/
static ASCCMDSTS CmdQryCnt( U8 nCmdEnum )
{
  U8  nCount;
  PC8 pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the device ount/ output it
  nCount = DALIBusCommissioner_GetDeviceCount( );
  SPRINTF_P( pcBuffer, ( char const * )szRspCnt, nCount, nCount );
  
  // return the output buffer status
  return ( ASCCMD_STS_OUTPUTBUFFER );
}

/******************************************************************************
 * @function CmdQryDev
 *
 * @brief displays the actual device table
 *
 * This command will dump the actual device table
 *
 * @return  ASCCMD_STS_OUTPUTBUFFER
 *****************************************************************************/
static ASCCMDSTS CmdQryDev( U8 nCmdEnum )
{
  U8            nDevIdx, nElemIdx, nBufCount;
  PC8           pcBuffer;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // output the header
  AsciiCommandHandler_OutputString( nCmdEnum, ( const PC8 )szHeader );
  
  // for each device
  for ( nDevIdx = 0; nDevIdx < DALI_MAX_NUM_OF_DEVICES; nDevIdx += NUM_DEVICES_LINE )
  {
    // clear the buffer count
    nBufCount = 0;

    // output the base device index
    nBufCount = SPRINTF_P( pcBuffer, ( const char* )g_szAsciiFmtHxb, nDevIdx );

    // now for each element
    for ( nElemIdx = 0; nElemIdx < NUM_DEVICES_LINE; nElemIdx++ )
    {
      // print the first element
      switch( DALIBusCommissioner_GetDeviceTableEntry( nDevIdx + nElemIdx ))
      {
        case DALIBUSCOMMISSIONER_DEVSTS_NOTPRESENT :
          nBufCount += SPRINTF_P(( pcBuffer + nBufCount ), szDevNpr );
          break;

        case DALIBUSCOMMISSIONER_DEVSTS_PRESENT  :
          nBufCount += SPRINTF_P(( pcBuffer + nBufCount ), szDevOK );
          break;

        case DALIBUSCOMMISSIONER_DEVSTS_COLLISION :
          nBufCount += SPRINTF_P(( pcBuffer + nBufCount ), szDevCol );
          break;

        case DALIBUSCOMMISSIONER_DEVSTS_BAD :
          nBufCount += SPRINTF_P(( pcBuffer + nBufCount ), szDevBad );
          break;
  
        default :
          nBufCount += SPRINTF_P(( pcBuffer + nBufCount ), szDevUnk );
          break;
      }
    }

    // now print a new line and output
    nBufCount += SPRINTF_P(( pcBuffer + nBufCount ), g_szAsciiNewLin );
    AsciiCommandHandler_OutputBuffer( nCmdEnum );
  }

  // force a new line
  AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )g_szAsciiNewLin );

  return( ASCCMD_STS_NONE );
}

/******************************************************************************
 * @function CmdBusCom
 *
 * @brief start/stop a bus comm
 *
 * This command will start/stop a bus commission
 *
 * @return  appropriate response
 *
 *****************************************************************************/
static ASCCMDSTS CmdBusCom( U8 nCmdEnum )
{
  PC8         pcBuffer;
  TASKARG     xState = 0;

  // store the command enum
  nLastCmdEnum = nCmdEnum;

  // get the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the argument
  AsciiCommandHandler_GetArg( nCmdEnum, 0, &pcBuffer );

  // check argument
  if ( STRCMP_P( pcBuffer, szComFull ) == 0 )
  {
    // start a full commission
    xState = DALIBUSCOM_START_COMMISSION_FULL;
  }
  else if ( STRCMP_P( pcBuffer, szComPart ) == 0 )
  {
    // start a partial commission
    xState = DALIBUSCOM_START_COMMISSION_PARTIAL;
  }
  else if ( STRCMP_P( pcBuffer, szComStop ) == 0 )
  {
    // stop the commission
    xState = DALIBUSCOM_ABORT_COMMISSION;
  }
  else
  {
    // report error
    AsciiCommandHandler_OutputString( nCmdEnum,( PC8 )szComMod );
  }

  // if we had a successful mode
  if (( xState == DALIBUSCOM_START_COMMISSION_FULL ) || ( xState == DALIBUSCOM_START_COMMISSION_PARTIAL ))
  {
    // attempt to set the system control manager into DALI Commission mode
    if ( SystemControlManager_SetMode( DALIBUSCOMMISSIONER_SYSCTRL_BUSCOM_MODE ) != SYSCTRLMNGR_ERROR_NONE )
    {
      // report error/clear the state
      AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szModErr );
      xState = 0;
    }
  }

  // now start the commission
  DALIBusCommissioner_Control( xState );

  // return status
  return( ASCCMD_STS_NONE );
}

#endif  // DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS

/**@} EOF DALIBusCommissioner.c */
