/******************************************************************************
 * @file DisplayST7565R_prm.h
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
 * \addtogroup DisplayST7565R
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DISPLAYST7565R_PRM_H
#define _DISPLAYST7565R_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the chip select enumeration
#define DISPLAYST7565R_CEN_GPIO_ENUM        ( GPIO_PIN_ENUM_ILLEGAL )

/// define the data/command select enumeration
#define DISPLAYST7565R_CDS_GPIO_ENUM        ( GPIO_PIN_ENUM_ILLEGAL )

/// define the display types
#define DISPLAYST7565R_TYPE_128M            ( 0 )
#define DISPLAYST7565R_TYPE_128L            ( 1 )
#define DISPLAYST7565R_TYPE_132M            ( 2 )

/// define the display selection maacro
#define DISPLAYST7565R_TYPE_SELECT          ( DISPLAYST7565R_TYPE_128M )

/// define the display orientation types
#define DISPLAYST7565R_ORIENTATION_TOP      ( 0 )
#define DISPLAYST7565R_ORIENTATION_BOTTOM   ( 1 )

// define the display orientation macro
#define DISPLAYST7565R_ORIENTATION_SELECT   ( DISPLAYST7565R_ORIENTATION_TOP )

/**@} EOF DisplayST7565R_prm.h */

#endif  // _DISPLAYST7565R_PRM_H