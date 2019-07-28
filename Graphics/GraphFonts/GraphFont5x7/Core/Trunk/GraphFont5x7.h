/******************************************************************************
 * @file GraphFont5x7.h
 *
 * @brief Graphics font 5x7 declarations 
 *
 * This file provides the declarations for the graphics font 5 x 7
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
 * \addtogroup GraphFont5x7
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GRAPHFONT5X7_H
#define _GRAPHFONT5X7_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of characters
#define GRAPHICFONT_MIN_VAL                ( 0x20 )
#define GRAPHICFONT_MAX_VAL                ( 0x7F )
#define GRAPHICFONT_MAX_CNT                ( GRAPHICFONT_MAX_VAL - GRAPHICFONT_MIN_VAL  + 1 )

/// define the X/y size
#define GRAPHICFONT_X_SIZE                 ( 5 )
#define GRAPHICFONT_Y_SIZE                 ( 7 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE U8 g_anGraphicFont5x7[  ][ GRAPHICFONT_X_SIZE ];

// global function prototypes --------------------------------------------------

/**@} EOF GraphFont5x7.h */

#endif  // _GRAPHFONT5X7_H