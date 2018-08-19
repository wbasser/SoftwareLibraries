/******************************************************************************
 * @file  DALIMessageHandler.c
 *
 * @brief
 *
 * This file
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ParameterManager/ParameterManager.h"
#include "ManchesterCodec/ManchesterCodec.h"

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler.h"
#include "DALIMessageHandler/NormalCommands/NormalCommands.h"
#include "DALIMessageHandler/SpecialCommands/SpecialCommands.h"
#include "DALIMessageHandler/ExtendedCommands/ExtendedCommands.h"
#if ( DALI_ENABLE_DEBUG == 1 )
#include "DebugCommands/DebugCommands.h"
#endif // DALI_DUMP_MSG

// Macros and Defines ---------------------------------------------------------
/// define the normal indirect level command max
#define NRM_INDIRECT_LVLCMD_MAX     ( 32 )

/// define the bus failue time
#define BUSFAIL_TIMEOUT             ( 500 )

/// define the number of consecutive bus fail events
#define NUM_BUSFAIL_EVENTS          ( TASK_TIME_MSECS( BUSFAIL_TIMEOUT ) / DALIMESSAGE_BUSFAIL_EXEC_RATE )

/// define the value for a dali mask value
#define DALI_MASK_VALUE             ( 0xFF )

/// define reset state value of actual dim level
#define ACTUAL_DIM_LVL_RESET        ( 0xFE )

// enumerations ---------------------------------------------------------------
/// enumerate the parse status
typedef enum _PARSESTS
{
  PARSE_STS_NONE = 0,         ///< no action
  PARSE_STS_REQ100MSTIMER,    ///< request a 100 millisecond timer be started
  PARSE_STS_SENDRESPONSE,     ///< send a response
  PARSE_STS_CLEARALLTIMER,    ///< clear the 100/200 millisecond timer
  PARSE_STS_REQ200MSTIMER,    ///< request a 200 millisecond timer
} PARSESTS;

/// enuemrate the parameter source
typedef enum _PARAMSRC
{
  PARAM_SRC_EEPROM = 0,       ///< EEPROM
  PARAM_SRC_DEFLT             ///< default
} PARAMSRC;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	DALICTL		tDaliCtl;						///< dali control structure

// local function prototypes --------------------------------------------------
static  PARSESTS	ParseCommand( void );
static  PARSESTS	ProcessCommandEntry( PCMDTBL ptCmdEntry, PBOOL pbExecuteCmd );
static	void			ProcessOutputChange( void );
static  void      SetDALIOff( void );
static  void      GetDefaultParameters( void );
static  void      GetAllParameters( PARAMSRC eSrc );
static  PARAMARG  GetParameterValue( PARAMSRC eSrc, PARAMSELENUM eParam );
static  void      SetStartupCondition( void );

// constant parameter initializations -----------------------------------------
// logarithmic dimming curve in tenths of a percent
#if ( DALI_DISABLE_DFLT_DIMTABLE == 0 )
static	const CODE	U16 awDimPercent[ ] =
{
    0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   5,   5,   5,   5,
    5,   5,   5,   6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   8,   8,
    8,   8,   9,   9,   9,   9,  10,  10,  10,  11,  11,  11,  12,  12,  12,  13,
   13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,  20,
   20,  21,  21,  22,  23,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  31,
   32,  32,  33,  34,  35,  36,  37,  38,  39,  40,  42,  43,  44,  45,  46,  48,
   49,  50,  52,  53,  55,  56,  58,  60,  61,  63,  65,  67,  68,  70,  72,  74,
   76,  78,  81,  83,  85,  88,  90,  92,  95,  98, 100, 103, 106, 109, 112, 115,
  118, 122, 125, 129, 132, 136, 140, 143, 147, 151, 156, 160, 164, 169, 174, 171,
  184, 189, 194, 199, 205, 210, 216, 222, 228, 235, 241, 248, 255, 262, 269, 277,
  284, 292, 300, 309, 317, 326, 335, 344, 354, 364, 374, 384, 395, 406, 417, 428,
  440, 453, 465, 478, 491, 505, 519, 533, 548, 563, 579, 595, 611, 628, 646, 663,
  682, 701, 720, 740, 761, 782, 803, 826, 848, 872, 896, 921, 946, 973, 1000
};
#endif // DALI_DISABLE_DFLT_DIMTABLE

/******************************************************************************
 * @function  DALIMessageHandler_Initialize
 *
 * @brief initialie the DALI message handler
 *
 * This function will initialize the DALI message handler
 *
 *****************************************************************************/
void DALIMessageHandler_Initialize( void )
{
  // clear the initialized flag
  tDaliCtl.bDaliInitialized = FALSE;

  // call the initializations for normal/special/extended commands
  NormalCommands_Initialize( );
  SpecialCommands_Initialize( );
  ExtendedCommands_Initialize( );

  // clear the forced message flag
  tDaliCtl.bForcedMessage = FALSE;

  // clear the bus fail count
  tDaliCtl.nBusFailCount = 0;

  // set the initial bus power off
  tDaliCtl.bTestBusPowerOn = FALSE;
}

/******************************************************************************
 * @function  DALIMessageHandler_Control
 *
 * @brief enable/disable the DALI message handler
 *
 * This function will enable disable the DALI message handler based on the
 * mode of the system.
 *
 * @param[in]   bState	the desired state of the DALI Message handler
 *
 *****************************************************************************/
