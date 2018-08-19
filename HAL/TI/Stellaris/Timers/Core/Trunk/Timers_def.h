/******************************************************************************
 * @file Timer_def.h
 *
 * @brief timers configuration definitions
 *
 * This file provides the definitions for the configuration
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup Timers
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TIMERS_DEF_H
#define _TIMERS_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// stellarisware library includes
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro to define a 32 bit timer
#define TIMER_32BIT( timeperiph, gpioperiph, timebase, gpiobase, gpiopin, funcpin, mode, edge, ints, prescale, count, match, enabled, callback ) \
                   { .uTimePeriph = timeperiph, \
                     .uGpioPeriph = gpioperiph, \
                     .uTimeBase = timebase, \
                     .uGpioBase = gpiobase, \
                     .uGpioPin = gpiopin, \
                     .uFuncPin = funcpin, \
                     .eSel = TIMER_SEL_BOTH, \
                     .eMode = mode, \
                     .eEdge = edge, \
                     .eInts = ints, \
                     .uPrescale = prescale, \
                     .uCount = count, \
                     .uMatch = match, \
                     .bEnabled = enabled, \
                     .pvCallBack = ( PVTIMEINTCALLBACK )callback \
                   }

/// define the helper macro to define a 16 BIT timer
#define TIMER_16BIT( timeperiph, gpioperiph, timebase, gpiobase, gpiopin, funcpin, timer, mode, edge, ints, prescale, count, match, enabled, callback ) \
                   { .uTimePeriph = timeperiph, \
                     .uGpioPeriph = gpioperiph, \
                     .uTimeBase = timebase, \
                     .uGpioBase = gpiobase, \
                     .uGpioPin = gpiopin, \
                     .uFuncPin = funcpin, \
                     .eSel = timer, \
                     .eMode = mode, \
                     .eEdge = edge, \
                     .eInts = ints, \
                     .uPrescale = prescale, \
                     .uCount = count, \
                     .uMatch = match, \
                     .bEnabled = enabled, \
                     .pvCallBack = ( PVTIMEINTCALLBACK )callback \
                   }

// enumerations ---------------------------------------------------------------

/// enumerate the timer select
typedef enum _TIMERSEL
{
  TIMER_SEL_A     = TIMER_A,
  TIMER_SEL_B     = TIMER_B,
  TIMER_SEL_BOTH  = TIMER_BOTH,
  TIMER_SEL_MAX
} TIMERSEL;

/// enumerate the timer mode
typedef enum _TIMERMODE
{
  TIMER_MODE_NONE                 = 0,
  TIMER_MODE_32BIT_OS_UP          = TIMER_CFG_32_BIT_OS,
  TIMER_MODE_32BIT_OS_DN          = TIMER_CFG_32_BIT_OS_UP,
  TIMER_MODE_32BIT_PER_UP         = TIMER_CFG_32_BIT_PER,
  TIMER_MODE_32BIT_PER_DN         = TIMER_CFG_32_BIT_PER_UP,
  TIMER_MODE_32BIT_RTC            = TIMER_CFG_32_RTC,
  TIMER_MODE_16BIT_A_ONESHOT_DN   = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_ONE_SHOT ),
  TIMER_MODE_16BIT_A_ONESHOT_UP   = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_ONE_SHOT_UP ),
  TIMER_MODE_16BIT_A_PERIODIC_DN  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PERIODIC ),
  TIMER_MODE_16BIT_A_PERIODIC_UP  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PERIODIC_UP ),
  TIMER_MODE_16BIT_A_CAP_COUNT_DN = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_COUNT  ),
  TIMER_MODE_16BIT_A_CAP_COUNT_UP = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP ),
  TIMER_MODE_16BIT_A_CAP_TIME_DN  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_TIME ),
  TIMER_MODE_16BIT_A_CAP_TIME_UP  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_TIME_UP ),
  TIMER_MODE_16BIT_A_PWM          = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PWM ),
  TIMER_MODE_16BIT_B_ONESHOT_DN   = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_ONE_SHOT ),
  TIMER_MODE_16BIT_B_ONESHOT_UP   = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_ONE_SHOT_UP ),
  TIMER_MODE_16BIT_B_PERIODIC_DN  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PERIODIC ),
  TIMER_MODE_16BIT_B_PERIODIC_UP  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PERIODIC_UP ),
  TIMER_MODE_16BIT_B_CAP_COUNT_DN = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_CAP_COUNT  ),
  TIMER_MODE_16BIT_B_CAP_COUNT_UP = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_CAP_COUNT_UP ),
  TIMER_MODE_16BIT_B_CAP_TIME_DN  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_CAP_TIME ),
  TIMER_MODE_16BIT_B_CAP_TIME_UP  = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_CAP_TIME_UP ),
  TIMER_MODE_16BIT_B_PWM          = ( TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PWM ),
  TIMER_MODE_MAX
} TIMERMODE;

/// enumerate the timer edge
typedef enum _TIMEREDGE
{
  TIMER_EDGE_NONE                 = 0,
  TIMER_EDGE_RISING               = TIMER_EVENT_POS_EDGE,
  TIMER_EDGE_FALLING              = TIMER_EVENT_NEG_EDGE,
  TIMER_EDGE_BOTH                 = TIMER_EVENT_BOTH_EDGES,
  TIMER_EDGE_MAX
} TIMEREDGE;

/// enumerate the interrupts
typedef enum _TIMERINTS
{
  TIMER_INTS_NONE                 = 0,
  TIMER_INTS_TIMB_MATCH           = TIMER_TIMB_MATCH,
  TIMER_INTS_CAPB_EVENT           = TIMER_CAPB_EVENT,
  TIMER_INTS_CAPB_MATCH           = TIMER_CAPB_MATCH,
  TIMER_INTS_TIMB_TIMEOUT         = TIMER_TIMB_TIMEOUT,
  TIMER_INTS_TIMA_MATCH           = TIMER_TIMA_MATCH,
  TIMER_INTS_RTC_MATCH            = TIMER_RTC_MATCH,
  TIMER_INTS_CAPA_EVENT           = TIMER_CAPA_EVENT,
  TIMER_INTS_CAPA_MATCH           = TIMER_CAPA_MATCH,
  TIMER_INTS_TIMA_TIMEOUT         = TIMER_TIMA_TIMEOUT,
  TIMER_INTS_MAX
} TIMERINTS;

/// define the interrupt callback type
typedef   void ( *PVTIMEINTCALLBACK )( BOOL bPinState );

// structures -----------------------------------------------------------------
/// define the timer definition structure
typedef struct _TIMERDEF
{
  U32               uTimePeriph;    ///< timer peripheral
  U32               uGpioPeriph;    ///< GPIO peripheral
  U32               uTimeBase;      ///< base address of the timer peripheral
  U32               uGpioBase;      ///< GPIO base
  U32               uGpioPin;       ///< GPIO pin
  U32               uFuncPin;       ///< GPIO Function pin for TEMPEST class parts
  TIMERSEL          eSel;           ///< timer selection
  TIMERMODE         eMode;          ///< timer mode
  TIMEREDGE         eEdge;          ///< edge A
  TIMERINTS         eInts;          ///< interrupt masks A
  U32               uPrescale;      ///< prescale value
  U32               uCount;         ///< count value
  U32               uMatch;         ///< match avlue
  BOOL              bEnabled;       ///< enabled
  PVTIMEINTCALLBACK pvCallBack;     ///< callback function
} TIMERDEF, *PTIMERDEF;
#define TIMERDEF_SIZE sizeof( TIMERDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF Timers_def.h */

#endif  // _TIMERS_DEF_H