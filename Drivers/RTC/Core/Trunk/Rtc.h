/******************************************************************************
 * @file Rtc.h
 *
 * @brief RTC ( software driven ) declarations
 *
 * This file provides the declarations for the software driven RTC
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
 
// ensure only one instantiation
#ifndef _RTC_H
#define _RTC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the execution rate
#define RTC_EXEC_RATE           ( TASK_TIME_SECS( 1 ))
#define RTC_NUM_EVENTS          ( 2 )

// enumerations ---------------------------------------------------------------
/// enumerate the alarm types
typedef enum _RTCALMTYPE
{
  RTC_ALMTYPE_OFF = 0,        ///< alaram off
  RTC_ALMTYPE_SS,             ///< seconds only
  RTC_ALMTYPE_MMSS,           ///< minutes/seconds only
  RTC_ALMTYPE_HHMMSS,         ///< hours, minutes, seconds
  RTC_ALMTYPE_DDHHMMSS,       ///< ddas, hour, minutes, seconds
  RTC_ALMTYPE_MMDDHHMMSS,     ///< months, days, hours minutes, seconds
  RTC_ALMTYPE_YYMMDDHHMMSS,   ///< years, months, days, hours, minutes, seconds
  RTC_ALMTYPE_MAX
} RTCALMTYPE;

// structures -----------------------------------------------------------------
/// define the callback
typedef void ( *PVRTCALARMCB )( void );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void  Rtc_Initialize( void );
extern  BOOL  Rtc_IncrementTime( TASKARG xArg );
extern  void  Rtc_Close( void );
extern	void  Rtc_SetDateTime( PDATETIME ptDateTime );
extern	void  Rtc_GetDateTime( PDATETIME ptDateTime );
extern  void  Rtc_SetAlarm( PDATETIME ptAlarmTime, RTCALMTYPE eAlarmType, BOOL bRecurring, PVRTCALARMCB pvCallback );
extern  BOOL  Rtc_GetValid( void );

/**@} EOF Rtc.h */

#endif  // _RTC_H