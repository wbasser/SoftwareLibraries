/******************************************************************************
 * @file Timers_cfg.c
 *
 * @brief Timers configuration implementation
 *
 * This file provides the impolementation of the congiruation
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
 * \addtogroup Timers
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const TIMERDEF  atTimerDef[ TIMER_ENUM_MAX ] =
{
  // instantatiate timers using the below macros
  // TIMERCAPDEF( chan, clksel, exttrig, edgetrig, edgea, edgeb, ovfirq, cmpairq, cmpbirq, initon, callback )
  // TIMERWAVEPER( chan, clksel, period, cmpacta, cmpactb, cmpairq, cmpbirq, initon, callback )
  
};

/**@} EOF Timers_cfg.c */
