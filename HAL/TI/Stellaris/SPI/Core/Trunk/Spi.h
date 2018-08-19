/******************************************************************************
 * @file Spi.h
 *
 * @brief SPI declarations
 *
 * This file provides the declarations for the SPI devices
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
 
// ensure only one instantiation
#ifndef _SPI_H
#define _SPI_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _SPIERR
{
  SPI_ERR_NONE = 0,         ///< no error
  SPI_ERR_ILLDEV,           ///< illegal device
  SPI_ERR_ILLPRM,           ///< illegal parameter
  SPI_ERR_ILLPER,           ///< illegal peripheral
  SPI_ERR_ILLACT,           ///< illegal IOCTL action
  SPI_ERR_BLKING            ///< device is blocked
} SPIERR;

/// enumerate the IOCTL actions
typedef enum _SPIACTION
{
  SPI_ACTION_NONE = 0,      ///< no action
  SPI_ACTION_SET_DEVADDR,   ///< set device address
  SPI_ACTION_GET_DEVADDR,   ///< get device address
  SPI_ACTION_MAX            ///< maximum action selection
} SPIACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  SPIERR Spi_Initialize( SPIDEVENUM eDev  );
extern  SPIERR Spi_Write( SPIDEVENUM eDev, PU8 pnData, U8 nXmtlen );
extern  SPIERR Spi_Read( SPIDEVENUM eDev, PU8 pnData, U8 nXmtLen, U8 nRcvLen );
extern  SPIERR Spi_Ioctl( SPIDEVENUM eDev, SPIACTION eAction, PVOID pvData );
extern  SPIERR Spi_Close( SPIDEVENUM eDev );

/**@} EOF Spi.h */

#endif  // _SPI_H