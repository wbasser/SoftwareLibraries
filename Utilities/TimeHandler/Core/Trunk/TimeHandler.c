/******************************************************************************
 * @file TimeHandler.c
 *
 * @brief Real Time Clock implementation
 *
 * This file the interface implementation to the Real Time Clock Peripheral
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
 * \addtogroup TimeHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TimeHandler/TimeHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// Define Constants used by RTC Functions
#define SECS_PER_MIN	    ( 60 )
#define MINS_PER_HOUR     ( 60 )
#define HOURS_PER_DAY     ( 24 )
#define SECS_PER_HOUR     ( SECS_PER_MIN * MINS_PER_HOUR )
#define SECS_PER_DAY	    ( SECS_PER_HOUR * HOURS_PER_DAY )
#define DAYS_PER_WEEK     ( 7 )
#define MONS_PER_YEAR     ( 12 )
#define YEAR_BASE         ( 0 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
typedef struct _OSPARAMS
{
  U16   wEpochYear;       ///< epoch year
  U8    nEpochDay;        ///< epoch weekday
  U16   wTimeResolution;  ///< resolution
} OSPARAMS, *POSPARAMS;
#define OSPARAMS_SIZE       sizeof( OSPARAMS )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static	U8	TestLeap( U16 wYear );

// constant parameter initializations -----------------------------------------
//// month lengths
static const CODE U8 anMonthLengths[ 2 ][ MONS_PER_YEAR ] = 
{
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

/// OS type paramters
static  const CODE OSPARAMS atOsParams[ TIME_OS_MAX ] =
{
  { 1970, 5, 1000 },
  { 1980, 3,  100 },
  { 2001, 1, 1000 },
  { 1970, 5,    1 },
  { 1980, 3,    1 },
  { 1900, 3,    1 }
};

/******************************************************************************
 * @function TimeHandler_TimeToHuge
 *
 * @brief RTC Time to Huge
 *
 * This function converts a the current date from the form of a DATETIME 
 * structure to a 64bit long time value.
 *
 * @param[in]   eOsType     OS type
 * @param[in]   ptDateTime  pointer to the data time structure
 *
 * @return hCounts     
 *
 *****************************************************************************/
U64 TimeHandler_TimeToHuge( TIMEOSTYPE eOsType, PDATETIME ptDateTime )
{
	U64       hCounts = 0l;
	U8        nIndex;
	U16       wIndex;
	PU8       pnMonths;
  POSPARAMS ptOsParams;
	
  // get a pointer to the parameters
  ptOsParams = ( POSPARAMS )&atOsParams[ eOsType ];
  
	// add in years
  for ( wIndex = PGM_RDWORD( ptOsParams->wEpochYear ); wIndex < ptDateTime->wYear; wIndex++ )
	{
		nIndex = TestLeap( wIndex );
		hCounts += ( nIndex ) ? 366 : 365;
	}
	
	// set the pointer to the correct months
	nIndex = TestLeap( ptDateTime->wYear );
	pnMonths = ( PU8 )&anMonthLengths[ nIndex ];
	
	// add in months
	for ( nIndex = 0; nIndex < ( ptDateTime->nMonth - 1 ); nIndex++ )
	{
		hCounts += PGM_RDBYTE( pnMonths[ nIndex ] );
	}
	
	// add in days
	hCounts += ( ptDateTime->nDay - 1 );
	
	// convert days to seconds
	hCounts *= SECS_PER_DAY;
	
	// add in hours/minutes/seconds
	hCounts += ( U64 )ptDateTime->nHours * SECS_PER_HOUR;
	hCounts += ( U64 )ptDateTime->nMinutes * SECS_PER_MIN;
	hCounts += ptDateTime->nSeconds;
	
	// multiply by resolution
	hCounts *= PGM_RDWORD( ptOsParams->wTimeResolution );
	
	// return the count
	return( hCounts );
}

/******************************************************************************
 * @function TimeHandler_HugeToTime
 *
 * @brief RTC huge to time
 *
 * This function converts huge time into time
 *
 * @param[in]   eOsType     OS type
 * @param[in]   hHugeTime   huge time
 * @param[in]   ptDateTime  pointer to the data time structure
 *
 *****************************************************************************/
void TimeHandler_HugeToTime( TIMEOSTYPE eOsType, U64 hHugeTime, PDATETIME ptDateTime )
{
	U64       hDays, hTemp;
	U16       wDaysInYear;
	U8        nYearIndex;
	PU8       pnMonths;
  POSPARAMS ptOsParams;
	
  // get a pointer to the parameters
  ptOsParams = ( POSPARAMS )&atOsParams[ eOsType ];
  
	// Compute number of days
	hDays = hHugeTime / SECS_PER_DAY;
	hTemp = hHugeTime - ( hDays * SECS_PER_DAY );
	
	// Compute hour, min, and sec
	ptDateTime->nHours = hTemp / SECS_PER_HOUR;
	hTemp -= ( U64 )(( U64 )ptDateTime->nHours * ( U64 )SECS_PER_HOUR );
	ptDateTime->nMinutes = hTemp / SECS_PER_MIN;
	ptDateTime->nSeconds = hTemp - ( ptDateTime->nMinutes * SECS_PER_MIN );
	
	// Compute day of week
	ptDateTime->nDayOfWeek = ( PGM_RDBYTE( ptOsParams->nEpochDay ) + hDays ) % DAYS_PER_WEEK;
	
	// Compute year & day of year
	ptDateTime->wYear = PGM_RDWORD( ptOsParams->wEpochYear );
	
	// Determine the year based on numbers of days
	FOREVER
	{
		nYearIndex = TestLeap( ptDateTime->wYear );
		wDaysInYear = ( nYearIndex ) ? 366 : 365;
		if ( hDays < wDaysInYear )
		{
			break;
		}
		ptDateTime->wYear++;
		hDays -= wDaysInYear;
	}
	
	// Add one to remaining number of days to make Julian in range 1-365
	ptDateTime->wJulian = ( U16 )hDays + 1;
	
	// get a pointer to the correct month array based on leap year test
	pnMonths = ( PU8 )&anMonthLengths[ nYearIndex ];
	
	// Get the day
	for ( ptDateTime->nMonth = 0; hDays >= pnMonths[ ptDateTime->nMonth ]; ++ptDateTime->nMonth )
	{
		hDays -= pnMonths[ ptDateTime->nMonth ];
	}
	ptDateTime->nDay = hDays + 1;
	
	// Add 1 to month to get into 1-12 range (from 0 - 11)
	ptDateTime->nMonth++;
}

