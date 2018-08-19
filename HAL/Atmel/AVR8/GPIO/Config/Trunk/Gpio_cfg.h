/******************************************************************************
 * @file Gpio_cfg.h
 *
 * @brief GPIO configuration declarations
 *
 * This file provides the declarations for the GPIO configuration 
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
  /// enumerate GPIO pins here
  
  /// this ends the list - do not remove
  GPIO_PIN_ENUM_MAX,
  GPIO_PIN_ENUM_ILLEGAL = 0xFF
} GPIOPINENUM;

/// declare the GPIO function pin enumerations
typedef enum  _GPIOFNCENUM
{
  /// enumerate function pins here
  
  /// this end s the list - do not remove
  GPIO_FNC_ENUM_MAX,
} GPIOFNCENUM;

/// declare the GPIO group enumerations
typedef enum  _GPIOGROUPENUM
{
  /// add user entries here

  /// this ends the list - do not remove
  GPIO_GRP_ENUM_MAX,
  GPIO_GRP_ENUM_ILLEGAL = 0xFF
} GPIOGROUPENUM;

// global function prototypes --------------------------------------------------
extern  const CODE GPIODEF    atGpioPinDefs[ ];
extern  const CODE GPIODEF    atGpioFncDefs[ ];
extern  const CODE GPIOGRPDEF atGpioGrpDefs[ ];

/**@} EOF Gipo_cfg.h */

#endif  // _GPIO_CFG_H