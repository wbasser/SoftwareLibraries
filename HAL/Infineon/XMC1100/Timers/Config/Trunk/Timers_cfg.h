/******************************************************************************
 * @file Timers_cfg.h
 *
 * @brief timers configuration declarations
 *
 * This file provides the declarations for the timer configuration
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
#ifndef _TIMERS_CFG_H
#define _TIMERS_CFG_H

// local includes -------------------------------------------------------------
#include "Timers/Timers_def.h"

// enumerations ---------------------------------------------------------------
/// enumerate the timers here
typedef enum _TIMERSENUM
{
  TIMERS_ENUM_PWMOUT = 0,
  TIMERS_ENUM_MANBITTIME,
  TIMERS_ENUM_MANCAPTURE,
  TIMERS_ENUM_MAX,
  TIMERS_ENUM_ILLEGAL
} TIMERSENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE TIMERDEF   atTimerDefs[ ];

/**@} EOF Timers_cfg.h */

#endif  // _TIMERS_CFG_H
