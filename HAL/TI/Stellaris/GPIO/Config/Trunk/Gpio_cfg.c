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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const GPIODEF atGpioDefs[ GPIO_PIN_ENUM_MAX ] = 
{
  // GPIO_PIN_INP( base, pin, invert, mode ),
  // GPIO_PIN_OUT( base, pin, invert, mode, strength ),
  // GPIO_PIN_INT( base, pin, invert, mode, inttype, callback ),
};

/**@} EOF GPIO_cfg.c */