/******************************************************************************
 * @function TimeHandler_GetFatTime
 *
 * @brief RTC get fat time
 *
 * This function will get the current time and convert it to fat time
 *
 * @param[in]   PFATTIME
 *
 *****************************************************************************/
void TimeHandler_GetFatTime( TIMEOSTYPE eOsType, U64 hHugeTime, PFATTIME ptFatTime )
{
	// Get the time structure
	//DATETIME tTime;
	//TimeHandler_GetDateTime( eOsType, &tTime );
	
	// Fill a fate time structure
	//ptFatTime->tFields.uSeconds	  = tTime.nSeconds >> 2;
	//ptFatTime->tFields.uMinutes   = tTime.nMinutes;
	//ptFatTime->tFields.uHours 	  = tTime.nHours;
	//ptFatTime->tFields.uDay 	  = tTime.nDay;
	//ptFatTime->tFields.uMonth 	  = tTime.nMonth;
	//ptFatTime->tFields.uYear 	  = tTime.wYear - PGM_RDWORD( atOsParams[ eOsType ].wEpochYear );
}

/******************************************************************************
 * @function TimeHandler_ConvertTime
 *
 * @brief convert time from one format to another
 *
 * This function will convert the time to the desired format
 *
 * @param[in]   eSrcType     source time type
 * @param[in]   eDstType     destination time type
 * @param[in]   hHugeTime     huge time
 *
 * @return  the converted time
 *
 *****************************************************************************/
U64 TimeHandler_ConvertTime( TIMEOSTYPE eSrcType, TIMEOSTYPE eDstType, U64 hHugeTime )
{
  DATETIME  tTemp;
  
  // convert to time in source format
  TimeHandler_HugeToTime( eSrcType, hHugeTime, &tTemp );
  
  // convert to huge in destination format
  hHugeTime = TimeHandler_TimeToHuge( eDstType, &tTemp );

  // return the converted time
  return( hHugeTime );
}

/******************************************************************************
 * @function TimeHandler_GetEpochYear
 *
 * @brief get the epoch year
 *
 * This function will return the epoch hear
 *
 * @param[in]   eSrcType     source time typoe
 *
 * @return  the epoch year
 *
 *****************************************************************************/
U16 TimeHandler_GetEpochYear( TIMEOSTYPE eOsType )
{
  U16 wEpochYear = 0;

  if ( eOsType < TIME_OS_MAX )
  {
    wEpochYear = PGM_RDWORD( atOsParams[ eOsType ].wEpochYear );
  }

  // return the year
  return( wEpochYear );
}

/******************************************************************************
 * @function TimeHandler_PopulateDayOfWeek
 *
 * @brief RTC Time to Huge
 *
 * This function converts a the current date from the form of a DATETIME 
 * structure to a 64bit long time value.
 *
 * @param[in]   eOsType     OS type
 * @param[in]   ptDateTime  pointer to the data time structure
 *
 * @return hCounts     
 *
 *****************************************************************************/
void TimeHandler_PopulateDayOfWeek( TIMEOSTYPE eOsType, PDATETIME ptDateTime )
{
  U64 hTime;
  
  // convert to huge/convert it back
  hTime = TimeHandler_TimeToHuge( eOsType, ptDateTime );
  TimeHandler_HugeToTime( eOsType, hTime, ptDateTime );
}

/******************************************************************************
 * @function TimeHandler_AddTime
 *
 * @brief RTC Time to Huge
 *
 * This function converts a the current date from the form of a DATETIME 
 * structure to a 64bit long time value.
 *
 * @param[in]   ptDateTime  pointer to the data time structure
 * @param[in]   ptTimeToAdd pointer to the data time structure
 *
 * @return hCounts     
 *
 *****************************************************************************/
void TimeHandler_AddTime( PDATETIME ptCurTime, PDATETIME ptTimeToAdd )
{
  U64 hCurTime;
  U64 hNewTime;
  
  // convert to huge
  hCurTime = TimeHandler_TimeToHuge( TIME_OS_MICRODOS, ptCurTime );
  hNewTime = TimeHandler_TimeToHuge( TIME_OS_MICRODOS, ptTimeToAdd );
  hCurTime += hNewTime;
  
  // convert it back
  TimeHandler_HugeToTime( TIME_OS_MICRODOS, hCurTime, ptCurTime );
}

/******************************************************************************
 * @function TestLeap
 *
 * @brief Test Leap Year
 *
 * This function tests a given year to see if it is a leap year
 *
 * @param[in] wYear  
 *
 * @return nLeap     
 *
 *****************************************************************************/
static U8 TestLeap( U16 wYear )
{
	U8	nLeap;

	nLeap = ((( wYear & 0x03 ) == 0 ) && 
	         ((( wYear % 100 ) != 0 ) || (( wYear % 400 ) == 0 )));
	return( nLeap );
}

/**@} EOF TimeHandler.c */
