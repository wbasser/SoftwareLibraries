/******************************************************************************
 * @file RtcDs1340_prm.h
 *
 * @brief RTC DS1340 parameters
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup RtcDs1340
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTCDS1340_PRM_H
#define _RTCDS1340_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the I2C device enumeration
#define RTCDS1340_I2C_DEV_ENUM              ( I2c_DEV_ENUM_ILLEGAL )

/**@} EOF RtcDs1340_prm.h */

#endif  // _RTCDS1340_PRM_H