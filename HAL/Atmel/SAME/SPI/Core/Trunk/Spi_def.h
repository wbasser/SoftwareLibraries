/******************************************************************************
 * @file Spi_def.h
 *
 * @brief SPI definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * SPI channels
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
 * \addtogroup Spi_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SPI_DEF_H
#define _SPI_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a SPI channel
#define SPI_DEF_MASTER( chan, clkpol, clkphase, baudrate, numbits, chipsel ) \
  { \
    .eChan      = chan, \
    .eMode      = SPI_MODE_MASTER, \
    .eClkPol    = clkpol, \
    .eClkPhase  = clkphase, \
    .uBaudRate  = baudrate, \
    .eNumBits   = numbits, \
    .eChipSel   = chipsel, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the SPI channels
typedef enum _SPICHAN
{
  SPI_CHAN_0 = 0,          ///< SERCOM channel 0
  SPI_CHAN_1,              ///< SERCOM channel 1
  SPI_CHAN_MAX
} SPICHAN;

/// enumerate the mode
typedef enum _SPIMODE
{
  SPI_MODE_SLAVE = 0,
  SPI_MODE_MASTER,
  SPI_MODE_MAX
} SPIMODE;

/// enumerate the clock polarity
typedef enum _SPICLKPOL
{
  SPI_CLKPOL_RISING = 0,
  SPI_CLKPOL_FALLING,
  SPI_CLKPOL_MAX
} SPICLKPOL;

/// enumerate the clock phase
typedef enum _SPICLKPHASE
{
  SPI_CLKPHASE_LEADING = 0,
  SPI_CLKPHASE_TRAILING,
  SPI_CLKPHASE_MAX
} SPICLKPHASE;

/// enumerate the bit count
typedef enum _SPIBITNUM
{
  SPI_BITNUM_8 = 0,
  SPI_BITNUM_9,
  SPI_BITNUM_10,
  SPI_BITNUM_11,
  SPI_BITNUM_12,
  SPI_BITNUM_13,
  SPI_BITNUM_14,
  SPI_BITNUM_15,
  SPI_BITNUM_16,
  SPI_BITNUM_MAX
} SPIBITNUM;

/// enumerate the chip selects
typedef enum _SPICHIPSEL
{
    SPI_CHIPSEL_0 = 0,
    SPI_CHIPSEL_1,
    SPI_CHIPSEL_2,
    SPI_CHIPSEL_3,
    SPI_CHIPSEL_MAX
} SPICHIPSEL;

// structures -----------------------------------------------------------------
/// define the structure to define a SPI channel
typedef struct _SPIDEF
{
  SPICHAN       eChan;          ///< SPI channel
  SPIMODE       eMode;          ///< mode
  SPICLKPOL     eClkPol;        ///< clock polarity
  SPICLKPHASE   eClkPhase;      ///< clock phase
  U32           uBaudRate;      ///< baud rate
  SPIBITNUM     eNumBits;       ///< number of bits
  SPICHIPSEL    eChipSel;       ///< chip select
} SPIDEF, *PSPIDEF;
#define SPIDEF_SIZE        sizeof( SPIDEF )

/**@} EOF SPI_def.h */
#endif  // _SPI_DEF_H