/******************************************************************************
 * @file Timers_cfg.h
 *
 * @brief Timer configuration declarations 
 *
 * This file provides the declarations for the timer confgiuration
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
 
// ensure only one instantiation
#ifndef _TIMERS_CFG_H
#define _TIMERS_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------
/// enumerate the timers
typedef enum _TIMERENUM
{
  // enumerate timers here
  
  // do not remove these entires
  TIMER_ENUM_MAX,
  TIMER_ENUM_ILLEGAL = 255
} TIMERENUM;

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const TIMERDEF  atTimerDef[ ];

// global function prototypes --------------------------------------------------

/**@} EOF Timers_cfg.h */

#endif  // _TIMERS_CFG_H