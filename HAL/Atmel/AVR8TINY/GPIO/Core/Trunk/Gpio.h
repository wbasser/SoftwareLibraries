/******************************************************************************
 * @file Gpio.h
 *
 * @brief GPIO handler declaraions 
 *
 * This file provides the declarations for the GPIO hal module
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
 * $Rev: $
 * 
 *
 * \addtogroup Gpio
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_H
#define _GPIO_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _GPIOERR
{
  GPIO_ERR_NONE = 0,        ///< no error
  GPIO_ERR_ILLENUM,         ///< illegal enum
} GPIOERR;

/// enumerate the ports
typedef enum _GPIOPORT
{
  #ifdef PORTA
  GPIO_PORT_A = 0,
  #endif
  #ifdef PORTB
  GPIO_PORT_B,
  #endif
  #ifdef PORTC
  GPIO_PORT_C,
  #endif
  #ifdef PORTD
  GPIO_PORT_D,
  #endif
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin direction
typedef enum _GPIODIR
{
  GPIO_DIR_IN = 0,      ///< input
  GPIO_DIR_OUT,         ///< output
  GPIO_DIR_MAX
} GPIODIR;

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_TOTEM = 0,  ///< totem mode
  GPIO_MODE_PULLUP,     ///< pull up
  GPIO_MODE_MAX
} GPIOMODE;

// structures -----------------------------------------------------------------
/// define the GPIOENU type
typedef U8  GPIOPINENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  GPIOPINENUM Gpio_Configure( GPIOPORT ePort, U8 nPin, GPIODIR eDir, GPIOMODE eMode, BOOL bInvert, BOOL bInitial );
extern  GPIOERR     Gpio_Set( GPIOPINENUM eEnum, BOOL bState );
extern  GPIOERR     Gpio_Get( GPIOPINENUM eEnum, PBOOL pbState );
extern  GPIOERR     Gpio_Toggle( GPIOPINENUM eEnum );

/**@} EOF Gpio.h */

#endif  // _GPIO_H