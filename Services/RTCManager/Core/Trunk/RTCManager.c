/******************************************************************************
 * @file RTCManager.c
 *
 * @brief RTC Manager implementation 
 *
 * This file provides the implementation for the Real Time Clock Manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup RTCManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RTCManager/RTCManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL      bFastTimeEnabled;
static  DATETIME  tFastTime;

// local function prototypes --------------------------------------------------
/// command handlers
#if ( RTCMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
static  ASCCMDSTS CmdQryDatTim( U8 nCmdEnum );
static  ASCCMDSTS CmdSetDatTim( U8 nCmdEnum );
#endif  // RTCMANAGER_ENABLE_DEBUG_COMMANDS

// constant parameter initializations -----------------------------------------
#if ( RTCMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
/// declare the command strings
static  const CODE C8 szQryDatTim[ ]   = { "QRYDATTIM" };
static  const CODE C8 szSetDatTim[ ]   = { "SETDATTIM" };

/// define the response string
static  const CODE C8 szRspDatTim[ ]   = { "RDATTIM, %02d/%02d/%4d-%02d:%02d:%02d\n\r" };
  
/// initialize the command table
const CODE ASCCMDENTRY g_atRtcManagerCmdHandlerTable[ ] =
{
  ASCCMD_ENTRY( szQryDatTim, 9, 0, ASCFLAG_COMPARE_NONE, 0, CmdQryDatTim ),
  ASCCMD_ENTRY( szSetDatTim, 9, 6, ASCFLAG_COMPARE_NONE, 0, CmdSetDatTim ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};
#endif  // RTCMANAGER_ENABLE_DEBUG_COMMANDS

/******************************************************************************
 * @function RTCManager_Initialize
 *
 * @brief initialization
 *
 * This function will perform any required module initialization
 *
 *****************************************************************************/
void RTCManager_Initialize( void )
{
  // disable fast time
  bFastTimeEnabled = 0;
}

/******************************************************************************
 * @function RTCManager_EnableDisableFastTime
 *
 * @brief fast time enable/disable 
 *
 * This function will enable/disable the fast time function
 *
 * @param[in]   bState      desired state
 *
 *****************************************************************************/
void RTCManager_EnableDisableFastTime( BOOL bState )
{
  // set the flag accordingly
  if (( bFastTimeEnabled = bState ) == TRUE )
  {
    // reset the fast time
    memset( &tFastTime, 0, DATETIME_SIZE );
    tFastTime.nDayOfWeek = 1;
  }
}

/******************************************************************************
 * @function RTCManager_GetDateTime
 *
 * @brief get date/time
 *
 * This function will get the current date/time from either the internal/external
 * RTC or the fast time generator
 *
 * @param[i0]   ptDataTime    pointer to store the returned data time
 *
 *****************************************************************************/
void RTCManager_GetDateTime( PDATETIME ptDateTime )
{
  // check for fast time
  if ( bFastTimeEnabled )
  {
    // copy the fast time data
    memcpy( ptDateTime, &tFastTime, DATETIME_SIZE );
  }
  else
  {
    // determine which real time clock we are using
    if ( RTCMANAGER_INTEXT_SELECTION == RTCMANAGER_EXTERNAL_RTC )
    {
      // get external RTC
      RTCManager_GetExternalRTC( ptDateTime );
    } 
    else
    {
      // get internal RTC
      RTCManager_GetInternalRTC( ptDateTime );
    }
  }
}

/******************************************************************************
 * @function RTCManager_SetDateTime
 *
 * @brief set date/time
 *
 * This function will set the current date/time from either the internal/external
 * RTC or the fast time generator
 *
 * @param[i0]   ptDataTime    pointer to store the returned data time
 *
 *****************************************************************************/
void RTCManager_SetDateTime( PDATETIME ptDateTime )
{
  // determine which real time clock we are using
  if ( RTCMANAGER_INTEXT_SELECTION == RTCMANAGER_EXTERNAL_RTC )
  {
    // get external RTC
    RTCManager_SetExternalRTC( ptDateTime );
  } 
  else
  {
    // get internal RTC
    RTCManager_SetInternalRTC( ptDateTime );
  }
}

/******************************************************************************
 * @function RTCManager_ProcessEvent
 *
 * @brief task handler
 *
 * This function will process the timeout event
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush events
 *
 *****************************************************************************/
BOOL RTCManager_ProcessEvent( TASKARG xArg )
{
  // if fast time is enabled
  if ( bFastTimeEnabled )
  {
    // now increment minutes
    if ( ++tFastTime.nMinutes > 59 )
    {
      // reset minutes/adjust hours
      tFastTime.nMinutes = 0;
      if ( ++tFastTime.nHours > 23 )
      {
        // reset hours/adjust day of week
        tFastTime.nHours = 0;
        if ( ++tFastTime.nDayOfWeek > 7 )
        {
          // reset day of week
          tFastTime.nDayOfWeek = 1;
        }
      }
    }
  }
  
  // always flush event
  return( TRUE );
}

#if ( RTCMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
/******************************************************************************
 * @function CmdQryDtm
 *
 * @brief query the Date Time
 *
 * This function outputs the current date/time
 *
 * @param[in]   nCmdEnum    command handler enumeration
 *
 * @return  appropriate status
 *****************************************************************************/
static ASCCMDSTS CmdQryDatTim( U8 nCmdEnum )
{
  DATETIME  tDateTime;
  PC8       pcBuffer;

  // get the pointer to the buffer
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the current date/time
  RTCManager_GetDateTime( &tDateTime );

  // output the result
  SPRINTF_P( pcBuffer, ( char const * )szRspDatTim, tDateTime.nMonth, tDateTime.nDay, tDateTime.wYear, tDateTime.nHours, tDateTime.nMinutes, tDateTime.nSeconds );

  // return status
  return( ASCCMD_STS_OUTPUTBUFFER );
}

/******************************************************************************
 * @function CmdSetDatTim
 *
 * @brief set the current date/time
 *
 * This function sets the current date/time
 *
 * @param[in]   nCmdEnum    command handler enumeration
 *
 * @return  Aappropriate statue
 *****************************************************************************/
static ASCCMDSTS CmdSetDatTim( U8 nCmdEnum )
{
  U32UN     tTemp;
  DATETIME  tDateTime;

  // get the date
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );
  tDateTime.nMonth = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 1, &tTemp.uValue );
  tDateTime.nDay = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 2, &tTemp.uValue );
  tDateTime.wYear = tTemp.awValue[ LE_U32_LSW_IDX ];

  // get the time
  AsciiCommandHandler_GetValue( nCmdEnum, 3, &tTemp.uValue );
  tDateTime.nHours = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 4, &tTemp.uValue );
  tDateTime.nMinutes = tTemp.anValue[ LE_U32_LSB_IDX ];
  AsciiCommandHandler_GetValue( nCmdEnum, 5, &tTemp.uValue );
  tDateTime.nSeconds = tTemp.anValue[ LE_U32_LSB_IDX ];

  // set it
  RTCManager_SetDateTime( &tDateTime );

  // return the error
  return( ASCCMD_STS_NONE );
}

#endif  // RTCMANAGER_ENABLE_DEBUG_COMMANDS

/**@} EOF RTCManager.c */
