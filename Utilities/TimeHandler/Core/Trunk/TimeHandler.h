/******************************************************************************
 * @file TimeHandler.h
 *
 * @brief TIme conversion declarations
 *
 * This file provides the declarations for the time conversion 
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
 * \addtogroup TimeHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TIMEHANDLER_H
#define _TIMEHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumeration ----------------------------------------------------------------
typedef enum _TIMEOSTYPE
{
  TIME_OS_ANDROID = 0,
  TIME_OS_MICRODOS,
  TIME_OS_IOS,
  TIME_OS_UNIX,
  TIME_OS_IBMPCBIOS,
  TIME_OS_NTP,
  TIME_OS_MAX
} TIMEOSTYPE;

// structures -----------------------------------------------------------------
/// define the date/time	structures
typedef	struct _DATETIME
{
  U8	nMonth;       ///< month ( 1-12 )
  U8	nDay;         ///< day ( 1-31 )
  U16	wYear;        ///< year ( xxxx )
  U16	wJulian;      ///< julian day( 1-365 )
  U8	nDayOfWeek;   ///< day of week ( 1-7)
  U8	nHours;       ///< hours ( 0-23 )
  U8	nMinutes;     ///< minutes ( 0-59 )
  U8	nSeconds;     ///< seconds ( 0-59 )
  C8  cZone;        ///< zone -12-12 )
} DATETIME, *PDATETIME;
#define DATETIME_SIZE	  sizeof( DATETIME )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	U64	  TimeHandler_TimeToHuge( TIMEOSTYPE eOsType, PDATETIME ptDateTime );
extern	void  TimeHandler_HugeToTime( TIMEOSTYPE eOsType, U64 hHugeTime, PDATETIME ptDateTime );
extern	void  TimeHandler_GetFatTime( TIMEOSTYPE eOsType, U64 hHugeTime, PFATTIME ptFatTime );
extern  U64   TimeHandler_ConvertTime( TIMEOSTYPE eSrcType, TIMEOSTYPE eDstType, U64 hHugeTime );
extern  U16   TimeHandler_GetEpochYear( TIMEOSTYPE eOsType );
extern  void  TimeHandler_PopulateDayOfWeek( TIMEOSTYPE eOsType, PDATETIME ptDateTime );
extern  void  TimeHandler_AddTime( PDATETIME ptCurTime, PDATETIME ptTimeToAdd );
extern  void  TimeHandler_AdjustTimeForZone( PDATETIME ptDateTime );
extern  void  TimeHandler_InitializeDateTime( TIMEOSTYPE eOsType, PDATETIME ptDateTime );

/**@} EOF TimeHandler.h */

#endif  // _TIMEHANDLER_H