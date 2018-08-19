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
const CODE GPIOPINDEF atGpioPinDefs[ GPIO_PIN_ENUM_MAX ] = 
{
  // declare the pins using one of the helper macros below
  // GPIOOUTPINDEF( port, pin, invert, init )
  // GPIOINPINDEF( port, pin, invert, init )
  
};

const CODE GPIOFNCDEF atGpioFncDefs[ GPIO_FNC_ENUM_MAX ] =
{
  // declare the pins using one of the helper macros below
  // GPIOFNCDEF( port, pin, func )
};

//const CODE GPIOIRQDEF atGpioIrqDefs[ GPIO_IRQ_ENUM_MAX ] =
//{
//  // declare the pins using one of the helper macros below
//  // GPIOIRQDEF( port, pin, sense, wakeup, filter, callback, event )
//  // GPIOEVNDEF( port, pin, sense, wakeup, filter )
//};

/**@} EOF GPIO_cfg.c */
