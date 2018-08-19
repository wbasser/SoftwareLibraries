/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
#define GPIOOUTPINDEF( port, pin, opendrn, speed, puppdn, invert, init ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_OUTPUT, \
    .eOpenDrn       = opendrn, \
    .eSpeed         = speed, \
    .ePupPdn        = puppdn, \
    .bInvert        = invert, \
    .bInitialState  = init, \
    .eFunc          = GPIO_FUNCMUX_00, \
    .eIrqSense      = GPIO_SENSE_NONE \
  }

/// define the helper macro for creating input pins
#define GPIOINPINDEF( port, pin, opendrn, speed, puppdn, invert, filt, schmidt ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_INPUT, \
    .eOpenDrn       = opendrn, \
    .eSpeed         = speed, \
    .ePupPdn        = puppdn, \
    .bInvert        = invert, \
    .eFunc          = GPIO_FUNCMUX_00, \
    .eIrqSense      = GPIO_SENSE_NONE \
  }

/// helper macro for creating a function output pin  
#define GPIOFNCDEF( port, pin, opendrn, speed, puppdn, func ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_ALTFUNC, \
    .eOpenDrn       = opendrn, \
    .eSpeed         = speed, \
    .ePupPdn        = puppdn, \
    .eFunc          = func, \
    .eIrqSense      = GPIO_SENSE_NONE \
  }
  
/// helper macro for creating an IRQ line
#define GPIOIRQDEF( port, pin, speed, puppdn, edge, callback ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = GPIO_MODE_INPUT, \
    .eOpenDrn       = GPIO_OPENDRN_DSB, \
    .eSpeed         = speed, \
    .ePupPdn        = puppdn, \
    .eFunc          = GPIO_FUNCMUX_00, \
    .eIrqSense      = edge, \
    .pvCallback     = callback \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the ports
typedef enum _GPIOPORT
{
  GPIO_PORT_A = 0,          ///< port A
  GPIO_PORT_B,              ///< port B
  GPIO_PORT_C,              ///< port C
  GPIO_PORT_D,              ///< port D
  GPIO_PORT_E,              ///< port E
  GPIO_PORT_F,              ///< port F
  GPIO_PORT_G,              ///< port G
  GPIO_PORT_H,              ///< port H
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_INPUT,          ///< input
  GPIO_MODE_OUTPUT,         ///< output mode
  GPIO_MODE_ALTFUNC,        ///< alternate function
  GPIO_MODE_ANALOG,         ///< analog
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the open drain mode
typedef enum _GPIOOPENDRN
{
  GPIO_OPENDRN_DSB = 0,     ///< disable
  GPIO_OPENDRN_ENB,         ///< enable
  GPIO_OPENDRN_MAX
} GPIOOPENDRN;

/// enumerate the speed
typedef enum _GPIOSPEED
{
  GPIO_SPEED_LOW = 0,       ///< low speed
  GPIO_SPEED_MED,           ///< medium speed
  GPIO_SPEED_HIGH,          ///< high speed
  GPIO_SPEED_VERYHIGH,      ///< very high speed
  GPIO_SPEED_MAX
} GPIOSPEED;

/// enumerate the pull up/down 
typedef enum _GPIOPUPPDN
{
  GPIO_PUPPDN_NONE = 0,     ///< none
  GPIO_PUPPDN_UP,           ///< pull up
  GPIO_PUPPDN_DN,           ///< pull down
  GPIO_PUPPDN_MAX
} GPIOPUPPDN;

/// enumerate the pin multiplexing
typedef enum _GPIOFUNCMUX
{
  GPIO_FUNCMUX_00 = 0,      ///< Alternate Function 00 function
  GPIO_FUNCMUX_01,          ///< Alternate Function 01 function
  GPIO_FUNCMUX_02,          ///< Alternate Function 02 function
  GPIO_FUNCMUX_03,          ///< Alternate Function 03 function
  GPIO_FUNCMUX_04,          ///< Alternate Function 04 function
  GPIO_FUNCMUX_05,          ///< Alternate Function 05 function
  GPIO_FUNCMUX_06,          ///< Alternate Function 06 function
  GPIO_FUNCMUX_07,          ///< Alternate Function 07 function
  GPIO_FUNCMUX_08,          ///< Alternate Function 08 function
  GPIO_FUNCMUX_09,          ///< Alternate Function 09 function
  GPIO_FUNCMUX_10,          ///< Alternate Function 10 function
  GPIO_FUNCMUX_11,          ///< Alternate Function 11 function
  GPIO_FUNCMUX_12,          ///< Alternate Function 12 function
  GPIO_FUNCMUX_13,          ///< Alternate Function 13 function
  GPIO_FUNCMUX_14,          ///< Alternate Function 14 function
  GPIO_FUNCMUX_15,          ///< Alternate Function 15 function
  GPIO_FUNCMUX_MAX
} GPIOFUNCMUX;

/// enumerate the IRQ pin edge
typedef enum _GPIOIRQSENSE
{
  GPIO_SENSE_NONE = 0,       ///< no sense
  GPIO_SENSE_FALL,          ///< falling edge
  GPIO_SENSE_RISE,          ///< rising edge
  GPIO_SENSE_BOTH,          ///< both edges
  GPIO_SENSE_MAX
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
  GPIOOPENDRN       eOpenDrn;       ///< open drain
  GPIOSPEED         eSpeed;         ///< speed
  GPIOPUPPDN        ePupPdn;        ///< pull up/pull down
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
  GPIOFUNCMUX       eFunc;          ///< port function
  GPIOIRQSENSE      eIrqSense;      ///< IRQ sense
  PVGPIOIRQCALLBACK pvCallback;     ///< IRQ callback
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H