/******************************************************************************
 * @file SPI_cfg.c
 *
 * @brief SPI configuraiton implementation
 *
 * This file pvoides the SPI configuration implementation
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// fill out the configuration table
const SPIDEF atSpiDefs[ SPI_DEV_MAX ] = 
{
  // SPI_DEVICE( devper, gpioper, devbase, gpiobase, sclkpin, mosipin, misopin, sspin, sclkfunc, mosifunc, misofunc, ssfunc )
};

/**@} EOF Spi_cfg.c */
