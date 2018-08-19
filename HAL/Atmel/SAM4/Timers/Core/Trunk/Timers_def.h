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
/// define the helper macro for defining a capture timer
#define TIMERCAPDEF( select, clksel, exttrig, edgetrig, edgea, edgeb, ovfirq, capairq, capbirq, initon, callback ) \
  { \
    .eSekect    = select, \
    .eMode      = TIMER_MODE_CAPTURE, \
    .eClockSel  = clksel, \
    .eExtTrig   = exttrig, \
    .eTrigEdge  = edgetrig, \
    .tEventEdge = \
    { \
      .aeCaptureEdges[ TIME_CMPCAP_CHAN_A ] = edgea, \
      .aeCaptureEdges[ TIME_CMPCAP_CHAN_B ] = edgeb, \
    }, \
    .tIntFlags = \
    { \
      .bCntOverFlow = ovfirq, \
      .bLoadRegA = capairq, \
      .bLoadRegB = capbirq, \
    }, \
    .pvCallback = callback, \
    .bInitialOn = initon, \
  }

/// define the helper macro for defining a compare timer
#define TIMERWAVEPER( select, clksel, period, cmpacta, cmpactb, cmpairq, cmpbirq, initon, callback ) \
  { \
    .eSekect    = select, \
    .eMode      = TIMER_MODE_WAVEUPRC, \
    .eClockSel  = clksel, \
    .uPeriod    = period, \
    .tEventEdge = \
    { \
      .aeEvents[ TIME_CMPCAP_CHAN_A ] = cmpacta, \
      .aeEvents[ TIME_CMPCAP_CHAN_B ] = cmpactb, \
    }, \
    .tIntFlags = \
    { \
      .bCntOverFlow = ovfirq, \
      .bCompRegA    = cmpairq, \
      .bCompRegB    = cmpbirq, \
    }, \
    .pvCallback = callback, \
    .bInitialOn = initon, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the callback event
typedef enum _TIMERCBEVENT
{
  TIMER_CBEVENT_NONE = 0,
  TIMER_CBEVENT_CAPTUREA,       ///< 1 - capture event
  TIMER_CBEVENT_CAPTUREB,       ///< 2 - capture event
  TIMER_CBEVENT_TMO,            ///< 3 - time out event
} TIMERCBEVENT;

/// enumerate the timer selects
typedef enum _TIMERSEL
{
  TIMER_SEL_0 = 0,              ///< timer select 0
  TIMER_SEL_1,                  ///< timer select 1
  TIEMR_SEL_MAX                 
} TIMERSEL;

/// enumerate the timer channels
typedef enum _TIMERCHAN
{
  TIMER_CHAN_0 = 0,             ///< timer channel 0
  TIMER_CHAN_1,                 ///< timer channel 1
  TIMER_CHAN_2,                 ///< timer channel 2
  TIMER_CHAN_MAX
} TIMERCHAN;

/// enumerate the external trigger select
typedef enum _TIMEREXTTRIG
{
  TIMER_EXTTRIG_TIOA = 0,       ///< TIOA trigger
  TIMER_EXTTRIG_TIOB,           ///< TIOB trigger
} TIMEREXTTRIG;

/// enumerate the compare/capture channels
typedef enum _TIMERCMPCAPCHAN
{
  TIME_CMPCAP_CHAN_A = 0,       ///< compare/capture channel A
  TIME_CMPCAP_CHAN_B,           ///< compare/capture channel B
  TIME_CMPCAP_CHAN_C,           ///< compare/capture channel C
  TIME_CMPCAP_CHAN_MAX
} TIMERCMPCAPCHAN;

/// enumerate the timer modes
typedef enum _TIMERMODE
{
  TIMER_MODE_CAPTURE = 0,       ///< simple capture mode
  TIMER_MODE_WAVEUP,            ///< waveform mode up
  TIMER_MODE_WAVEUPDOWN,        ///< waveform mode up-down
  TIMER_MODE_WAVEUPRC,          ///< waveform mode up with RC compare
  TIMER_MODE_WAVEUPDOWNRC,      ///< waveform mode up-down with RC compare
  TIMER_MODE_MAX
} TIMERMODE;

/// enumerate the clock selection
typedef enum _TIMERCLOCKSEL
{
  TIMER_CLOCKSEL_TIMCLK1 = 0,   ///< MCLK / 2
  TIMER_CLOCKSEL_TIMCLK2,       ///< MCLK / 8
  TIMER_CLOCKSEL_TIMCLK3,       ///< MCLK / 32
  TIMER_CLOCKSEL_TIMCLK4,       ///< MCLK / 128
  TIMER_CLOCKSEL_TIMCLK5,       ///< SCLK
  TIMER_CLOCKSEL_XC0,           ///< external clock 0
  TIMER_CLOCKSEL_XC1,           ///< external clock 1
  TIMER_CLOCKSEL_XC2,           ///< external clock 2
  TIMER_CLOCKSEL_MAX
} TIMERCLOCKSEL;

/// enumerate the compare/trigger effects
typedef enum _TIMERCMPTRIGEVENTS
{
  TIMER_CMPTRIG_EVENTS_NONE = 0,  ///< no event
  TIMER_CMPTRIG_EVENTS_SET,       ///< set
  TIMER_CMPTRIG_EVENTS_CLEAR,     ///< clear
  TIMER_CMPTRIG_EVENTS_TOGGLE,    ///< toggle
  TIMER_CMPTRIG_EVENTS_MAX
} TIMERCMPTRIGEVENTS;

/// enumerate the capture edge
typedef enum _TIMERCAPEDGE
{
  TIMER_CAPEDGE_NONE = 0,         ///< do nothing
  TIMER_CAPEDGE_RISING,           ///< rising edge
  TIMER_CAPEDGE_FALLING,          ///< falling edge
  TIMER_CAPEDGE_BOTH,             ///< both edges
  TIMER_CAPEDGE_MAX
} TIMERCAPEDGE;

// structures -----------------------------------------------------------------
/// define the callback structure
typedef void ( *PVTIMERCALLBACK )( TIMERCBEVENT, TIMERCHAN, U16 );

/// define the timer definition
typedef struct _TIMERDEF
{
  TIMERSEL            eSelect;                      ///< timer select
  TIMERCLOCKSEL       eClockSel;                    ///< clock select
  TIMERMODE           aeMode[ TIMER_CHAN_MAX ];     ///< timer(s) mode
  TIMEREXTTRIG        aeExtTrig[ TIMER_CHAN_MAX ];  ///< external trigger select
  TIMERCMPTRIGEVENTS  eTrigEdge;      ///< edge trigger
  U32                 auCompareRegs[ TIME_CMPCAP_CHAN_MAX ];
  U32                 uPeriod;        ///< period
  union
  {
    TIMERCAPEDGE        aeCaptureEdges[ TIME_CMPCAP_CHAN_MAX ];
    TIMERCMPTRIGEVENTS  aeEvents[ TIME_CMPCAP_CHAN_MAX ];
  } tEventEdge;
  PVTIMERCALLBACK   pvCallback;     ///< timer callback
  struct  
  {
    BOOL  bCntOverflow  : 1;        ///< counter overflow interrupt
    BOOL  bLoadOverrun  : 1;        ///< load overrun interrupt
    BOOL  bCompRegA     : 1;        ///< compare register A
    BOOL  bCompRegB     : 1;        ///< compare register B
    BOOL  bCompRegC     : 1;        ///< compare register C
    BOOL  bLoadRegA     : 1;        ///< register load A
    BOOL  bLoadRegB     : 1;        ///< register load B
    BOOL  bLoadRegC     : 1;        ///< register load C
    BOOL  bExtTrig      : 1;        ///< external trigger
  } tIntFlags;
  BOOL              bInitialOn;     ///< initial on
} TIMERDEF, *PTIMERDEF;
#define TIMERDEF_SIZE               sizeof( TIMERDEF )

/**@} EOF Timers_def.h */

#endif  // _TIMERS_DEF_H
