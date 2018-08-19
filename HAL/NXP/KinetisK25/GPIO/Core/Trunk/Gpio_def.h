/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
 *
 * @copyright Copyright (c) 2012 Guardhat
 * This document contains proprietary data and information of Guardhat 
 * LLC. It is the exclusive property of Guardhat, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Guardhat, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Guardhat, LLC.
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
#define GPIOPINDEF( port, pin, direction, function, hidrive, invert, init ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eDir           = direction, \
    .eFunc          = function, \
    .bHiDriveEnb    = hidrive, \
    .bInvert        = invert, \
    .bInitialState  = init \
  }

/// define the helper macro for creating an interrupt pin
#define GPIOIRQDEF( port, pin, direction, function, sense, filter, callback, event ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eDir           = direction, \
    .eFunc          = function, \
    .eSense         = sense, \
    .bFilterEnb     = filter, \
    .pvCallback     = callback, \
    .nEvent         = event \
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
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the pin multiplexing
typedef enum _GPIOFUNCMUX
{
  GPIO_FUNCMUX_A = 0,       ///< Analog
  GPIO_FUNCMUX_1,           ///< GPIO
  GPIO_FUNCMUX_2,           ///< Alternate #2
  GPIO_FUNCMUX_3,           ///< Alternate #3
  GPIO_FUNCMUX_4,           ///< Alternate #4
  GPIO_FUNCMUX_5,           ///< Alternate #5
  GPIO_FUNCMUX_6,           ///< Alternate #6
  GPIO_FUNCMUX_7,           ///< Alternate #7
  GPIO_FUNCMUX_MAX
} GPIOFUNCMUX;

/// enumerate the IRQ pin edge
typedef enum _GPIOIRQSENSE
{
  GPIO_SENSE_NONE = 0,      ///< no sense
  GPIO_SENSE_DMA_RISE,      ///< DMA request - rising edge
  GPIO_SENSE_DMA_FALL,      ///< DMA reqeust - falling edge
  GPIO_SENSE_DMA_BOTH,      ///< DMA request - both edges
  GPIO_SENSE_IRQ_LOLVL,     ///< IRQREQ LO level
  GPIO_SENSE_IRQ_FALL,      ///< falling edge
  GPIO_SENSE_IRQ_RISE,      ///< rising edge
  GPIO_SENSE_IRQ_BOTH,      ///< both edges
  GPIO_SENSE_IRQ_HILVL,     ///< HI level
  GPIO_EDGE_MAX
} GPIOIRQSENSE;

/// enumerate the GPIO pin direction
typedef enum _GPIODIR
{
  GPIO_DIR_IN = 0,          ///< input
  GPIO_DIR_OUT,             ///< output
  GPIO_DIR_MAX          
} GPIODIR;

/// enumrate the GPIO

// structures -----------------------------------------------------------------
/// define the callback
typedef void ( *PVGPIOIRQCALLBACK )( U8, U8 );

/// define the pin definition structure
typedef struct _GPIOPINDEF
{
  GPIOPORT          ePort;          ///< port selection
  U8                nPin;           ///< port pin
  GPIOIRQSENSE      eIrqSense;      ///< irq sense
  GPIOFUNCMUX       eFunc;          ///< port function
  GPIODIR           eDir;           ///< direction
  BOOL              bHiDriveEnb;    ///< high drive enable
  BOOL              bFilterEnb;     ///< passive filter enable
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
  BOOL              bPullEnable;    ///< enable pullup/pulldown
  BOOL              bPullDir;       ///< pulldown/pullup selection
  PVGPIOIRQCALLBACK pvCallback;     ///< callback
  U8                nEvent;         ///< callback event
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H