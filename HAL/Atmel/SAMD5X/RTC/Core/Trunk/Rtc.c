/******************************************************************************
 * @file Rtc.c
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
 * \addtogroup RTC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Rtc/Rtc.h"
#include "Rtc/Rtc_prm.h"

// library includes -----------------------------------------------------------
#include "TimeHandler/TimeHandler.h"
#if ( RTC_ENABLE_TIMESET_LOG == 1 )
#include "LogHandler/LogHandler.h"
#endif

// Macros and Defines ---------------------------------------------------------
// Define Constants used by RTC Functions
#define RTC_YEAR_OFFSET     ( 2000 )		// Starting year for storing hours in clock register
#define SECS_PER_MIN        ( 60l )
#define MINS_PER_HOUR       ( 60l )
#define HOURS_PER_DAY       ( 24l )

/// 	define the Prescaler for RTC Clock - 1/1024
#define PRESCALE_VALUE          ( RTC_MODE2_CTRL_PRESCALER_DIV1024_Val	)

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PVRTCALARMCB  pvAlarmCallback;
static  BOOL          bRecurringAlarm;
static  BOOL          bValid;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RTC_Initialize
 *
 * @brief  RTC Initialization
 *
 * This function will initialize the Real Time Clock (RTC) in calender mode. 
 *
 *****************************************************************************/
void Rtc_Initialize( void )
{
  RTC_MODE2_CTRL_Type tMode2Ctrl;

  // reset the device
  RTC->MODE2.CTRL.bit.SWRST = ON;
  while( RTC->MODE2.STATUS.reg != 0 );
	
  // Write the local control register structure
  tMode2Ctrl.reg = RTC_MODE2_CTRL_MODE( 2 );
  tMode2Ctrl.reg |= RTC_MODE2_CTRL_PRESCALER( PRESCALE_VALUE );
	
  // Write the hardware register
  RTC->MODE2.CTRL = tMode2Ctrl;
	
  // Enable the RTC
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE;	
  while( RTC->MODE2.STATUS.reg != 0 );

  // reset the valid flag
  bValid = FALSE;
}

/******************************************************************************
 * @function RTC_Close
 *
 * @brief  RTC close
 *
 * This function will close the RTC and disable all interrupts
 *
 *****************************************************************************/
void Rtc_Close( void )
{
  // reset the device
  RTC->MODE2.CTRL.bit.SWRST = ON;
  while( RTC->MODE2.STATUS.reg != 0 );
  
  // disable the interrupt
  NVIC_DisableIRQ( RTC_IRQn );
}

/******************************************************************************
 * @function RTC_SetAlarm
 *
 * @brief  RTC Initialization
 *
 * This function will initialize the Real Time Clock (RTC) in calender mode. 
 *
 * @param[in]   ptAlarmTime   alarm time
 * @param[in]   eALarmType    alarm type
 * @param[in]   bRecurring    true if recurring alarm
 * @param[in]   pvCallback    pointer to the callback
 *
 *****************************************************************************/