void DALIMessageHandler_Control( BOOL bState )
{
  U32UN			tSeed;

  // check the requested state
  if (( bState == TRUE ) && ( tDaliCtl.bDaliInitialized == FALSE ))
  {
    // set the initialized flag
    tDaliCtl.bDaliInitialized = TRUE;

    // get all the parameters
    GetAllParameters( PARAM_SRC_EEPROM );
    tDaliCtl.tDaliPrm.nShortAddress = GetParameterValue( PARAM_SRC_EEPROM, PARAMSEL_DALI_SHORT_ADDR );

    // update status bits
    tDaliCtl.tDaliSts.tBits.bPowerFailure = TRUE;

    // clear any flags
    tDaliCtl.tFlags.b15MinTimerRunning = FALSE;
    
    // seed the random number generator from the serial number
    tSeed.anValue[ LE_U32_MSB_IDX ] = GetParameterValue( PARAM_SRC_EEPROM, PARAMSEL_DALI_MB0_OEM_SNUM1 );
    tSeed.anValue[ LE_U32_MS1_IDX ] = GetParameterValue( PARAM_SRC_EEPROM, PARAMSEL_DALI_MB0_OEM_SNUM2 );
    tSeed.anValue[ LE_U32_MS2_IDX ] = GetParameterValue( PARAM_SRC_EEPROM, PARAMSEL_DALI_MB0_OEM_SNUM3 );
    tSeed.anValue[ LE_U32_LSB_IDX ] = GetParameterValue( PARAM_SRC_EEPROM, PARAMSEL_DALI_MB0_OEM_SNUM4 );
    DALIMessageHandler_SeedRandom( tSeed.uValue );

    // set the startup condition
    SetStartupCondition( );
  }
  else if ( bState == FALSE )
  {
    // turn off DALI
    SetDALIOff( );

    // clear the initialized flag
    tDaliCtl.bDaliInitialized = FALSE;
  }
}

