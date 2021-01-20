/******************************************************************************
 * @file SystemTick.c
 *
 * @brief system tick 
 *
 * This file contains the functions to initialize the system tick controller
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick.h"

// library includes -----------------------------------------------------------
#include "SystemConfig/SystemConfig_prm.h"
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMTICK_TIMER_SELECTED == SYSTEMTICK_TIMER_0 )
  #define TIMER_SIZE          ( TIMER_SIZE_8BIT )
  #define TIMER_MODE          ( TIMER_MODE8_CTC )
  #if ( SYSTEM_CONFIG_CPUFREQ_SELECTED == SYSTEM_CONFIG_CPUFREQ_8MHZ )
    #if ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_1000HZ )
      #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV64 )
      #define TIMER_RELOAD      ( SYSTEM_CONFIG_CPUFREQ_SELECTED / 64 / 1000 )
      #define SYSTEMTICK_MS     ( 1 )
    #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_500HZ )
      #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV64 )
      #define TIMER_RELOAD      ( SYSTEM_CONFIG_CPUFREQ_SELECTED / 64 / 500 )
      #define SYSTEMTICK_MS     ( 2 )
    #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_200HZ )
      #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV256 )
      #define TIMER_RELOAD      ( SYSTEM_CONFIG_CPUFREQ_SELECTED / 256 / 200  )
      #define SYSTEMTICK_MS     ( 5 )
    #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_100HZ )
      #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV256 )
      #define TIMER_RELOAD      ( SYSTEM_CONFIG_CPUFREQ_SELECTED / 256 / 100 )
      #define SYSTEMTICK_MS     ( 10 )
    #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_40HZ )
      #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV1024 )
      #define TIMER_RELOAD      ( SYSTEM_CONFIG_CPUFREQ_SELECTED / 1024 / 40 )
      #define SYSTEMTICK_MS     ( 25 )
    #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_20HZ )
      #error Tick rate can not be acheived with CPU frequency!
    #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_10HZ )
      #error Tick rate can not be acheived with CPU frequency!
    #else
      #error  Illegal system tick rate selection!
    #endif
  #elif ( SYSTEMTICK_TIMER_SELECTED == SYSTEMTICK_TIMER_1 )
    #if ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_1000HZ )
    #error Not implemented yet!
      #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_500HZ )
    #error Not implemented yet!
      #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_200HZ )
    #error Not implemented yet!
      #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_100HZ )
    #error Not implemented yet!
      #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_40HZ )
    #error Not implemented yet!
      #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_20HZ )
    #error Not implemented yet!
      #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_10HZ )
    #else
      #error  Illegal system tick rate selection!
    #endif
  #elif (( SYSTEM_CONFIG_CPUFREQ_SELECTED == SYSTEM_CONFIG_CPUFREQ_4MHZ )
    #error Not implemented yet!
  #elif (( SYSTEM_CONFIG_CPUFREQ_SELECTED == SYSTEM_CONFIG_CPUFREQ_2MHZ )
    #error Not implemented yet!
  #elif (( SYSTEM_CONFIG_CPUFREQ_SELECTED == SYSTEM_CONFIG_CPUFREQ_1MHZ )
    #error Not implemented yet!
  #elif (( SYSTEM_CONFIG_CPUFREQ_SELECTED == SYSTEM_CONFIG_CPUFREQ_USER )
    #error Not implemented yet!
  #else
    #error Not implemented yet!
  #endif
#else
  #error  Illegal system tick timer selection!
#endif // SYSTEMTICK_TIMER_SELECTED

// check for validity
#if ( SYSTEMTICK_TIMER_SELECTED == SYSTEMTICK_TIMER_0 )
  #if ( TIME_RELOAD > 255 )
    #error  Illegal system tick rate selection!
  #endif // valid value
#elif ( SYSTEMTICK_TIMER_SELECTED == SYSTEMTICK_TIMER_1 )
  #if ( TIME_RELOAD > 65535 )
    #error  Illegal system tick rate selection!
  #endif // valid value
#endif // SYSTEMTICK_TIMER_SELECTED

// local parameter declarations -----------------------------------------------
static  PVSYSTICKCALLBACK pvLclCallback;

// local function prototypes --------------------------------------------------
static  void  TickHandler( TIMERCBEVENT eTimeEvent, U16 wData );

/******************************************************************************
 * @function SystemTick_Initialize
 *
 * @brief this initialized the system tick
 *
 * This function initializes the system tick 
 *
 * @param[in] pvCallback    tick callback
 * 
 *****************************************************************************/
void SystemTick_Initialize( PVSYSTICKCALLBACK pvCallback )
{
  // store the call back
  pvLclCallback = pvCallback;

  // initialize the timer
  Timers_Create( TIMER_SIZE, TIMER_MODE, TIMER_PRESCALE, TIMER_RELOAD, TickHandler );
}

/******************************************************************************
 * @function TickHandler
 *
 * @brief this is the tick handler
 *
 * This function will perform any given procsiing and then call the callback funciton
 *
 *****************************************************************************/
static void TickHandler( TIMERCBEVENT eTimeEvent, U16 wData )
{
  // check for null
  if ( pvLclCallback != NULL )
  {
    // if not execute it
    pvLclCallback( SYSTEMTICK_MS );
  }
}
 
/**@} EOF SystemTick.c */
