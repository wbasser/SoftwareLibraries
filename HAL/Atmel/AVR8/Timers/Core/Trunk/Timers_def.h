/******************************************************************************
 * @file Timers_def.h  
 *
 * @brief timer definition declarations
 *
 * This file provides the declarations for use in defining a timer 
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
#ifndef _TIMERS_DEF_H
#define _TIMERS_DEF_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum compare channels for 8/16 bit timers
#define MAX_COMPCHANS_8BIT    2
#define MAX_COMPCHANS_10BIT   4
#define MAX_COMPCHANS_16BIT   3

/// define the helper macro for a 8 bit Clear on compare timer 
#define TIMER_DEF80CTC( ratehz, prescale, outenb, initon, callback ) \
  { \
    .eSize            = TIMER_SIZE_8BIT, \
    .eSelect          = TIMER_SEL_0, \
    .uRateHz          = ratehz, \
    .nPrescale        = prescale, \
    .bOutputOn        = outenb,  \
    .bInitialOn       = initon, \
    .pvCallback       = callback, \
    .tControl.tTim8   = \
    { \
      .eMode          = TIMER_MODE8_CTC, \
    } \
  }
 
/// define the helper macro for a 8 bit Clear on compare timer
#define TIMER_DEF82CTC( ratehz, prescale, outenb, initon, callback ) \
  { \
    .eSize            = TIMER_SIZE_8BIT, \
    .eSelect          = TIMER_SEL_2, \
    .uRateHz          = ratehz, \
    .nPrescale        = prescale, \
    .bOutputOn        = outenb,  \
    .bInitialOn       = initon, \
    .pvCallback       = callback, \
    .tControl.tTim8   = \
    { \
      .eMode          = TIMER_MODE8_CTC, \
    } \
  }

/// define the helper macro for a 16 bit Clear on compare timer 
#define TIMER_DEF16CTC( timsel, ratehz, prescale, outenb, initon, callback ) \
  { \
    .eSize            = TIMER_SIZE_16BIT, \
    .eSelect          = timsel, \
    .uRateHz          = ratehz, \
    .nPrescale        = prescale, \
    .bOutputOn        = outenb,  \
    .bInitialOn       = initon, \
    .pvCallback       = callback, \
    .tControl.tTim16  = \
    { \
      .eMode          = TIMER_MODE16_CTCO, \
    } \
  }
  
/// define the helper macro to define a 8 bit PWM
#define TIMER_DEF8PWMFAST( timsel, prescale, comparepct, actiona, actionb, initon, callback ) \
  { \
  }
  
#define TIMER_DEF16PWMFAST8( timsel, prescale, comparepct, actiona, actionb, actionc, initon, callback ) \
  { \
  }
  
#define TIMER_DEF16PWMFAST9( timsel, prescale, comparepct, actiona, actionb, actionc, initon, callback ) \
  { \
  }
  
#define TIMER_DEF16PWMFAST10( timsel, prescale, comparepct, actiona, actionb, actionc, initon, callback ) \
  { \
  }
  
#define TIMER_DEF16CAPTURE( timsel, prescale, edge, initedge, filter, initon, tovenb, callback ) \
  { \
    .eSize            = TIMER_SIZE_16BIT, \
    .eSelect          = timsel, \
    .nPrescale        = prescale, \
    .bInitialOn       = initon, \
    .pvCallback       = callback, \
    .bTovEnb          = tovenb, \
    .bIcpEnb          = TRUE, \
    .tControl.tTim16  = \
    { \
      .eMode          = TIMER_MODE16_CAPT, \
      .bIcFilterEnb   = filter, \
      .eIcpEdge       = edge, \
      .eInitEdge      = initedge, \
    } \
  }
  
#define TIMER_DEF16NORM( timsel ) \
  { \
    .eSize            = TIMER_SIZE_16BIT, \
    .eSelect          = timsel, \
    .nPrescale        = prescale, \
    .bInitialOn       = OFF, \
    .bTovEnb          = FALSE, \
    .bIcpEnb          = FALSE, \
    .tControl.tTim16  = \
    { \
      .eMode          = TIMER_MODE16_CAPT, \
    } \
  }

#define TIMER_DEF16PWMFASTI( timsel, prescale, period, tovenb, actiona, actionb, actionc, initon, callback ) \
  { \
    .eSize            = TIMER_SIZE_16BIT, \
    .eSelect          = timsel, \
    .nPrescale        = prescale, \
    .uRateHz          = period, \
    .bInitialOn       = initon, \
    .pvCallback       = callback, \
    .bTovEnb          = tovenb, \
    .tControl.tTim16  = \
    { \
      .eMode          = TIMER_MODE16_PWMFASTI, \
      .aeOCRActions[ TIMER_CHAN_A ] = actiona, \
      .aeOCRActions[ TIMER_CHAN_B ] = actionb, \
      .aeOCRActions[ TIMER_CHAN_C ] = actionc, \
    } \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the timer type
typedef enum _TIMERSIZE
{
  TIMER_SIZE_8BIT = 0,
  TIMER_SIZE_10BIT,
  TIMER_SIZE_16BIT,
} TIMERSIZE;

/// enumerate the timers
typedef enum _TIMERSEL
{
  TIMER_SEL_0,
  TIMER_SEL_1,
  TIMER_SEL_2,
  TIMER_SEL_3,
  TIMER_SEL_MAX
} TIMERSEL;

/// enumerate the channels
typedef enum _TIMERCHAN
{
  TIMER_CHAN_A = 0,
  TIMER_CHAN_B,
  TIMER_CHAN_C,
  TIMER_CHAN_D,
  TIMER_CHAN_MAX,
} TIMERCHAN;

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

/// define the mode for 10 bit timers
typedef enum _TIMERMODE10
{
  TIMER_MODE10_NORM = 0,      ///< normal mode
  TIMER_MODE10_PWMFAST,       ///< PWM fast mode
  TIMER_MODE10_PWMPFC,        ///< PWM, phase and frequency correct
  TIMER_MODE10_PWM6SING,      ///< PWM 6, single slope
  TIMER_MODE10_PWM6DOUB,      ///< PWM 6, double slope
  TIMER_MODE10_MAX
} TIMERMODE10;

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

/// enuemrate the prescale 8 bit timer 2 selections
typedef enum _TIMERPRESCALE82
{
  TIMER_PRESCALE82_DIV1 = 1,    ///< timer prescale divide by 1
  TIMER_PRESCALE82_DIV8,        ///< timer prescale divide by 8
  TIMER_PRESCALE82_DIV32,       ///< timer prescale divide by 32
  TIMER_PRESCALE82_DIV64,       ///< timer prescale divide by 64
  TIMER_PRESCALE82_DIV128,      ///< timer prescale divide by 128
  TIMER_PRESCALE82_DIV256,      ///< timer prescale divide by 256
  TIMER_PRESCALE82_DIV1024,     ///< timer prescale divide by 1024
  TIMER_PRESCALE82_MAX,
  TIMER_PRESCALE82_MASK = 0x07  ///< timer prescale mask
} TIMERPRESCALE82;

/// enumerate the prescale for 10 bit timers
typedef enum _TIMERPRESCALE10
{
  TIMER_PRESCALE10_DIV1 = 1,  ///< timer prescale divide by 1
  TIMER_PRESCALE10_DIV2,      ///< timer prescale divide by 2
  TIMER_PRESCALE10_DIV4,      ///< timer prescale divide by 4
  TIMER_PRESCALE10_DIV8,      ///< timer prescale divide by 8
  TIMER_PRESCALE10_DIV16,     ///< timer prescale divide by 16
  TIMER_PRESCALE10_DIV32,     ///< timer prescale divide by 32
  TIMER_PRESCALE10_DIV64,     ///< timer prescale divide by 64
  TIMER_PRESCALE10_DIV128,    ///< timer prescale divide by 128
  TIMER_PRESCALE10_DIV256,    ///< timer prescale divide by 256
  TIMER_PRESCALE10_DIV512,    ///< timer prescale divide by 512
  TIMER_PRESCALE10_DIV1024,   ///< timer prescale divide by 1024
  TIMER_PRESCALE10_DIV2048,   ///< timer prescale divide by 2048
  TIMER_PRESCALE10_DIV4096,   ///< timer prescale divide by 4096
  TIMER_PRESCALE10_DIV8192,   ///< timer prescale divide by 8192
  TIMER_PRESCALE10_DIV16384,  ///< timer prescale divide by 16384
  TIMER_PRESCALE10_MAX,
  TIMER_PRESCALE10_MASK = 0x0F
} TIMERPRESCALE10;

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

/// define the Output Compare actions
typedef enum _TIMEROCACTION
{
  TIMER_OCACTION_NONE = 0,    ///< timer out compare noting
  TIMER_OCACTION_TOGGLE,      ///< timer out camopre toggle pin
  TIMER_OCACTION_CLEAR,       ///< timer out compare clear pin
  TIMER_OCACTION_SET,         ///< timer out compare set pin
  TIMER_OCACTION_MAX
} TIMEROCACTION;

/// define the input capture edge select
typedef enum _TIMERICPEDGE
{
  TIMER_ICPEDGE_FALLING = 0,   ///< falling edge
  TIMER_ICPEDGE_RISING,        ///< rising edge
  TIMER_ICPEDGE_BOTH,          ///< both edges
  TIMER_ICPEDGE_MAX
} TIMERICPEDGE;

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

/// enumerate the ioctl actions
typedef enum _TIMERIOCTL
{
  TIMER_IOCTL_CHANGEDEF = 0,
  TIMER_IOCTL_STOPSTART,
  TIMER_IOCTL_GETCOUNT,
  TIMER_IOCTL_SETCOUNT,
  TIMER_IOCTL_GETPERIOD,
  TIMER_IOCTL_SETPERIOD,
  TIMER_IOCTL_GETCOMPAREVAL,
  TIMER_IOCTL_SETCOMPAREVAL,
  TIMER_IOCTL_SETCOMPAREPCT,
  TIMER_IOCTL_GETCAPTURE,
  TIMER_IOCTL_SETCAPTURE,
  TIMER_IOCTL_GETPRESCALE,
  TIMER_IOCTL_MAX
} TIMERIOCTL;

// structures -----------------------------------------------------------------
/// define the callback structure
typedef void ( *PVTIMERCALLBACK )( TIMERCBEVENT, U16 );

/// define the 8 bit timer definition
typedef struct _TIM8BITDEF
{
  TIMERMODE8      eMode;
  TIMEROCACTION   aeOCRActions[ MAX_COMPCHANS_8BIT ];
} TIM8BITDEF, *PTIM8BITDEF;
#define TIM8BITDEF_SIZE   sizeof( TIM8BITDEF )

/// define the 16 bit timer definition
typedef struct _TIM16BITDEF
{
  TIMERMODE16     eMode;
  BOOL            bIcFilterEnb;
  TIMEROCACTION   aeOCRActions[ MAX_COMPCHANS_16BIT ];
  TIMERICPEDGE    eIcpEdge;
  TIMERICPEDGE    eInitEdge;
} TIM16BITDEF, *PTIM16BITDEF;
#define TIM16BITDEF_SIZE   sizeof( TIM16BITDEF )

/// define the timer definition 
typedef struct _TIMERDEF
{
  TIMERSIZE       eSize;
  TIMERSEL        eSelect;
  U8              nPrescale;
  U32             uRateHz;      ///< rate in HZ
  BOOL            bInvert;      ///< invert if high pulsing low
  BOOL            bOutputOn;    ///< output on on CTC
  BOOL            bInitialOn;   ///< if TRUE, turn on timer initially
  PVTIMERCALLBACK pvCallback;   ///< timer callback
  BOOL            bTovEnb;      ///< timer overflow
  BOOL            bIcpEnb;      ///< interrupt capture
  BOOL            bCmpEnb;      ///< compare 
  union
  {
    TIM8BITDEF    tTim8;
    TIM16BITDEF   tTim16;
  } tControl;
} TIMERDEF, *PTIMERDEF;
#define TIMERDEF_SIZE   sizeof( TIMERDEF )

/// define the PWM set value structure
typedef struct _TIMERPWMSET
{
  TIMERCHAN       eChan;        ///< timer channel
  U16             wValue;       ///< value, absolute or percent in thousands
} TIMERPWMSET, *PTIMERPWMSET;
#define TIMERPWMSET_SIZE    sizeof( TIMERPWMSET )

// constants intializations ------------------------------------------
/// initialize the prescale table for timers 0 and 1
static  const CODE U16 awPreScales80[ TIMER_PRESCALE80_MAX ] =
{
  0, 1, 8, 64, 256, 1024
};

/// initialize the prescale table for timers 0 and 1
static  const CODE U16 awPreScales16[ TIMER_PRESCALE16_MAX ] =
{
  0, 1, 8, 64, 256, 1024
};

/// initialize the prescale table for timers 2
static  const CODE U16 awPreScales82[ TIMER_PRESCALE82_MAX ] =
{
  0, 1, 8, 32, 64, 128, 256, 1024
};

/**@} EOF Timers_def.h */
#endif  // _TIMERS_DEF_H