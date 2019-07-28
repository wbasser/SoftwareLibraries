/******************************************************************************
 * @file I2C_cfg.c
 *
 * @brief I2C configuraiton implementation
 *
 * This file pvoides the I2C configuration implementation
 *  
 * @copyright Copyright (c) 2014 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// fill out the configuration table
const I2CDEF g_atI2cDefs[ I2C_DEV_ENUM_MAX ] = 
{
  // add items below using the below macros
  // I2C_DEVICE_NONBLK( chan, fastspeed, standby, callback )
  // I2C_DEVICE_BLKING( chan, fastspeed, standby )
};

/**@} EOF I2c_cfg.c */
