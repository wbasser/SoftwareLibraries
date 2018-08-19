/******************************************************************************
 * @file Timers_cfg.c
 *
 * @brief timers configuration implementation
 *
 * This file provides the configuration table for the timers
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

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------
// constant parameter initializations -----------------------------------------
const CODE TIMERDEF     atTimerDefs[ TIMERS_ENUM_MAX ] =
{
  // using the below macro add timers to the definition table
  // TIMER_CMPDEF( chan, align, pre, irqsel, periodhz, comparehz, invert, initon, callback, irqpri )
  // TIMER_CAPDEF( chan, pre, irqsel, event, edge, level, initon, callback, irqpri )

};
 
/**@} EOF Timers_cfg.c */
