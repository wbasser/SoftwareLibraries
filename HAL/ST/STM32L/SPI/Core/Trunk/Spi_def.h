/******************************************************************************
 * @file Spi_def.h
 *
 * @brief SPI definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * SPI channels
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
 * \addtogroup Spi_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SPI_DEF_H
#define _SPI_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a SPI channel
#define SPI_DEF_MASTER( chan, prescale, clkpol, clkphase, bitorder, datasize, ssmode  ) \
  { \
    .eChan          = chan, \
    .eprescale      = prescale, \
    .eMode          = SPI_MODE_MASTER, \
    .eClkPol        = clkpol, \
    .eClkPhase      = clkphase, \
    .eBitOrder      = bitorder, \
    .eDataSize      = datasize, \
    .eCrcMode       = SPI_CRCMODE_NONE, \
    .eSsMode        = ssmode, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the SPI channels
typedef enum _SPICHAN
{
  SPI_CHAN_1 = 0,          ///< SPI channel 1
  SPI_CHAN_2,              ///< SPI channel 2
  SPI_CHAN_3,              ///< SPI channel 3
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

/// enuemrate the bit order
typedef enum _SPIBITORDER
{
  SPI_BITORDER_MSB = 0,
  SPI_BITORDER_LSB
  SPI_BITORDER_MAX
} SPIBITORDER;

/// enumerate the prescale
typedef enum _SPIPRESCALE
{
  SPI_PRESCALE_2 = 0,       ///< prescale of 2
  SPI_PRESCALE_4,           ///< prescale of 4
  SPI_PRESCALE_8,           ///< prescale of 8
  SPI_PRESCALE_16,          ///< prescale of 16
  SPI_PRESCALE_32,          ///< prescale of 32
  SPI_PRESCALE_64,          ///< prescale of 64
  SPI_PRESCALE_128,         ///< prescale of 128
  SPI_PRESCALE_256,         ///< prescale of 256
  SPI_PRESCALE_MAX
} SPIPRESCALE;

/// enumerate the data size
typedef enum _SPIDATASIZE
{
  SPI_DATASIZE_4 = 3,       ///< data size 4
  SPI_DATASIZE_5,           ///< data size 5
  SPI_DATASIZE_6,           ///< data size 6
  SPI_DATASIZE_7,           ///< data size 7
  SPI_DATASIZE_8,           ///< data size 8
  SPI_DATASIZE_9,           ///< data size 9
  SPI_DATASIZE_10,          ///< data size 10
  SPI_DATASIZE_11,          ///< data size 11
  SPI_DATASIZE_12,          ///< data size 12
  SPI_DATASIZE_13,          ///< data size 13
  SPI_DATASIZE_14,          ///< data size 14
  SPI_DATASIZE_15,          ///< data size 15
  SPI_DATASIZE_16,          ///< data size 16
  SPI_DATASIZE_MAX
} SPIDATASIZE;

/// enumerate the CRC mode
typedef enum _SPICRCMODE
{
  SPI_CRCMODE_NONE = 0,     ///< none
  SPI_CRCMODE_8BIT,         ///< CRC enbled, 8 bit
  SPI_CRCMODE_16BIT,        ///< CRC enabled, 16 bit
  SPI_CRCMODE_MAX
} SPICRCMODE;

/// enumerate the SS mode
typedef enum _SPISSMODE
{
  SPI_SSMODE_NONE = 0,      ///< none
  SPI_SSMODE_ENBNOP,        ///< enabled, no pulse
  SPI_SSMODE_ENBPUL,        ///< enabled pulse
} SPISSMODE;

// structures -----------------------------------------------------------------
/// define the structure to define a SPI channel
typedef struct _SPIDEF
{
  SPICHAN       eChan;          ///< SPI channel
  SPIMODE       eMode;          ///< mode
  SPICLKPOL     eClkPol;        ///< clock polarity
  SPICLKPHASE   eClkPhase;      ///< clock phase
  SPIBITORDER   eBitOrder;      ///< bit order
  SPIPRESCALE   ePrescale;      ///< prescale
  SPIDATASIZE   eDataSize;      ///< data size
  SPICRCMODE    eCrcMode;       ///< crc mode
  SPISSMODE     eSsMode;        ///< SS mode
} SPIDEF, *PSPIDEF;
#define SPIDEF_SIZE        sizeof( SPIDEF )

/**@} EOF SPI_def.h */

#endif  // _SPI_DEF_H