void Rtc_SetAlarm( PDATETIME ptAlarmTime, RTCALMTYPE eAlarmType, BOOL bRecurring, PVRTCALARMCB pvCallback )
{
  RTC_MODE2_ALARM_Type  tAlarm;
  U8                    nReg;

  // test for off
  if ( eAlarmType == RTC_ALMTYPE_OFF )
  {
    // clear the interrupt
    NVIC_DisableIRQ( RTC_IRQn );

    // clear the interrupt enabled
    RTC->MODE2.INTENCLR.bit.ALARM0 = ON;
  }
  else
  {
    tAlarm.reg = 0;

    // determine the type
    switch( eAlarmType )
    {
      case RTC_ALMTYPE_SS :
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( ptAlarmTime->nSeconds );
        break;

      case RTC_ALMTYPE_MMSS :
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( ptAlarmTime->nSeconds );
        tAlarm.reg |= RTC_MODE2_ALARM_MINUTE( ptAlarmTime->nMinutes );
        break;

      case RTC_ALMTYPE_HHMMSS :
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( ptAlarmTime->nSeconds );
        tAlarm.reg |= RTC_MODE2_ALARM_MINUTE( ptAlarmTime->nMinutes );
        tAlarm.reg |= RTC_MODE2_ALARM_HOUR( ptAlarmTime->nHours );
        break;

      case RTC_ALMTYPE_DDHHMMSS :
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( ptAlarmTime->nSeconds );
        tAlarm.reg |= RTC_MODE2_ALARM_MINUTE( ptAlarmTime->nMinutes );
        tAlarm.reg |= RTC_MODE2_ALARM_HOUR( ptAlarmTime->nHours );
        tAlarm.reg |= RTC_MODE2_ALARM_DAY( ptAlarmTime->nDayOfWeek );
        break;

      case RTC_ALMTYPE_MMDDHHMMSS :
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( ptAlarmTime->nSeconds );
        tAlarm.reg |= RTC_MODE2_ALARM_MINUTE( ptAlarmTime->nMinutes );
        tAlarm.reg |= RTC_MODE2_ALARM_HOUR( ptAlarmTime->nHours );
        tAlarm.reg |= RTC_MODE2_ALARM_DAY( ptAlarmTime->nDayOfWeek );
        tAlarm.reg |= RTC_MODE2_ALARM_MONTH( ptAlarmTime->nMonth );
        break;

      case RTC_ALMTYPE_YYMMDDHHMMSS :
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( ptAlarmTime->nSeconds );
        tAlarm.reg |= RTC_MODE2_ALARM_MINUTE( ptAlarmTime->nMinutes );
        tAlarm.reg |= RTC_MODE2_ALARM_HOUR( ptAlarmTime->nHours );
        tAlarm.reg |= RTC_MODE2_ALARM_DAY( ptAlarmTime->nDayOfWeek );
        tAlarm.reg |= RTC_MODE2_ALARM_MONTH( ptAlarmTime->nMonth );
        tAlarm.reg |= RTC_MODE2_ALARM_YEAR( ptAlarmTime->wYear - TimeHandler_GetEpochYear( TIME_OS_UNIX ));
        break;

      default :
        break;
    }

    // copy the callback/alarm type/recurring
    pvAlarmCallback = pvCallback;
    bRecurringAlarm = bRecurring;

    // set the register
    RTC->MODE2.Mode2Alarm->ALARM.reg = tAlarm.reg;
    RTC->MODE2.Mode2Alarm->MASK.reg = eAlarmType;
    while( RTC->MODE2.STATUS.reg == RTC_STATUS_SYNCBUSY );

    // clear the flag
    nReg = RTC->MODE2.INTFLAG.reg;
    nReg &= ~RTC_MODE2_INTFLAG_ALARM_Msk;
    RTC->MODE2.INTFLAG.reg = nReg;
    RTC->MODE2.INTENSET.bit.ALARM0 = ON;

    // enabled the interrupt
    NVIC_EnableIRQ( RTC_IRQn );
  }
}

/******************************************************************************
 * @function Rtc_Handler
 *
 * @brief RTC interrupt handler
 *
 * This function process the handler for RTC
 *
 *****************************************************************************/
void RTC_Handler( void )
{
  RTC_MODE2_ALARM_Type  tAlarm;
  U8                    nTempValue, nReg;

  // process the callback
  pvAlarmCallback( );

  // clear the flag
  nReg = RTC->MODE2.INTFLAG.reg;
  nReg &= ~RTC_MODE2_INTFLAG_ALARM_Msk;
  RTC->MODE2.INTFLAG.reg = nReg;

  // if this is a recurring,
  if ( bRecurringAlarm )
  {
    // get the current alarm register
    tAlarm.reg = RTC->MODE2.Mode2Alarm->ALARM.reg;

    // determine the type
    switch( RTC->MODE2.Mode2Alarm->MASK.reg )
    {
      case RTC_ALMTYPE_SS :
        // increment the seconds
        nTempValue = ( tAlarm.reg & RTC_MODE2_ALARM_SECOND_Msk ) >> RTC_MODE2_ALARM_SECOND_Pos;
        nTempValue++;
        nTempValue %= SECS_PER_MIN;
        tAlarm.reg &= ~RTC_MODE2_ALARM_SECOND_Msk;
        tAlarm.reg |= RTC_MODE2_ALARM_SECOND( nTempValue );
        break;

      case RTC_ALMTYPE_MMSS :
        // increment the minutes
        nTempValue = ( tAlarm.reg & RTC_MODE2_ALARM_MINUTE_Msk ) >> RTC_MODE2_ALARM_MINUTE_Pos;
        nTempValue++;
        nTempValue %= MINS_PER_HOUR;
        tAlarm.reg &= ~RTC_MODE2_ALARM_MINUTE_Msk;
        tAlarm.reg |= RTC_MODE2_ALARM_MINUTE( nTempValue );
        break;

      case RTC_ALMTYPE_HHMMSS :
        // increment the hours
        nTempValue = ( tAlarm.reg & RTC_MODE2_ALARM_HOUR_Msk ) >> RTC_MODE2_ALARM_HOUR_Pos;
        nTempValue++;
        nTempValue %= HOURS_PER_DAY;
        tAlarm.reg &= ~RTC_MODE2_ALARM_HOUR_Msk;
        tAlarm.reg |= RTC_MODE2_ALARM_HOUR( nTempValue );
        break;

      case RTC_ALMTYPE_DDHHMMSS :
        // increment the days
        nTempValue = ( tAlarm.reg & RTC_MODE2_ALARM_DAY_Msk ) >> RTC_MODE2_ALARM_DAY_Pos;
        nTempValue++;
        nTempValue %= 30;
        tAlarm.reg &= ~RTC_MODE2_ALARM_DAY_Msk;
        tAlarm.reg |= RTC_MODE2_ALARM_DAY( nTempValue );
        break;

      case RTC_ALMTYPE_MMDDHHMMSS :
        // increment the month
        nTempValue = ( tAlarm.reg & RTC_MODE2_ALARM_MONTH_Msk ) >> RTC_MODE2_ALARM_MONTH_Pos;
        nTempValue++;
        nTempValue %= 12;
        tAlarm.reg &= ~RTC_MODE2_ALARM_MONTH_Msk;
        tAlarm.reg |= RTC_MODE2_ALARM_MONTH( nTempValue );
        break;

      case RTC_ALMTYPE_YYMMDDHHMMSS :
        // increment the month
        nTempValue = ( tAlarm.reg & RTC_MODE2_ALARM_YEAR_Msk ) >> RTC_MODE2_ALARM_YEAR_Pos;
        nTempValue++;
        tAlarm.reg &= ~RTC_MODE2_ALARM_YEAR_Msk;
        tAlarm.reg |= RTC_MODE2_ALARM_YEAR( nTempValue );
        break;

      default :
        break;
    }
  }
  else
  {
    // clear the interrupts
    RTC->MODE2.INTENCLR.bit.ALARM0 = ON;
    NVIC_DisableIRQ( RTC_IRQn );
  }
}

