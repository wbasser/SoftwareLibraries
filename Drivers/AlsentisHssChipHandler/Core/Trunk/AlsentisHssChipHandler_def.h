/******************************************************************************
 * @file AlsentisHssCHipHandler_def.h
 *
 * @brief Alsentis HSS touch chip handler defitinons
 *
 * This file provides the declarations for the Alsentis HSS touch controller
 * chip handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AlsentisHssCHipHandler_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALSENTISHSSCHIPHANDLER_DEF_H
#define _ALSENTISHSSCHIPHANDLER_DEF_H

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining the 
#define ALSENTISHSSCHIPDEFM( i2cenum, address, numsens, gpio ) \
  { \
    .eI2cDevEnum = i2cenum, \
    .nAddress = address, \
    .nNumSensors = numsens, \
    .eGpioIrq = gpio, \
  }
  
// structures -----------------------------------------------------------------
/// define the structure for the definition of an ALsentis HSS Touch controller
typedef struct _ALSENTISHSSCHIPDEF
{
  I2CDEVENUM  eI2cDevEnum;      ///< I2C device enumeration
  U8          nAddress;         ///< device address
  U8          nNumSensors;      ///< number senseors
  GPIOPINENUM eGpioIrq;         ///< GPIO irq pin enum
} ALSENTISHSSCHIPDEF, *PALSENTISHSSCHIPDEF;
#define ALSENTISHSSCHIPDEF_SIZE          sizeof( ALSENTISHSSCHIPDEF )

/**@} EOF AlsentisHssCHipHandler_def.h */

#endif  // _ALSENTISHSSCHIPHANDLER_DEF_H