/******************************************************************************
 * @function  DALIMessageHandler_ProcessEvent
 *
 * @brief process the incoming event
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   xArg    this is an DALI Message event
 *
 * @return      TRUE    if the event is to be removed from the event queue
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessEvent( TASKARG xArg )
{
  // check for a error
  if ( xArg == MANCHESTERCODEC_RECV_EROR  )
  {
    // restart the receiver
    ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
  }
  // check for timeout events
  else if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // check for repeat timer running
    if ( tDaliCtl.tFlags.bRepeatTimerRequested == TRUE )
    {
      // clear the repeat timer flag/Application Extended Requested flag
     tDaliCtl.tFlags.bRepeatTimerRequested = FALSE;
     tDaliCtl.tFlags.bAppExtRequested = FALSE;
    }
    else if ( tDaliCtl.tFlags.bDAPCSequenceRunning == TRUE )
    {
      // clear the sequence running flag/restore the previous fade time
      tDaliCtl.tFlags.bDAPCSequenceRunning = FALSE;
      tDaliCtl.tDaliPrm.nFadeTime = tDaliCtl.nCurFadeTime;
      tDaliCtl.uFadePeriod = 0;
      tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
    }
    else
    {
      if ( tDaliCtl.tFlags.bResponsePending )
      {
        // transmit a response
        ManchesterCodec_Xmit( ( PU8 )&tDaliCtl.nResponseValue, 1 );

        // clear the response requested flag/response pending flags
        tDaliCtl.tFlags.bResponseRequested = FALSE;
        tDaliCtl.tFlags.bResponsePending = FALSE;

        #if ( DALI_ENABLE_DEBUG == 1 )
        DebugCommands_OutputDaliXmtMsg( tDaliCtl.nResponseValue );
        #endif // DALI_DUMP_MSG
      }
    }
  }

  // determine which state we are in
  switch( tDaliCtl.eMsgState )
  {
    case MSG_STATE_IDLE :
      // we only process receive done here
      if ( xArg == MANCHESTERCODEC_RECV_DONE )
      {
        #if ( DALI_ENABLE_DEBUG == 1 )
        DebugCommands_OutputDaliRcvMsg( &tDaliCtl.tCurMessage );
        #endif // DALI_DUMP_MSG

        // parse the message
        switch( ParseCommand( ))
        {
          case PARSE_STS_REQ100MSTIMER :
            // set the repeat timer running flag/start a 100 MS timer/restart receive
            tDaliCtl.tFlags.bRepeatTimerRequested = TRUE;
            TaskManager_StartTimer( TASK_SCHD_DALIEVENT, TASK_TIME_MSECS( 100 ));
            ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
            break;

          case PARSE_STS_REQ200MSTIMER :
            // set the repeat timer running flag/start a 100 MS timer/restart receive
            TaskManager_StartTimer( TASK_SCHD_DALIEVENT, TASK_TIME_MSECS( 200 ));
            ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
            break;

          case PARSE_STS_SENDRESPONSE :
            // delay for three MSECS
            TaskManager_StartTimer( TASK_SCHD_DALIEVENT, TASK_TIME_MSECS( DALI_FORWARD_BACKWARD_DELAY_TIME ));
            tDaliCtl.tFlags.bResponsePending = TRUE;
            eMsgState = MSG_STATE_WAITXMITDONE;
            break;

          case PARSE_STS_CLEARALLTIMER :
            // kill the repeat timer
            TaskManager_StopTimer( TASK_SCHD_DALIEVENT );

            // clear the 100MS timer status/bAppExtRequested
            tDaliCtl.tFlags.bRepeatTimerRequested = FALSE;
            tDaliCtl.tFlags.bAppExtRequested = FALSE;
            ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
            break;

          case PARSE_STS_NONE :
          default :
            // restart the manchester codec for receive
            ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
            break;
        }
      }
      break;

    case MSG_STATE_WAITXMITDONE :
      // was this a transmit done
      if ( xArg == MANCHESTERCODEC_XMIT_DONE )
      {
        // restart the receive and go back to idle
        ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
        tDaliCtl.eMsgState = MSG_STATE_IDLE;
      }
      break;

    default :
      break;
  }

  // return true to pop event of queue
  return( TRUE );
}

/******************************************************************************
 * @function  DALIMessageHandler_ProcessTimer
 *
 * @brief process the incoming event
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   xArg    this is an DALI Message event
 *
 * @return      TRUE    if the event is to be removed from the event queue
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessTimer( TASKARG xArg )
{
  // clear the 15 min timer
  tDaliCtl.tFlags.b15MinTimerRunning = FALSE;

  // return true to pop event of queue
  return( TRUE );
}

/******************************************************************************
 * @function  DALIMessageHandler_ProcessOutput
 *
 * @brief process the output
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   xArg    this is an DALI Message event
 *
 * @return      TRUE    if the event is to be removed from the event queue
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessOutput( TASKARG xArg )
{
  U16	wOutputLevel;

  // process the fading
  if ( tDaliCtl.uFadePeriod != 0 )
  {
    // increment the fade counter
    tDaliCtl.uFadeCounter += 1000;

    // decrement fade period only if processing a fade rate
    if ( tDaliCtl.tFlags.bFadeRateReq == TRUE )
    {
      tDaliCtl.uFadePeriod--;
    }

    // check for counter greater than increment - this is when the actual level is increased
    if ( tDaliCtl.uFadeCounter >= tDaliCtl.uFadeIncrement )
    {
      // reset the fade counter - but keep remainder
      tDaliCtl.uFadeCounter -= tDaliCtl.uFadeIncrement;

      // which direction are we running in
      switch( tDaliCtl.eFadeDir )
      {
        case FADE_DIR_UP :
          // if actual level is 0, set to min
          if ( tDaliCtl.nActualDimLevel == 0 )
          {
            // set to minimum
            tDaliCtl.nActualDimLevel = tDaliCtl.tDaliPrm.nMinLevel;
          }
          else if ( tDaliCtl.nActualDimLevel < tDaliCtl.tDaliPrm.nMaxLevel )
          {
            // increment it
            tDaliCtl.nActualDimLevel++;
          }
          break;

        case FADE_DIR_DN :
          // if actual level is above minimum
          if ( tDaliCtl.nActualDimLevel > tDaliCtl.tDaliPrm.nMinLevel )
          {
            // decrement the actual level
            tDaliCtl.nActualDimLevel--;
          }
          else
          {
            // set it to 0 if we are not using fade rate
            if ( tDaliCtl.tFlags.bFadeRateReq == FALSE )
            {
              tDaliCtl.nActualDimLevel = 0;
            }
          }
          break;

        default :
          break;
      }

      // have we completed the fade
      if (( tDaliCtl.nActualDimLevel == tDaliCtl.nRequestedLevel ) && ( tDaliCtl.tFlags.bFadeRateReq == FALSE ))
      {
        // if we have completed a fade using fade time
        // reset the fade period/reset the fade status/fade increment/fade counter progress
        tDaliCtl.uFadePeriod = 0;
        tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
        tDaliCtl.uFadeIncrement = 0;
        tDaliCtl.uFadeCounter = 0;
      }
      else if (( tDaliCtl.uFadePeriod == 0 ) && ( tDaliCtl.tFlags.bFadeRateReq == TRUE ))
      {
        // if we have completed a fade using fade rate
        // reset the fade status/fade increment/fade counter progress/set fade rate request to false
        tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
        tDaliCtl.uFadeIncrement = 0;
        tDaliCtl.tFlags.bFadeRateReq = FALSE;
        tDaliCtl.uFadeCounter = 0;
      }
      else
      {
        // set the fade status to true
        tDaliCtl.tDaliSts.tBits.bFadeRunning = TRUE;
      }
    }
    else if (( tDaliCtl.uFadePeriod == 0 ) && ( tDaliCtl.tFlags.bFadeRateReq ))
    {
      // if we have completed a fade using a fade rate but not incremented the light level
      // reset the fade status/fade increment/fade counter progress/set fade rate request to false
      tDaliCtl.uFadeIncrement = 0;
      tDaliCtl.uFadeCounter = 0;
      tDaliCtl.tFlags.bFadeRateReq = FALSE;
      tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
    }
  }

  // is the actual limit equal or above the minimum level
  if ( tDaliCtl.nActualDimLevel >= tDaliCtl.tDaliPrm.nMinLevel )
  {
    // clamp the actual limit to the max
    if ( tDaliCtl.nActualDimLevel > tDaliCtl.tDaliPrm.nMaxLevel )
    {
      // set it to max
      tDaliCtl.nActualDimLevel = tDaliCtl.tDaliPrm.nMaxLevel;
    }

    // set the lamp status to on/set the output level based on table
    tDaliCtl.tDaliSts.tBits.bLampOn = ( DALI_GET_LED_STATUS( ) ) ? TRUE : FALSE;
    tDaliCtl.tDaliSts.tBits.bLampFailure = ( DALI_GET_LED_STATUS( ) ) ? FALSE : TRUE;
    #if ( DALI_DISABLE_DFLT_DIMTABLE == 0 )
    wOutputLevel = awDimPercent[ tDaliCtl.nActualDimLevel ];
    #else
    wOutputLevel = tDaliCtl.nActualDimLevel;
    #endif  // DALI_DISABLE_DFLT_DIMTABLE
  }
  else
  {
    // set the lamp status to off/set the output level to 0
    tDaliCtl.tDaliSts.tBits.bLampOn = FALSE;
    wOutputLevel = 0;
  }
  
  // compare to reset state value
  tDaliCtl.tResetState.tBits.bActualLevel = ( tDaliCtl.nActualDimLevel != ACTUAL_DIM_LVL_RESET ) ? TRUE : FALSE;

  // now determine the action
  #if ( DALI_EXTERN_DIMCURVE_ENABLE == 1 )
  wOutputLevel = tDaliCtl.nActualDimLevel;
  #endif

  // post the new level to the led manager
  DALIMessageHandler_SetLedLevel( tDaliCtl.tDaliPrm.tOpMode.tFields.bDimCurve, wOutputLevel );

  // always return true
  return( TRUE );
}

/******************************************************************************
 * @function  DALIMessageHandler_ProcessBusFail
 *
 * @brief process the output
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   xArg    this is an DALI Message event
 *
 * @return      TRUE    if the event is to be removed from the event queue
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessBusFail( TASKARG xArg )
{
  BOOL bState;

  // get the state of the DALI input pin
  Gpio_Get( DALI_BUS_STATUS, &bState );

  // if test for bus power is true
  if ( tDaliCtl.bTestBusPowerOn && bState )
  {
    // test for stable bus power
    // increment the bus fail count/test for bus power has been on for at least two sample periods
    if ( ++tDaliCtl.nBusFailCount >= 2 )
    {
      // clear the test power on flag
      tDaliCtl.bTestBusPowerOn = FALSE;

      // clear the bus fail count
      tDaliCtl.nBusFailCount = 0;
  
      // enable the manchester receiver
      ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
    }
  }
  else
  {
    // check pin state
    if ( !bState )
    {
      // if our max count 
      if ( tDaliCtl.nBusFailCount < NUM_BUSFAIL_EVENTS )
      {
        // increment the count
        tDaliCtl.nBusFailCount++;

        // check for bus fail
        if ( nBusFailCount >= NUM_BUSFAIL_EVENTS )
        {
          // bus failure
          if ( tDaliCtl.tDaliPrm.nSystemFailureLevel != 0xFF )
          {
            // set the actual level to the system failure level
            //clamp to min or max level
            tDaliCtl.nActualDimLevel = MIN(tDaliCtl.tDaliPrm.nSystemFailureLevel, tDaliCtl.tDaliPrm.nMaxLevel);
            if ( tDaliCtl.tDaliPrm.nSystemFailureLevel != 0 )
            {
              tDaliCtl.nActualDimLevel = MAX(tDaliCtl.tDaliPrm.nSystemFailureLevel, tDaliCtl.tDaliPrm.nMinLevel);
            }

            DALI_SET_OUTPUT_LEVEL( tDaliCtl.tDaliPrm.tOpMode.tFields.bDimCurve, tDaliCtl.nActualDimLevel );
          }
        }
      }
    }
    else
    {
      // reset the count
      tDaliCtl.nBusFailCount = 0;
    }
  }

  // always return true
  return( TRUE );
}

/******************************************************************************
 * @function  DALIMessageHandler_ForceMessage
 *
 * @brief stuff a message into the event handler
 *
 * This function will force a message into the handler
 *
 * @input[in]   nAddr     message address
 * @input[in]   nDataCmd  message data/command
 *
 *****************************************************************************/
