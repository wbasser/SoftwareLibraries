/******************************************************************************
 * @file Timers.h
 *
 * @brief Timer declaration
 *
 * This file provides the declarations for the Timer module
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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
#ifndef _TIMER_H
#define _TIMER_H

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the timer errors
typedef enum _TIMERSERR
{
  TIMERS_ERR_NONE = 0,      ///< no error
  TIMERS_ERR_ILLTIMER,      ///< illegal timer
  TIMERS_ERR_ILLCHAN,       ///< illegal channel
  TIMERS_ERR_ILLTIMESIZE,   ///< illegal timer size
  TIMERS_ERR_NOCAPTURE,     ///< no capture register
  TIMERS_ERR_ILLACTION,     ///< illegal iOCTL action
} TIMERSERR;

/// enumerate the timer type
typedef enum _TIMERSIZE
{
  TIMER_SIZE_8BIT = 0,
  TIMER_SIZE_16BIT,
} TIMERSIZE;

/// define the mode for 8 bit timers
typedef enum _TIMERMODE8
{
  TIMER_MODE8_NORM = 0,       ///< normal mode
  TIMER_MODE8_PWMPCM,         ///< PWM, phase correct, timeout on MAX
  TIMER_MODE8_CTC,            ///< Clear timer on compare match
  TIMER_MODE8_PWMFASTM,       ///< PWM, fast, timerout on MAX
  TIMER_MODE8_PWMPCO = 5,     ///< PWM, phase correct, timeout on OCRA
  TIMER_MODE8_PWMFASTO = 7,   ///< PWM, fast, timeout on OCRA
  TIMER_MODE8_MAX
} TIMERMODE8;

/// define the mode for 16 bit timers
typedef enum _TIMERMODE16
{
  TIMER_MODE16_CAPT = 0,      ///< capture mode
  TIMER_MODE16_PWMPC8M,       ///< PWM, phase correct, 8 bit timeout on FF
  TIMER_MODE16_PWMPC9M,       ///< PWM, phase correct, 9 bit timeout on 1FF
  TIMER_MODE16_PWMPC10M,      ///< PWM, phase correct, 10 bit timeout on 3FF
  TIMER_MODE16_CTCO,          ///< Clear timer on compare match OCRA
  TIMER_MODE16_PWMFAST8M,     ///< PWM, fast, 8 bit timeouton FF
  TIMER_MODE16_PWMFAST9M,     ///< PWM, fast, 9 bit timeouton 1FF
  TIMER_MODE16_PWMFAST10M,    ///< PWM, fast, 10 bit timeouton 3FF
  TIMER_MODE16_PWMPFCI,       ///< PWM, phase and frequency correct, timeout on ICR
  TIMER_MODE16_PWMPFCO,       ///< PWM, phase and frequency correct, timeout on OCRA
  TIMER_MODE16_PWMPCI,        ///< PWM, phase correct, timeout on ICR
  TIMER_MODE16_PWMPCO,        ///< PWM, phase correct, timeout on OCRA
  TIMER_MODE16_CTCI,          ///< Clear timer on compare match ICR
  TIMER_MODE16_PWMFASTI = 14, ///< PWM, fast, timerout on ICR
  TIMER_MODE16_PWMFASTO,      ///< PWM, fast, timerout on OCRA
  TIMER_MODE16_MAX
} TIMERMODE16;

/// enuemrate the prescale 8 bit timer 0 selections
typedef enum _TIMERPRESCALE80
{
  TIMER_PRESCALE80_DIV1 = 1,    ///< timer prescale divide by 1
  TIMER_PRESCALE80_DIV8,        ///< timer prescale divide by 8
  TIMER_PRESCALE80_DIV64,       ///< timer prescale divide by 64
  TIMER_PRESCALE80_DIV256,      ///< timer prescale divide by 256
  TIMER_PRESCALE80_DIV1024,     ///< timer prescale divide by 1024
  TIMER_PRESCALE80_MAX,
  TIMER_PRESCALE80_MASK = 0x07  ///< timer prescale mask
} TIMERPRESCALE80;


/// enuemrate the prescale 16 bit timer 
typedef enum _TIMERPRESCALE16
{
  TIMER_PRESCALE16_DIV1 = 1,    ///< timer prescale divide by 1
  TIMER_PRESCALE16_DIV8,        ///< timer prescale divide by 8
  TIMER_PRESCALE16_DIV64,       ///< timer prescale divide by 64
  TIMER_PRESCALE16_DIV256,      ///< timer prescale divide by 256
  TIMER_PRESCALE16_DIV1024,     ///< timer prescale divide by 1024
  TIMER_PRESCALE16_MAX,
  TIMER_PRESCALE16_MASK = 0x07  ///< timer prescale mask
} TIMERPRESCALE16;

/// enumerate the callback event
typedef enum _TIMERCBEVENT
{
  TIMER_CBEVENT_NONE = 0,       ///< 0 - no event
  TIMER_CBEVENT_TMO,            ///< 1 - time out event
  TIMER_CBEVENT_CMPUP,          ///< 2 - compare match - counting up
  TIMER_CBEVENT_CMPDN,          ///< 3 - compare match - counting down
  TIMER_CBEVENT_PERUP,          ///< 4 - period match - counting up
  TIMER_CBEVENT_CAPRE,          ///< 5 - capture event - rising edge
  TIMER_CBEVENT_CAPFE,          ///< 6 - capture event - falling edge
} TIMERCBEVENT;

// structures -----------------------------------------------------------------
/// define the TIMERENUM type
typedef U8    TIMERENUM;

/// define the callback
typedef void ( *PVTIMERCALLBACK )( TIMERCBEVENT, U16 );

// global function prototypes --------------------------------------------------
extern  TIMERENUM   Timers_Create( TIMERSIZE eTimerSize, U8 nTimerMode, U8 nPrescale, U8 nReload, PVTIMERCALLBACK pvCallback );

/**@} EOF Timer.h */

#endif  // _TIMER_H