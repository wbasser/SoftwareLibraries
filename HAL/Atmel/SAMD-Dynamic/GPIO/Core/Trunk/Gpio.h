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
  GPIO_PORT_A = 0,          ///< port A
  GPIO_PORT_B,              ///< port B
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_DISABLED = 0,   ///< disabled - analog I/O
  GPIO_MODE_INPDSB_PULLDN,  ///< pull down, input disabled
  GPIO_MODE_INPDSB_PULLUP,  ///< pull up, input disabled
  GPIO_MODE_INPUT,          ///< input
  GPIO_MODE_INPUT_PULLDN,   ///< input, pull down
  GPIO_MODE_INPUT_PULLUP,   ///< input, pull up
  GPIO_MODE_OUTPUT_INPDSB,  ///< output, input disabled
  GPIO_MODE_OUTPUT_INPENB,  ///< output, input enabled
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the pin multiplexing
typedef enum _GPIOFUNCMUX
{
  GPIO_FUNCMUX_A = 0,       /// A function
  GPIO_FUNCMUX_B,           ///< B function
  GPIO_FUNCMUX_C,           ///< C function
  GPIO_FUNCMUX_D,           ///< D function
  GPIO_FUNCMUX_E,           ///< E function
  GPIO_FUNCMUX_F,           ///< F function
  GPIO_FUNCMUX_G,           ///< G function
  GPIO_FUNCMUX_H,           ///< H function
  GPIO_FUNCMUX_MAX
} GPIOFUNCMUX;

/// enumerate the errors
typedef enum _GPIOERR
{
  GPIO_ERR_NONE = 0,        ///< no error
  GPIO_ERR_ILLENUM,         ///< illegal enum
} GPIOERR;

// structures -----------------------------------------------------------------
typedef U32     GPIOENUM;

// global function prototypes --------------------------------------------------
extern  GPIOENUM Gpio_Configure( GPIOPORT ePort, U8 nPin, GPIOMODE eMode, BOOL bHiDriveEnb, GPIOFUNCMUX eFunc, BOOL bInvert );
extern  GPIOERR  Gpio_Set( GPIOENUM uGpioSel, BOOL bState );
extern  GPIOERR  Gpio_Get( GPIOENUM uGpioSel, PBOOL pbState );
extern  GPIOERR  Gpio_Toggle( GPIOENUM uGpioSel );

/**@} EOF Gpio.h */

#endif  // _GPIO_H