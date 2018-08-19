/******************************************************************************
 * @file Timers.h
 *
 * @brief Timer declaration
 *
 * This file provides the declarations for the Timer module
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
} TIMERSERR;

// global function prototypes --------------------------------------------------
extern  void      Timers_Initialize( void );
extern  TIMERSERR Timers_StartStop( TIMERSENUM eTimerSel, BOOL bState );
//extern  TIMERSERR Timers_GetCounter( TIMERSENUM eTimerSel, PU16 pwCounter );
//extern  TIMERSERR Timers_SetCounter( TIMERSENUM eTimerSel, U16 wCounter );
extern  TIMERSERR Timers_GetPeriod( TIMERSENUM eTimerSel, PU16 pwPeriod );
extern  TIMERSERR Timers_SetPeriod( TIMERSENUM eTimerSel, U16 wPeriod );
extern  TIMERSERR Timers_GetCompareVal( TIMERSENUM eTimerSel, PU16 pwCompare );
extern  TIMERSERR Timers_SetCompareVal( TIMERSENUM eTimerSel, U16 wCompare );
extern	TIMERSERR Timers_SetComparePct( TIMERSENUM eTimerSel, U16 wPercent );
//extern  TIMERSERR Timers_GetCapture( TIMERSENUM eTimerSel, TIMERCHAN eChanSel, PU16 pwCapture );

/**@} EOF Timer.h */

#endif  // _TIMER_H
