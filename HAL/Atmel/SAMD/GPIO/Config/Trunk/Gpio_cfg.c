/******************************************************************************
 * @file GPIO_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file pvoides the GPIO configuration implementation
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

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// constant parameter initializations -----------------------------------------
const CODE GPIOPINDEF g_atGpioPinDefs[ GPIO_PIN_ENUM_MAX ] = 
{
  // declare the pins using one of the helper macros below
  // GPIOPINDEF( port, pin, mode, hidrive, invert, init )
};

const CODE GPIOFNCDEF g_atGpioFncDefs[ GPIO_FNC_ENUM_MAX ] =
{
  // declare the pins using one of the helper macros below
  // GPIOFNCDEF( port, pin, func, allowinp, pullenb )
};

const CODE GPIOIRQDEF g_atGpioIrqDefs[ GPIO_IRQ_ENUM_MAX ] =
{
  // declare the pins using one of the helper macros below
  // GPIOIRQDEF( port, pin, pupdn, sense, wakeup, filter, callback, event, initon )
  // GPIOEVNDEF( port, pin, pupdn, sense, wakeup, filter )
};

/**@} EOF GPIO_cfg.c */
