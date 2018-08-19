/******************************************************************************
 * @file  DALIProtocolHandler.c
 *
 * @brief
 *
 * This file
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup  DALIProtocolHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler.h"
#include "DALIProtocolHandler/NormalCommands/NormalCommands.h"
#include "DALIProtocolHandler/SpecialCommands/SpecialCommands.h"
#include "DALIProtocolHandler/ExtendedCommands/ExtendedCommands.h"

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

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	PDALICTL	g_ptLclDaliCtl;	  ///< dali control structure

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

/******************************************************************************
 * @function  DALIProtocolHandler_Initialize
 *
 * @brief initialie the DALI message handler
 *
 * This function will initialize the DALI message handler
 *
 * @param[in]   ptDaliCtl   pointer to the DALI control structure
 *
 *****************************************************************************/
void DALIProtocolHandler_Initialize( PDALICTL ptDaliCtl )
{
  // set the pointer
  g_ptLclDaliCtl = ptDaliCtl;
  
  // clear the initialized flag
  g_ptLclDaliCtl->tFlags.bDaliInitialized = FALSE;

  // call the initializations for normal/special/extended commands
  NormalCommands_Initialize( );
  SpecialCommands_Initialize( );
  ExtendedCommands_Initialize( );

  // clear the forced message flag
  g_ptLclDaliCtl->tFlags.bForcedMessage = FALSE;

  // clear the bus fail count
  g_ptLclDaliCtl->nBusFailCount = 0;

  // set the message state to idle
  g_ptLclDaliCtl->eMsgState == MSG_STATE_IDLE;

  // set the initial bus power off
  g_ptLclDaliCtl->tFlags.bTestBusPowerOn = FALSE;
}

/******************************************************************************
 * @function  DALIProtocolHandler_Control
 *
 * @brief enable/disable the DALI message handler
 *
 * This function will enable disable the DALI message handler based on the
 * mode of the system.
 *
 * @param[in]   ptDaliCtl   pointer to the DALI control structure
 * @param[in]   bState	the desired state of the DALI Message handler
 *
 *****************************************************************************/
void DALIProtocolHandler_Control( PDALICTL ptDaliCtl, BOOL bState )
{
  // set the pointer
  g_ptLclDaliCtl = ptDaliCtl;
  
  // check the requested state
  if (( bState == TRUE ) && ( g_ptLclDaliCtl->tFlags.bDaliInitialized == FALSE ))
  {
    // set the initialized flag
    g_ptLclDaliCtl->tFlags.bDaliInitialized = TRUE;

    // get all the parameters
    GetAllParameters( PARAM_SRC_EEPROM );
    g_ptLclDaliCtl->tDaliPrm.nShortAddress = GetParameterValue( PARAM_SRC_EEPROM, DALI_PARAM_SHORT_ADDR );

    // update status bits
    g_ptLclDaliCtl->tDaliSts.tBits.bPowerFailure = TRUE;

    // clear any flags
    g_ptLclDaliCtl->tFlags.b15MinTimerRunning = FALSE;
    
    // seed the random number generator from the serial number
    //DALIProtocolHandler_SeedRandom( );

    // set the startup condition
    SetStartupCondition( );
  }
  else if ( bState == FALSE )
  {
    // turn off DALI
    SetDALIOff( );

    // clear the initialized flag
    g_ptLclDaliCtl->tFlags.bDaliInitialized = FALSE;
  }
}

/******************************************************************************
 * @function  DALIProtocolHandler_ProcessEvent
 *
 * @brief process the incoming event
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   ptDaliCtl   pointer to the DALI control structure
 * @param[in]   xArg    this is an DALI Message event
 *
 *****************************************************************************/
