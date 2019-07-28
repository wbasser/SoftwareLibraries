/******************************************************************************
 * @file Gpio_cfg.h
 *
 * @brief GPIO configuration declarations
 *
 * This file provides the declarations for the GPIO configuration 
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_CFG_H
#define _GPIO_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the GPIO user pin enumerations
typedef enum  _GPIOPINENUM
{
  // add enumerations here
  
  // this ends the list - do not remove
  GPIO_PIN_ENUM_MAX,
  GPIO_PIN_ENUM_ILLEGAL = 0xFF
} GPIOPINENUM;

/// declare the GPIO function pin enumerations
typedef enum  _GPIOFNCENUM
{
  // add enumerations here

  // this ends the list - do not remove
  GPIO_FNC_ENUM_MAX,
  GPIO_FNC_ENUM_ILLEGAL = 0xFF
} GPIOFNCENUM;

/// declare the GPIO interrupt pin enumerations
typedef enum  _GPIOIRQENUM
{
  // add enumerations here
  
  // this end s the list - do not remove
  GPIO_IRQ_ENUM_MAX,
  GPIO_IRQ_ENUM_ILLEGAL = 0xFF
} GPIOIRQENUM;

// global function prototypes --------------------------------------------------
extern  const CODE GPIOPINDEF   g_atGpioPinDefs[ ];
extern  const CODE GPIOFNCDEF   g_atGpioFncDefs[ ];
extern  const CODE GPIOIRQDEF   g_atGpioIrqDefs[ ];

/**@} EOF Gipo_cfg.h */

#endif  // _GPIO_CFG_H