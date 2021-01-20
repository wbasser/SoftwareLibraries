/******************************************************************************
 * @file Spi.h
 *
 * @brief SPI module declarations 
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
 * \addtogroup SPI
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SPI_H
#define _SPI_H

// system includes
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _SPIERR
{
  SPI_ERR_NONE = 0,         ///< no error
  SPI_ERR_ILLDEV = 0xE0,    ///< illegal device
} SPIERR;

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

// enumerate the SPI modes
typedef enum _SPIMODE
{
  SPI_MODE_0 = 0,
  SPI_MODE_1,
  SPI_MODE_2,
  SPI_MODE_3,
  SPI_MODE_MAX
} SPIMODE;

/// enuemrate the bit order
typedef enum _SPIBITORDER
{
  SPI_BITORDER_MSB = 0,
  SPI_BITORDER_LSB
} SPIBITORDER;

/// enumerate the MISO pin select
typedef enum _SPIMISOSEL
{
  SPI_MISOSEL_0 = 0,
  SPI_MISOSEL_1,
  SPI_MISOSEL_2,
  SPI_MISOSEL_3,
  SPI_MISOSEL_MAX
} SPIMISOSEL;

/// enumerate the MOSI/SCLK/SS pin select
typedef enum _SPIMOSISCLKSEL
{
  SPI_MOSISCLKSEL_MOSI0_SCLK1 = 0,
  SPI_MOSISCLKSEL_MOSI2_SCLK3,
  SPI_MOSISCLKSEL_MOSI3_SCLK1,
  SPI_MOSISCLKSEL_MOSI0_SCLK3,
  SPI_MOSISCLKSEL_MAX,
} SPIMOSISCLKSEL;

// structures -----------------------------------------------------------------
/// define the task handle
typedef PVOID   PVSPIHANDLE;

/// define the structure for defining the SPI
typedef struct _SPIDEF
{
  SPICHAN         eChan;          ///< channel
  SPIMODE         eMode;          ///< mode
  SPIBITORDER     eBitOrder;      ///< bit order
  SPIMISOSEL      eMisoSel;       ///< MISO select
  SPIMOSISCLKSEL  eMosiSclkSel;   ///< pin select
  U32             uBaudRate;      ///< baud rate
  GPIOPORT        eDevPort;       ///< device port
  GPIOFUNCMUX     eDevMux;        ///< device mux
  U8              nMosiPin;       ///< mosi pin
  U8              nMisoPin;       ///< miso pin
  U8              nSclkPin;       ///< sclk pin
} SPIDEF, *PSPIDEF;
#define SPIDEF_SIZE               sizeof( SPIDEF )

// global function prototypes --------------------------------------------------
extern  PVSPIHANDLE Spi_Configure( PSPIDEF ptDef );
extern  SPIERR      Spi_Write( PVSPIHANDLE pvHandle, PU8 pnBuffer, U16 wSize, BOOL bStoreRead );
extern  SPIERR      Spi_Read( PVSPIHANDLE pvHandle, U8 nOutData, PU8 pnBuffer, U16 wSize );

/**@} EOF Spi.h */

#endif  // _SPI_H