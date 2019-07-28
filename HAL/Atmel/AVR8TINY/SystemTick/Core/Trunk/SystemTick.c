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
#include "SystemTick/SystemTick_prm.h"
#include "SystemTick/SystemTick.h"

// library includes -----------------------------------------------------------
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMTICK_TIMER_SELECTED == SYSTEMTICK_TIMER_0 )
  #define TIMER_SIZE          ( TIMER_SIZE_8BIT )
  #define TIMER_MODE          ( TIMER_MODE8_CTC )
  #if ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_1000HZ )
    #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV64 )
    #define TIMER_RELOAD      ( F_CPU / 64 / 1000 )
    #define SYSTEMRICK_MS     ( 1 )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_500HZ )
    #define TIMER_PRESCALE    ( TIMER_PRESCALE80_DIV64 )
    #define TIMER_RELOAD      ( F_CPU / 64 / 500 )
    #define SYSTEMRICK_MS     ( 2 )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_200HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_100HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_40HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_20HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_10HZ )
  #else
    #error  Illegal system tick rate selection!
  #endif
#elif ( SYSTEMTICK_TIMER_SELECTED == SYSTEMTICK_TIMER_1 )
  #if ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_1000HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_500HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_200HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_100HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_40HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_20HZ )
  #elif ( SYSTEMTICK_RATE_SELECTED == SYSTEMTICK_RATE_10HZ )
  #else
    #error  Illegal system tick rate selection!
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
    pvLclCallback( SYSTEMRICK_MS );
  }
}
 
/**@} EOF SystemTick.c */
