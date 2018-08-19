/******************************************************************************
 * @file I2C_cfg.c
 *
 * @brief I2C configuraiton implementation
 *
 * This file pvoides the I2C configuration implementation
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// fill out the configuration table
const I2CDEF atI2cDefs[ I2C_DEV_MAX ] = 
{
  // I2C_DEVICENONBLK( devper, gpioper, devbase, gpiobase, sdapin, sclpin, sdafunc, sclfunc, fastmode, callback )
  // I2C_DEVICE_BLKING( devper, gpioper, devbase, gpiobase, sdapin, sclpin, sdafunc, sclfunc, fastmode )
};

/**@} EOF I2c_cfg.c */
