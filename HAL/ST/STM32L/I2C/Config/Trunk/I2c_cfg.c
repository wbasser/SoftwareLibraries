/******************************************************************************
 * @file I2C_cfg.c
 *
 * @brief I2C configuraiton implementation
 *
 * This file pvoides the I2C configuration implementation
 *  
 * @copyright Copyright (c) 2014 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used
 * without the prior written permission of Cyber Integration, LLC.
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
  // add items below using the below macros
  // I2C_DEVICE_NONBLK( chan, slvadr, fastspeed, callback )
  // I2C_DEVICE_BLKING( chan, slvadr, fastspeed )
};

/**@} EOF I2c_cfg.c */
