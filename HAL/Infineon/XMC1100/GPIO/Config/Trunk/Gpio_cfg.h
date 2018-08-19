/******************************************************************************
 * @file Gpio_cfg.h
 *
 * @brief GPIO configuration declarations
 *
 * This file provides the declarations for the GPIO configuration 
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_CFG_H
#define _GPIO_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the GPIO pin enuemrations
typedef enum  _GPIOPINENUM
{
  // add user enumerations below

  // do not remove the entries below
  GPIO_PIN_ENUM_MAX,
  GPIO_PIN_ENUM_ILLEGAL
} GPIOPINENUM;

/// declare the GPIO function enumerations
typedef enum _GPIOFUNCENUM
{
  // add user enumerations below

  // do not remove the entries below
  GPIO_FUNC_ENUM_MAX
} GPIOFUNCENUM;

// global function prototypes --------------------------------------------------
extern  const GPIODEF   atGpioDefs[ ];
extern  const GPIODEF   atGpioFuncs[ ];

/**@} EOF Gipo_cfg.h */

#endif  // _GPIO_CFG_H
