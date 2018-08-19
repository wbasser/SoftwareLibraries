/******************************************************************************
 * @file SPI_cfg.h
 *
 * @brief SPI configuration declarations
 *
 * This file contains the SPI configuration declarations
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
 
// ensure only one instatiation
#ifndef _SPI_CFG_H
#define _SPI_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the SPI enuemrations
typedef enum  _SPIDEVENUM
{
  SPI_DEV_MAX
} SPIDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const SPIDEF atSpiDefs[ ];

/**@} EOF Spi_cfg.h */

#endif  // _SPI_CFG_