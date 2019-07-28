/******************************************************************************
 * @file SPI_cfg.c
 *
 * @brief SPI configuraiton implementation
 *
 * This file pvoides the SPI configuration implementation
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// fill out the configuration table
const SPIDEF g_atSpiDefs[ SPI_DEV_ENUM_MAX ] = 
{
  /// instantiate SPI channels here using the below helper macros
  // SPI_DEF_MASTER( chan, clkpol, clkphase, bitorder, pinconfig, baudrate, runstdby )
};

/**@} EOF Spi_cfg.c */
