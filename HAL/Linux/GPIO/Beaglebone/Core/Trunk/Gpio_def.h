/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
 *
 * @copyright Copyright (c) 2017 CyberIntegration
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
#ifndef _GPIO_DEF_H
#define _GPIO_DEF_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating output pins
#define GPIOOUTPINDEF( port, pin, invert, init ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_OUTPUT, \
    .bInvert        = invert, \
    .bInitialState  = init \
  }

/// define the helper macro for creating input pins
#define GPIOINPINDEF( port, pin, invert ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_INPUT, \
    .bInvert        = invert, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the ports
typedef enum _GPIOPORT
{
  GPIO_PORT_0 = 0,          ///< port 0
  GPIO_PORT_1,              ///< port 1
  GPIO_PORT_2,              ///< port 2
  GPIO_PORT_3,              ///< port 3
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_INPUT,          ///< input
  GPIO_MODE_OUTPUT,         ///< output mode
  GPIO_MODE_INPUT_PULLDN,   ///< input, pull down
  GPIO_MODE_INPUT_PULLUP,   ///< input, pull up
  GPIO_MODE_INPUT_PULLUPDN, ///< input, pull up/down
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the GPIO pin direction
typedef enum _GPIODIR
{
  GPIO_DIR_IN = 0,          ///< input
  GPIO_DIR_OUT,             ///< output
  GPIO_DIR_MAX          
} GPIODIR;

// structures -----------------------------------------------------------------

/// define the pin definition structure
typedef struct _GPIOPINDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H