/******************************************************************************
 * @file Timers.h
 *
 * @brief timers implementation declarations
 *
 * This file provides the declarations for the timer control module
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup Timers
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TIMERS_H
#define _TIMERS_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the timer errors
typedef enum _TIMERERR
{
  TIMER_ERR_NONE = 0,   // no error
  TIMER_ERR_ILLTIM,     // illegal timer selection
  TIMER_ERR_ILLACT,     // illegal IOCTL action
} TIMERERR;

/// enumerate the timer IOCTL actions
typedef enum _TIMERACT
{
  TIMER_ACT_NOOP = 0,   // no operation
  TIMER_ACT_IRQENB,     // enable interrupts
  TIMER_ACT_IRQDSB,     // disable interrupts
  TIMER_ACT_SETCNT,     // set the counter
  TIMER_ACT_GETPRE,     // get the prescale value
  TIMER_ACT_SETPRE,     // set the prescale value
  TIMER_ACT_ENBTIM,     // enable timer
  TIMER_ACT_DSBTIM,     // disable timer
  TIMER_ACT_MAX,        // maximum number
} TIMERACT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      Timers_Initialize( void );
extern  TIMERERR  Timers_GetCount( TIMERENUM eTimerSel, PU32 puCount );
extern  TIMERERR  Timers_GetCapture( TIMERENUM eTimerSel, PU32 puCapture );
extern  TIMERERR  Timers_Ioctl( TIMERENUM eTimerSel, TIMERACT eAction, PVOID pvData );

/**@} EOF Timers.h */

#endif  // _TIMERS_H