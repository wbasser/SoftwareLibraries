/******************************************************************************
 * @file TPL0401XDigitalPot_prm.h
 *
 * @brief TPL0401X digital pot parameters 
 *
 * This file provides the parameters for the TPL0401X digital pot
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
 * \addtogroup TPL0401XDigitalPot
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TPL0401XDIGITALPOT_PRM_H
#define _TPL0401XDIGITALPOT_PRM_H

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the I2C bus enumeration
#define TPL0401XDIGITALPOT_I2C_ENUM                   ( I2C_DEV_ENUM_LCLBUS )

/**@} EOF TPL0401XDigitalPot_prm.h */

#endif  // _TPL0401XDIGITALPOT_PRM_H