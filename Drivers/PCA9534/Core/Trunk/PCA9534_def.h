/******************************************************************************
 * @file PCA9534_def.h
 *
 * @brief PCA9534 port expander driver definitions 
 *
 * This file provides the definitions for the PCA 9534
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup PCA9534
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PCA9534_DEF_H
#define _PCA9534_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"
#include "I2C/I2c.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the helper macro for creating a device
#define DEFPCA9534DEVM( i2cdev, addr, dir0, inv0, init0, dir1, inv1, init1, dir2, inv2, init2, dir3, inv3, init3, dir4, inv4, init4, dir5, inv5, init5, dir6, inv6, init6, dir7, inv7, init7  ) \
  { \
    .eI2cDevEnum    = i2cdev, \
    .eDeviceAddress = addr, \
    .atPinDefs[ 0 ] = \
    { \
      .eDirection   = dir0, \
      .bInvert      = inv0, \
      .bInitial     = init0 \
    }, \
    .atPinDefs[ 1 ] = \
    { \
      .eDirection   = dir1, \
      .bInvert      = inv1, \
      .bInitial     = init1 \
    }, \
    .atPinDefs[ 2 ] = \
    { \
      .eDirection   = dir2, \
      .bInvert      = inv2, \
      .bInitial     = init2 \
    }, \
    .atPinDefs[ 3 ] = \
    { \
      .eDirection   = dir3, \
      .bInvert      = inv3, \
      .bInitial     = init3 \
    }, \
    .atPinDefs[ 4 ] = \
    { \
      .eDirection   = dir4, \
      .bInvert      = inv4, \
      .bInitial     = init4 \
    }, \
    .atPinDefs[ 5 ] = \
    { \
      .eDirection   = dir5, \
      .bInvert      = inv5, \
      .bInitial     = init5 \
    }, \
    .atPinDefs[ 6 ] = \
    { \
      .eDirection   = dir6, \
      .bInvert      = inv6, \
      .bInitial     = init6 \
    }, \
    .atPinDefs[ 7 ] = \
    { \
      .eDirection   = dir7, \
      .bInvert      = inv7, \
      .bInitial     = init7 \
    }, \
  } \

// enumerations ---------------------------------------------------------------
/// enumerate the direction
typedef enum _PCA9534DIRENUM
{
  PCA9534_DIR_OUT = 0,
  PCA9534_DIR_IN,
  PCA9534_DIR_MAX
} PCA9534DIRENUM;

/// enumerate the device addresses
typedef enum  _PCA9534DEVADDR
{
  PCA9534_DEV_ADDR_20 = 0x20,     ///< base device
  PCA9534_DEV_ADDR_21,            ///< base device + 1
  PCA9534_DEV_ADDR_22,            ///< base device + 2
  PCA9534_DEV_ADDR_23,            ///< base device + 3
  PCA9534_DEV_ADDR_24,            ///< base device + 4
  PCA9534_DEV_ADDR_25,            ///< base device + 5
  PCA9534_DEV_ADDR_26,            ///< base device + 6
  PCA9534_DEV_ADDR_27,            ///< base device + 7
} PCA9534DEVADDR;

/// enuemrate the pins
typedef enum _PCA9534PINENUM
{
  PCA9534_PIN_ENUM_0 = 0,       ///< pin 0
  PCA9534_PIN_ENUM_1,           ///< pin 1
  PCA9534_PIN_ENUM_2,           ///< pin 2
  PCA9534_PIN_ENUM_3,           ///< pin 3
  PCA9534_PIN_ENUM_4,           ///< pin 4
  PCA9534_PIN_ENUM_5,           ///< pin 5
  PCA9534_PIN_ENUM_6,           ///< pin 6
  PCA9534_PIN_ENUM_7,           ///< pin 7
  PCA9534_PIN_ENUM_MAX
} PCA9534PINENUM;

// structures -----------------------------------------------------------------
/// define the pin defnition structure
typedef struct _PCA9534PINDEF
{
  PCA9534DIRENUM    eDirection;     ///< direction
  BOOL              bInvert;        ///< invert
  BOOL              bInitial;       ///< initial state
} PCA9534PINDEF, *PPCA9534PINDEF;
#define PCA9534PINDEF_SIZE         sizeof( PCA9534PINDEF )

/// define the device deinition structure
typedef struct _PCA9534DEVDEF
{
  I2CDEVENUM      eI2cDevEnum;                    ///< I2C device enumeration
  PCA9534DEVADDR  eDeviceAddress;                 ///< device address
  PCA9534PINDEF   atPinDefs[ PCA9534_PIN_ENUM_MAX ];  ///< pin definitions
} PCA9534DEVDEF, *PPCA9534DEVDEF;
#define PCA9534DEVDEF_SIZE          sizeof( PCA9534DEVDEF )

/**@} EOF PCA9534_def.h */

#endif  // _PCA9534_DEF_H