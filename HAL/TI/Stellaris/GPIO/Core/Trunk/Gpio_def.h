/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO definitions
 *
 * This file contains the GPIO configuration definitions
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup GPIO
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _GPIO_DEF_H
#define _GPIO_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// stellarisware library includes
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for input pins
#define GPIO_PIN_INP( periph, base, pin, invert, mode ) \
                    { .uPeripheral = periph,\
                      .uPortBase = base, \
                      .nPin = pin, \
                      .bInvert = invert, \
                      .ePinDir = GPIO_PIN_DIR_IN, \
                      .ePinMode = mode, \
                      .ePinStrength = GPIO_PIN_STRENGTH_2MA, \
                      .eIntType = GPIO_PIN_INT_TYPE_NONE, \
                      .pvCallBack = NULL, \
                    }
  
/// define the helper macro for output pins
#define GPIO_PIN_OUT( periph, base, pin, invert, initial, mode, strength ) \
                    { .uPeripheral = periph, \
                      .uPortBase = base, \
                      .nPin = pin, \
                      .bInvert = invert, \
                      .bInitial = initial, \
                      .ePinDir = GPIO_PIN_DIR_OUT, \
                      .ePinMode = mode, \
                      .ePinStrength = strength, \
                      .eIntType = GPIO_PIN_INT_TYPE_NONE, \
                      .pvCallBack = NULL \
                    }
  
/// define the helper macro for interrupt pins
#define GPIO_PIN_INT( periph, base, pin, invert, mode, inttype, callback ) \
                    { .uPeripheral = periph, \
                      .uPortBase = base, \
                      .nPin = pin, \
                      .bInvert = invert, \
                      .ePinDir = GPIO_PIN_DIR_IN, \
                      .ePinMode = mode, \
                      .ePinStrength = GPIO_PIN_STRENGTH_2MA, \
                      .eIntType = inttype, \
                      .pvCallBack = ( PVGPIOINTCALLBACK )callback \
                    }

// enumerations ---------------------------------------------------------------
/// enumerate the pin direction
typedef enum _GPIOPINDIR
{
  GPIO_PIN_DIR_IN  = GPIO_DIR_MODE_IN,
  GPIO_PIN_DIR_OUT = GPIO_DIR_MODE_OUT,
} GPIOPINDIR;

/// enumerate the pin type
typedef enum _GPIOPINMODE
{
  GPIO_PIN_MODE_STD   = GPIO_PIN_TYPE_STD,
  GPIO_PIN_MODE_SWPU  = GPIO_PIN_TYPE_STD_WPU,
  GPIO_PIN_MODE_SWPD  = GPIO_PIN_TYPE_STD_WPD,
  GPIO_PIN_MODE_OD    = GPIO_PIN_TYPE_OD,
  GPIO_PIN_MODE_ODPU  = GPIO_PIN_TYPE_OD_WPU,
  GPIO_PIN_MODE_ODPD  = GPIO_PIN_TYPE_OD_WPD,
  GPIO_PIN_MODE_ANA   = GPIO_PIN_TYPE_ANALOG
} GPIOPINMODE;

/// enumerate the pin strength
typedef enum _GPIOPINSTRENGTH
{
  GPIO_PIN_STRENGTH_2MA   = GPIO_STRENGTH_2MA,
  GPIO_PIN_STRENGTH_4MA   = GPIO_STRENGTH_4MA,
  GPIO_PIN_STRENGTH_8MA   = GPIO_STRENGTH_8MA,
  GPIO_PIN_STRENGTH_8MASC = GPIO_STRENGTH_8MA_SC,
} GPIOPINSTRENGTH;

/// enumerate the interrupt type
typedef enum  _GPIOINTTYPE
{
  GPIO_PIN_INT_TYPE_FALLEDGE  = GPIO_FALLING_EDGE,
  GPIO_PIN_INT_TYPE_RISEEDGE  = GPIO_RISING_EDGE,
  GPIO_PIN_INT_TYPE_BOTHEDGES = GPIO_BOTH_EDGES,
  GPIO_PIN_INT_TYPE_LOLEVEL   = GPIO_LOW_LEVEL,
  GPIO_PIN_INT_TYPE_HILEVEL   = GPIO_HIGH_LEVEL,
  GPIO_PIN_INT_TYPE_NONE      = 0xFF,
} GPIOINTTYPE;

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVGPIOINTCALLBACK )( void );

/// define the GPIO definition structure
typedef struct _GPIODEF
{
  U32               uPeripheral;    ///< peripheral
  U32               uPortBase;      ///< base address of the GPIO Port
  U8                nPin;           ///< pin
  BOOL              bInvert;        ///< invert
  BOOL              bInitial;       ///< initial state
  GPIOPINDIR        ePinDir;        ///< pin direction                                     
  GPIOPINMODE       ePinMode;       ///< pin type
  GPIOPINSTRENGTH   ePinStrength;   ///< pin strength
  GPIOINTTYPE       eIntType;       ///< interrupt mode
  PVGPIOINTCALLBACK pvCallBack;     ///< callback function
  
} GPIODEF, *PGPIODEF;
#define GPIODEF_SIZE sizeof( GPIODEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_Hd