/******************************************************************************
 * @file Event_def.h
 *
 * @brief event generator definition declarations
 *
 * This file provides the defintions for used in defining an event channel
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
 * \addtogroup Event_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EVENT_DEF_H
#define _EVENT_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining an event
#define EVENTDEF( chan, eventgen, user, edge ) \
  { \
    .eChannel = chan, \
    .eGenerator = eventgen, \
    .eUserMux = user, \
    .eEdge = edge, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the event channel
typedef enum _EVENTCHAN
{
  EVENT_CHAN_0 = 0,           ///< event channel 0
  EVENT_CHAN_1,               ///< event channel 1
  EVENT_CHAN_2,               ///< event channel 2
  EVENT_CHAN_3,               ///< event channel 3
  EVENT_CHAN_4,               ///< event channel 4
  EVENT_CHAN_5,               ///< event channel 5
  #ifndef __SAM_D10_SUBFAMILY
  EVENT_CHAN_6,               ///< event channel 6
  EVENT_CHAN_7,               ///< event channel 7
  EVENT_CHAN_8,               ///< event channel 8
  EVENT_CHAN_9,               ///< event channel 9
  EVENT_CHAN_10,              ///< event channel 10
  EVENT_CHAN_11,              ///< event channel 11
  #endif
  EVENT_CHAN_MAX
} EVENTCHAN;

/// enumerate the event generator
typedef enum _EVENTGEN
{
  EVENT_GEN_NONE = 0,         ///< no event generator
  EVENT_GEN_RTC_COMP0,        ///< RTC compare 0
  EVENT_GEN_RTC_COMP1,        ///< RTC compare 1
  EVENT_GEN_RTC_OVF,          ///< RTC overflow
  EVENT_GEN_RTC_PER0,         ///< RTC period 0
  EVENT_GEN_RTC_PER1,         ///< RTC period 1
  EVENT_GEN_RTC_PER2,         ///< RTC period 2
  EVENT_GEN_RTC_PER3,         ///< RTC period 3
  EVENT_GEN_RTC_PER4,         ///< RTC period 4
  EVENT_GEN_RTC_PER5,         ///< RTC period 5
  EVENT_GEN_RTC_PER6,         ///< RTC period 6
  EVENT_GEN_RTC_PER7,         ///< RTC period 7
  EVENT_GEN_EXT_INT0,         ///< external interupt 0
  EVENT_GEN_EXT_INT1,         ///< external interupt 1
  EVENT_GEN_EXT_INT2,         ///< external interupt 2
  EVENT_GEN_EXT_INT3,         ///< external interupt 3
  EVENT_GEN_EXT_INT4,         ///< external interupt 4
  EVENT_GEN_EXT_INT5,         ///< external interupt 5
  EVENT_GEN_EXT_INT6,         ///< external interupt 6
  EVENT_GEN_EXT_INT7,         ///< external interupt 7
  #ifndef __SAM_D10_SUBFAMILY
  EVENT_GEN_EXT_INT8,         ///< external interupt 8
  EVENT_GEN_EXT_INT9,         ///< external interupt 9
  EVENT_GEN_EXT_INT10,        ///< external interupt 10
  EVENT_GEN_EXT_INT11,        ///< external interupt 11
  EVENT_GEN_EXT_INT12,        ///< external interupt 12
  EVENT_GEN_EXT_INT13,        ///< external interupt 13
  EVENT_GEN_EXT_INT14,        ///< external interupt 14
  EVENT_GEN_EXT_INT15,        ///< external interupt 15
  #endif // __SAM_D10_SUBFAMILY
  EVENT_GEN_DMAC_CH0 = 0x1E,  ///< DMAC channel 0
  EVENT_GEN_DMAC_CH1,         ///< DMAC channel 1
  EVENT_GEN_DMAC_CH2,         ///< DMAC channel 2
  EVENT_GEN_DMAC_CH3,         ///< DMAC channel 3
  EVENT_GEN_TCC0_OVF,         ///< TCC0 overflow
  EVENT_GEN_TCC0_TRG,         ///< TCC0 trigger
  EVENT_GEN_TCC0_CNT,         ///< TCC0 counter
  EVENT_GEN_TCC0_MCX0,        ///< TCC0 match/capture 0
  EVENT_GEN_TCC0_MCX1,        ///< TCC0 match/capture 1
  EVENT_GEN_TCC0_MCX2,        ///< TCC0 match/capture 2
  EVENT_GEN_TCC0_MCX3,        ///< TCC0 match/capture 3
  #ifndef __SAM_D10_SUBFAMILY
  EVENT_GEN_TCC1_OVF,         ///< TCC1 overflow
  EVENT_GEN_TCC1_TRG,         ///< TCC1 trigger
  EVENT_GEN_TCC1_CNT,         ///< TCC1 counter
  EVENT_GEN_TCC1_MCX0,        ///< TCC1 match/capture 0
  EVENT_GEN_TCC1_MCX1,        ///< TCC1 match/capture 1
  EVENT_GEN_TCC1_MCX2,        ///< TCC1 match/capture 2
  EVENT_GEN_TCC1_MCX3,        ///< TCC1 match/capture 3
  EVENT_GEN_TCC2_OVF,         ///< TCC2 overflow
  EVENT_GEN_TCC2_TRG,         ///< TCC2 trigger
  EVENT_GEN_TCC2_CNT,         ///< TCC2 counter
  EVENT_GEN_TCC2_MCX0,        ///< TCC2 match/capture 0
  EVENT_GEN_TCC2_MCX1,        ///< TCC2 match/capture 1
  EVENT_GEN_TCC2_MCX2,        ///< TCC2 match/capture 2
  EVENT_GEN_TCC2_MCX3,        ///< TCC2 match/capture 3
  EVENT_GEN_TC3_OVF,          ///< TC3 overflow
  EVENT_GEN_TC3_MC0,          ///< TC3 match/capture 0
  EVENT_GEN_TC3_MC1,          ///< TC3 match/capture 1
  EVENT_GEN_TC4_OVF,          ///< TC4 overflow
  EVENT_GEN_TC4_MC0,          ///< TC4 match/capture 0
  EVENT_GEN_TC4_MC1,          ///< TC4 match/capture 1
  EVENT_GEN_TC5_OVF,          ///< TC5 overflow
  EVENT_GEN_TC5_MC0,          ///< TC5 match/capture 0
  EVENT_GEN_TC5_MC1,          ///< TC5 match/capture 1
  EVENT_GEN_TC6_OVF,          ///< TC6 overflow
  EVENT_GEN_TC6_MC0,          ///< TC6 match/capture 0
  EVENT_GEN_TC6_MC1,          ///< TC6 match/capture 1
  EVENT_GEN_TC7_OVF,          ///< TC7 overflow
  EVENT_GEN_TC7_MC0,          ///< TC7 match/capture 0
  EVENT_GEN_TC7_MC1,          ///< TC7 match/capture 1
  #else
  EVENT_GEN_TC1_OVF,          ///< TC1 overflow
  EVENT_GEN_TC1_MC0,          ///< TC1 match/capture 0
  EVENT_GEN_TC1_MC1,          ///< TC1 match/capture 1
  EVENT_GEN_TC2_OVF,          ///< TC2 overflow
  EVENT_GEN_TC2_MC0,          ///< TC2 match/capture 0
  EVENT_GEN_TC2_MC1,          ///< TC2 match/capture 1
  #endif // __SAM_D10_SUBFAMILY
  EVENT_GEN_ADC_RESRDY,       ///< ADC result ready
  EVENT_GEN_ADC_WINMON,       ///< ADC window monitor
  EVENT_GEN_ADC_COMP0,        ///< ADC comparator 0
  EVENT_GEN_ADC_COMP1,        ///< ADC comparator 1
  EVENT_GEN_ADC_WIN0,         ///< ADC window 0
  EVENT_GEN_DAC_EMPTY,        ///< DAC empty
  EVENT_GEN_PTC_EOC,          ///< PTC end of conversion
  EVENT_GEN_PTC_WCOMP,        ///< window comparator
  EVENT_GEN_MAX
} EVENTGEN;

/// enumerate the edge
typedef enum _EVENTEDGE
{
  EVENT_EDGE_NONE = 0,        ///< no event
  EVENT_EDGE_RISING,          ///< rising edge
  EVENT_EDGE_FALLING,         ///< falling edge
  EVENT_EDGE_BOTH,            ///< both edges
  EVENT_EDGE_MAX
} EVENTEDGE;

/// enumerate the user multiplexer selection
typedef enum _EVENTUSERMUX
{
  EVENT_USERMUX_DMAC_CH0 = 0, ///< DMAC channel 0
  EVENT_USERMUX_DMAC_CH1,     ///< DMAC channel 1
  EVENT_USERMUX_DMAC_CH2,     ///< DMAC channel 2
  EVENT_USERMUX_DMAC_CH3,     ///< DMAC channel 3
  EVENT_USERMUX_TCC0_EV0,     ///< TCC0 event 0
  EVENT_USERMUX_TCC0_EV1,     ///< TCC0 event 1
  EVENT_USERMUX_TCC0_MC0,     ///< TCC0 match/capture 0
  EVENT_USERMUX_TCC0_MC1,     ///< TCC0 match/capture 1
  EVENT_USERMUX_TCC0_MC2,     ///< TCC0 match/capture 2
  EVENT_USERMUX_TCC0_MC3,     ///< TCC0 match/capture 3
  #ifndef __SAM_D10_SUBFAMILY
  EVENT_USERMUX_TCC1_EV0,     ///< TCC1 event 0
  EVENT_USERMUX_TCC1_EV1,     ///< TCC1 event 1
  EVENT_USERMUX_TCC1_MC0,     ///< TCC1 match/capture 0
  EVENT_USERMUX_TCC1_MC1,     ///< TCC1 match/capture 1
  EVENT_USERMUX_TCC2_EV0,     ///< TCC2 event 0
  EVENT_USERMUX_TCC2_EV1,     ///< TCC2 event 1
  EVENT_USERMUX_TCC2_MC0,     ///< TCC2 match/capture 0
  EVENT_USERMUX_TCC2_MC1,     ///< TCC2 match/capture 1
  EVENT_USERMUX_TC3,          ///< TC3 
  EVENT_USERMUX_TC4,          ///< TC4 
  EVENT_USERMUX_TC5,          ///< TC5 
  EVENT_USERMUX_TC6,          ///< TC6 
  EVENT_USERMUX_TC7,          ///< TC7
  #else
  EVENT_USERMUX_TC1,          ///< TC1 
  EVENT_USERMUX_TC2,          ///< TC2 
  #endif // __SAM_D10_SUBFAMILY
  EVENT_USERMUX_ADC_START,    ///< ADC start
  EVENT_USERMUX_ADC_SYNC,     ///< ADC sync
  EVENT_USERMUX_ADC_COMP0,    ///< ADC start comparator 0
  EVENT_USERMUX_ADC_COMP1,    ///< ADC start comparator 1
  EVENT_USERMUX_DAC_START,    ///< DAC start
  EVENT_USERMUX_PTC_STCONV,   ///< PTC start conversion
} EVENTUSERMUX;

// structures -----------------------------------------------------------------
typedef struct _EVENTDEF
{
  EVENTCHAN       eChannel;     ///< event channel
  EVENTGEN        eGenerator;   ///< generator
  EVENTEDGE       eEdge;        ///< edge
  EVENTUSERMUX    eUserMux;     ///< user mux
} EVENTDEF, *PEVENTDEF;
#define  EVENTDEF_SIZE     sizeof( EVENTDEF )

/**@} EOF Event_def.h */

#endif  // _EVENT_DEF_H