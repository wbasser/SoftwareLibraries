/******************************************************************************
 * @file DisplayILI9341_prv.h
 *
 * @brief Display ILI9341 private declarations 
 *
 * This file provides the private declarations for the ILI9341 display
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration,  LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration,  LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration,  LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DisplayILI9341_prv
 * @{ 
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DISPLAYILI9341_PRV_H
#define _DISPLAYILI9341_PRV_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of characters
#define FONT_MIN_VAL                ( 0x20 )
#define FONT_MAX_VAL                ( 0x7F )
#define FONT_MAX_SIZE               ( FONT_MAX_VAL - FONT_MIN_VAL )

/// define the X/y size
#define FONT_X_SIZE                 ( 8 )
#define FONT_Y_SIZE                 ( 8 )
#define FONT_SPACE_SIZE             ( 6 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE U8  anFonts[ ][ ];

// global function prototypes --------------------------------------------------

/**@} EOF DisplayILI9341_prv.h */

#endif  // _DISPLAYILI9341_PRV_H