/******************************************************************************
 * @file TTimers_def.h  
 *
 * @brief timer definition declarations
 *
 * This file provides the declarations for use in defining a timer 
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
 * $Rev: $
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

/// define the helper macro for defining a compare timer
#define TIMER_TCWAVMFDEF( chan, invert, oneshot, initon, standby, outenable, ovfenable, matchirqenable, ovfirqenable, direction, prescale, ratehz, priority, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTc = TIMER_TCMODE_WAVMF, \
    .abInvertOutputs[ TIMER_CMPCAP_CHAN0 ] = invert, \
    .abInvertOutputs[ TIMER_CMPCAP_CHAN1 ] = OFF,\
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ] = outenable, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ] = OFF, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN0 ] = matchirqenable, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN1 ] = OFF, \
    .tFlags = \
    { \
      .bOneShot = oneshot, \
      .bInitialOn = initon, \
      .bRunStandby = standby, \
      .bOvfIrqEnable = ovfirqenable, \
      .bHighPriority = priority, \
    }, \
    .eDirection = direction, \
    .ePrescale = prescale, \
    .auChanValues[ TIMER_CMPCAP_CHAN0 ] = ratehz, \
    .auChanValues[ TIMER_CMPCAP_CHAN1 ] = 0, \
    .pvCallback = callback, \
  }

/// define the helper macro for PWM
#define TIMER_TCMPWMFDEF( chan, invert0, invert1, initon, standby, ovfenable, matchirqenable0, matchirqenable1, ovfirqenable, direction, prescale, priority, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTc = TIMER_TCMODE_MPWM, \
    .abInvertOutputs[ TIMER_CMPCAP_CHAN0 ] = invert0, \
    .abInvertOutputs[ TIMER_CMPCAP_CHAN1 ] = invert1,\
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN0 ] = matchirqenable0, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN1 ] = matchirqenable1, \
    .tFlags = \
    { \
      .bInitialOn = initon, \
      .bRunStandby = standby, \
      .bOvfIrqEnable = ovfirqenable, \
      .bHighPriority = priority, \
    }, \
    .eDirection = direction, \
    .ePrescale = prescale, \
    .auChanValues[ TIMER_CMPCAP_CHAN0 ] = 0, \
    .auChanValues[ TIMER_CMPCAP_CHAN1 ] = 0, \
    .pvCallback = callback, \
  }

/// define the helper macro for defining a capture time
#define TIMER_TCCAPDEF( chan, prescale, initon, standby, ovfirqenable, resettozero, invertedge, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTc = TIMER_TCMODE_CAPTURE, \
    .ePrescale = prescale, \
    .tFlags = \
    { \
      .bInitialOn = initon, \
      .bOvfIrqEnable = ovfirqenable, \
      .bResetToZero = resettozero, \
      .bInvertEdge = invertedge, \
    }, \
    .pvCallback = callback, \
  }

#define TIMER_TCCAPPWPDEF( chan, prescale, initon, standby, cc0enb, cc1enb, ovfirqenable, resettozero, invertedge, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTc = TIMER_TCMODE_CAPPWP, \
    .ePrescale = prescale, \
    .eEventAction = TIMER_EVENTACT_PWP, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN0 ] = cc0enb, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN1 ] = cc1enb, \
    .tFlags.bInitialOn = initon, \
    .tFlags.bOvfIrqEnable = ovfirqenable, \
    .tFlags.bResetToZero = resettozero, \
    .tFlags.bInvertEdge = invertedge, \
    .pvCallback = callback, \
  }

/// define the helper macro for defining a capture timer - PPW
#define TIMER_TCCAPPPWDEF( chan, prescale, initon, standby, cc0enb, cc1enb, ovfirqenable, resettozero, invertedge, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTc = TIMER_TCMODE_CAPPWP, \
    .ePrescale = prescale, \
    .eEventAction = TIMER_EVENTACT_PPW, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN0 ] = cc0enb, \
    .abChanIrqEnables[ TIMER_CMPCAP_CHAN1 ] = cc1enb, \
    .tFlags.bInitialOn = initon, \
    .tFlags.bOvfIrqEnable = ovfirqenable, \
    .tFlags.bResetToZero = resettozero, \
    .tFlags.bInvertEdge = invertedge, \
    .pvCallback = callback, \
  }

/// define the helper macro for defining a single slope PWM for TCCs
#define TIMER_TCCPWMSINGUS( chan, prescale, initon, standby, oneshot, period, cc0pct, cc1pct, cc2pct, cc3pct, cc0enb, cc1enb, cc2enb, cc3enb, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTcc = TIMER_TCCMODE_PWMSING, \
    .ePrescale = prescale, \
    .tFlags.bInitialOn = initon, \
    .tFlags.bRunStandby = standby, \
    .tFlags.bOneShot = oneshot, \
    .tFlags.bTimesInUsec = TRUE, \
    .uPwmPeriod = period, \
    .auChanValues[ TIMER_CMPCAP_CHAN0 ] = cc0pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN1 ] = cc1pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN2 ] = cc2pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN3 ] = cc3pct, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ] = cc0enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ] = cc1enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN2 ] = cc2enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN3 ] = cc3enb, \
    .pvCallback = callback, \
  }

/// define the helper macro for defining a single slope PWM for TCCs
#define TIMER_TCCPWMSINGHZ( chan, prescale, initon, standby, oneshot, period, cc0pct, cc1pct, cc2pct, cc3pct, cc0enb, cc1enb, cc2enb, cc3enb, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTcc = TIMER_TCCMODE_PWMSING, \
    .ePrescale = prescale, \
    .tFlags.bInitialOn = initon, \
    .tFlags.bRunStandby = standby, \
    .tFlags.bOneShot = oneshot, \
    .tFlags.bTimesInUsec = FALSE, \
    .uPwmPeriod = period, \
    .auChanValues[ TIMER_CMPCAP_CHAN0 ] = cc0pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN1 ] = cc1pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN2 ] = cc2pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN3 ] = cc3pct, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ] = cc0enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ] = cc1enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN2 ] = cc2enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN3 ] = cc3enb, \
    .pvCallback = callback, \
  }

/// define the helper macro for defining a dual slope bottom timer for TCCs
#define TIMER_TCCPWMDUALBOTUS( chan, prescale, initon, standby, oneshot, period, cc0pct, cc1pct, cc2pct, cc3pct, cc0enb, cc1enb, cc2enb, cc3enb, callback ) \
  { \
    .eChannel = chan, \
    .tModes.eTcc = TIMER_TCCMODE_PWMDUAL_BOT, \
    .ePrescale = prescale, \
    .tFlags.bInitialOn = initon, \
    .tFlags.bRunStandby = standby, \
    .tFlags.bOneShot = oneshot, \
    .tFlags.bTimesInUsec = TRUE, \
    .uPwmPeriod = period, \
    .auChanValues[ TIMER_CMPCAP_CHAN0 ] = cc0pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN1 ] = cc1pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN2 ] = cc2pct, \
    .auChanValues[ TIMER_CMPCAP_CHAN3 ] = cc3pct, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN0 ] = cc0enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN1 ] = cc1enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN2 ] = cc2enb, \
    .abChanOpsEnables[ TIMER_CMPCAP_CHAN3 ] = cc3enb, \
    .pvCallback = callback, \
  }

/// define the helper macro for configuraiton capture on TCC's
#define TIMER_TCCCAPDEF( chan, prescale, initon, standby, ovfirqenable, resettozero, invertedge, callback ) \
{ \
  .eChannel = chan, \
  .tModes.eTcc = TIMER_TCCMODE_CAPTURE, \
  .ePrescale = prescale, \
  .tFlags.bInitialOn = initon, \
  .tFlags.bOvfIrqEnable = ovfirqenable, \
  .tFlags.bResetToZero = resettozero, \
  .tFlags.bInvertEdge = invertedge, \
  .pvCallback = callback , \
}

// enumerations ---------------------------------------------------------------
/// enumerate the callback event
typedef enum _TIMERCBEVENT
{
  TIMER_CBEVENT_NONE = 0,
  TIMER_CBEVENT_TMO,            ///< 1 - time out event
  TIMER_CBEVENT_CMPUP,          ///< 2 - compare match - counting up
  TIMER_CBEVENT_CMPDN,          ///< 3 - compare match - counting down
  TIMER_CBEVENT_PERUP,          ///< 4 - period match - counting up
  TIMER_CBEVENT_CAPRE,          ///< 5 - capture event - rising edge
  TIMER_CBEVENT_CAPFE,          ///< 6 - capture event - falling edge
  TIMER_CBEVENT_CAPTURE,        ///< 7 - capture event
} TIMERCBEVENT;

/// enumerate the timer channels
typedef enum _TIMERCHAN
{
  TIMER_CHAN_0 = 0,             ///< timer channel 0
  TIMER_CHAN_1,                 ///< timer channel 1
  TIMER_CHAN_2,                 ///< timer channel 2
  TIMER_CHAN_3,                 ///< timer channel 3
  TIMER_CHAN_4,                 ///< timer channel 4
  TIMER_CHAN_5,                 ///< timer channel 5
  #ifdef TC6
  TIMER_CHAN_6,                 ///< timer channel 6
  #endif  // TC6
  #ifdef TC7
  TIMER_CHAN_7,                 ///< timer channel 7
  #endif  // TC7
  TIMER_CHAN_MAX
} TIMERCHAN;


/// enumerate the timer compare/capture channels
typedef enum _TIMERCMPCAPCHAN
{
  TIMER_CMPCAP_CHAN_0 = 0,
  TIMER_CMPCAP_CHAN_1,
  TIMER_CMPCAP_CHAN_2,
  TIMER_CMPCAP_CHAN_3,
} TIMERCMPCAPCHAN;

/// enumerate the timer CTT modes
typedef enum _TIMERTCCMODE
{
  TIMER_TCCMODE_WAVNF = 0,       ///< wave output, normal frequency
  TIMER_TCCMODE_WAVMF,           ///< wave output, match frequency
  TIMER_TCCMODE_PWMSING,         ///< PWM, single slope
  TIMER_TCCMODE_PWMDUAL_TOP = 4, ///< PWM, dual slope, int/event at top
  TIMER_TCCMODE_PWMDUAL_BOT,     ///< PWM, dual slope, int/event at 
  TIMER_TCCMODE_PWMDUAL_TOPBOT,  ///< PWM, dual slope, int/event at top/bottom
  TIMER_TCCMODE_PWMDUAL_CRITBOT, ///< PWM, dual slope, critical int/event at bottom
  TIMER_TCCMODE_CAPTURE,         ///< simple capture mode
  TIMER_TCCMODE_MAX
} TIMERTCCMODE;

/// enumerate the timer CT modes
typedef enum _TIMERTCMODE
{
  TIMER_TCMODE_WAVNF = 0,       ///< wave output, normal frequency
  TIMER_TCMODE_WAVMF,           ///< wave output, match frequency
  TIMER_TCMODE_NPWM,            ///< NPWM - MAX = top
  TIMER_TCMODE_MPWM,            ///< MPWM - MAX = CC0
  TIMER_TCMODE_CAPTURE,         ///< capture
  TIMER_TCMODE_CAPPPW,          ///< capture PPW
  TIMER_TCMODE_CAPPWP,          ///< capture PWP
  TIMER_TCMODE_MAX
} TIMERTCMODE;

/// enumerate the presclae values
typedef enum _TIMERPRESCALE
{
  TIMER_PRESCALE_1 = 0,         ///< no division
  TIMER_PRESCALE_2,             ///< divide by 2
  TIMER_PRESCALE_4,             ///< divide by 4
  TIMER_PRESCALE_8,             ///< divide by 8
  TIMER_PRESCALE_16,            ///< divide by 16
  TIMER_PRESCALE_64,            ///< divide by 64
  TIMER_PRESCALE_256,           ///< divide by 256
  TIMER_PRESCALE_1024,          ///< divide by 1024
  TIMER_PRESCALE_MAX
} TIMERPRESCALE;

/// enumerate the direction
typedef enum _TIMERDIRECTION
{
  TIMER_DIRECTION_UP = 0,        ///< count up
  TIMER_DIRECTION_DN,            ///< count done
  TIMER_DIRECTION_MAX
} TIMERDIRECTION;

/// enumerate the event action
typedef enum _TIMEREVENTACT
{
  TIMER_EVENTACT_OFF = 0,       ///< no action
  TIMER_EVENTACT_RETRIG,        ///< retrigger
  TIMER_EVENTACT_COUNT,         ///< count on event
  TIMER_EVENTACT_START,         ///< start TC on event
  TIMER_EVENTACT_PPW = 0x05,    ///< period capture in CC0, pulse width in CC1
  TIMER_EVENTACT_PWP,           ///< period capture in CC1, pulse width in CC0
  TIMER_EVENTACT_MAX
} TIMEREVENTACT;

/// enumerate the compare/capture channels
typedef enum _TIMERCMPCAP
{
  TIMER_CMPCAP_CHAN0 = 0,       ///< compare/capture 0
  TIMER_CMPCAP_CHAN1,           ///< compare/capture 1
  TIMER_CMPCAP_CHAN2,           ///< compare/capture 2
  TIMER_CMPCAP_CHAN3,           ///< compare/capture 3
  TIMER_CMPCAP_MAX
} TIMERCMPCAP;

// structures -----------------------------------------------------------------
/// define the callback structure
typedef void ( *PVTIMERCALLBACK )( TIMERCBEVENT, TIMERCMPCAP, U16 );

/// define the timer definition
typedef struct _TIMERDEF
{
  TIMERCHAN        eChannel;        ///< timer channel
  union
  {
    TIMERTCCMODE    eTcc;           ///< timer TCC mode
    TIMERTCMODE     eTc;            ///< timer TC mode
  } tModes;
  struct  
  {
    BOOL  bOneShot          : 1;    ///< oneshot mode
    BOOL  bInitialOn        : 1;    ///< initially on
    BOOL  bRunStandby       : 1;    ///< run in standby
    BOOL  bOvfOutEnable     : 1;    ///< overflow output enable
    BOOL  bOvfIrqEnable     : 1;    ///< interrupt on overflow
    BOOL  bResetToZero      : 1;    ///< reset to zero
    BOOL  bTimesInUsec      : 1;    ///< times in USEC
    BOOL  bInvertEdge       : 1;    ///< invert the first edge in capture
    BOOL  bHighPriority     : 1;    ///< high priority
  } tFlags;
  BOOL              abChanIrqEnables[ TIMER_CMPCAP_MAX ]; // enable the match/capture IRQ's
  BOOL              abChanOpsEnables[ TIMER_CMPCAP_MAX ]; // enable the match/capture channels
  BOOL              abInvertOutputs[ TIMER_CMPCAP_MAX ];  // timer output enables
  TIMERDIRECTION    eDirection;     ///< direction
  TIMERPRESCALE     ePrescale;      ///< prescale
  TIMEREVENTACT     eEventAction;   ///< event action
  U32               uPwmPeriod;     ///< period for PWM generation
  U32               auChanValues[ TIMER_CMPCAP_MAX ] ;  ///< rate in hertz channel 0
  PVTIMERCALLBACK   pvCallback;     ///< timer callback
} TIMERDEF, *PTIMERDEF;
#define TIMERDEF_SIZE   sizeof( TIMERDEF )

/**@} EOF Timers_def.h */

#endif  // _TIMERS_DEF_H
