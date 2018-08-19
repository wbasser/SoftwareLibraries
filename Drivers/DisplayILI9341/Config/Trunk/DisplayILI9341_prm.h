/******************************************************************************
 * @file DisplayILI9341_prm.h
 *
 * @brief IL9341 display driver parameter declarations
 *
 * This file provides the declaration of the parameters for the ILI9341 display
 * driver
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
 * \addtogroup DisplayILI9341
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DISPLAYILI9341_PRM_H
#define _DISPLAYILI9341_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the GPIO pins for the command data select and the chip enable
#define DISPLAYILI9341_DCSEL_ENUM       ( GPIO_PIN_ENUM_ILLEGAL )
#define DISPLAYILI9341_CENB_ENUM        ( GPIO_PIN_ENUM_ILLEGAL )

/// define the SPI enumeration
#define DISPLAYIL9341_SPI_ENUM          ( SPI_ENUM_ILLEGAL )

/**@} EOF DisplayILI9341_prm.h */

#endif  // _x_H