void DALIMessageHandler_ForceMessage( U8 nAddr, U8 nDataCmd )
{
  // ensure we are not off
  if ( eMsgState == MSG_STATE_IDLE )
  {
    // copy the data into 
    tDaliCtl.tCurMessage.anBuffer[ 0 ] = nAddr;
    tDaliCtl.tCurMessage.anBuffer[ 1 ] = nDataCmd;

    // set the forced message flag
    tDaliCtl.bForcedMessage = TRUE;

    // force  a message received
    TaskManager_PostPriorityEvent( TASK_SCHD_DALIEVENT, MANCHESTERCODEC_RECV_DONE );
  }
}

/******************************************************************************
 * @function  ParseCommand
 *
 * @brief parse the incoming command
 *
 * This function will parse the command
 *
 * @return      appropriate parse status value
 *
 *****************************************************************************/
static PARSESTS ParseCommand( void )
{
  PCMDTBL		ptCmdEntry = NULL;
  U8				nCommand, nType;
  PARSESTS	eStatus = PARSE_STS_NONE;
  BOOL			bExecuteCommand = FALSE;

  // first determine the the type of command
  if (( tDaliCtl.tCurMessage.anBuffer[ 0 ] >= DALI_MIN_SPCLEXT_CMD ) && 
      ( tDaliCtl.tCurMessage.anBuffer[ 0 ] <= DALI_MAX_SPCLEXT_CMD ) &&
      ( tDaliCtl.tCurMessage.tFields.tAddr.bValueCmd )) 
  {
    // check to see if a DAPC sequence is funning
    if ( tDaliCtl.tFlags.bDAPCSequenceRunning == TRUE )
    {
      // clear the sequence running flag/restore the previous fade time
      tDaliCtl.tFlags.bDAPCSequenceRunning = FALSE;
      tDaliCtl.tDaliPrm.nFadeTime = tDaliCtl.nCurFadeTime;
      tDaliCtl.uFadePeriod = 0;
      tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
    
      // kill the timer
      TaskManager_StopTimer( TASK_SCHD_DALIEVENT );
    }

    // are we in an extended state
    if ( tDaliCtl.tFlags.bAppExtRequested )
    {
      // clear the Application extended requested flag
      tDaliCtl.tFlags.bAppExtRequested = FALSE;
    }
    else
    {
      // determine the type of command/and the actual command
      nType = DALI_GET_TYPE( tDaliCtl.tCurMessage.tFields.tAddr.nAddress );
      nCommand = DALI_GET_CMND( tDaliCtl.tCurMessage.tFields.tAddr.nAddress );
      switch( nType )
      {
        case COMMAND_TYPE_SPCL :
          // get the entry
          ptCmdEntry = ( PCMDTBL )PGM_RDWORD_PTR( &atScplCmds[ nCommand ] );
          
          // test for execution
          eStatus = ProcessCommandEntry( ptCmdEntry, &bExecuteCommand );
          break;

        case COMMAND_TYPE_EXTN :
          // get the entry
          ptCmdEntry = ( PCMDTBL )PGM_RDWORD_PTR( &atExtnCmds[ nCommand ] );
         
          // test for execution
          eStatus = ProcessCommandEntry( ptCmdEntry, &bExecuteCommand );

          // test for enable write memory command
          if ( bExecuteCommand && ( nCommand != SPECIALCOMMANDS_WRITEMEMLOC_CMDOFS ) )
          {
            // reset write memory enable flag if not a write memory command
            tDaliCtl.tFlags.bWriteMemoryEnabled = FALSE;
          }
          break;

        default :
          break;
      }
    }
  }
  else
  {
    // normal message
    if ((( tDaliCtl.tCurMessage.tFields.tAddr.bDirectGroup == TRUE ) && ( tDaliCtl.tCurMessage.tFields.tAddr.nAddress == 0x3F )) ||
        (( tDaliCtl.tCurMessage.tFields.tAddr.bDirectGroup == FALSE ) && ( tDaliCtl.tCurMessage.tFields.tAddr.nAddress == tDaliCtl.tDaliPrm.nShortAddress )) ||
        (( tDaliCtl.tCurMessage.tFields.tAddr.bDirectGroup == TRUE ) && (( BIT(( tDaliCtl.tCurMessage.tFields.tAddr.nAddress & 0x0F )) & tDaliCtl.tDaliPrm.tGroup.wValue ) != 0 )))
    {
      // clear write memory enabled
      tDaliCtl.tFlags.bWriteMemoryEnabled = FALSE;
      
       // first determine if this is a direct ARC power control
      if ( tDaliCtl.tCurMessage.tFields.tAddr.bValueCmd == FALSE )
      {
        // set the requested level/clear the reset/power failure status bits/set the output change request flag
        tDaliCtl.nRequestedLevel = tDaliCtl.tCurMessage.tFields.nData;
        tDaliCtl.tDaliSts.tBits.bPowerFailure = FALSE;
        tDaliCtl.tFlags.bOutputChangeRequested = TRUE;
        
        // if DAPC is running
        if ( tDaliCtl.tFlags.bDAPCSequenceRunning == TRUE )
        {
          // request a reset of the timer
          eStatus = PARSE_STS_REQ200MSTIMER;
        }
      }
      else
      {
        // check to see if a DAPC sequence is funning
        if ( tDaliCtl.tFlags.bDAPCSequenceRunning == TRUE )
        {
          // clear the sequence running flag/restore the previous fade time
          tDaliCtl.tFlags.bDAPCSequenceRunning = FALSE;
          tDaliCtl.tDaliPrm.nFadeTime = tDaliCtl.nCurFadeTime;
          tDaliCtl.uFadePeriod = 0;
          tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
    
          // kill the timer
          TaskManager_StopTimer( TASK_SCHD_DALIEVENT );
        }

        // get the command/table entry
        nCommand = tDaliCtl.tCurMessage.tFields.nData;
        ptCmdEntry = ( PCMDTBL )PGM_RDWORD_PTR( &atNormCmds[ nCommand ] );
        
        // test for application extended mismatch
        if ( !( ptCmdEntry->tFlags.bDevTypeEnabled && !tDaliCtl.tFlags.bAppExtRequested ))
        {
          // test for execution
          eStatus = ProcessCommandEntry( ptCmdEntry, &bExecuteCommand );
          
          // if execute command
          if ( bExecuteCommand ) 
          {
            // clear the application extension requested flag
            tDaliCtl.tFlags.bAppExtRequested = FALSE;
            
            // reset power failure if a level command
            if ( nCommand <= NRM_INDIRECT_LVLCMD_MAX )
            {
              // set it to false
              tDaliCtl.tDaliSts.tBits.bPowerFailure = FALSE;
            }
          }
        }
        else
        {
          // clear the application extension requested flag
          tDaliCtl.tFlags.bAppExtRequested = FALSE;
        }
      }
    }
  }

  // check for execute command
  if ( bExecuteCommand )
  {
    //  execute the command
    ptCmdEntry->pvCmdHandler( &tDaliCtl );

    // check for response
    if (( ptCmdEntry->tFlags.eResponseType == RESPONSE_TYPE_SEND ) || 
       (( ptCmdEntry->tFlags.eResponseType == RESPONSE_TYPE_FLAG ) && ( tDaliCtl.tFlags.bResponseRequested == TRUE )))
    {
      // set return type to indicate a response needs to be sent
      eStatus = PARSE_STS_SENDRESPONSE;
    }

    // check for a DAPC sequence running flag
    if ( tDaliCtl.tFlags.bDAPCSequenceRunning == TRUE )
    {
      // request a reset of the 200 ms timer
      eStatus = PARSE_STS_REQ200MSTIMER;
    }
  }

  // check for change in actual dim level
  if ( tDaliCtl.tDaliPrm.nLastReqLevel != tDaliCtl.nActualDimLevel )
  {
     // update the NVM
     ParameterManager_PutValue( PARAMSEL_DALI_LASTREQ_LEVEL, tDaliCtl.nActualDimLevel );
     tDaliCtl.tDaliPrm.nLastReqLevel = tDaliCtl.nActualDimLevel;
  }

  // check for a output change
  if ( tDaliCtl.tFlags.bOutputChangeRequested )
  {
    // update the NVM
    ParameterManager_PutValue( PARAMSEL_DALI_LASTREQ_LEVEL, tDaliCtl.nRequestedLevel );
    tDaliCtl.tDaliPrm.nLastReqLevel = tDaliCtl.nRequestedLevel;

    // process it/clear the flag
    ProcessOutputChange( );
    tDaliCtl.tFlags.bOutputChangeRequested = FALSE;
  }

  // check for a reset requested flag
  if ( tDaliCtl.tFlags.bResetRequested )
  {
    // turn off DALI
    SetDALIOff( );

    // reset the parameters to their default condition
    ParameterManager_ResetUserDefaults( );

    // get the default parameters
    GetDefaultParameters( );
  
    // clear the reset flag/power fail flag
    tDaliCtl.tFlags.bResetRequested = FALSE;
    tDaliCtl.tDaliSts.tBits.bPowerFailure = FALSE;
  }

  // check if a lamp is physically disconnected
  if ( tDaliCtl.tDaliSts.tBits.bLampFailure == TRUE)
  {
    // set the lamp failure/set the phsycial selected if requested
    if ( tDaliCtl.ePhysicalSelEnabled == PHYSEL_STATE_REQUESTED )
    {
      // set the selection to OK
      tDaliCtl.ePhysicalSelEnabled = PHYSEL_STATE_ENABLED;
    }
  }
  else
  {
    // clear the phsycial selected if enabled
    if ( tDaliCtl.ePhysicalSelEnabled == PHYSEL_STATE_ENABLED )
    {
      // set the selection to OK
      tDaliCtl.ePhysicalSelEnabled = PHYSEL_STATE_REQUESTED;
    }
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function  ProcessCommandEntry
 *
 * @brief process a command entry
 *
 * This function return TRUE if a command is to be executed FALSE if not
 *
 * @input[in]   ptCmdEntry        pointer to the command entry
 * @input[io]   pbExeuteCommand   pointer to the storage for execute
 * @return      appropriate parse status value
 *
 *****************************************************************************/
static PARSESTS ProcessCommandEntry( PCMDTBL ptCmdEntry, PBOOL pbExecuteCmd )
{
  PARSESTS	eStatus = PARSE_STS_NONE;
  BOOL      bEOLEnabled;
  
  // check for a valid command
  if ( ptCmdEntry->pvCmdHandler != NULL )
  {
    // get the state of the end of line test
    bEOLEnabled = DALIMessageHandler_GetEOLTestMode( );
    
    // check for repeat
    if ( ptCmdEntry->tFlags.bRepeatFlag && !tDaliCtl.tFlags.bRepeatTimerRequested && !bForcedMessage && !bEOLEnabled )
    {
      // copy the command for compare/set the status to reqeuest 100MS timer
      memcpy( &tDaliCtl.tPrvMessage, &tDaliCtl.tCurMessage, DALIMSG_SIZE );
      eStatus = PARSE_STS_REQ100MSTIMER;
    }
    else if ( ptCmdEntry->tFlags.bRepeatFlag && tDaliCtl.tFlags.bRepeatTimerRequested  )
    {
      if ( memcmp( &tDaliCtl.tPrvMessage, &tDaliCtl.tCurMessage, DALIMSG_SIZE ) == 0 )
      {
        // check for a execute in time
        if (( ptCmdEntry->tFlags.bExecInTime && tDaliCtl.tFlags.b15MinTimerRunning ) ||
            ( !ptCmdEntry->tFlags.bExecInTime ))
        {
          // set the execute command flag
          *( pbExecuteCmd ) = TRUE;
        }
      }
        
      // clear the timer
      eStatus = PARSE_STS_CLEARALLTIMER;
    }
    else if ( !ptCmdEntry->tFlags.bRepeatFlag && tDaliCtl.tFlags.bRepeatTimerRequested )
    {
      // clear the timer
      eStatus = PARSE_STS_CLEARALLTIMER;
    }
    else if ( !ptCmdEntry->tFlags.bRepeatFlag || bForcedMessage || bEOLEnabled )
    {
      // check for a execute in time
      if (( ptCmdEntry->tFlags.bExecInTime && tDaliCtl.tFlags.b15MinTimerRunning ) ||
          ( !ptCmdEntry->tFlags.bExecInTime ))
      {
        // set the execute command flag
        *pbExecuteCmd = TRUE;

        // clear forced message
        tDaliCtl.bForcedMessage = FALSE;
      }
    }
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function  ProcessOutputChange
 *
 * @brief process any change in the output
 *
 * This function will parse the command
 *
 *****************************************************************************/
static void ProcessOutputChange( void )
{
  U32   uFadeTime;
  
  // check for a valid level
  if ( tDaliCtl.nRequestedLevel != 0xFF )
  {
    // clear fade rate request/fade counter progress
    tDaliCtl.tFlags.bFadeRateReq = FALSE;
    tDaliCtl.uFadeCounter = 0;

    // clear error
    tDaliCtl.tDaliSts.tBits.bLimitError = FALSE;

    // check for requested level greater than min
    if (( tDaliCtl.nRequestedLevel < tDaliCtl.tDaliPrm.nMinLevel ) && ( tDaliCtl.nRequestedLevel != 0 ))
    {
      // set the requested level to the min level/set error
      tDaliCtl.nRequestedLevel = tDaliCtl.tDaliPrm.nMinLevel;
      tDaliCtl.tDaliSts.tBits.bLimitError = TRUE;
    }

    // check for requested level greater than max
    if ( tDaliCtl.nRequestedLevel > tDaliCtl.tDaliPrm.nMaxLevel )
    {
      // set the requested level to the max level/set error
      tDaliCtl.nRequestedLevel = tDaliCtl.tDaliPrm.nMaxLevel;
      tDaliCtl.tDaliSts.tBits.bLimitError = TRUE;
    }

    // is there a fade time
    if ( tDaliCtl.tDaliPrm.nFadeTime != 0 )
    {
      // use the entry from the table and multiply by 1000 to change from fade in Milliseconds to fade in Microseconds
      tDaliCtl.uFadePeriod = 1000 * auFadeTimes[ tDaliCtl.tDaliPrm.nFadeTime ] ;
    }
    else
    {
      // calculate the fast fade time in microseconds
      tDaliCtl.uFadePeriod = 1000 * tDaliCtl.tDaliPrm.nFastFadeTime * FAST_FADE_TIME_MULT;
    }

    // determine fade up/fade down
    if ( tDaliCtl.nRequestedLevel > tDaliCtl.nActualDimLevel + 1 )
    {
      // set direction to up
      tDaliCtl.eFadeDir = FADE_DIR_UP;

      // if off
      if ( tDaliCtl.nActualDimLevel == 0 )
      {
        // set the fade increment to microseconds per step ( fade period = fade in microseconds ) based on min level
        tDaliCtl.uFadeIncrement =  ( tDaliCtl.uFadePeriod ) / ( tDaliCtl.nRequestedLevel - tDaliCtl.tDaliPrm.nMinLevel );
      }
      else
      {
        // set the fade increment base on dim level
        tDaliCtl.uFadeIncrement =  ( tDaliCtl.uFadePeriod ) / ( tDaliCtl.nRequestedLevel - tDaliCtl.nActualDimLevel );
      }
    }
    else
    {
      // check to see if we are below the actual level
      if ( tDaliCtl.nRequestedLevel < tDaliCtl.nActualDimLevel - 1 )
      {
        // set direction to down
        tDaliCtl.eFadeDir = FADE_DIR_DN;
        
        // check for a zero level
        if ( tDaliCtl.tCurMessage.tFields.nData == 0 )
        {
          // set the fade increment to microseconds per step ( fade period = fade in microseconds ) based on min level
          tDaliCtl.uFadeIncrement =  ( tDaliCtl.uFadePeriod ) / ( tDaliCtl.nActualDimLevel - tDaliCtl.tDaliPrm.nMinLevel );
        }
        else
        {
          // set the fade increment based on request level
          tDaliCtl.uFadeIncrement =  ( tDaliCtl.uFadePeriod ) / ( tDaliCtl.nActualDimLevel - tDaliCtl.nRequestedLevel );
        }
      }
      else
      {
        // fade time is zero
        tDaliCtl.uFadePeriod = 0l;
      }
    }

    // test for zero
    if ( tDaliCtl.uFadePeriod == 0l )
    {
      // fade time is zero/set the requested level
      tDaliCtl.nActualDimLevel = tDaliCtl.nRequestedLevel;
    }
  }
  else
  {
    // fade time is zero/set the requested level/clear fade running
    tDaliCtl.nRequestedLevel = tDaliCtl.nActualDimLevel;
    tDaliCtl.uFadePeriod = 0l;
    tDaliCtl.tDaliSts.tBits.bFadeRunning = FALSE;
  }
}

/******************************************************************************
 * @function SetDALIOff
 *
 * @brief set DALI off
 *
 * This function determine what time of bit was receiveid
 *
 *****************************************************************************/
static void SetDALIOff( void )
{
  // set the message state to off
  tDaliCtl.eMsgState = MSG_STATE_OFF;

  // turn off the LED
  DALIMessageHandler_SetLedLevel( tDaliCtl.tDaliPrm.tOpMode.tFields.bDimCurve, 0 );

  // stop the receiver
  ManchesterCodec_StopRecv( );
}

/******************************************************************************
 * @function GetDefaultParamaeters
 *
 * @brief get the default parameters and reset the condition to startup
 *
 * This function determine what time of bit was receiveid
 *
 *****************************************************************************/
static void GetDefaultParameters( void )
{
  // get the parameters
  GetAllParameters( PARAM_SRC_DEFLT );

  // set the startup condition
  SetStartupCondition(  );
}

/******************************************************************************
 * @function  GetParameter
 *
 * @brief get a parameter
 *
 * This function returns the pamarater value from eighter the EEPROM 
 * or default values
 *
 * @input[in]   eSrc        parameter source
 *
 *****************************************************************************/
static void GetAllParameters( PARAMSRC eSrc )
{
  U8  nScene;

  // get the normal parameters
  tDaliCtl.tDaliPrm.nPowerOnLevel = GetParameterValue( eSrc, PARAMSEL_DALI_POWERON_LEVEL );
  tDaliCtl.tDaliPrm.nSystemFailureLevel = GetParameterValue( eSrc, PARAMSEL_DALI_SYSFAIL_LEVEL );
  tDaliCtl.tDaliPrm.nMinLevel = GetParameterValue( eSrc, PARAMSEL_DALI_MIN_LEVEL );
  tDaliCtl.tDaliPrm.nMaxLevel = GetParameterValue( eSrc, PARAMSEL_DALI_MAX_LEVEL );
  tDaliCtl.tDaliPrm.nLastReqLevel = GetParameterValue( eSrc, PARAMSEL_DALI_LASTREQ_LEVEL );
  tDaliCtl.tDaliPrm.nFadeRate = GetParameterValue( eSrc, PARAMSEL_DALI_FADE_RATE );
  tDaliCtl.tDaliPrm.nFadeTime = GetParameterValue( eSrc, PARAMSEL_DALI_FADE_TIME );
  tDaliCtl.tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] = GetParameterValue( eSrc, PARAMSEL_DALI_RANDOM_ADDR_H );
  tDaliCtl.tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] = GetParameterValue( eSrc, PARAMSEL_DALI_RANDOM_ADDR_M );
  tDaliCtl.tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] = GetParameterValue( eSrc, PARAMSEL_DALI_RANDOM_ADDR_L );
  tDaliCtl.tDaliPrm.tGroup.anValue[ LE_U16_LSB_IDX ] = GetParameterValue( eSrc, PARAMSEL_DALI_GROUP0007 );
  tDaliCtl.tDaliPrm.tGroup.anValue[ LE_U16_MSB_IDX ] = GetParameterValue( eSrc, PARAMSEL_DALI_GROUP0815 );

  // now copy the scenes
  for ( nScene = 0; nScene < DALI_NUM_SCENES; nScene++ )
  {
    // read the scene
    tDaliCtl.tDaliPrm.anScenes[ nScene ] = GetParameterValue( eSrc, PARAMSEL_DALI_SCENE00 + nScene );
  }

  // Get the local copies for the led variables
  tDaliCtl.tDaliPrm.tFailureStatus.nFailureStatus = GetParameterValue( eSrc, PARAMSEL_DALI_FAILURESTATUS );
  tDaliCtl.tDaliPrm.tOpMode.nOpMode = GetParameterValue( eSrc, PARAMSEL_DALI_OPMODE );
  tDaliCtl.tDaliPrm.nFastFadeTime = GetParameterValue( eSrc, PARAMSEL_DALI_FASTFADETIME );

  // Get Values from memory bank 0
  tDaliCtl.nMinFastFadeTime = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_MINFASTFADETIME );
  tDaliCtl.tGearType.nGearType = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_GEARTYPE );
  tDaliCtl.tPossibleOpTypes.nPossibleOpTypes = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_POSSOPTYPES );
  tDaliCtl.tFeatures.nFeatures = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_FEATURES );
  tDaliCtl.nExtendedVerNum = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_EXTVERNUM );
  tDaliCtl.nDeviceType = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_DEVTYPE );
  tDaliCtl.nPhysicalMinLvl = GetParameterValue( eSrc, PARAMSEL_DALI_MB0_PHYSICALMINLVL );
}

