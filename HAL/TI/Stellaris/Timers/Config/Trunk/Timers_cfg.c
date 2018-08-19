/******************************************************************************
 * @file Timers_cfg.c
 *
 * @brief TImers configuraiton implementation
 *
 * This file pvoides the timers configuration implementation
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const TIMERDEF atTimerDefs[ TIMER_ENUM_MAX ] = 
{
  // TIMER_32BIT( timeperiph, gpioperiph, timebase, gpiobase, gpiopin, funcpin, mode, edge, ints, prescale, count, match, enabled, callback ),
  // TIMER_16BIT( timeperiph, gpioperiph, timebase, gpiobase, gpiopin, funcpin, timer, mode, edge, ints, prescale, count, match, enabled, callback ),
};

/**@} EOF GPIO_cfg.c */
