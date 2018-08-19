/******************************************************************************
 * @file I2C_def.h
 *
 * @brief I2C definitions
 *
 * This file contains the I2C configuration definitions
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _I2C_DEF_H
#define _I2C_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// stellarisware library includes
#include "driverlib/gpio.h"
//#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a UART device
#define I2C_DEVICE_NONBLK( devper, gpioper, devbase, gpiobase, sdapin, sclpin, sdafunc, sclfunc, fastmode, callback ) \
                  { .uDevPeriph = devper, \
                    .uGpioPeriph = gpioper, \
                    .uDevBase = devbase, \
                    .uGpioBase = gpiobase, \
                    .nSdaPin = sdapin, \
                    .nSclPin = sclpin, \
                    .uSdaPinFunc = sdafunc, \
                    .uSclPinFunc = sclfunc, \
                    .bFastMode = fastmode, \
                    .pvCallBack = ( PVI2CINTCALLBACK )callback \
                  }
#define I2C_DEVICE_BLKING( devper, gpioper, devbase, gpiobase, sdapin, sclpin, sdafunc, sclfunc, fastmode ) \
                  { .uDevPeriph = devper, \
                    .uGpioPeriph = gpioper, \
                    .uDevBase = devbase, \
                    .uGpioBase = gpiobase, \
                    .nSdaPin = sdapin, \
                    .nSclPin = sclpin, \
                    .uSdaPinFunc = sdafunc, \
                    .uSclPinFunc = sclfunc, \
                    .bFastMode = fastmode, \
                    .pvCallBack = NULL \
                  }

// enumerations ---------------------------------------------------------------
typedef enum _I2CIRQEVENTS
{
  I2C_IRQ_EVENT_RXCHAR,              ///< receive character event
} I2CIRQEVENTS;

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVI2CINTCALLBACK )( I2CIRQEVENTS eEvent );

/// define the I2C definition structure
typedef struct _I2CDEF
{
  U32               uDevPeriph;     ///< device peripheral
  U32               uGpioPeriph;    ///< GPIO peripheral
  U32               uDevBase;       ///< base address of the device
  U32               uGpioBase;      ///< base address of the GPIO
  U8                nSdaPin;        ///< SDA pin
  U8                nSclPin;        ///< SCL pin
  U32               uSdaPinFunc;    ///< SDA pin function
  U32               uSclPinFunc;    ///< SCL pin function
  BOOL              bFastMode;      ///< fast mode
  PVI2CINTCALLBACK  pvCallBack;     ///< callback function
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE sizeof( I2CDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF I2C_def.h */

#endif  // _I2C_DEF_H