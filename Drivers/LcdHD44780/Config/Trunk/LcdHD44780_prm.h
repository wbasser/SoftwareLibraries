/******************************************************************************
 * @file LcdHD44780_prm.h
 *
 * @brief LCD Display Driver parameterazation, HD44780 chip set 
 *
 * This file provides the parameters for the HD44780 chip
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
 * \addtogroup LcdHD44780
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LCDHD44780_PRM_H
#define _LCDHD44780_PRM_H

// library includes -----------------------------------------------------------
#include "Micro/Types.h"
#include "GPIOP/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the register select pin
#define LCDHD44780_REGSEL_PIN                ( GPIO_PIN_ENUM_ILLEGAL )

/// define the write enable pin
#define LCDHD44780_WRTENB_PIN                ( GPIO_PIN_ENUM_ILLEGAL )

/// define the chip enable pin
#define LCDHD44780_ENABLE_PIN                ( GPIO_PIN_ENUM_ILLEGAL )

/// define the data port
#define LCDHD44780_DATA_PORT                 ( GPIO_GRP_ENUM_ILLEGAL )

/// define the number of rows
#define LCDHD44780_NUM_ROWS                   ( 4 )

/// define the number of characters/row
#define LCDHD44780_NUM_CHARS                 ( 20 )

/// define the macro for enabling four bit mode
#define LCDHD44780_ENABLE_FOUR_BIT_MODE      ( 0 )

 
/**@} EOF LcdHD44780_prm.h */

#endif  // _LCDHD44780_PRM_H