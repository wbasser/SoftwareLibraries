/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
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
#define GPIOINPINDEF( port, pin, invert, filt, schmidt ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_INPUT, \
    .bInvert        = invert, \
    .bFilterEnable  = filt, \
    .bSmidthTrigEnb = schmidt, \
  }

/// helper macro for creating a function output pin  
#define GPIOFNCDEF( port, pin, func ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eFunc          = func, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the ports
typedef enum _GPIOPORT
{
  GPIO_PORT_A = 0,          ///< port A
  GPIO_PORT_B,              ///< port B
  GPIO_PORT_C,              ///< port C
  #ifdef PIOD
  GPIO_PORT_D,               ///< port D
  #endif // PIOD
  #ifdef PIOE
  GPIO_PORT_E,               ///< port E
  #endif // PIOE
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_DISABLED = 0,   ///< disabled - analog I/O
  GPIO_MODE_OUTPUT,         ///< output mode
  GPIO_MODE_INPUT,          ///< input
  GPIO_MODE_INPUT_PULLDN,   ///< input, pull down
  GPIO_MODE_INPUT_PULLUP,   ///< input, pull up
  GPIO_MODE_INPUT_PULLUPDN, ///< input, pull up/down
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the pin multiplexing
typedef enum _GPIOFUNCMUX
{
  GPIO_FUNCMUX_A = 0,       ///< A function
  GPIO_FUNCMUX_B,           ///< B function
  GPIO_FUNCMUX_C,           ///< C function
  GPIO_FUNCMUX_D,           ///< D function
  GPIO_FUNCMUX_MAX
} GPIOFUNCMUX;

/// enumerate the IRQ pin edge
typedef enum _GPIOIRQSENSE
{
  GPIO_SEND_NONE = 0,       ///< no sense
  GPIO_SENSE_FALL,          ///< falling edge
  GPIO_SENSE_RISE,          ///< rising edge
  GPIO_SENSE_BOTH,          ///< both edges
  GPIO_SENSE_HILVL,         ///< HI level
  GPIO_SENSE_LOLVL,         ///< LO level
  GPIO_EDGE_MAX
} GPIOIRQSENSE;

/// enumerate the GPIO pin direction
typedef enum _GPIODIR
{
  GPIO_DIR_IN = 0,          ///< input
  GPIO_DIR_OUT,             ///< output
  GPIO_DIR_MAX          
} GPIODIR;

// structures -----------------------------------------------------------------
/// define the callback
typedef void ( *PVGPIOIRQCALLBACK )( U8, U8 );

/// define the pin definition structure
typedef struct _GPIOPINDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
  BOOL              bFilterEnable;  ///< enable filter for inputs
  BOOL              bSmidthTrigEnb; ///< enable smitch trigger
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/// define the pin definition structure
typedef struct _GPIOFNCDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  GPIOFUNCMUX       eFunc;          ///< port function
} GPIOFNCDEF, *PGPIOFNCDEF;
#define GPIOFNCDEF_SIZE  sizeof( GPIOFNCDEF )

/// define the pin definition structure
typedef struct _GPIOIRQDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  GPIOIRQSENSE      eSense;         ///< sense
  BOOL              bFilterEnb;     ///< filter enable
  BOOL              bSmidthTrigEnb; ///< enable smitch trigger
  PVGPIOIRQCALLBACK pvCallback;     ///< callback
  U8                nEvent;         ///< callback event
} GPIOIRQDEF, *PGPIOIRQDEF;
#define GPIOIRQDEF_SIZE  sizeof( GPIOIRQDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H