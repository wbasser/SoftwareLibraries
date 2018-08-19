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

// enumerations ---------------------------------------------------------------
/// enumerate the SPI errors
typedef enum _SPIERR
{
  SPI_ERR_NONE = 0,    ///< no error
  SPI_ERR_ILLACT,      ///< illegal IOCTL action
  SPI_ERR_ILLMODE,     ///< illegal mode
  SPI_ERR_BUFFAIL,     ///< no room for buffer
  SPI_ERR_COLLISION,   ///< bus collision
} SPIERR;

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

/// enumerate the clock prescale
typedef enum _SPICLKPRESCALE
{
  SPI_CLKPRESCALE_4     = 0,
  SPI_CLKPRESCALE_16,
  SPI_CLKPRESCALE_64,
  SPI_CLKPRESCALE_128,
  SPI_CLKPRESCALE_2,
  SPI_CLKPRESCALE_8,
  SPI_CLKPRESCALE_32,
  SPI_CLKPRESCLAE_MAX
} SPICLKPRESCALE;

/// enumerate the IOCTL actions
typedef enum _SPIIOCTLACTIONS
{
  SPI_IOCTLACTIONS_SETPOL = 0,
  SPI_IOCTLACTIONS_SETPHA,
  SPI_IOCTLACTIONS_SETSPEED,
  SPI_IOCTLACTION_ENABLEDISABLE,
  SPI_IOCTLACTIONS_MAX
} SPIIOCTLACTIONS;

/// enuemrate the bit order
typedef enum _SPIBITORDER
{
  SPI_BITORDER_MSB = 0,
  SPI_BITORDER_LSB
} SPIBITORDER;


// global function prototypes --------------------------------------------------
extern  void    Spi_Initialize( void );
extern  void    Spi_Write( U8 nData );
extern  U8      Spi_Read( U8 nData );
extern  void    Spi_WriteBlock( PU8 pnBuffer, U16 wSize, BOOL bStoreRead );
extern  void    Spi_WriteBlockP( PFU8 pnBuffer, U16 wSize, BOOL bStoreRead );
extern  void    Spi_ReadBlock( U8 nOutData, PU8 pnBuffer, U16 wSize );
extern  SPIERR  Spi_Ioctl( SPIIOCTLACTIONS eAction, PVOID pvValue );
extern  void    Spi_Close( void );

/**@} EOF Spi.h */

#endif  // _SPI_H