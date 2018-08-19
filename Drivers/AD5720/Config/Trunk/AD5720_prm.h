/******************************************************************************
 * @file AD5720_prm.h
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
 * \addtogroup AD5720
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AD5720_PRM_H
#define _AD5720_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "SPI/Spi.h"

// Macros and Defines ---------------------------------------------------------
/// define the GPIO pin to use for the chip select
#define AD5720_CEN_PIN_ENUM               ( GPIO_PIN_ENUM_ILLEGAL )

/// define the SPI bus enum
#define AD5720_SPI_DEV_ENUM               ( SPI_DEV_ENUM_ILLEGAL )

/**@} EOF AD5720.h */

#endif  // _AD5720_PRM_H