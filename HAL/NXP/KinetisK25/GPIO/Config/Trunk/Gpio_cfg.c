/******************************************************************************
 * @file GPIO_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file pvoides the GPIO configuration implementation
 *
 * @copyright Copyright (c) 2012 Guardhat
 * This document contains proprietary data and information of Guardhat 
 * LLC. It is the exclusive property of Guardhat, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Guardhat, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Guardhat, LLC.
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
  // GPIOPINDEF( port, pin, direction, function, hidrive, invert, init )
  // GPIOIRQDEF( port, pin, direction, function, sense, filter, callback, event )
  GPIOPINDEF( GPIO_PORT_B, 18, GPIO_DIR_OUT, GPIO_FUNCMUX_1, ON, ON, OFF ),
  GPIOPINDEF( GPIO_PORT_B, 19, GPIO_DIR_OUT, GPIO_FUNCMUX_1, ON, ON, OFF ),
  GPIOPINDEF( GPIO_PORT_D,  1, GPIO_DIR_OUT, GPIO_FUNCMUX_1, ON, ON, OFF ),
};

/**@} EOF GPIO_cfg.c */
