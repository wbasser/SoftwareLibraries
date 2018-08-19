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
#include "SystemTick/SystemTick.h"

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler.h"
#include "DALIMessageHandler/NormalCommands/NormalCommands.h"
#include "DALIMessageHandler/SpecialCommands/SpecialCommands.h"
#include "DALIMessageHandler/ExtendedCommands/ExtendedCommands.h"

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
PDALICTL  g_ptDaliCtl;  ///< pointer to the control structure

// local parameter declarations -----------------------------------------------
static	DALICTL		tDaliCtl;	  ///< dali control structure

// local function prototypes --------------------------------------------------
static  PARSESTS	ParseCommand( void );
static  PARSESTS	ProcessCommandEntry( PCMDTBL ptCmdEntry, PBOOL pbExecuteCmd );
static	void			ProcessOutputChange( void );
static  void      SetDALIOff( void );
static  void      GetDefaultParameters( void );
static  void      GetAllParameters( PARAMSRC eSrc );
static  U8        GetParameterValue( PARAMSRC eSrc, U8  nParam );
static  void      SetStartupCondition( void );
static  void      OutputLightLevel( void );

// constant parameter initializations -----------------------------------------
// logarithmic dimming curve in tenths of a percent
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
  // set the pointer
  g_ptDaliCtl = &tDaliCtl;
  
  // clear the initialized flag
  g_ptDaliCtl->tFlags.bDaliInitialized = FALSE;

  // call the initializations for normal/special/extended commands
  NormalCommands_Initialize( );
  SpecialCommands_Initialize( );
  ExtendedCommands_Initialize( );

  // clear the forced message flag
  g_ptDaliCtl->tFlags.bForcedMessage = FALSE;

  // clear the bus fail count
  g_ptDaliCtl->nBusFailCount = 0;

  // set the message state to idle
  g_ptDaliCtl->eMsgState == MSG_STATE_IDLE;

  // set the initial bus power off
  g_ptDaliCtl->tFlags.bTestBusPowerOn = FALSE;
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
  // check the requested state
  if (( bState == TRUE ) && ( g_ptDaliCtl->tFlags.bDaliInitialized == FALSE ))
  {
    // set the initialized flag
    g_ptDaliCtl->tFlags.bDaliInitialized = TRUE;

    // get all the parameters
    GetAllParameters( PARAM_SRC_EEPROM );
    g_ptDaliCtl->tDaliPrm.nShortAddress = GetParameterValue( PARAM_SRC_EEPROM, DALI_PARAM_SHORT_ADDR );

    // update status bits
    g_ptDaliCtl->tDaliSts.tBits.bPowerFailure = TRUE;

    // clear any flags
    g_ptDaliCtl->tFlags.b15MinTimerRunning = FALSE;
    
    // seed the random number generator from the serial number
    DALIMessageHandler_SeedRandom( );

    // set the startup condition
    SetStartupCondition( );
  }
  else if ( bState == FALSE )
  {
    // turn off DALI
    SetDALIOff( );

    // clear the initialized flag
    g_ptDaliCtl->tFlags.bDaliInitialized = FALSE;
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
  BOOL      bStartReceive = FALSE;
  U8        nResponse;

  // check for a error
  if ( xArg == DALI_MANCHESTER_RCV_EROR_EVENT  )
  {
    // restart the receiver
    bStartReceive = TRUE;
  }
  // check for timeout events
  else if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // check for repeat timer running
    if ( g_ptDaliCtl->tFlags.bRepeatTimerRequested == TRUE )
    {
      // clear the repeat timer flag/Application Extended Requested flag
     g_ptDaliCtl->tFlags.bRepeatTimerRequested = FALSE;
     g_ptDaliCtl->tFlags.bAppExtRequested = FALSE;
    }
    else if ( g_ptDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
    {
      // clear the sequence running flag/restore the previous fade time
      g_ptDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
      g_ptDaliCtl->tDaliPrm.nFadeTime = g_ptDaliCtl->nCurFadeTime;
      g_ptDaliCtl->uFadePeriod = 0;
      g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
    }
    else
    {
      if ( g_ptDaliCtl->tFlags.bResponsePending )
      {
        // clear the response requested flag/response pending flags
        g_ptDaliCtl->tFlags.bResponseRequested = FALSE;
        g_ptDaliCtl->tFlags.bResponsePending = FALSE;

        // transmit a response
        DALIMessageHandler_StartTransmit(( PU8 )&g_ptDaliCtl->nResponseValue, 1 );

        // call the debug handler
        DALIMessageHandler_OutputXmtMsg( g_ptDaliCtl->nResponseValue );
      }
    }
  }

  // determine which state we are in
  switch( g_ptDaliCtl->eMsgState )
  {
    case MSG_STATE_IDLE :
      // we only process receive done here
      if ( xArg == DALI_MANCHESTER_RCV_DONE_EVENT )
      {
        // call the diagnostic output
        DALIMessageHandler_OutputRcvMsg( &g_ptDaliCtl->tCurMessage );

        // parse the message
        switch( ParseCommand( ))
        {
          case PARSE_STS_REQ100MSTIMER :
            // set the repeat timer running flag/start a 100 MS timer/restart receive
            g_ptDaliCtl->tFlags.bRepeatTimerRequested = TRUE;
            TaskManager_StartTimer( DALI_MESSAGE_TASK_ENUM_EVENT, TASK_TIME_MSECS( 100 ));
            bStartReceive = TRUE;
            break;

          case PARSE_STS_REQ200MSTIMER :
            // set the repeat timer running flag/start a 100 MS timer/restart receive
            TaskManager_StartTimer( DALI_MESSAGE_TASK_ENUM_EVENT, TASK_TIME_MSECS( 200 ));
            bStartReceive = TRUE;
            break;

          case PARSE_STS_SENDRESPONSE :
            // set the flags, change states
            g_ptDaliCtl->tFlags.bResponsePending = TRUE;
            g_ptDaliCtl->eMsgState = MSG_STATE_WAITXMITDONE;

            // delay for three MSECS
            TaskManager_StartTimer( DALI_MESSAGE_TASK_ENUM_EVENT, TASK_TIME_MSECS( DALI_FORWARD_BACKWARD_DELAY_TIME ));
            break;

          case PARSE_STS_CLEARALLTIMER :
            // kill the repeat timer
            TaskManager_StopTimer( DALI_MESSAGE_TASK_ENUM_EVENT );

            // clear the 100MS timer status/bAppExtRequested
            g_ptDaliCtl->tFlags.bRepeatTimerRequested = FALSE;
            g_ptDaliCtl->tFlags.bAppExtRequested = FALSE;
            bStartReceive = TRUE;
            break;

          case PARSE_STS_NONE :
          default :
            // restart the manchester codec for receive
            bStartReceive = TRUE;
            break;
        }
      }
      break;

    case MSG_STATE_WAITXMITDONE :
      // was this a transmit done
      if ( xArg == DALI_MANCHESTER_RCV_DONE_EVENT )
      {
        // restart the receive and go back to idle
        bStartReceive = TRUE;
        g_ptDaliCtl->eMsgState = MSG_STATE_IDLE;
      }
      break;

    default :
      break;
  }

  // check for a start receive
  if ( bStartReceive )
  {
    DALIMessageHandler_StartReceive(( PU8 )&g_ptDaliCtl->tCurMessage.anBuffer, DALIMSG_SIZE );
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
  g_ptDaliCtl->tFlags.b15MinTimerRunning = FALSE;

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
  U16	      wOutputLevel;
  
  // process the fading
  if ( g_ptDaliCtl->uFadePeriod != 0 )
  {
    // increment the fade counter
    g_ptDaliCtl->uFadeCounter += 1000;

    // decrement fade period only if processing a fade rate
    if ( g_ptDaliCtl->tFlags.bFadeRateReq == TRUE )
    {
      g_ptDaliCtl->uFadePeriod--;
    }

    // check for counter greater than increment - this is when the actual level is increased
    if ( g_ptDaliCtl->uFadeCounter >= g_ptDaliCtl->uFadeIncrement )
    {
      // reset the fade counter - but keep remainder
      g_ptDaliCtl->uFadeCounter -= g_ptDaliCtl->uFadeIncrement;

      // which direction are we running in
      switch( g_ptDaliCtl->eFadeDir )
      {
        case FADE_DIR_UP :
          // if actual level is 0, set to min
          if ( g_ptDaliCtl->nActualDimLevel == 0 )
          {
            // set to minimum
            g_ptDaliCtl->nActualDimLevel = g_ptDaliCtl->tDaliPrm.nMinLevel;
          }
          else if ( g_ptDaliCtl->nActualDimLevel < g_ptDaliCtl->tDaliPrm.nMaxLevel )
          {
            // increment it
            g_ptDaliCtl->nActualDimLevel++;
          }
          break;

        case FADE_DIR_DN :
          // if actual level is above minimum
          if ( g_ptDaliCtl->nActualDimLevel > g_ptDaliCtl->tDaliPrm.nMinLevel )
          {
            // decrement the actual level
            g_ptDaliCtl->nActualDimLevel--;
          }
          else
          {
            // set it to 0 if we are not using fade rate
            if ( g_ptDaliCtl->tFlags.bFadeRateReq == FALSE )
            {
              g_ptDaliCtl->nActualDimLevel = 0;
            }
          }
          break;

        default :
          break;
      }

      // have we completed the fade
      if (( g_ptDaliCtl->nActualDimLevel == g_ptDaliCtl->nRequestedLevel ) && ( g_ptDaliCtl->tFlags.bFadeRateReq == FALSE ))
      {
        // if we have completed a fade using fade time
        // reset the fade period/reset the fade status/fade increment/fade counter progress
        g_ptDaliCtl->uFadePeriod = 0;
        g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
        g_ptDaliCtl->uFadeIncrement = 0;
        g_ptDaliCtl->uFadeCounter = 0;
      }
      else if (( g_ptDaliCtl->uFadePeriod == 0 ) && ( g_ptDaliCtl->tFlags.bFadeRateReq == TRUE ))
      {
        // if we have completed a fade using fade rate
        // reset the fade status/fade increment/fade counter progress/set fade rate request to false
        g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
        g_ptDaliCtl->uFadeIncrement = 0;
        g_ptDaliCtl->tFlags.bFadeRateReq = FALSE;
        g_ptDaliCtl->uFadeCounter = 0;
      }
      else
      {
        // set the fade status to true
        g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = TRUE;
      }
    }
    else if (( g_ptDaliCtl->uFadePeriod == 0 ) && ( g_ptDaliCtl->tFlags.bFadeRateReq ))
    {
      // if we have completed a fade using a fade rate but not incremented the light level
      // reset the fade status/fade increment/fade counter progress/set fade rate request to false
      g_ptDaliCtl->uFadeIncrement = 0;
      g_ptDaliCtl->uFadeCounter = 0;
      g_ptDaliCtl->tFlags.bFadeRateReq = FALSE;
      g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
    }
  }

  // is the actual limit equal or above the minimum level
  if ( g_ptDaliCtl->nActualDimLevel >= g_ptDaliCtl->tDaliPrm.nMinLevel )
  {
    // clamp the actual limit to the max
    if ( g_ptDaliCtl->nActualDimLevel > g_ptDaliCtl->tDaliPrm.nMaxLevel )
    {
      // set it to max
      g_ptDaliCtl->nActualDimLevel = g_ptDaliCtl->tDaliPrm.nMaxLevel;
    }

    // set the lamp status to on/set the output level based on table
    g_ptDaliCtl->tDaliSts.tBits.bLampOn = ( DALIMessageHandler_GetLedStatus( )) ? TRUE : FALSE;
    g_ptDaliCtl->tDaliSts.tBits.bLampFailure = !( g_ptDaliCtl->tDaliSts.tBits.bLampOn );
  }
  else
  {
    // set the lamp status to off/set the output level to 0
    g_ptDaliCtl->tDaliSts.tBits.bLampOn = FALSE;
    g_ptDaliCtl->nActualDimLevel = 0;
  }
  
  // compare to reset state value
  g_ptDaliCtl->tResetState.tBits.bActualLevel = ( g_ptDaliCtl->nActualDimLevel != ACTUAL_DIM_LVL_RESET ) ? TRUE : FALSE;

  // set the actual/set the new level to the led manager
  OutputLightLevel( );
  
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
  BOOL      bState;
  
  // get the state of the DALI input pin
  Gpio_Get( DALI_BUS_STATUS, &bState );

  // if test for bus power is true
  if ( g_ptDaliCtl->tFlags.bTestBusPowerOn && bState )
  {
    // test for stable bus power
    // increment the bus fail count/test for bus power has been on for at least two sample periods
    if ( ++g_ptDaliCtl->nBusFailCount >= 2 )
    {
      // clear the test power on flag
      g_ptDaliCtl->tFlags.bTestBusPowerOn = FALSE;

      // clear the bus fail count
      g_ptDaliCtl->nBusFailCount = 0;

      // enable the manchester receiver
      DALIMessageHandler_StartReceive(( PU8 )&g_ptDaliCtl->tCurMessage.anBuffer, DALIMSG_SIZE );
    }
  }
  else
  {
    // check pin state
    if ( !bState )
    {
      // if our max count 
      if ( g_ptDaliCtl->nBusFailCount < NUM_BUSFAIL_EVENTS )
      {
        // increment the count
        g_ptDaliCtl->nBusFailCount++;

        // check for bus fail
        if ( g_ptDaliCtl->nBusFailCount >= NUM_BUSFAIL_EVENTS )
        {
          // bus failure
          if ( g_ptDaliCtl->tDaliPrm.nSystemFailureLevel != 0xFF )
          {
            // set the actual level to the system failure level
            //clamp to min or max level
            g_ptDaliCtl->nActualDimLevel = MIN(g_ptDaliCtl->tDaliPrm.nSystemFailureLevel, g_ptDaliCtl->tDaliPrm.nMaxLevel);
            if ( g_ptDaliCtl->tDaliPrm.nSystemFailureLevel != 0 )
            {
              g_ptDaliCtl->nActualDimLevel = MAX(g_ptDaliCtl->tDaliPrm.nSystemFailureLevel, g_ptDaliCtl->tDaliPrm.nMinLevel);
            }

            // set the level
            OutputLightLevel( );
          }
        }
      }
    }
    else
    {
      // reset the count
      g_ptDaliCtl->nBusFailCount = 0;
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
  if ( g_ptDaliCtl->eMsgState == MSG_STATE_IDLE )
  {
    // copy the data into 
    g_ptDaliCtl->tCurMessage.anBuffer[ 0 ] = nAddr;
    g_ptDaliCtl->tCurMessage.anBuffer[ 1 ] = nDataCmd;

    // set the forced message flag/increment count
    g_ptDaliCtl->tFlags.bForcedMessage = TRUE;

    // force  a message received
    TaskManager_PostPriorityEvent( DALI_MESSAGE_TASK_ENUM_EVENT, DALI_MANCHESTER_RCV_DONE_EVENT );
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
static PARSESTS ParseCommand( )
{
  PCMDTBL		ptCmdEntry = NULL;
  U8				nCommand, nType;
  PARSESTS	eStatus = PARSE_STS_NONE;
  BOOL			bExecuteCommand = FALSE;

  // first determine the the type of command
  if (( g_ptDaliCtl->tCurMessage.anBuffer[ 0 ] >= DALI_MIN_SPCLEXT_CMD ) && 
      ( g_ptDaliCtl->tCurMessage.anBuffer[ 0 ] <= DALI_MAX_SPCLEXT_CMD ) &&
      ( g_ptDaliCtl->tCurMessage.tFields.tAddr.bValueCmd )) 
  {
    // check to see if a DAPC sequence is funning
    if ( g_ptDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
    {
      // clear the sequence running flag/restore the previous fade time
      g_ptDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
      g_ptDaliCtl->tDaliPrm.nFadeTime = g_ptDaliCtl->nCurFadeTime;
      g_ptDaliCtl->uFadePeriod = 0;
      g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
    
      // kill the timer
      TaskManager_StopTimer( DALI_MESSAGE_TASK_ENUM_EVENT );
    }

    // are we in an extended state
    if ( g_ptDaliCtl->tFlags.bAppExtRequested )
    {
      // clear the Application extended requested flag
      g_ptDaliCtl->tFlags.bAppExtRequested = FALSE;
    }
    else
    {
      // determine the type of command/and the actual command
      nType = DALI_GET_TYPE( g_ptDaliCtl->tCurMessage.tFields.tAddr.nAddress );
      nCommand = DALI_GET_CMND( g_ptDaliCtl->tCurMessage.tFields.tAddr.nAddress );
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
            g_ptDaliCtl->tFlags.bWriteMemoryEnabled = FALSE;
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
    if ((( g_ptDaliCtl->tCurMessage.tFields.tAddr.bDirectGroup == TRUE ) && ( g_ptDaliCtl->tCurMessage.tFields.tAddr.nAddress == 0x3F )) ||
        (( g_ptDaliCtl->tCurMessage.tFields.tAddr.bDirectGroup == FALSE ) && ( g_ptDaliCtl->tCurMessage.tFields.tAddr.nAddress == g_ptDaliCtl->tDaliPrm.nShortAddress )) ||
        (( g_ptDaliCtl->tCurMessage.tFields.tAddr.bDirectGroup == TRUE ) && (( BIT(( g_ptDaliCtl->tCurMessage.tFields.tAddr.nAddress & 0x0F )) & g_ptDaliCtl->tDaliPrm.tGroup.wValue ) != 0 )))
    {
      // clear write memory enabled
      g_ptDaliCtl->tFlags.bWriteMemoryEnabled = FALSE;
      
       // first determine if this is a direct ARC power control
      if ( g_ptDaliCtl->tCurMessage.tFields.tAddr.bValueCmd == FALSE )
      {
        // set the requested level/clear the reset/power failure status bits/set the output change request flag
        g_ptDaliCtl->nRequestedLevel = g_ptDaliCtl->tCurMessage.tFields.nData;
        g_ptDaliCtl->tDaliSts.tBits.bPowerFailure = FALSE;
        g_ptDaliCtl->tFlags.bOutputChangeRequested = TRUE;
        
        // if DAPC is running
        if ( g_ptDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
        {
          // request a reset of the timer
          eStatus = PARSE_STS_REQ200MSTIMER;
        }
      }
      else
      {
        // check to see if a DAPC sequence is funning
        if ( g_ptDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
        {
          // clear the sequence running flag/restore the previous fade time
          g_ptDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
          g_ptDaliCtl->tDaliPrm.nFadeTime = g_ptDaliCtl->nCurFadeTime;
          g_ptDaliCtl->uFadePeriod = 0;
          g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
    
          // kill the timer
          TaskManager_StopTimer( DALI_MESSAGE_TASK_ENUM_EVENT );
        }

        // get the command/table entry
        nCommand = g_ptDaliCtl->tCurMessage.tFields.nData;
        ptCmdEntry = ( PCMDTBL )PGM_RDWORD_PTR( &atNormCmds[ nCommand ] );
        
        // test for application extended mismatch
        if ( !( ptCmdEntry->tFlags.bDevTypeEnabled && !g_ptDaliCtl->tFlags.bAppExtRequested ))
        {
          // test for execution
          eStatus = ProcessCommandEntry( ptCmdEntry, &bExecuteCommand );
          
          // if execute command
          if ( bExecuteCommand ) 
          {
            // clear the application extension requested flag
            g_ptDaliCtl->tFlags.bAppExtRequested = FALSE;
            
            // reset power failure if a level command
            if ( nCommand <= NRM_INDIRECT_LVLCMD_MAX )
            {
              // set it to false
              g_ptDaliCtl->tDaliSts.tBits.bPowerFailure = FALSE;
            }
          }
        }
        else
        {
          // clear the application extension requested flag
          g_ptDaliCtl->tFlags.bAppExtRequested = FALSE;
        }
      }
    }
  }

  // check for execute command
  if ( bExecuteCommand )
  {
    //  execute the command
    ptCmdEntry->pvCmdHandler( );

    // check for response
    if (( ptCmdEntry->tFlags.eResponseType == RESPONSE_TYPE_SEND ) || 
       (( ptCmdEntry->tFlags.eResponseType == RESPONSE_TYPE_FLAG ) && ( g_ptDaliCtl->tFlags.bResponseRequested == TRUE )))
    {
      // set return type to indicate a response needs to be sent
      eStatus = PARSE_STS_SENDRESPONSE;
    }

    // check for a DAPC sequence running flag
    if ( g_ptDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
    {
      // request a reset of the 200 ms timer
      eStatus = PARSE_STS_REQ200MSTIMER;
    }
  }

  // check for change in actual dim level
  if (( g_ptDaliCtl->tDaliPrm.nLastReqLevel != g_ptDaliCtl->nActualDimLevel ) && ( !g_ptDaliCtl->tFlags.bOutputChangeRequested ))
  {
     // update the NVM
     DALIMessageHandler_PutValue( DALI_PARAM_LASTREQ_LEVEL, g_ptDaliCtl->nActualDimLevel );
     g_ptDaliCtl->tDaliPrm.nLastReqLevel = g_ptDaliCtl->nActualDimLevel;
  }
  
  // check for a output change
  if ( g_ptDaliCtl->tFlags.bOutputChangeRequested )
  {
    // update the NVM
    DALIMessageHandler_PutValue( DALI_PARAM_LASTREQ_LEVEL, g_ptDaliCtl->nRequestedLevel );
    g_ptDaliCtl->tDaliPrm.nLastReqLevel = g_ptDaliCtl->nRequestedLevel;

    // process it/clear the flag
    ProcessOutputChange( );
    g_ptDaliCtl->tFlags.bOutputChangeRequested = FALSE;
  }

  // check for a reset requested flag
  if ( g_ptDaliCtl->tFlags.bResetRequested )
  {
    // turn off DALI
    SetDALIOff( );

    // reset the parameters to their default condition
    DALIMessageHandler_ResetUserDefaults( );

    // get the default parameters
    GetDefaultParameters( );
  
    // clear the reset flag/power fail flag
    g_ptDaliCtl->tFlags.bResetRequested = FALSE;
    g_ptDaliCtl->tDaliSts.tBits.bPowerFailure = FALSE;
  }

  // check if a lamp is physically disconnected
  if ( g_ptDaliCtl->tDaliSts.tBits.bLampFailure == TRUE)
  {
    // set the lamp failure/set the phsycial selected if requested
    if ( g_ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_REQUESTED )
    {
      // set the selection to OK
      g_ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_ENABLED;
    }
  }
  else
  {
    // clear the phsycial selected if enabled
    if ( g_ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED )
    {
      // set the selection to OK
      g_ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_REQUESTED;
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
  
  // check for a valid command
  if ( ptCmdEntry->pvCmdHandler != NULL )
  {
    // check for repeat
    if ( ptCmdEntry->tFlags.bRepeatFlag && !g_ptDaliCtl->tFlags.bRepeatTimerRequested && !g_ptDaliCtl->tFlags.bForcedMessage )
    {
      // copy the command for compare/set the status to reqeuest 100MS timer
      memcpy( &g_ptDaliCtl->tPrvMessage, &g_ptDaliCtl->tCurMessage, DALIMSG_SIZE );
      eStatus = PARSE_STS_REQ100MSTIMER;
    }
    else if ( ptCmdEntry->tFlags.bRepeatFlag && g_ptDaliCtl->tFlags.bRepeatTimerRequested  )
    {
      if ( memcmp( &g_ptDaliCtl->tPrvMessage, &g_ptDaliCtl->tCurMessage, DALIMSG_SIZE ) == 0 )
      {
        // check for a execute in time
        if (( ptCmdEntry->tFlags.bExecInTime && g_ptDaliCtl->tFlags.b15MinTimerRunning ) ||
            ( !ptCmdEntry->tFlags.bExecInTime ))
        {
          // set the execute command flag
          *( pbExecuteCmd ) = TRUE;
        }
      }
        
      // clear the timer
      eStatus = PARSE_STS_CLEARALLTIMER;
    }
    else if ( !ptCmdEntry->tFlags.bRepeatFlag && g_ptDaliCtl->tFlags.bRepeatTimerRequested )
    {
      // clear the timer
      eStatus = PARSE_STS_CLEARALLTIMER;
    }
    else if ( !ptCmdEntry->tFlags.bRepeatFlag || g_ptDaliCtl->tFlags.bForcedMessage )
    {
      // check for a execute in time
      if (( ptCmdEntry->tFlags.bExecInTime && g_ptDaliCtl->tFlags.b15MinTimerRunning ) ||
          ( !ptCmdEntry->tFlags.bExecInTime ))
      {
        // set the execute command flag
        *pbExecuteCmd = TRUE;

        // clear forced message
        g_ptDaliCtl->tFlags.bForcedMessage = FALSE;
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
static void ProcessOutputChange( )
{
  U32   uFadeTime;
  
  // check for a valid level
  if ( g_ptDaliCtl->nRequestedLevel != 0xFF )
  {
    // clear fade rate request/fade counter progress
    g_ptDaliCtl->tFlags.bFadeRateReq = FALSE;
    g_ptDaliCtl->uFadeCounter = 0;

    // clear error
    g_ptDaliCtl->tDaliSts.tBits.bLimitError = FALSE;

    // check for requested level greater than min
    if (( g_ptDaliCtl->nRequestedLevel < g_ptDaliCtl->tDaliPrm.nMinLevel ) && ( g_ptDaliCtl->nRequestedLevel != 0 ))
    {
      // set the requested level to the min level/set error
      g_ptDaliCtl->nRequestedLevel = g_ptDaliCtl->tDaliPrm.nMinLevel;
      g_ptDaliCtl->tDaliSts.tBits.bLimitError = TRUE;
    }

    // check for requested level greater than max
    if ( g_ptDaliCtl->nRequestedLevel > g_ptDaliCtl->tDaliPrm.nMaxLevel )
    {
      // set the requested level to the max level/set error
      g_ptDaliCtl->nRequestedLevel = g_ptDaliCtl->tDaliPrm.nMaxLevel;
      g_ptDaliCtl->tDaliSts.tBits.bLimitError = TRUE;
    }

    // is there a fade time
    if ( g_ptDaliCtl->tDaliPrm.nFadeTime != 0 )
    {
      // use the entry from the table and multiply by 1000 to change from fade in Milliseconds to fade in Microseconds
      g_ptDaliCtl->uFadePeriod = auFadeTimes[ g_ptDaliCtl->tDaliPrm.nFadeTime ] ;
    }
    else
    {
      // calculate the fast fade time in microseconds
      g_ptDaliCtl->uFadePeriod = g_ptDaliCtl->tDaliPrm.nFastFadeTime * FAST_FADE_TIME_MULT;
    }

    // determine fade up/fade down
    if ( g_ptDaliCtl->nRequestedLevel > g_ptDaliCtl->nActualDimLevel + 1 )
    {
      // set direction to up
      g_ptDaliCtl->eFadeDir = FADE_DIR_UP;

      // if off
      if ( g_ptDaliCtl->nActualDimLevel == 0 )
      {
        // set the fade increment to microseconds per step ( fade period = fade in microseconds ) based on min level
        g_ptDaliCtl->uFadeIncrement =  ( g_ptDaliCtl->uFadePeriod ) / ( g_ptDaliCtl->nRequestedLevel - g_ptDaliCtl->tDaliPrm.nMinLevel );
      }
      else
      {
        // set the fade increment base on dim level
        g_ptDaliCtl->uFadeIncrement =  ( g_ptDaliCtl->uFadePeriod ) / ( g_ptDaliCtl->nRequestedLevel - g_ptDaliCtl->nActualDimLevel );
      }
    }
    else
    {
      // check to see if we are below the actual level
      if ( g_ptDaliCtl->nRequestedLevel < g_ptDaliCtl->nActualDimLevel - 1 )
      {
        // set direction to down
        g_ptDaliCtl->eFadeDir = FADE_DIR_DN;
        
        // check for a zero level
        if ( g_ptDaliCtl->tCurMessage.tFields.nData == 0 )
        {
          // set the fade increment to microseconds per step ( fade period = fade in microseconds ) based on min level
          g_ptDaliCtl->uFadeIncrement =  ( g_ptDaliCtl->uFadePeriod ) / ( g_ptDaliCtl->nActualDimLevel - g_ptDaliCtl->tDaliPrm.nMinLevel );
        }
        else
        {
          // set the fade increment based on request level
          g_ptDaliCtl->uFadeIncrement =  ( g_ptDaliCtl->uFadePeriod ) / ( g_ptDaliCtl->nActualDimLevel - g_ptDaliCtl->nRequestedLevel );
        }
      }
      else
      {
        // fade time is zero
        g_ptDaliCtl->uFadePeriod = 0l;
      }
    }

    // test for zero
    if ( g_ptDaliCtl->uFadePeriod == 0l )
    {
      // fade time is zero/set the requested level
      g_ptDaliCtl->nActualDimLevel = g_ptDaliCtl->nRequestedLevel;
    }
  }
  else
  {
    // fade time is zero/set the requested level/clear fade running
    g_ptDaliCtl->nRequestedLevel = g_ptDaliCtl->nActualDimLevel;
    g_ptDaliCtl->uFadePeriod = 0l;
    g_ptDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
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
static void SetDALIOff( )
{
  // set the message state to off
  g_ptDaliCtl->eMsgState = MSG_STATE_OFF;

  // turn off the LED
  g_ptDaliCtl->nActualDimLevel = 0;
  OutputLightLevel( );

  // stop the receiver
  DALIMessageHandler_StopReceive( );
}

/******************************************************************************
 * @function GetDefaultParamaeters
 *
 * @brief get the default parameters and reset the condition to startup
 *
 * This function determine what time of bit was receiveid
 *
 *****************************************************************************/
static void GetDefaultParameters( )
{
  // get the parameters
  GetAllParameters( PARAM_SRC_DEFLT );

  // set the startup condition
  SetStartupCondition( );
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
  g_ptDaliCtl->tDaliPrm.nPowerOnLevel = GetParameterValue( eSrc, DALI_PARAM_POWERON_LEVEL );
  g_ptDaliCtl->tDaliPrm.nSystemFailureLevel = GetParameterValue( eSrc, DALI_PARAM_SYSFAIL_LEVEL );
  g_ptDaliCtl->tDaliPrm.nMinLevel = GetParameterValue( eSrc, DALI_PARAM_MIN_LEVEL );
  g_ptDaliCtl->tDaliPrm.nMaxLevel = GetParameterValue( eSrc, DALI_PARAM_MAX_LEVEL );
  g_ptDaliCtl->tDaliPrm.nLastReqLevel = GetParameterValue( eSrc, DALI_PARAM_LASTREQ_LEVEL );
  g_ptDaliCtl->tDaliPrm.nFadeRate = GetParameterValue( eSrc, DALI_PARAM_FADE_RATE );
  g_ptDaliCtl->tDaliPrm.nFadeTime = GetParameterValue( eSrc, DALI_PARAM_FADE_TIME );
  g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] = GetParameterValue( eSrc, DALI_PARAM_RANDOM_ADDR_H );
  g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] = GetParameterValue( eSrc, DALI_PARAM_RANDOM_ADDR_M );
  g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] = GetParameterValue( eSrc, DALI_PARAM_RANDOM_ADDR_L );
  g_ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_LSB_IDX ] = GetParameterValue( eSrc, DALI_PARAM_GROUP0007 );
  g_ptDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_MSB_IDX ] = GetParameterValue( eSrc, DALI_PARAM_GROUP0815 );

  // now copy the scenes
  for ( nScene = 0; nScene < DALI_NUM_SCENES; nScene++ )
  {
    // read the scene
    g_ptDaliCtl->tDaliPrm.anScenes[ nScene ] = GetParameterValue( eSrc, DALI_PARAM_SCENE00 + nScene );
  }

  // Get the local copies for the led variables
  g_ptDaliCtl->tDaliPrm.tFailureStatus.nFailureStatus = GetParameterValue( eSrc, DALI_PARAM_FAILURESTATUS );
  g_ptDaliCtl->tDaliPrm.tOpMode.nOpMode = GetParameterValue( eSrc, DALI_PARAM_OPMODE );
  g_ptDaliCtl->tDaliPrm.nFastFadeTime = GetParameterValue( eSrc, DALI_PARAM_FASTFADETIME );

  // Get Values from memory bank 0
  g_ptDaliCtl->nMinFastFadeTime = GetParameterValue( eSrc, DALI_PARAM_MB0_MINFASTFADETIME );
  g_ptDaliCtl->tGearType.nGearType = GetParameterValue( eSrc, DALI_PARAM_MB0_GEARTYPE );
  g_ptDaliCtl->tPossibleOpTypes.nPossibleOpTypes = GetParameterValue( eSrc, DALI_PARAM_MB0_POSSOPTYPES );
  g_ptDaliCtl->tFeatures.nFeatures = GetParameterValue( eSrc, DALI_PARAM_MB0_FEATURES );
  g_ptDaliCtl->nExtendedVerNum = GetParameterValue( eSrc, DALI_PARAM_MB0_EXTVERNUM );
  g_ptDaliCtl->nDeviceType = GetParameterValue( eSrc, DALI_PARAM_MB0_DEVTYPE );
  g_ptDaliCtl->nPhysicalMinLvl = GetParameterValue( eSrc, DALI_PARAM_MB0_PHYSICALMINLVL );
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
static U8 GetParameterValue( PARAMSRC eSrc, U8 nParam )
{
  U8 nValue;

  // which source
  switch( eSrc )
  {
    case PARAM_SRC_EEPROM :
      DALIMessageHandler_GetValue( nParam, &nValue );
      break;

    case PARAM_SRC_DEFLT :
      DALIMessageHandler_GetDefaultValue( nParam, &nValue );
      break;

    default :
      nValue = 0;
      break;
  }

  // return the value
  return( nValue );
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
static void SetStartupCondition( )
{
  BOOL bState;

  // reset the message state
  g_ptDaliCtl->eMsgState = MSG_STATE_IDLE;
  
  // reset the reset state indicator
  g_ptDaliCtl->tResetState.uWord = 0;

  //reset limit error
  g_ptDaliCtl->tDaliSts.tBits.bLimitError = FALSE;

  // reset any special flags
  g_ptDaliCtl->tFlags.bDAPCSeqEnabled = FALSE;
  g_ptDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
  g_ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;

  // initialize the search address to 0xFFFFFF
  g_ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_H ] = 0xFF;
  g_ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_M ] = 0xFF;
  g_ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_L ] = 0xFF;

  // reset the repeat timer state
  g_ptDaliCtl->tFlags.bRepeatTimerRequested = FALSE;

  // set the state of the missing short address flag
  g_ptDaliCtl->tDaliSts.tBits.bMissingShortAddr = ( g_ptDaliCtl->tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;  

  if ( g_ptDaliCtl->tFlags.bResetRequested == FALSE )
  {
    // check for power on level greater than maximum
    if ( g_ptDaliCtl->tDaliPrm.nPowerOnLevel == 0xFF )
    {
      // set the actual dim level to physical min level
      g_ptDaliCtl->nActualDimLevel = g_ptDaliCtl->tDaliPrm.nLastReqLevel;
    }
    else
    {
      // set the actual dim level
      g_ptDaliCtl->nActualDimLevel = g_ptDaliCtl->tDaliPrm.nPowerOnLevel;
    }
  }
  else
  { 
    //Reset Requested = true - set output to maximum
    g_ptDaliCtl->nActualDimLevel = ACTUAL_DIM_LVL_RESET;
  }

  // set the initial level
  OutputLightLevel( );

  // get the state of the DALI input pin
  Gpio_Get( DALI_BUS_STATUS, &bState );

  // is the bus power on
  if ( bState )
  {
    // start the receiver/clear test for bus power on flag
    DALIMessageHandler_StartReceive(( PU8 )&g_ptDaliCtl->tCurMessage.anBuffer, DALIMSG_SIZE );
    g_ptDaliCtl->tFlags.bTestBusPowerOn = FALSE;
  }
  else
  {
    // set the test for bus power on flag
    g_ptDaliCtl->tFlags.bTestBusPowerOn = TRUE;
  }
}

/******************************************************************************
 * @function  OutputLightLevel
 *
 * @brief output the light level
 *
 * This function will determine the appropriate light level mode and output
 * the value
 *
 *****************************************************************************/
static void OutputLightLevel( void )
{
  U16 wPercent;

  // determine if we are in linear or log
  if ( g_ptDaliCtl->tDaliPrm.tOpMode.tFields.bDimCurve )
  {
    // calculate the percentage based on scaling
    wPercent = MAP( g_ptDaliCtl->nActualDimLevel, 0, 254, 0, 1000 );
  }
  else
  {
    // get the percent from the table
    wPercent = awDimPercent[ g_ptDaliCtl->nActualDimLevel ];
  }

  // now output it
  DALIMessageHandler_SetLedLevel( wPercent );
}

/**@} EOF DALIMessageHandler.c */
