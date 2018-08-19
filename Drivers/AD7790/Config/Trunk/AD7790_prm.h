/******************************************************************************
 * @file AD7790_prm.h
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup AD7790
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AD7790_PRM_H
#define _AD7790_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "SPI/Spi.h"

// Macros and Defines ---------------------------------------------------------
/// define the GPIO pin to use for the chip select
#define AD7790_CEN_PIN_ENUM               ( GPIO_PIN_ENUM_ILLEGAL )

/// define the SPI bus enum
#define AD7790_SPI_DEV_ENUM               ( SPI_DEV_ENUM_ILLEGAL )

/**@} EOF AD7790.h */

#endif  // _AD7790_PRM_H