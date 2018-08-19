/******************************************************************************
 * @file A2d_def.h
 *
 * @brief A2D definition declarations
 *
 * This file 
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
 * $Log: $
 * 
 *
 * \addtogroup A2D
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _A2D_DEF_H
#define _A2D_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// stellarisware library includes
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/a2d.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a A2D channel
#define A2D_DEVICE( devper, gpioper, devbase, gpiobase, pin, channel ) \
                  { .uDevPeriph = devper, \
                    .uGpioPeriph = gpioper, \
                    .uDevBase = devbase, \
                    .uGpioBase = gpiobase, \
                    .nPin = pin, \
                    .nChannel = channel \
                  }             

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the A2D definition strutcure
typedef struct _A2DDEF
{
  U32 uDevPeriph;     ///< device peripheral
  U32 uGpioPeriph;    ///< GPIO peripheral
  U32 uDevBase;       ///< base address of the device
  U32 uGpioBase;      ///< base address of the GPIO
  U8  nPin;           ///< pin
  U8  nChannel;       ///< channel
} A2DDEF, *PA2DDEF;
#define A2DDEF_SIZE sizeof( A2DDEF )

/**@} EOF A2d_def.h */

#endif  // _A2D_DEF_H