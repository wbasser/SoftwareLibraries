/******************************************************************************
 * @file Timers_cfg.h
 *
 * @brief Timers configuration declarations
 *
 * This file provides the configuration declarations
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
 
// ensure only one instatiation
#ifndef _TIEMRS_CFG_H
#define _TIMERS_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the Timers channel enuemrations
typedef enum  _TIMERENUM
{
  TIMER_ENUM_MAX
} TIMERENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const TIMERSDEF atTimersDefs[ ];

/**@} EOF Timers_cfg.h */

#endif  // _TIMERS_CFG_