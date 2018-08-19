/******************************************************************************
 * @file GPIO_cfg.h
 *
 * @brief GPIO configuration declarations
 *
 * This file 
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
 
// ensure only one instatiation
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
  GPIO_PIN_ENUM_MAX
} GPIOPINENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const GPIODEF atGpioDefs[ ];

/**@} EOF Gpio_cfg.h */

#endif  // _GPIO_CFG_