/******************************************************************************
 * @file StellarisLwipHandler_prm.h
 *
 * @brief Stellaris LWIP handler parameter declarations
 *
 * This file provides the declarations for the ethernet handler configuration
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup StellarisLwipHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _STELLARISLWIPHANDLER_PRM_H
#define _STELLARISLWIPHANDLER_PRM_H

// include
#include "SystemControlManager/SystemControlManager.h"

/// define the ethernet LED base
#define STELLARISLWIP_ETHLED_PERIPH     ( 0 )

/// define the GPIO port base for ethernet leds
#define STELLARISLWIP_PORT_BASE         ( 0 )

/// define the pins for the link/activity leds
#define STELLARISLWIP_ETHLED_LINK_PIN   ( GPIO_PIN_ENUM_ILLEGAL )
#define STELLARISLWIP_ETHLED_ACT_PIN    ( GPIO_PIN_ENUM_ILLEGAL )

/// define the host timer function
#define STELLARISLWIP_HOST_FUNCTION( )   

/**@} EOF StellarisLwipHandler_prm.h */

#endif  // _STELLARISLWIPHANDLER_PRM_H