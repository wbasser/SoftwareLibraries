/******************************************************************************
 * @file Timers.h
 *
 * @brief Timer declaration
 *
 * This file provides the declarations for the Timer module
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
 
// ensure only one instantiation
#ifndef _TIMER_H
#define _TIMER_H

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the timer errors
typedef enum _TIMERSERR
{
  TIMERS_ERR_NONE = 0,      ///< no error
  TIMERS_ERR_ILLTIMER,      ///< illegal timer
  TIMERS_ERR_ILLCHAN,       ///< illegal channel
  TIMERS_ERR_ILLTIMESIZE,   ///< illegal timer size
  TIMERS_ERR_NOCAPTURE,     ///< no capture register
  TIMERS_ERR_ILLACTION,     ///< illegal iOCTL action
} TIMERSERR;

// global function prototypes --------------------------------------------------
extern  void      Timers_Initialize( void );
extern  TIMERSERR Timers_Ioctl( TIMERSENUM eTimerSel, TIMERIOCTL eAction, PVOID pvParam );

/**@} EOF Timer.h */

#endif  // _TIMER_H