/******************************************************************************
 * @function Rtc_SetDateTime
 *
 * @brief RTC set the date and time
 *
 * This function will set the date and time in the RTC Mode 2 calender register
 *
 * @param[in] ptDateTime
 *
 *****************************************************************************/
void Rtc_SetDateTime( PDATETIME ptDateTime )
{
	RTC_MODE2_CLOCK_Type tMode2Clk;
	
	// Wait for synchronization to be complete
	while( RTC->MODE2.STATUS.reg == RTC_STATUS_SYNCBUSY )
	
	// Write the Clock Register local structure from DATE/TIME structure
	tMode2Clk.bit.SECOND = ptDateTime->nSeconds;
	tMode2Clk.bit.MINUTE = ptDateTime->nMinutes;
	tMode2Clk.bit.HOUR = ptDateTime->nHours;
	tMode2Clk.bit.DAY = ptDateTime->nDay;
	tMode2Clk.bit.MONTH = ptDateTime->nMonth;
	tMode2Clk.bit.YEAR = ptDateTime->wYear - RTC_YEAR_OFFSET;
	
	// Write the Hardware Register using local structure
	RTC->MODE2.CLOCK = tMode2Clk;

  #if ( RTC_ENABLE_TIMESET_LOG == 1 )
  LogHandler_AddEntry( LOG_TYPE_RTCSET, 0, 0 );
  #endif

  // set the valid flag
  bValid = TRUE;
}

/******************************************************************************
 * @function Rtc_GetDateTime
 *
 * @brief RTC Get Date and Time
 *
 * This function gets the current date and time stored in the RTC Clock register
 *
 * @param[in]   ptDateTime
 *
 * @return      
 *
 *****************************************************************************/
void Rtc_GetDateTime( PDATETIME ptDateTime )
{
	RTC_MODE2_CLOCK_Type tMode2Clk;
	
	// Request /wait for Synchronization
	RTC->MODE2.READREQ.reg = RTC_READREQ_RREQ;
	while( RTC->MODE2.STATUS.reg == RTC_STATUS_SYNCBUSY )
	
	// Read the clock hardware Register to local structure
	tMode2Clk.reg = RTC->MODE2.CLOCK.reg;
	
	// Read the local structure into DATE/TIME structure
	ptDateTime->nSeconds = tMode2Clk.bit.SECOND;
	ptDateTime->nMinutes = tMode2Clk.bit.MINUTE;
	ptDateTime->nHours = tMode2Clk.bit.HOUR;
	ptDateTime->nDay = tMode2Clk.bit.DAY;
	ptDateTime->nMonth = tMode2Clk.bit.MONTH;
	ptDateTime->wYear = tMode2Clk.bit.YEAR + RTC_YEAR_OFFSET;
}

/******************************************************************************
 * @function Rtc_GetValid
 *
 * @brief RTC get validity flag
 *
 * This function will return the validity flag
 *
 * @return  the current state of the validity flag
 *
 *****************************************************************************/
BOOL Rtc_GetValid( void )
{
  // return he validiy flag
  return( bValid );
}

/**@} EOF Rtc.c */
