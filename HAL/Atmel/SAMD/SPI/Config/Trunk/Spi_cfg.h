/******************************************************************************
 * @file SPI_cfg.h
 *
 * @brief SPI configuration declarations
 *
 * This file contains the SPI configuration declarations
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
 
// ensure only one instatiation
#ifndef _SPI_CFG_H
#define _SPI_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// set these to one when selecting a channel to use
#define SPI_CHAN0_IN_USE      ( 0 )
#define SPI_CHAN1_IN_USE      ( 0 )
#define SPI_CHAN2_IN_USE      ( 0 )
#define SPI_CHAN3_IN_USE      ( 1 )
#define SPI_CHAN4_IN_USE      ( 0 )
#define SPI_CHAN5_IN_USE      ( 0 )

// enumerations ---------------------------------------------------------------
/// declare the SPI enuemrations
typedef enum  _SPIDEVENUM
{
  // enumerate the user devices here

  // do not remove these entries
  SPI_DEV_ENUM_MAX,
  SPI_DEV_ENUM_ILLEGAL = 255
} SPIDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const SPIDEF atSpiDefs[ ];

/**@} EOF Spi_cfg.h */

#endif  // _SPI_CFG_