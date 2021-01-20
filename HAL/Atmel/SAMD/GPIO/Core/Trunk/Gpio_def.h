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
#define GPIOPINDEF( port, pin, mode, hidrive, invert, init ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = mode, \
    .bHiDriveEnb    = hidrive, \
    .bInvert        = invert, \
    .bInitialState  = init \
  }

/// helper macro for creating a function output pin  
#define GPIOFNCDEF( port, pin, func, allowinp, pullenb ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eFunc          = func, \
    .bAllowInput    = allowinp, \
    .bPullUpEnable  = pullenb, \
  }
  
/// define the helper macro for creating an interrupt pin
#define GPIOIRQDEF( port, pin, pupdn, sense, wakeup, filter, callback, event, initon ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .ePupdn         = pupdn, \
    .eSense         = sense, \
    .bWakeupEnb     = wakeup, \
    .bFilterEnb     = filter, \
    .pvCallback     = callback, \
    .nEvent         = event, \
    .bEventType     = FALSE, \
    .bInitOn        = initon \
  }

/// define the helper macro for creating an event pin
#define GPIOEVNDEF( port, pin, pupdn, sense, wakeup, filter ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .ePupdn         = pupdn, \
    .eSense         = sense, \
    .bWakeupEnb     = wakeup, \
    .bFilterEnb     = filter, \
    .pvCallback     = NULL, \
    .bEventType     = TRUE \
  }

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

/// enumerate the IRQ pin edge
typedef enum _GPIOIRQSENSE
{
  GPIO_SENSE_NONE = 0,       ///< no sense
  GPIO_SENSE_RISE,          ///< rising edge
  GPIO_SENSE_FALL,          ///< falling edge
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

/// enumerate the GPIO IRQ pullup modes
typedef enum _GPIOIRQPUPDN
{
  GPIO_IRQPUPDN_NONE = 0,   ///< no pull up/down
  GPIO_IRQPUPDN_PULLDN,     ///< pull down
  GPIO_IRQPUPDN_PULLUP,     ///< pull up
  GPIO_IRQPUPDN_MAX
} GPIOIRQPUPDN;

// structures -----------------------------------------------------------------
/// define the callback
typedef void ( *PVGPIOIRQCALLBACK )( U8 nIrq, U8 nEvent, BOOL bState );

/// define the pin definition structure
typedef struct _GPIOPINDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  BOOL              bHiDriveEnb;    ///< high drive enable
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/// define the pin definition structure
typedef struct _GPIOFNCDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOFUNCMUX       eFunc;          ///< port function
  BOOL              bAllowInput;    ///< allow pin to be an input
  BOOL              bPullUpEnable;  ///< pull up enable
} GPIOFNCDEF, *PGPIOFNCDEF;
#define GPIOFNCDEF_SIZE  sizeof( GPIOFNCDEF )

/// define the pin definition structure
typedef struct _GPIOIRQDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOIRQPUPDN      ePupdn;         ///< pull up/down
  GPIOIRQSENSE      eSense;         ///< sense
  BOOL              bWakeupEnb;     ///< wakeup enable
  BOOL              bFilterEnb;     ///< filter enable
  PVGPIOIRQCALLBACK pvCallback;     ///< callback
  U8                nEvent;         ///< callback event
  BOOL              bEventType;     ///< event type
  BOOL              bInitOn;        ///< initialial on
} GPIOIRQDEF, *PGPIOIRQDEF;
#define GPIOIRQDEF_SIZE  sizeof( GPIOIRQDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H