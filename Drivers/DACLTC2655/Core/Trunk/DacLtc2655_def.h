/******************************************************************************
 * @file DacLtc2655_def.h
 *
 * @brief DAC LTC2655 definitions
 *
 * This file provides the definitions for the LTC2655 DAC configuration
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DacLtc2655
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DACLTC2655_DEF_H
#define _DACLTC2655_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for instantiating a DAC
#define DACLTC2655DEFM( i2cdev, addr, refsel, numbits ) \
  { \
    .eI2cDevEnum = i2cdev, \
    .eDevAddr = addr, \
    .eRefSelect = refsel, \
    .eNumBits = numbits, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the device address
typedef enum _DACLTC2655DEVADDR
{
  DACLTC2655_DEV_ADDR_10 = 0x10,      /// (GGG) base addresss 
  DACLTC2655_DEV_ADDR_11,             /// (GGF) base address + 1
  DACLTC2655_DEV_ADDR_12,             /// (GGV) base address + 2
  DACLTC2655_DEV_ADDR_13,             /// (GFG) base address + 3
  DACLTC2655_DEV_ADDR_20 = 0x20,      /// (GFF) base addresss 
  DACLTC2655_DEV_ADDR_21,             /// (GFV) base address + 1
  DACLTC2655_DEV_ADDR_22,             /// (GVG) base address + 2
  DACLTC2655_DEV_ADDR_23,             /// (GVF) base address + 3
  DACLTC2655_DEV_ADDR_30 = 0x30,      /// (GVV) base addresss 
  DACLTC2655_DEV_ADDR_31,             /// (FGG) base address + 1
  DACLTC2655_DEV_ADDR_32,             /// (FGF) base address + 2
  DACLTC2655_DEV_ADDR_33,             /// (FGV) base address + 3
  DACLTC2655_DEV_ADDR_40 = 0x40,      /// (FFG) base addresss 
  DACLTC2655_DEV_ADDR_41,             /// (FFF) base address + 1
  DACLTC2655_DEV_ADDR_42,             /// (FFV) base address + 2
  DACLTC2655_DEV_ADDR_43,             /// (FVG) base address + 3
  DACLTC2655_DEV_ADDR_50 = 0x50,      /// (FVF) base addresss 
  DACLTC2655_DEV_ADDR_51,             /// (FVV) base address + 1
  DACLTC2655_DEV_ADDR_52,             /// (VGG) base address + 2
  DACLTC2655_DEV_ADDR_53,             /// (VGF) base address + 3
  DACLTC2655_DEV_ADDR_60 = 0x60,      /// (VGV) base addresss 
  DACLTC2655_DEV_ADDR_61,             /// (VFG) base address + 1
  DACLTC2655_DEV_ADDR_62,             /// (VFF) base address + 2
  DACLTC2655_DEV_ADDR_63,             /// (VFV) base address + 3
  DACLTC2655_DEV_ADDR_70 = 0x70,      /// (VVG) base addresss 
  DACLTC2655_DEV_ADDR_71,             /// (VVF) base address + 1
  DACLTC2655_DEV_ADDR_72,             /// (VVV) base address + 2
  DACLTC2655_DEF_ADDR_GLOBAL,         /// global address
} DACLTC2655DEVADDR;

/// enumerate the reference select
typedef enum _DACLTC2655REFSEL
{
  DACLTC2655_REF_SEL_INT = 0,         /// internal reference
  DACLTC2655_REF_SEL_EXT,             /// external reference
} DACLTC2655REFSEL;

/// enumerate the resolution
typedef enum _DACLTC2655NUMBITS
{
  DACLTC2655_NUMBITS_12BITS = 0,   /// 12 bit resolution
  DACLTC2655_NUMBITS_16BITS,       /// 16 bit resolution
} DACLTC2655NUMBITS;

// structures -----------------------------------------------------------------
typedef struct _DACLTC2655DEF
{
  I2CDEVENUM        eI2cDevEnum;              ///< I2C device enumeration
  DACLTC2655DEVADDR eDevAddr;                 ///< device address
  DACLTC2655REFSEL  eRefSelect;               ///< reference select
  DACLTC2655NUMBITS eNumBits;                 ///< number of bits
} DACLTC2655DEF, *PDACLTC2655DEF;
#define DACLTC2655DEF_SIZE                sizeof( DACLTC2655DEF )

/**@} EOF DacLtc2655_def.h */

#endif  // _DACLTC2655_DEF_H