/******************************************************************************
 * @function  GetParameterValue
 *
 * @brief get a parameter
 *
 * This function returns the pamarater value from eighter the EEPROM 
 * or default values
 *
 * @input[in]   eSrc        parameter source
 * @input[io]   eParam      parameter enumeration
 *
 * @return      parameter value
 *
 *****************************************************************************/
static PARAMARG GetParameterValue( PARAMSRC eSrc, PARAMSELENUM eParam )
{
  PARAMARG  xValue;

  // which source
  switch( eSrc )
  {
    case PARAM_SRC_EEPROM :
      ParameterManager_GetValue( eParam, &xValue );
      break;

    case PARAM_SRC_DEFLT :
      ParameterManager_GetDefaultValue( eParam, &xValue );
      break;

    default :
      xValue = 0;
      break;
  }

  // return the value
  return( xValue );
}

/******************************************************************************
 * @function  SetStartupCondition
 *
 * @brief sets the startup condition
 *
 * This function sets all flags to there startup condition and starts the
 * manchester decoder
 *
 *****************************************************************************/
static void SetStartupCondition( void )
{
  BOOL bState;

  // reset the message state
  tDaliCtl.eMsgState = MSG_STATE_IDLE;
  
  // reset the reset state indicator
  tDaliCtl.tResetState.uWord = 0;

  //reset limit error
  tDaliCtl.tDaliSts.tBits.bLimitError = FALSE;

  // reset any special flags
  tDaliCtl.tFlags.bDAPCSeqEnabled = FALSE;
  tDaliCtl.tFlags.bDimmingCurveEnable = FALSE;
  tDaliCtl.tFlags.bDAPCSequenceRunning = FALSE;
  tDaliCtl.ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;

  // initialize the search address to 0xFFFFFF
  tDaliCtl.tSearchAddr.anValue[ RANDOM_ADDR_H ] = 0xFF;
  tDaliCtl.tSearchAddr.anValue[ RANDOM_ADDR_M ] = 0xFF;
  tDaliCtl.tSearchAddr.anValue[ RANDOM_ADDR_L ] = 0xFF;

  // reset the repeat timer state
  tDaliCtl.tFlags.bRepeatTimerRequested = FALSE;

  // set the state of the missing short address flag
  tDaliCtl.tDaliSts.tBits.bMissingShortAddr = ( tDaliCtl.tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;  

  if ( tDaliCtl.tFlags.bResetRequested == FALSE )
  {
    // check for power on level greater than maximum
    if ( tDaliCtl.tDaliPrm.nPowerOnLevel == 0xFF )
    {
      // set the actual dim level to physical min level
      tDaliCtl.nActualDimLevel = tDaliCtl.tDaliPrm.nLastReqLevel;
    }
    else
    {
      // set the actual dim level
      tDaliCtl.nActualDimLevel = tDaliCtl.tDaliPrm.nPowerOnLevel;
    }
  }
  else
  { 
    //Reset Requested = true - set output to maximum
    tDaliCtl.nActualDimLevel = ACTUAL_DIM_LVL_RESET;
  }

  // set the initial level
  DALIMessageHandler_SetLedLevel( tDaliCtl.tDaliPrm.tOpMode.tFields.bDimCurve, tDaliCtl.nActualDimLevel );

  // get the state of the DALI input pin
  Gpio_Get( DALI_BUS_STATUS, &bState );

  // is the bus power on
  if ( bState )
  {
    // start the receiver/clear test for bus power on flag
    ManchesterCodec_Recv(( PU8 )&tDaliCtl.tCurMessage.anBuffer, DALIMSG_SIZE );
    tDaliCtl.bTestBusPowerOn = FALSE;
  }
  else
  {
    // set the test for bus power on flag
    tDaliCtl.bTestBusPowerOn = TRUE;
  }
}

/**@} EOF DALIMessageHandler.c */