DALIPROTEVNSTS DALIProtocolHandler_ProcessEvent( PDALICTL ptDaliCtl, TASKARG xArg )
{
  BOOL            bStartReceive = FALSE;
  U8              nResponse;
  DALIPROTEVNSTS  eStatus;

  // set the pointer
  g_ptLclDaliCtl = ptDaliCtl;
  
  // determine which state we are in
  switch( g_ptLclDaliCtl->eMsgState )
  {
    case MSG_STATE_IDLE :
      // we only process receive done here
      if ( xArg == DALI_MANCHESTER_RCV_DONE_EVENT )
      {
        // parse the message
        switch( ParseCommand( ))
        {
          case PARSE_STS_REQ100MSTIMER :
            // set the repeat timer running flag/start a 100 MS timer/restart receive
            g_ptLclDaliCtl->tFlags.bRepeatTimerRequested = TRUE;
            eStatus = DALIPROT_EVNSTS_REQ100MSTIMER;
            break;

          case PARSE_STS_REQ200MSTIMER :
            // set the repeat timer running flag/start a 100 MS timer/restart receive
            eStatus = DALIPROT_EVNSTS_REQ200MSTIMER;
            break;

          case PARSE_STS_SENDRESPONSE :
            // set the flags, change states
            g_ptLclDaliCtl->tFlags.bResponsePending = TRUE;
            g_ptLclDaliCtl->eMsgState = MSG_STATE_WAITXMITDONE;
            eStatus = DALIPROT_EVNSTS_XMTMSG;
            break;

          case PARSE_STS_CLEARALLTIMER :
            // clear the 100MS timer status/bAppExtRequested
            g_ptLclDaliCtl->tFlags.bRepeatTimerRequested = FALSE;
            g_ptLclDaliCtl->tFlags.bAppExtRequested = FALSE;
            eStatus = DALIPROT_EVNSTS_CLEARALLTIMERS;
            break;

          case PARSE_STS_NONE :
          default :
            // restart the manchester codec for receive
            eStatus = DALIPROT_EVNSTS_ENBRCV;
            break;
        }
      }
      break;

    case MSG_STATE_WAITXMITDONE :
      // was this a transmit done
      if ( xArg == DALI_MANCHESTER_XMT_DONE_EVENT )
      {
        // restart the receive and go back to idle
        g_ptLclDaliCtl->eMsgState = MSG_STATE_IDLE;

        // restart the manchester codec for receive
        eStatus = DALIPROT_EVNSTS_ENBRCV;
      }
      break;

    default :
      break;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function  DALIProtocolHandler_ProcessTimer
 *
 * @brief process the incoming event
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   ptDaliCtl   pointer to the DALI control structure
 *
 *****************************************************************************/
void DALIProtocolHandler_ProcessTimer( PDALICTL ptDaliCtl )
{
  // clear the 15 min timer
  ptDaliCtl->tFlags.b15MinTimerRunning = FALSE;
}

/******************************************************************************
 * @function  DALIProtocolHandler_ProcessOutput
 *
 * @brief process the output
 *
 * This function will determine the event and apply the process each event
 * appropriately
 *
 * @param[in]   ptDaliCtl       pointer to the DALI control structure
 * @param[in]   uTimeIncrement  time increment in microseconds
 *
 *****************************************************************************/
void DALIProtocolHandler_ProcessOutput( PDALICTL ptDaliCtl, U32 uTimeIncrement )
{
  U16 wOutputLevel;

  // set the pointer
  g_ptLclDaliCtl = ptDaliCtl;

  // process the fading
  if ( g_ptLclDaliCtl->uFadePeriod != 0 )
  {
    // increment the fade counter
    g_ptLclDaliCtl->uFadeCounter += uTimeIncrement;

    // decrement fade period only if processing a fade rate
    if ( g_ptLclDaliCtl->tFlags.bFadeRateReq == TRUE )
    {
      g_ptLclDaliCtl->uFadePeriod--;
    }

    // check for counter greater than increment - this is when the actual level is increased
    if ( g_ptLclDaliCtl->uFadeCounter >= g_ptLclDaliCtl->uFadeIncrement )
    {
      // reset the fade counter - but keep remainder
      g_ptLclDaliCtl->uFadeCounter -= g_ptLclDaliCtl->uFadeIncrement;

      // which direction are we running in
      switch( g_ptLclDaliCtl->eFadeDir )
      {
        case FADE_DIR_UP :
          // if actual level is 0, set to min
          if ( g_ptLclDaliCtl->nActualDimLevel == 0 )
          {
            // set to minimum
            g_ptLclDaliCtl->nActualDimLevel = g_ptLclDaliCtl->tDaliPrm.nMinLevel;
          }
          else if ( g_ptLclDaliCtl->nActualDimLevel < g_ptLclDaliCtl->tDaliPrm.nMaxLevel )
          {
            // increment it
            g_ptLclDaliCtl->nActualDimLevel++;
          }
          break;

        case FADE_DIR_DN :
          // if actual level is above minimum
          if ( g_ptLclDaliCtl->nActualDimLevel > g_ptLclDaliCtl->tDaliPrm.nMinLevel )
          {
            // decrement the actual level
            g_ptLclDaliCtl->nActualDimLevel--;
          }
          else
          {
            // set it to 0 if we are not using fade rate
            if ( g_ptLclDaliCtl->tFlags.bFadeRateReq == FALSE )
            {
              g_ptLclDaliCtl->nActualDimLevel = 0;
            }
          }
          break;

        default :
          break;
      }

      // have we completed the fade
      if (( g_ptLclDaliCtl->nActualDimLevel == g_ptLclDaliCtl->nRequestedLevel ) && ( g_ptLclDaliCtl->tFlags.bFadeRateReq == FALSE ))
      {
        // if we have completed a fade using fade time
        // reset the fade period/reset the fade status/fade increment/fade counter progress
        g_ptLclDaliCtl->uFadePeriod = 0;
        g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
        g_ptLclDaliCtl->uFadeIncrement = 0;
        g_ptLclDaliCtl->uFadeCounter = 0;
      }
      else if (( g_ptLclDaliCtl->uFadePeriod == 0 ) && ( g_ptLclDaliCtl->tFlags.bFadeRateReq == TRUE ))
      {
        // if we have completed a fade using fade rate
        // reset the fade status/fade increment/fade counter progress/set fade rate request to false
        g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
        g_ptLclDaliCtl->uFadeIncrement = 0;
        g_ptLclDaliCtl->tFlags.bFadeRateReq = FALSE;
        g_ptLclDaliCtl->uFadeCounter = 0;
      }
      else
      {
        // set the fade status to true
        g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = TRUE;
      }
    }
    else if (( g_ptLclDaliCtl->uFadePeriod == 0 ) && ( g_ptLclDaliCtl->tFlags.bFadeRateReq ))
    {
      // if we have completed a fade using a fade rate but not incremented the light level
      // reset the fade status/fade increment/fade counter progress/set fade rate request to false
      g_ptLclDaliCtl->uFadeIncrement = 0;
      g_ptLclDaliCtl->uFadeCounter = 0;
      g_ptLclDaliCtl->tFlags.bFadeRateReq = FALSE;
      g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
    }
  }

  // is the actual limit equal or above the minimum level
  if ( g_ptLclDaliCtl->nActualDimLevel >= g_ptLclDaliCtl->tDaliPrm.nMinLevel )
  {
    // clamp the actual limit to the max
    if ( g_ptLclDaliCtl->nActualDimLevel > g_ptLclDaliCtl->tDaliPrm.nMaxLevel )
    {
      // set it to max
      g_ptLclDaliCtl->nActualDimLevel = g_ptLclDaliCtl->tDaliPrm.nMaxLevel;
    }

    // set the lamp status to on/set the output level based on table
    g_ptLclDaliCtl->tDaliSts.tBits.bLampOn = ( DALIProtocolHandler_GetLedStatus( )) ? TRUE : FALSE;
    g_ptLclDaliCtl->tDaliSts.tBits.bLampFailure = !( g_ptLclDaliCtl->tDaliSts.tBits.bLampOn );
  }
  else
  {
    // set the lamp status to off/set the output level to 0
    g_ptLclDaliCtl->tDaliSts.tBits.bLampOn = FALSE;
    g_ptLclDaliCtl->nActualDimLevel = 0;
  }
  
  // compare to reset state value
  g_ptLclDaliCtl->tResetState.tBits.bActualLevel = ( g_ptLclDaliCtl->nActualDimLevel != ACTUAL_DIM_LVL_RESET ) ? TRUE : FALSE;

  // set the actual/set the new level to the led manager
  OutputLightLevel( );
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
  if (( g_ptLclDaliCtl->tCurMessage.anBuffer[ 0 ] >= DALI_MIN_SPCLEXT_CMD ) && 
      ( g_ptLclDaliCtl->tCurMessage.anBuffer[ 0 ] <= DALI_MAX_SPCLEXT_CMD ) &&
      ( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.bValueCmd )) 
  {
    // check to see if a DAPC sequence is funning
    if ( g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
    {
      // clear the sequence running flag/restore the previous fade time
      g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
      g_ptLclDaliCtl->tDaliPrm.nFadeTime = g_ptLclDaliCtl->nCurFadeTime;
      g_ptLclDaliCtl->uFadePeriod = 0;
      g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
    }

    // are we in an extended state
    if ( g_ptLclDaliCtl->tFlags.bAppExtRequested )
    {
      // clear the Application extended requested flag
      g_ptLclDaliCtl->tFlags.bAppExtRequested = FALSE;
    }
    else
    {
      // determine the type of command/and the actual command
      nType = DALI_GET_TYPE( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.nAddress );
      nCommand = DALI_GET_CMND( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.nAddress );
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
            g_ptLclDaliCtl->tFlags.bWriteMemoryEnabled = FALSE;
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
    if ((( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.bDirectGroup == TRUE ) && ( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.nAddress == 0x3F )) ||
        (( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.bDirectGroup == FALSE ) && ( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.nAddress == g_ptLclDaliCtl->tDaliPrm.nShortAddress )) ||
        (( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.bDirectGroup == TRUE ) && (( BIT(( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.nAddress & 0x0F )) & g_ptLclDaliCtl->tDaliPrm.tGroup.wValue ) != 0 )))
    {
      // clear write memory enabled
      g_ptLclDaliCtl->tFlags.bWriteMemoryEnabled = FALSE;
      
       // first determine if this is a direct ARC power control
      if ( g_ptLclDaliCtl->tCurMessage.tFields.tAddr.bValueCmd == FALSE )
      {
        // set the requested level/clear the reset/power failure status bits/set the output change request flag
        g_ptLclDaliCtl->nRequestedLevel = g_ptLclDaliCtl->tCurMessage.tFields.nData;
        g_ptLclDaliCtl->tDaliSts.tBits.bPowerFailure = FALSE;
        g_ptLclDaliCtl->tFlags.bOutputChangeRequested = TRUE;
        
        // if DAPC is running
        if ( g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
        {
          // request a reset of the timer
          eStatus = PARSE_STS_REQ200MSTIMER;
        }
      }
      else
      {
        // check to see if a DAPC sequence is funning
        if ( g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
        {
          // clear the sequence running flag/restore the previous fade time
          g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
          g_ptLclDaliCtl->tDaliPrm.nFadeTime = g_ptLclDaliCtl->nCurFadeTime;
          g_ptLclDaliCtl->uFadePeriod = 0;
          g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
        }

        // get the command/table entry
        nCommand = g_ptLclDaliCtl->tCurMessage.tFields.nData;
        ptCmdEntry = ( PCMDTBL )PGM_RDWORD_PTR( &atNormCmds[ nCommand ] );
        
        // test for application extended mismatch
        if ( !( ptCmdEntry->tFlags.bDevTypeEnabled && !g_ptLclDaliCtl->tFlags.bAppExtRequested ))
        {
          // test for execution
          eStatus = ProcessCommandEntry( ptCmdEntry, &bExecuteCommand );
          
          // if execute command
          if ( bExecuteCommand ) 
          {
            // clear the application extension requested flag
            g_ptLclDaliCtl->tFlags.bAppExtRequested = FALSE;
            
            // reset power failure if a level command
            if ( nCommand <= NRM_INDIRECT_LVLCMD_MAX )
            {
              // set it to false
              g_ptLclDaliCtl->tDaliSts.tBits.bPowerFailure = FALSE;
            }
          }
        }
        else
        {
          // clear the application extension requested flag
          g_ptLclDaliCtl->tFlags.bAppExtRequested = FALSE;
        }
      }
    }
  }

  // check for execute command
  if ( bExecuteCommand )
  {
    //  execute the command
    ptCmdEntry->pvCmdHandler( );

    // check for a reset requested flag
    if ( g_ptLclDaliCtl->tFlags.bResetRequested )
    {
      // turn off DALI
      SetDALIOff( );

      // reset the parameters to their default condition
      DALIProtocolHandler_ResetUserDefaults( );

      // get the default parameters
      GetDefaultParameters( );
    
      // clear the reset flag/power fail flag
      g_ptLclDaliCtl->tFlags.bResetRequested = FALSE;
      g_ptLclDaliCtl->tDaliSts.tBits.bPowerFailure = FALSE;
    }
    else
    {
      // check for response
      if (( ptCmdEntry->tFlags.eResponseType == RESPONSE_TYPE_SEND ) || 
         (( ptCmdEntry->tFlags.eResponseType == RESPONSE_TYPE_FLAG ) && ( g_ptLclDaliCtl->tFlags.bResponseRequested == TRUE )))
      {
        // set return type to indicate a response needs to be sent
        eStatus = PARSE_STS_SENDRESPONSE;
      }

      // check for a DAPC sequence running flag
      if ( g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning == TRUE )
      {
        // request a reset of the 200 ms timer
        eStatus = PARSE_STS_REQ200MSTIMER;
      }
    }

    // check for change in actual dim level
    if (( g_ptLclDaliCtl->tDaliPrm.nLastReqLevel != g_ptLclDaliCtl->nActualDimLevel ) && ( !g_ptLclDaliCtl->tFlags.bOutputChangeRequested ))
    {
       // update the NVM
       DALIProtocolHandler_PutValue( DALI_PARAM_LASTREQ_LEVEL, g_ptLclDaliCtl->nActualDimLevel );
       g_ptLclDaliCtl->tDaliPrm.nLastReqLevel = g_ptLclDaliCtl->nActualDimLevel;
    }
    
    // check for a output change
    if ( g_ptLclDaliCtl->tFlags.bOutputChangeRequested )
    {
      // update the NVM
      DALIProtocolHandler_PutValue( DALI_PARAM_LASTREQ_LEVEL, g_ptLclDaliCtl->nRequestedLevel );
      g_ptLclDaliCtl->tDaliPrm.nLastReqLevel = g_ptLclDaliCtl->nRequestedLevel;

      // process it/clear the flag
      ProcessOutputChange( );
      g_ptLclDaliCtl->tFlags.bOutputChangeRequested = FALSE;
    }
  }

  // check if a lamp is physically disconnected
  if ( g_ptLclDaliCtl->tDaliSts.tBits.bLampFailure == TRUE)
  {
    // set the lamp failure/set the phsycial selected if requested
    if ( g_ptLclDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_REQUESTED )
    {
      // set the selection to OK
      g_ptLclDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_ENABLED;
    }
  }
  else
  {
    // clear the phsycial selected if enabled
    if ( g_ptLclDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED )
    {
      // set the selection to OK
      g_ptLclDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_REQUESTED;
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
    if ( ptCmdEntry->tFlags.bRepeatFlag && !g_ptLclDaliCtl->tFlags.bRepeatTimerRequested && !g_ptLclDaliCtl->tFlags.bForcedMessage )
    {
      // copy the command for compare/set the status to reqeuest 100MS timer
      memcpy( &g_ptLclDaliCtl->tPrvMessage, &g_ptLclDaliCtl->tCurMessage, DALIMSG_SIZE );
      eStatus = PARSE_STS_REQ100MSTIMER;
    }
    else if ( ptCmdEntry->tFlags.bRepeatFlag && g_ptLclDaliCtl->tFlags.bRepeatTimerRequested  )
    {
      if ( memcmp( &g_ptLclDaliCtl->tPrvMessage, &g_ptLclDaliCtl->tCurMessage, DALIMSG_SIZE ) == 0 )
      {
        // check for a execute in time
        if (( ptCmdEntry->tFlags.bExecInTime && g_ptLclDaliCtl->tFlags.b15MinTimerRunning ) ||
            ( !ptCmdEntry->tFlags.bExecInTime ))
        {
          // set the execute command flag
          *( pbExecuteCmd ) = TRUE;
        }
      }
        
      // clear the timer
      eStatus = PARSE_STS_CLEARALLTIMER;
    }
    else if ( !ptCmdEntry->tFlags.bRepeatFlag && g_ptLclDaliCtl->tFlags.bRepeatTimerRequested )
    {
      // clear the timer
      eStatus = PARSE_STS_CLEARALLTIMER;
    }
    else if ( !ptCmdEntry->tFlags.bRepeatFlag || g_ptLclDaliCtl->tFlags.bForcedMessage )
    {
      // check for a execute in time
      if (( ptCmdEntry->tFlags.bExecInTime && g_ptLclDaliCtl->tFlags.b15MinTimerRunning ) ||
          ( !ptCmdEntry->tFlags.bExecInTime ))
      {
        // set the execute command flag
        *pbExecuteCmd = TRUE;

        // clear forced message
        g_ptLclDaliCtl->tFlags.bForcedMessage = FALSE;
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
  if ( g_ptLclDaliCtl->nRequestedLevel != 0xFF )
  {
    // clear fade rate request/fade counter progress
    g_ptLclDaliCtl->tFlags.bFadeRateReq = FALSE;
    g_ptLclDaliCtl->uFadeCounter = 0;

    // clear error
    g_ptLclDaliCtl->tDaliSts.tBits.bLimitError = FALSE;

    // check for requested level greater than min
    if (( g_ptLclDaliCtl->nRequestedLevel < g_ptLclDaliCtl->tDaliPrm.nMinLevel ) && ( g_ptLclDaliCtl->nRequestedLevel != 0 ))
    {
      // set the requested level to the min level/set error
      g_ptLclDaliCtl->nRequestedLevel = g_ptLclDaliCtl->tDaliPrm.nMinLevel;
      g_ptLclDaliCtl->tDaliSts.tBits.bLimitError = TRUE;
    }

    // check for requested level greater than max
    if ( g_ptLclDaliCtl->nRequestedLevel > g_ptLclDaliCtl->tDaliPrm.nMaxLevel )
    {
      // set the requested level to the max level/set error
      g_ptLclDaliCtl->nRequestedLevel = g_ptLclDaliCtl->tDaliPrm.nMaxLevel;
      g_ptLclDaliCtl->tDaliSts.tBits.bLimitError = TRUE;
    }

    // is there a fade time
    if ( g_ptLclDaliCtl->tDaliPrm.nFadeTime != 0 )
    {
      // use the entry from the table and multiply by 1000 to change from fade in Milliseconds to fade in Microseconds
      g_ptLclDaliCtl->uFadePeriod = auFadeTimes[ g_ptLclDaliCtl->tDaliPrm.nFadeTime ] ;
    }
    else
    {
      // calculate the fast fade time in microseconds
      g_ptLclDaliCtl->uFadePeriod = g_ptLclDaliCtl->tDaliPrm.nFastFadeTime * FAST_FADE_TIME_MULT;
    }

    // determine fade up/fade down
    if ( g_ptLclDaliCtl->nRequestedLevel > g_ptLclDaliCtl->nActualDimLevel + 1 )
    {
      // set direction to up
      g_ptLclDaliCtl->eFadeDir = FADE_DIR_UP;

      // if off
      if ( g_ptLclDaliCtl->nActualDimLevel == 0 )
      {
        // set the fade increment to microseconds per step ( fade period = fade in microseconds ) based on min level
        g_ptLclDaliCtl->uFadeIncrement =  ( g_ptLclDaliCtl->uFadePeriod ) / ( g_ptLclDaliCtl->nRequestedLevel - g_ptLclDaliCtl->tDaliPrm.nMinLevel );
      }
      else
      {
        // set the fade increment base on dim level
        g_ptLclDaliCtl->uFadeIncrement =  ( g_ptLclDaliCtl->uFadePeriod ) / ( g_ptLclDaliCtl->nRequestedLevel - g_ptLclDaliCtl->nActualDimLevel );
      }
    }
    else
    {
      // check to see if we are below the actual level
      if ( g_ptLclDaliCtl->nRequestedLevel < g_ptLclDaliCtl->nActualDimLevel - 1 )
      {
        // set direction to down
        g_ptLclDaliCtl->eFadeDir = FADE_DIR_DN;
        
        // check for a zero level
        if ( g_ptLclDaliCtl->tCurMessage.tFields.nData == 0 )
        {
          // set the fade increment to microseconds per step ( fade period = fade in microseconds ) based on min level
          g_ptLclDaliCtl->uFadeIncrement =  ( g_ptLclDaliCtl->uFadePeriod ) / ( g_ptLclDaliCtl->nActualDimLevel - g_ptLclDaliCtl->tDaliPrm.nMinLevel );
        }
        else
        {
          // set the fade increment based on request level
          g_ptLclDaliCtl->uFadeIncrement =  ( g_ptLclDaliCtl->uFadePeriod ) / ( g_ptLclDaliCtl->nActualDimLevel - g_ptLclDaliCtl->nRequestedLevel );
        }
      }
      else
      {
        // fade time is zero
        g_ptLclDaliCtl->uFadePeriod = 0l;
      }
    }

    // test for zero
    if ( g_ptLclDaliCtl->uFadePeriod == 0l )
    {
      // fade time is zero/set the requested level
      g_ptLclDaliCtl->nActualDimLevel = g_ptLclDaliCtl->nRequestedLevel;
    }
  }
  else
  {
    // fade time is zero/set the requested level/clear fade running
    g_ptLclDaliCtl->nRequestedLevel = g_ptLclDaliCtl->nActualDimLevel;
    g_ptLclDaliCtl->uFadePeriod = 0l;
    g_ptLclDaliCtl->tDaliSts.tBits.bFadeRunning = FALSE;
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
  g_ptLclDaliCtl->eMsgState = MSG_STATE_OFF;

  // turn off the LED
  g_ptLclDaliCtl->nActualDimLevel = 0;
  OutputLightLevel( );

  // stop the receiver
  DALIProtocolHandler_StopReceive( );
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
  g_ptLclDaliCtl->tDaliPrm.nPowerOnLevel = GetParameterValue( eSrc, DALI_PARAM_POWERON_LEVEL );
  g_ptLclDaliCtl->tDaliPrm.nSystemFailureLevel = GetParameterValue( eSrc, DALI_PARAM_SYSFAIL_LEVEL );
  g_ptLclDaliCtl->tDaliPrm.nMinLevel = GetParameterValue( eSrc, DALI_PARAM_MIN_LEVEL );
  g_ptLclDaliCtl->tDaliPrm.nMaxLevel = GetParameterValue( eSrc, DALI_PARAM_MAX_LEVEL );
  g_ptLclDaliCtl->tDaliPrm.nLastReqLevel = GetParameterValue( eSrc, DALI_PARAM_LASTREQ_LEVEL );
  g_ptLclDaliCtl->tDaliPrm.nFadeRate = GetParameterValue( eSrc, DALI_PARAM_FADE_RATE );
  g_ptLclDaliCtl->tDaliPrm.nFadeTime = GetParameterValue( eSrc, DALI_PARAM_FADE_TIME );
  g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] = GetParameterValue( eSrc, DALI_PARAM_RANDOM_ADDR_H );
  g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] = GetParameterValue( eSrc, DALI_PARAM_RANDOM_ADDR_M );
  g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] = GetParameterValue( eSrc, DALI_PARAM_RANDOM_ADDR_L );
  g_ptLclDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_LSB_IDX ] = GetParameterValue( eSrc, DALI_PARAM_GROUP0007 );
  g_ptLclDaliCtl->tDaliPrm.tGroup.anValue[ LE_U16_MSB_IDX ] = GetParameterValue( eSrc, DALI_PARAM_GROUP0815 );

  // now copy the scenes
  for ( nScene = 0; nScene < DALI_NUM_SCENES; nScene++ )
  {
    // read the scene
    g_ptLclDaliCtl->tDaliPrm.anScenes[ nScene ] = GetParameterValue( eSrc, DALI_PARAM_SCENE00 + nScene );
  }

  // Get the local copies for the led variables
  g_ptLclDaliCtl->tDaliPrm.tFailureStatus.nFailureStatus = GetParameterValue( eSrc, DALI_PARAM_FAILURESTATUS );
  g_ptLclDaliCtl->tDaliPrm.tOpMode.nOpMode = GetParameterValue( eSrc, DALI_PARAM_OPMODE );
  g_ptLclDaliCtl->tDaliPrm.nFastFadeTime = GetParameterValue( eSrc, DALI_PARAM_FASTFADETIME );

  // Get Values from memory bank 0
  g_ptLclDaliCtl->nMinFastFadeTime = GetParameterValue( eSrc, DALI_PARAM_MB0_MINFASTFADETIME );
  g_ptLclDaliCtl->tGearType.nGearType = GetParameterValue( eSrc, DALI_PARAM_MB0_GEARTYPE );
  g_ptLclDaliCtl->tPossibleOpTypes.nPossibleOpTypes = GetParameterValue( eSrc, DALI_PARAM_MB0_POSSOPTYPES );
  g_ptLclDaliCtl->tFeatures.nFeatures = GetParameterValue( eSrc, DALI_PARAM_MB0_FEATURES );
  g_ptLclDaliCtl->nExtendedVerNum = GetParameterValue( eSrc, DALI_PARAM_MB0_EXTVERNUM );
  g_ptLclDaliCtl->nDeviceType = GetParameterValue( eSrc, DALI_PARAM_MB0_DEVTYPE );
  g_ptLclDaliCtl->nPhysicalMinLvl = GetParameterValue( eSrc, DALI_PARAM_MB0_PHYSICALMINLVL );
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
      DALIProtocolHandler_GetValue( nParam, &nValue );
      break;

    case PARAM_SRC_DEFLT :
      DALIProtocolHandler_GetDefaultValue( nParam, &nValue );
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
  g_ptLclDaliCtl->eMsgState = MSG_STATE_IDLE;
  
  // reset the reset state indicator
  g_ptLclDaliCtl->tResetState.uWord = 0;

  //reset limit error
  g_ptLclDaliCtl->tDaliSts.tBits.bLimitError = FALSE;

  // reset any special flags
  g_ptLclDaliCtl->tFlags.bDAPCSeqEnabled = FALSE;
  g_ptLclDaliCtl->tFlags.bDAPCSequenceRunning = FALSE;
  g_ptLclDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;

  // initialize the search address to 0xFFFFFF
  g_ptLclDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_H ] = 0xFF;
  g_ptLclDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_M ] = 0xFF;
  g_ptLclDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_L ] = 0xFF;

  // reset the repeat timer state
  g_ptLclDaliCtl->tFlags.bRepeatTimerRequested = FALSE;

  // set the state of the missing short address flag
  g_ptLclDaliCtl->tDaliSts.tBits.bMissingShortAddr = ( g_ptLclDaliCtl->tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;  

  if ( g_ptLclDaliCtl->tFlags.bResetRequested == FALSE )
  {
    // check for power on level greater than maximum
    if ( g_ptLclDaliCtl->tDaliPrm.nPowerOnLevel == 0xFF )
    {
      // set the actual dim level to physical min level
      g_ptLclDaliCtl->nActualDimLevel = g_ptLclDaliCtl->tDaliPrm.nLastReqLevel;
    }
    else
    {
      // set the actual dim level
      g_ptLclDaliCtl->nActualDimLevel = g_ptLclDaliCtl->tDaliPrm.nPowerOnLevel;
    }
  }
  else
  { 
    //Reset Requested = true - set output to maximum
    g_ptLclDaliCtl->nActualDimLevel = ACTUAL_DIM_LVL_RESET;
  }

  // set the initial level
  OutputLightLevel( );

/**
 *  // get the state of the DALI input pin
 *  Gpio_Get( DALI_BUS_STATUS, &bState );
 *
 *  // is the bus power on
 *  if ( bState )
 *  {
 *    // start the receiver/clear test for bus power on flag
 *    DALIProtocolHandler_StartReceive(( PU8 )&g_ptLclDaliCtl->tCurMessage.anBuffer, DALIMSG_SIZE );
 *    g_ptLclDaliCtl->tFlags.bTestBusPowerOn = FALSE;
 *  }
 *  else
 *  {
 *    // set the test for bus power on flag
 *    g_ptLclDaliCtl->tFlags.bTestBusPowerOn = TRUE;
 *  }
 */
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
  if ( g_ptLclDaliCtl->tDaliPrm.tOpMode.tFields.bDimCurve )
  {
    // calculate the percentage based on scaling
    wPercent = MAP( g_ptLclDaliCtl->nActualDimLevel, 0, 254, 0, 1000 );
  }
  else
  {
    // get the percent from the table
    wPercent = awDimPercent[ g_ptLclDaliCtl->nActualDimLevel ];
  }

  // now output it
  DALIProtocolHandler_SetLedLevel( wPercent );
}

/**@} EOF DALIProtocolHandler.c */
