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
#define SPI_DEF_MASTER( chan, clkpol, clkphase, bitorder, pinconfig, divider, runstdby  ) \
  { \
    .eChan          = chan, \
    .eMode          = SPI_MODE_MASTER, \
    .eClkPol        = clkpol, \
    .eClkPhase      = clkphase, \
    .eBitOrder      = bitorder, \
    .ePinConfig     = pinconfig, \
    .uBaudrate      = divider, \
    .bRunInStandby  = runstdby \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the SPI channels
typedef enum _SPICHAN
{
  SPI_CHAN_0 = 0,          ///< SERCOM channel 0
  SPI_CHAN_1,              ///< SERCOM channel 1
  #ifdef SERCOM2
  SPI_CHAN_2,              ///< SERCOM channel 2
  #endif // SERCOM2
  #ifdef SERCOM3
  SPI_CHAN_3,              ///< SERCOM channel 3
  #endif // SERCOM3
  #ifdef SERCOM4
  SPI_CHAN_4,              ///< SERCOM channel 4
  #endif // SERCOM4
  #ifdef SERCOM5
  SPI_CHAN_5,              ///< SERCOM channel 5
  #endif // SERCOM5
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
} SPIBITORDER;

/// enumerate the pin configs
typedef enum _SPIPINCONFIG
{
  SPI_PINCONFIG_MISO0_MOSI2_SCLK3 = 0,
  SPI_PINCONFIG_MISO0_MOSI3_SCLK1,
  SPI_PINCONFIG_MISO1_MOSI2_SCLK3,
  SPI_PINCONFIG_MISO1_MOSI0_SCLK3,
  SPI_PINCONFIG_MISO2_MOSI0_SCLK1,
  SPI_PINCONFIG_MISO2_MOSI3_SCLK1,
  SPI_PINCONFIG_MISO2_MOSI0_SCLK3,
  SPI_PINCONFIG_MISO3_MOSI0_SCLK1,
} SPIPINCONFIG;

// structures -----------------------------------------------------------------
/// define the structure to define a SPI channel
typedef struct _SPIDEF
{
  SPICHAN       eChan;          ///< SERCOM channel
  SPIMODE       eMode;          ///< mode
  SPICLKPOL     eClkPol;        ///< clock polarity
  SPICLKPHASE   eClkPhase;      ///< clock phase
  SPIBITORDER   eBitOrder;      ///< bit order
  SPIPINCONFIG  ePinConfig;     ///< pin configuration
  U32           uBaudrate;      ///< baudrate
  BOOL          bRunInStandby;  ///< run in standby
} SPIDEF, *PSPIDEF;
#define SPIDEF_SIZE        sizeof( SPIDEF )

/**@} EOF SPI_def.h */

#endif  // _SPI_DEF_H