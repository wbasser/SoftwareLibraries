/******************************************************************************
 * @file SenSI7210A20_prm.h
 *
 * @brief Silicon Labs SI7210A20 parameter declarations
 *
 * This file provides the parametric declarations for the SI7210A20 sensor
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
 * $Rev: $
 * 
 *
 * \addtogroup SenSI7210A20
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENSI7210A20_PRM_H
#define _SENSI7210A20_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the I2C channel
#define SENSI7210A20_I2C_ENUM             ( I2c_ENUM_ILLEGAL )

// define the scan rate
#define SENSI7210A20_SCAN_RATE            


/**@} EOF SenSI7210A20_prm.h */

#endif  // _SENSI7210A20_PRM_H