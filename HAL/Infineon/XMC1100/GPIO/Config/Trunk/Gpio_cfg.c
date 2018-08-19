/******************************************************************************
 * @file GPIO_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file pvoides the GPIO configuration implementation
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

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// constant parameter initializations -----------------------------------------
const GPIODEF atGpioDefs[ GPIO_PIN_ENUM_MAX ] =
{
  // declare the pins using one of the helper macros below
  // GPIOPIN_DEF( port, pin, mode, hyst, invert, init )

};

const GPIODEF atGpioFuncs[ GPIO_FUNC_ENUM_MAX ] =
{
  // declare the pins using ont of the help macros below
  // GPIOFNCDEF( port, pin, mode, hyst )

};

/**@} EOF GPIO_cfg.c */
