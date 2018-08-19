/******************************************************************************
 * @file Timers_cfg.c
 *
 * @brief timers configuration implementation
 *
 * This file provides the configuration table for the timers
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
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------
// constant parameter initializations -----------------------------------------
const CODE TIMERDEF atTimerDefs[ TIMERS_ENUM_MAX ] =
{
  // using the below macro add timers to the definition table
  // TIMER_DEF80CTC( ratehz, prescale, outenb, initon, callback ) 
  // TIMER_DEF82CTC( ratehz, prescale, outenb, initon, callback )
  // TIMER_DEF16CTC( timsel, ratehz, prescale, outenb, initon, callback )
  // TIMER_DEF8PWMFAST( timsel, prescale, comparepct, actiona, actionb, initon, callback )
  // TIMER_DEF16PWMFAST8( timsel, prescale, comparepct, actiona, actionb, actionc, initon, callback )
  // TIMER_DEF16PWMFAST9( timsel, prescale, comparepct, actiona, actionb, actionc, initon, callback )
  // TIMER_DEF16PWMFAST10( timsel, prescale, comparepct, actiona, actionb, actionc, initon, callback )
  // TIMER_DEF16CAPTURE( timsel, prescale, edge, initedge, filter, initon, tovenb, callback )
  // TIMER_DEF16PWMFASTI( timsel, prescale, period, tovenb, actiona, actionb, actionc, initon, callback )
  // TIMER_DEF16NORM( timsel )
};
 
/**@} EOF Timers_cfg.c */

