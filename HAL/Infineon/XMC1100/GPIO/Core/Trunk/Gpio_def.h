/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
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
#include "Micro/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating output pins
#define GPIOPINDEF( port, pin, mode, hyst, invert, init ) \
  { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = mode, \
    .eHyst          = hyst, \
    .bInvert        = invert, \
    .bInitialState  = init \
  }

/// define the helper macro for creating function pins
#define GPIOFNCDEF( port, pin, mode, hyst ) \
 { \
    .ePort          = port, \
    .nPin           = pin, \
    .eMode          = mode, \
    .eHyst          = hyst, \
 }

// enumerations ---------------------------------------------------------------
/// enumerate the ports
typedef enum _GPIOPORT
{
  GPIO_PORT_P0 = 0,     ///< port 0
  GPIO_PORT_P1,         ///< port 1
  GPIO_PORT_P2,         ///< port 2
  GPIO_PORT_MAX
} GPIOPORT;

/// enumerate the mode
typedef enum _GPIOMODE
{
  GPIO_MODE_INP_NRMOD = 0,      ///< input normal open drain
  GPIO_MODE_INP_NRMPD,          ///< input normal pull down
  GPIO_MODE_INP_NRMPU,          ///< input normal pull up
  GPIO_MODE_OUT_NRMPP = 0x10,   ///< output push pull
  GPIO_MODE_OUT_NRAF1,          ///< output push pull aux func 1
  GPIO_MODE_OUT_NRAF2,          ///< output push pull aux func 2
  GPIO_MODE_OUT_NRAF3,          ///< output push pull aux func 3
  GPIO_MODE_OUT_NRAF4,          ///< output push pull aux func 4
  GPIO_MODE_OUT_NRAF5,          ///< output push pull aux func 5
  GPIO_MODE_OUT_NRAF6,          ///< output push pull aux func 6
  GPIO_MODE_OUT_NRAF7,          ///< output push pull aux func 7
  GPIO_MODE_OUT_NRMOD = 0x18,   ///< output open drain
  GPIO_MODE_OUT_ODAF1,          ///< output push pull aux func 1
  GPIO_MODE_OUT_ODAF2,          ///< output push pull aux func 2
  GPIO_MODE_OUT_ODAF3,          ///< output push pull aux func 3
  GPIO_MODE_OUT_ODAF4,          ///< output push pull aux func 4
  GPIO_MODE_OUT_ODAF5,          ///< output push pull aux func 5
  GPIO_MODE_OUT_ODAF6,          ///< output push pull aux func 6
  GPIO_MODE_OUT_ODAF7,          ///< output push pull aux func 7
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the hystereisis
typedef enum _GPIOHYST
{
  GPIO_HYST_STD = 0,    ///< standard
  GPIO_HYST_LRG,        ///< large
  GPIO_HYST_MAX,
} GPIOHYST;

// structures -----------------------------------------------------------------
/// define the pin definition structure
typedef struct _GPIODEF
{
  GPIOPORT          ePort;          ///< port
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  GPIOHYST          eHyst;          ///< hysteresis
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
} GPIODEF, *PGPIODEF;
#define GPIODEF_SIZE  sizeof( GPIODEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H
