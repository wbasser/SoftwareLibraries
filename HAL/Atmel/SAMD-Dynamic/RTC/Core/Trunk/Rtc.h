/******************************************************************************
 * @file Rtc.h
 *
 * @brief Real Time Clock definitions
 *
 * This file provides the interface definitions to the Real Time Clock Peripheral
 *
 * @copyright Copyright (c) 2014 CyberIntegration
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
 * \addtogroup RTC
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTC_H
#define _RTC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
/// define the prescaler for the clock generator
#define RTC_GCLK_PRESCALER     ( 32 )

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
typedef void ( *PVRTCALARMCB )( void );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void  Rtc_Initialize( void );
extern  void  Rtc_Close( void );
extern	void  Rtc_SetDateTime( PDATETIME ptDateTime );
extern	void  Rtc_GetDateTime( PDATETIME ptDateTime );
extern  void  Rtc_SetAlarm( PDATETIME ptAlarmTime, RTCALMTYPE eAlarmType, BOOL bRecurring, PVRTCALARMCB pvCallback );
extern  BOOL  Rtc_GetValid( void );

/**@} EOF Rtc.h */

#endif  // _RTC_H