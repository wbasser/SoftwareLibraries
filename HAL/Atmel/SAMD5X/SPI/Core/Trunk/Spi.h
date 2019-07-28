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

// local includes -------------------------------------------------------------
#include "SPI/Spi_cfg.h"

// enumerations ---------------------------------------------------------------
/// enumerate the SPI errors
typedef enum _SPIERR
{
  SPI_ERR_NONE = 0,    ///< no error
  SPI_ERR_ILLDEV,      ///, illegal device
  SPI_ERR_ILLACT,      ///< illegal IOCTL action
  SPI_ERR_ILLMODE,     ///< illegal mode
  SPI_ERR_BUFFAIL,     ///< no room for buffer
  SPI_ERR_COLLISION,   ///< bus collision
} SPIERR;

/// enumerate the IOCTL actions
typedef enum _SPIIOCTLACTIONS
{
  SPI_IOCTLACTIONS_SETPOL = 0,
  SPI_IOCTLACTIONS_SETPHA,
  SPI_IOCTLACTIONS_SETSPEED,
  SPI_IOCTLACTION_ENABLEDISABLE,
  SPI_IOCTLACTIONS_MAX
} SPIIOCTLACTIONS;

// global function prototypes --------------------------------------------------
extern  void    Spi_Initialize( void );
extern  SPIERR  Spi_Write( SPIDEVENUM eDev, U8 nData );
extern  SPIERR  Spi_Read( SPIDEVENUM eDev, U8 nXmtData, PU8 pnRcvData );
extern  SPIERR  Spi_WriteBlock( SPIDEVENUM eDev, PU8 pnBuffer, U16 wSize, BOOL bStoreRead );
extern  SPIERR  Spi_ReadBlock( SPIDEVENUM eDev, U8 nOutData, PU8 pnBuffer, U16 wSize );
extern  SPIERR  Spi_Ioctl( SPIDEVENUM eDev, SPIIOCTLACTIONS eAction, PVOID pvValue );

/**@} EOF Spi.h */

#endif  // _SPI_H