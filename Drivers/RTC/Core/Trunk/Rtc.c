/******************************************************************************
 * @file Rtc.c
 *
 * @brief RTC ( software driven ) declarations
 *
 * This file provides the declarations for the RTC software driven
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
 * \addtogroup Rtc
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RTC/Rtc.h"
#include "RTC/Rtc_prm.h"

// library includes -----------------------------------------------------------
#include "TimeHandler/TimeHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the alarm control structure
typedef struct _ALARMCTL
{
  BOOL          bRecurring;         ///< recurring alarm
  BOOL          bProcessEnable;     ///< enable alarm processing
  PVRTCALARMCB  pvCallback;         ///< alarm callback
  RTCALMTYPE    eType;              ///< type
  DATETIME      tAlarmTime;
} ALARMCTL, *PALARMCTL;
#define ALARMCTL_SIZE         sizeof( ALARMCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U64       hCounts;
static  ALARMCTL  tAlmCtl;
static  BOOL      bValid;

// local function prototypes --------------------------------------------------
static  void  ProcessAlarm( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Rtc_Initialize
 *
 * @brief initialize
 *
 * This function will initialize the RTC clock
 *
 *****************************************************************************/
void Rtc_Initialize( void )
{
  // reset the count
  hCounts = 0ull;

  // clear the alarm recurring/process flags
  tAlmCtl.bRecurring = FALSE;
  tAlmCtl.bProcessEnable = FALSE;
  tAlmCtl.pvCallback = NULL;

  // clear the valid flag
  bValid = FALSE;
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 *
 *****************************************************************************/
void Rtc_Close( void )
{
}

/******************************************************************************
 * @function Rtc_Increment
 *
 * @brief set the date time
 *
 * This function will set the date time
 *
 * @param[in]   ptDateTime    pointer to the date/time structure
 *
 *****************************************************************************/
BOOL Rtc_IncrementTime( TASKARG xArg )
{
  // increment the counts
  hCounts++;

  // process the alarm if running
  if ( tAlmCtl.bProcessEnable )
  {
    // process the alarm
    ProcessAlarm( );
  }
  
  // always return true
  return( TRUE );
}

/******************************************************************************
 * @function Rtc_SetDateTime
 *
 * @brief set the date time
 *
 * This function will set the date time
 *
 * @param[in]   ptDateTime    pointer to the date/time structure
 *
 *****************************************************************************/
void Rtc_SetDateTime( PDATETIME ptDateTime )
{
  // convert the date time to count
  hCounts = TimeHandler_TimeToHuge( RTC_OS_SELECT, ptDateTime );

  // set the valid flag
  bValid = TRUE;
}

/******************************************************************************
 * @function Rtc_GetDateTime
 *
 * @brief get the date time
 *
 * This function will get the current date time
 *
 * @param[in]   ptDateTime    pointer to the date/time structure
 *
 *****************************************************************************/
void Rtc_GetDateTime( PDATETIME ptDateTime )
{
  // convert the counts to date/time
  TimeHandler_HugeToTime( RTC_OS_SELECT, hCounts, ptDateTime );
}

/******************************************************************************
 * @function   Rtc_SetAlarm
 *
 * @brief set an alarm time
 *
 * This function will set the alarm time
 *
 * @param[in]   ptAlarmTime   pointer to the alarm time
 * @param[in]   eALarmType    type of alarm
 * @param[in]   bRecurring    recurring alarm
 * @param[in]   pvCallback    pointer to the callback function
 *
 *****************************************************************************/
void Rtc_SetAlarm( PDATETIME ptAlarmTime, RTCALMTYPE eAlarmType, BOOL bRecurring, PVRTCALARMCB pvCallback )
{
  // set the recurring flag/callback/alarmtype/enable
  tAlmCtl.bRecurring = bRecurring;
  tAlmCtl.pvCallback = pvCallback;
  tAlmCtl.eType = eAlarmType;
  tAlmCtl.bProcessEnable = TRUE;

  // clear the alarm time
  memset( &tAlmCtl.tAlarmTime, 0, DATETIME_SIZE );

  // now compute an alarm type
  switch( eAlarmType )
  {
    case RTC_ALMTYPE_YYMMDDHHMMSS :
      tAlmCtl.tAlarmTime.wYear = ptAlarmTime->wYear;
      // fall through to set other elements

    case RTC_ALMTYPE_MMDDHHMMSS :
      tAlmCtl.tAlarmTime.nMonth = ptAlarmTime->nMonth;
      // fall through to set other elements

    case RTC_ALMTYPE_DDHHMMSS :
      tAlmCtl.tAlarmTime.nDay = ptAlarmTime->nDay;
      // fall through to set other elements

    case RTC_ALMTYPE_HHMMSS :
      tAlmCtl.tAlarmTime.nHours = ptAlarmTime->nHours;
      // fall through to set other elements

    case RTC_ALMTYPE_MMSS :
      tAlmCtl.tAlarmTime.nMinutes = ptAlarmTime->nMinutes;
      // fall through to set other elements

    case RTC_ALMTYPE_SS :
      tAlmCtl.tAlarmTime.nSeconds = ptAlarmTime->nSeconds;
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function Rtc_GetValid
 *
 * @brief tests RTC validty
 *
 * This function will test for a valid RTC
 *
 * @return      TRUE if valid RTC
 *
 *****************************************************************************/
BOOL Rtc_GetValid( void )
{
  // return the validity flag
  return( bValid );
}

/******************************************************************************
 * @function ProcessAlarm
 *
 * @brief process alarm routine
 *
 * This function will test to see if the alarm condition is met
 *
 *****************************************************************************/
static void ProcessAlarm( void )
{
  DATETIME  tCurTime;
  BOOL      bAlarmDetected = FALSE;

  // convert current time to structure
  TimeHandler_HugeToTime( RTC_OS_SELECT, hCounts, &tCurTime );

  // now process the alarm type
  switch( tAlmCtl.eType )
  {
    case RTC_ALMTYPE_YYMMDDHHMMSS :
      if ( !( bAlarmDetected = ( tAlmCtl.tAlarmTime.wYear == tCurTime.wYear ) ? TRUE : FALSE ))
      {
        // just exit
        break;
      }
      // fall through to set other elements

    case RTC_ALMTYPE_MMDDHHMMSS :
      if ( !( bAlarmDetected = ( tAlmCtl.tAlarmTime.nMonth == tCurTime.nMonth ) ? TRUE : FALSE ))
      {
        // just exit
        break;
      }
      // fall through to set other elements

    case RTC_ALMTYPE_DDHHMMSS :
      if ( !( bAlarmDetected = ( tAlmCtl.tAlarmTime.nDay == tCurTime.nDay ) ? TRUE : FALSE ))
      {
        // just exit
        break;
      }
      // fall through to set other elements

    case RTC_ALMTYPE_HHMMSS :
      if ( !( bAlarmDetected = ( tAlmCtl.tAlarmTime.nHours == tCurTime.nHours ) ? TRUE : FALSE ))
      {
        // just exit
        break;
      }
      // fall through to set other elements

    case RTC_ALMTYPE_MMSS :
      if ( !( bAlarmDetected = ( tAlmCtl.tAlarmTime.nMinutes == tCurTime.nMinutes ) ? TRUE : FALSE ))
      {
        // just exit
        break;
      }
      // fall through to set other elements

    case RTC_ALMTYPE_SS :
      bAlarmDetected = ( tAlmCtl.tAlarmTime.nSeconds == tCurTime.nSeconds ) ? TRUE : FALSE;
      break;

    default :
      break;
  }

  // did we get an alarm
  if ( bAlarmDetected )
  {
    // is there a callback
    if ( tAlmCtl.pvCallback != NULL )
    {
      // call it
      tAlmCtl.pvCallback( );
    }

    // is this a recurring alarm
    if ( !tAlmCtl.bRecurring )
    {
      // clear the process flag
      tAlmCtl.bProcessEnable = FALSE;
    }
  }
}

/**@} EOF Rtc.c */
