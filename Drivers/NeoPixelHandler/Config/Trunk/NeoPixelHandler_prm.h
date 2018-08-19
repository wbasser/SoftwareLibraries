/******************************************************************************
 * @file NeoPixelHandler_prm.h
 *
 * @brief Neo Pixel Handler parameter declarations
 *
 * This file provides the declarations for the Neo Pixel Handler
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
 * \addtogroup NeoPixelHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NEOPIXELHANDLER_PRM_H
#define _NEOPIXELHANDLER_PRM_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the NEO pixel handler port
#define NEOPIXELHANDLER_OUTPUT_PORT           ( PORTB )

/// define the NEO pixel handler pin
#define NEOPIXELHANDLER_OUTPUT_PIN            ( PINB4 )

/**@} EOF NeoPixelHandler_prm.h */

#endif  // _NEOPIXELHANDLER_PRM_H