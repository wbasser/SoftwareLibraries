/******************************************************************************
 * @file DisplaySSD1306_prm.h
 *
 * @brief Display SSD1306 parameter declarations
 *
 * This file provides the declarations for the display SSD1306 driver
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
 * \addtogroup DisplaySSD1306
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DISPLAYSSD1306_PRM_H
#define _DISPLAYSSD1306_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the interface types
#define DISPLAYSSD1306_INTERFACE_SPI        ( 0 )
#define DISPLAYSSD1306_INTERFACE_I2C        ( 1 )

/// define the interface selection
#define DISPLAYSSD1306_INTERFACE_SELECT     ( DISPLAYSSD1306_INTERFACE_SPI )

/// include the appropriate interface file
#if ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_SPI )
  #include "SPI/Spi.h"

  /// define the SPI enum/chip select/data command select pins for SPI
  #define DISPLAYSSD1306_SPI_DEV_ENUM         ( SPI_DEV_ENUM_ILLEGAL )
  #define DISPLAYSSD1306_SPI_CEN_ENUM         ( GPIO_PIN_ENUM_ILLEGAL )
  #define DISPLAYSSD1306_SPI_DCS_ENUM         ( GPIO_PIN_ENUM_ILLEGAL )
#elif ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_I2C )
  #include "I2C/I2c.h"

  /// define the I2C enum
  #define DISPLAYSSD1306_I2C_EVN_ENUM         ( I2C_DEV_ENUM_ILLEGAL )
#elif
  #error "DISPLAYSSD1306_INTERFACE_SELECT must be set to eigther DISPLAYSSD1306_INTERFACE_SPI or DISPLAYSSD1306_INTERFACE_I2C" 
#endif

/// define the reset pin
#define DISPLAYSSD1306_RST_PIN_ENUM         ( GPIO_PIN_ENUM_ILLEGAL )

/// define the display types
#define DISPLAYSSD1306_TYPE_128_64          ( 0 )
#define DISPLAYSSD1306_TYPE_128_32          ( 1 )
#define DISPLAYSSD1306_TYPE_96_16           ( 2 )

/// define the display type selection
#define DISPLAYSSD1306_TYPE_SELECT          ( DISPLAYSSD1306_TYPE_128_64 )

/// define the VCC TYPES
#define DISPLAYSSD1306_VCC_EXTRNAL          ( 0 )
#define DISPLAYSSD1306_VCC_SWITCHCAP        ( 1 )

/// define the VCC type selection
#define DISPLAYSSD1306_VCC_SELECT           ( DISPLAYSSD1306_VCC_EXTERNAL )

/// define the 
// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF DisplaySSD1306_prm.h */

#endif  // _DISPLAYSSD1306_PRM_H