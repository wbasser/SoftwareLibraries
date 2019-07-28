/******************************************************************************
 * @file Timers_cfg.h
 *
 * @brief timers configuration declarations
 *
 * This file provides the declarations for the timer configuration
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
#ifndef _TIMERS_CFG_H
#define _TIMERS_CFG_H

// local includes -------------------------------------------------------------
#include "Timers/Timers_def.h"

// enumerations ---------------------------------------------------------------
/// enumerate the timers here
typedef enum _TIMERENUM
{
  // enumerate user timers below
  
  // do not remove the entries below
  TIMER_ENUM_MAX,
  TIMER_ENUM_ILLEGAL = 0xFF
} TIMERENUM;

// global parameter declarations -----------------------------------------------
extern  const TIMERDEF   atTimerDefs[ ];

/**@} EOF Timers_cfg.h */

#endif  // _TIMERS_CFG_H
