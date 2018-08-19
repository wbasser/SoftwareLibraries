/******************************************************************************
 * @file GPIO_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file pvoides the GPIO configuration implementation
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// constant parameter initializations -----------------------------------------
const CODE GPIODEF atGpioPinDefs[ GPIO_PIN_ENUM_MAX ] = 
{
  // declare the pins using one of the helper macros below
  // GPIO_OUT( port, pin, mode, invert, initval ) 
  // GPIO_IN( port, pin, mode, invert ) 
  // GPIO_INOUT( port, pin, mode, invert, initmode, initdir ) 
  // GPIO_PINIRQ( port, pin, irq, mode, edge, enabled, callback ) 
  // GPIO_EXTIRQ( port, pin, irq, mode, edge, enabled, callback )  
    
  // user pins
};

const CODE GPIODEF atGpioFncDefs[ GPIO_FNC_ENUM_MAX ] =
{
  // declare the pins using one of the helper macros below
  // GPIO_FUNC( port, pin )
};

// user groups
const CODE GPIOGRPDEF atGpioGrpDefs[ GPIO_GRP_ENUM_MAX ] =
{
  // delare groups here
  // GPIO_GROUP( port, mask, shift )
};

/**@} EOF GPIO_cfg.c */
