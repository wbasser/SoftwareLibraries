/******************************************************************************
 * @file VSK10XXHandler_prm.h
 *
 * @brief VSK10XX handler parameter declarations
 *
 * This file provides the customization pamaters for the VSK10XX handler
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
 * \addtogroup VSK10XXHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _VSK10XXHANDLER_PRM_H
#define _VSK10XXHANDLER_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the GPIO enumeration for Chip select
#define VSK10XX_CEN_ENUM            ( GPIO_PIN_ENUM_MP3CEN )

/// define the GPIO enumeration for command/data select
#define VSK10XX_CDS_ENUM            ( GPIO_PIN_ENUM_MP3DCS )

/// define the GPIO enumeration for data request
#define VSK10XX_DRQ_ENUM            ( GPIO_PIN_ENUM_MP3DREQ )

/// define the GPIO enuemration for the reset
#define VSK10XX_RST_ENUM            ( GPIO_PIN_ENUM_MP3RST )

/**@} EOF VSK10XXHandler_prm.h */

#endif  // _VSK10XXHANDLER_PRM_H_ */