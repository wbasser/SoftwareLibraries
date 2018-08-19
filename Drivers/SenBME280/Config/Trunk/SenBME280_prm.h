/******************************************************************************
 * @file SenBME280_prm.h
 *
 * @brief BME280 sensor parameters 
 *
 * This file provides the parameters for the BME280 sensor
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup SenBME280
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENBME280_PRM_H
#define _SENBME280_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the scan rate for the sensor
#define SENBME280_SCAN_RATE_MSECS            ( 500 )

/// define the I2C device enumeration
#define SENBME280_I2C_ENUM                   ( 0 )

/**@} EOF SenBME280_prm.h */

#endif  // _SENBME280_PRM_H