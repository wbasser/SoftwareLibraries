/******************************************************************************
 * @file SenMPL115A2_prm.h
 *
 * @brief  parameter declarations
 *
 * This file provides the parametric declarations for the MPL115A2 sensor
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
 * \addtogroup SenMPL115A2
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENMPL115A2_PRM_H
#define _SENMPL115A2_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the I2C channel
#define SENMPL115A2_I2C_ENUM             ( I2C_ENUM_ILLEGAL )

/**@} EOF SenMPL115A2_prm.h */

#endif  // _SENMPL115A2_PRM_H