/******************************************************************************
 * @file Gpio.h
 *
 * @brief GPIO declarations
 *
 * This file provides the declarations to the GPIO API's
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
 
// ensure only one instatiation
#ifndef _GPIO_H
#define _GPIO_H

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

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

/// enumerate the GPIO errors
typedef enum _GPIOERR
{
  GPIO_ERR_NONE = 0,    ///< no error
  GPIO_ERR_ILLGPIO,     ///< illegal GPIO selection
  GPIO_ERR_NOTVALID,    ///< pin not valid
  GPIO_ERR_ILLACT,      ///< illegal IOCTL action
  GPIO_ERR_ILLMODE,     ///< illegal mode, (I.e. trying to set an input pin )
  GPIO_ERR_ILLDIR,      ///< illegal direction, (i.e. tryint to change a direction on a non IO pin )
} GPIOERR;

// structures -----------------------------------------------------------------
typedef U32     GPIOENUM;

// global function prototypes --------------------------------------------------
extern  void      Gpio_Initialize( void );
extern  void      Gpio_Close( void );
extern  GPIOENUM  Gpio_Configure( GPIOPORT ePort, U8 nPin, GPIOMODE eMode, BOOL bInvert, BOOL bIntial );
extern  GPIOERR   Gpio_Set( GPIOENUM eGpioEnum, BOOL bState );
extern  GPIOERR   Gpio_Get( GPIOENUM eGpioEnum, PBOOL pbState );
extern  GPIOERR   Gpio_Toggle( GPIOENUM eGpioEnum );

/**@} EOF Gpio.h */

#endif  // _GPIO_H