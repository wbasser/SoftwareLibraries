/******************************************************************************
 * @file AdestoAT25SFxx_prm.h
 *
 * @brief AdestoAT25SFxx parameter declarations
 *
 * This file provides the parameter declarations for the Adesto aT25SFxx 
 * series of serial flash memories
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
 * \addtogroup AdestoAT25SFxx
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADESTOAT25SFXX_PRM_H
#define _ADESTOAT25SFXX_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "SPI/Spi.h"

// Macros and Defines ---------------------------------------------------------
/// define the GPIO enumeraion
#define	ADESTOAT25SF_GPIO_CS_ENUM							( GPIO_PIN_ENUM_ILLEGAL )

/// define the SPI enumeration
#define	ADESTOAT25SF_SPI_ENUM									( SPI_DEV_ENUM_ILLEGAL )

/// define the address size
#define	ADESTOAT25SF_MAX_ADDRESS							( 1048576 )

/// define the maximum page size
#define	ADESTOAT25SF_PAGE_SIZE								( 256 )



/**@} EOF AdestoAT25SFxx_prm.h */

#endif  // _ADESTOAT25SFXX_PRM_H