/******************************************************************************
 * @file Timers_def.h  
 *
 * @brief timer definition declarations
 *
 * This file provides the declarations for use in defining a timer 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
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
#include "Micro/Types.h"

// Macros and Defines ---------------------------------------------------------

/// define the helper macro for defining a compare timer
#define TIMER_CMPDEF( chan, align, pre, irqsel, periodhz, comparepct, invert, initon, callback, irqpri ) \
  { \
    .eChan = chan, \
    .eMode = TIMER_MODE_COMPARE, \
    .eAlign = align, \
    .ePreScale = pre, \
    .tIrq.eCmpIrq = irqsel, \
    .uPeriodHz = periodhz, \
    .wComparePct = comparepct, \
    .bInvert = invert, \
    .bInitialOn = initon, \
    .pvCallback = callback, \
    .nIrqPri = irqpri, \
  }

/// define the helper macro for defining a capture timer
#define TIMER_CAPDEF( chan, pre, event, edge, initon, callback, irqpri ) \
  { \
    .eChan = chan, \
    .eMode = TIMER_MODE_CAPTURE, \
    .ePreScale = pre, \
    .eEventSel = event, \
    .eEdgeSel = edge, \
    .bInitialOn = initon, \
    .pvCallback = callback, \
    .nIrqPri = irqpri, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the the slices
typedef enum _TIMERCHAN
{
  TIMER_CHAN_CCU0 = 0,
  TIMER_CHAN_CCU1,
  TIMER_CHAN_CCU2,
  TIMER_CHAN_CCU3,
  TIMER_CHAN_MAX
} TIMERCHAN;

/// enumerate the modes of operation
typedef enum _TIMERMODE
{
  TIMER_MODE_COUNTER = 0,     ///< counter mode
  TIMER_MODE_COMPARE,         /// output compare
  TIMER_MODE_CAPTURE,         /// capture mode
  TIMER_MODE_MAX  
} TIMERMODE;

/// enumerate the compare alignment
typedef enum _TIMERCMPALIGN
{
  TIMER_CMPALIGN_EDGE = 0,   ///< left edge align
  TIMER_CMPALIGN_CENTER,     ///< center align
} TIMERCMPALIGN;

/// enumerate the prescale values
typedef enum _TIMERPRESCALE
{
  TIMER_PRESCALE_1 = 0,
  TIMER_PRESCALE_2,
  TIMER_PRESCALE_4,
  TIMER_PRESCALE_8,
  TIMER_PRESCALE_16,
  TIMER_PRESCALE_32,
  TIMER_PRESCALE_64,
  TIMER_PRESCALE_128,
  TIMER_PRESCALE_256,
  TIMER_PRESCALE_512,
  TIMER_PRESCALE_1024,
  TIMER_PRESCALE_2048,
  TIMER_PRESCALE_4096,
  TIMER_PRESCALE_8192,
  TIMER_PRESCALE_16384,
  TIMER_PRESCALE_32768,
  TIMER_PRESCALE_MAX
} TIMERPRESCALE;

/// enumerate the event input selection
typedef enum _TIMEREVNSEL
{
  TIMER_EVNSEL_INA = 0,
  TIMER_EVNSEL_INB,
  TIMER_EVNSEL_INC,
  TIMER_EVNSEL_IND,
  TIMER_EVNSEL_INE,
  TIMER_EVNSEL_INF,
  TIMER_EVNSEL_ING,
  TIMER_EVNSEL_INH,
  TIMER_EVNSEL_INI,
  TIMER_EVNSEL_INJ,
  TIMER_EVNSEL_INK,
  TIMER_EVNSEL_INL,
  TIMER_EVNSEL_INM,
  TIMER_EVNSEL_INN,
  TIMER_EVNSEL_INO,
  TIMER_EVNSEL_INP,
  TIMER_EVNSEL_MAX
} TIMEREVNSEL;

/// enumerate the event edge selection
typedef enum _TIMEREDGESEL
{
  TIMER_EDGESEL_NONE = 0,
  TIMER_EDGESEL_RISING,
  TIMER_EDGESEL_FALLING,
  TIMER_EDGESEL_BOTH,
  TIMER_EDGESEL_MAX
} TIMEREDGESEL;

/// enumerate the level selection
typedef enum _TIMERLVLSEL
{
  TIMER_LVLSEL_LO = 0,
  TIMER_LVLSEL_HI,
  TIMER_LVLSEL_MAX
} TIMERLVLSEL;

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
} TIMERCBEVENT;

/// enumerate the compare interrupts
typedef enum _TIMERCMPIRQ
{
  TIMER_CMPIRQ_NONE = 0,        ///< no interrupt
  TIMER_CMPIRQ_CMPUP,           ///< interrupt on compare match counting up
  TIMER_CMPIRQ_CMPDN,           ///< interrupt on comapre match counting down
  TIMER_CMPIRQ_CMPBOTH,         ///< interrupt on compare match on both up/down
  TIMER_CMPIRQ_PERUP,           ///< interrupt on period match counting up
} TIMERCMPIRQ;

/// emumerate the capture interrupts
typedef enum _TIMERCAPIRQ
{
  TIMER_CAPIRQ_NONE = 0,        ///< no interrupt
  TIMER_CAPIRQ_EVN0,            /// interrupt on event 0
  TIMER_CAPIRQ_EVN1,            /// interrupt on event 1
  TIMER_CAPIRQ_EVN2,            /// interrupt on event 2
  TIMER_CAPIRQ_EVN01,           /// interrupt on event 0 and 1
  TIMER_CAPIRQ_EVN02,           /// interrupt on event 0 and 2
  TIMER_CAPIRQ_EVN12,           /// interrupt on event 1 and 2
  TIMER_CAPIRQ_EVN012,          /// interrupt on event 0, 1 and 2
} TIMERCAPIRQ;

// structures -----------------------------------------------------------------
/// define the callback structure
typedef void ( *PVTIMERCALLBACK )( TIMERCBEVENT, U16 );

/// define the timer definition
typedef struct _TIMERDEF
{
  TIMERCHAN       eChan;        ///< timer channel
  TIMERMODE       eMode;        ///< timer mode
  TIMERCMPALIGN   eAlign;       ///< compare alignment
  TIMERPRESCALE   ePreScale;    ///< timer prescale
  union
  {
    TIMERCMPIRQ   eCmpIrq;      ///< compare IRQ selection
    TIMERCAPIRQ   eCapIrq;      ///< capture IRQ selection
  } tIrq;
  TIMEREVNSEL     eEventSel;    ///< event select
  TIMEREDGESEL    eEdgeSel;     ///< edge select
  TIMERLVLSEL     eLevelSel;    ///< level select
  U32             uPeriodHz;    ///< period value - HZ
  U16             wComparePct;  ///< compare value - duty cycle in tenths
  BOOL            bInvert;      ///< invert if high pulsing low
  BOOL            bInitialOn;   ///< if TRUE, turn on timer initially
  U8              nIrqPri;      ///< interrupt priority
  PVTIMERCALLBACK pvCallback;   ///< timer callback
} TIMERDEF, *PTIMERDEF;
#define TIMERDEF_SIZE   sizeof( TIMERDEF )

/**@} EOF Timers_def.h */

#endif  // _TIMERS_DEF_H
