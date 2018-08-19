/******************************************************************************
 * @file GPIO_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file pvoides the GPIO configuration implementation
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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
  // GPIOOUTPINDEF( port, pin, opendrn, speed, puppdn, invert, init )
  // GPIOINPINDEF( port, pin, opendrn, speed, puppdn, invert, filt, schmidt ) 
  // GPIOFNCDEF( port, pin, mode, opendrn, speed, puppdn, func )
};

//const CODE GPIOIRQDEF atGpioIrqDefs[ GPIO_IRQ_ENUM_MAX ] =
//{
//  // declare the pins using one of the helper macros below
//};

/**@} EOF GPIO_cfg.c */
