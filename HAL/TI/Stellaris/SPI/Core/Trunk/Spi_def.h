/******************************************************************************
 * @file SPI_def.h
 *
 * @brief SPI definitions
 *
 * This file contains the SPI configuration definitions
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
 * \addtogroup SPI
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _SPI_DEF_H
#define _SPI_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// stellarisware library includes
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a UART device
#define SPI_DEVICE( devper, gpioper, devbase, gpiobase, sclkpin, mosipin, misopin, sspin, sclkfunc, mosifunc, misofunc, ssfunc ) \
                  { .uDevPeriph = devper, \
                    .uGpioPeriph = gpioper, \
                    .uDevBase = devbase, \
                    .uGpioBase = gpiobase, \
                    .nSclkPin = sclkpin, \
                    .nMosiPin = mosipin, \
                    .nMisoPin = misopin, \
                    .uSclkPinFunc = sclkfunc, \
                    .uMosiPinFunc = mosifunc, \
                    .uMisoPinFunc = misofunc, \
                    .bFastMode = fastmode, \
                    .pvCallBack = ( PVSPIINTCALLBACK )callback \
                  }

// enumerations ---------------------------------------------------------------
typedef enum _SPIIRQEVENTS
{
  SPI_IRQ_EVENT_RXCHAR,              ///< receive character event
} SPIIRQEVENTS;

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVSPIINTCALLBACK )( SPIIRQEVENTS eEvent );

/// define the SPI definition structure
typedef struct _SPIDEF
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
  PVSPIINTCALLBACK  pvCallBack;     ///< callback function
} SPIDEF, *PSPIDEF;
#define SPIDEF_SIZE sizeof( SPIDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF SPI_def.h */

#endif  // _SPI_DEF_H