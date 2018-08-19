/******************************************************************************
 * @file Timers.h
 *
 * @brief Timer declaration
 *
 * This file provides the declarations for the Timer module
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
#ifndef _TIMERS_H
#define _TIMERS_H

// local includes -------------------------------------------------------------
#include "Timers/Timers_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the timer errors
typedef enum _TIMERERR
{
  TIMER_ERR_NONE = 0,      ///< no error
  TIMER_ERR_ILLTIMER,      ///< illegal timer
  TIMER_ERR_ILLACTION,     ///< illegal IOCTL action
} TIMERERR;

/// enumerate the ioctl actions
typedef enum _TIMERIOCTL
{
  TIMER_IOCTL_CHANGEDEF = 0,    ///< change the timer definition
  TIMER_IOCTL_STOPSTART,        ///< stop/start the timer
  TIMER_IOCTL_GETCOUNT,         ///< get the count
  TIMER_IOCTL_SETCOUNT,         ///< set the count
  TIMER_IOCTL_GETPERIOD,        ///< get the period
  TIMER_IOCTL_SETPERIOD,        ///< set the period
  TIMER_IOCTL_GETCOMPAREVAL,    ///< get the compare value
  TIMER_IOCTL_SETCOMPAREVAL,    ///< set the compare value
  TIMER_IOCTL_SETCOMPAREPCT,    ///< set the compare in percent
  TIMER_IOCTL_GETCAPTURE,       ///< get the capture value
  TIMER_IOCTL_SETCAPTURE,       ///< set the capture value
  TIMER_IOCTL_GETPRESCALEDIV,   ///< get the prescale divisor
  TIMER_IOCTL_MAX
} TIMERIOCTL;

// structures -----------------------------------------------------------------
/// define the IOCTL parameter structure
typedef struct _TIMERIOCTLPARAM
{
  TIMERCMPCAPCHAN   eCmpCapChan;    ///< compare/capture channel
  U32               uValue;         ///< value
} TIMERIOCTLPARAM, *PTIMERIOCTLPARAM;
#define TIMERIOCTLPARAM_SIZE        sizeof( TIMERIOCTLPARAM )

// global function prototypes --------------------------------------------------
extern  void      Timers_Initialize( void );
extern  void      Timers_Close( void );
extern  TIMERERR  Timers_Ioctl( TIMERENUM eTimerSel, TIMERIOCTL eAction, PVOID pvParam );

/**@} EOF Timers.h */

#endif  // _TIMERS_H
