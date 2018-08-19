/******************************************************************************
 * @file ShapeTable.h
 *
 * @brief shape table declarations
 *
 * This file provides the declarations for the transmit shape table
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
 * \addtogroup ShapeTable
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SHAPETABLE_H
#define _SHAPETABLE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
// enumerate the shapes
typedef enum	_SHAPES
{
	SHAPE_ZZ = 0,
	SHAPE_PP,
	SHAPE_MM,
	SHAPE_ZP,
	SHAPE_PZ,
	SHAPE_MZ,
	SHAPE_PM,
	SHAPE_MP,
  SHAPE_MAX
} SHAPES;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	FLOAT	Shape_GetValue( SHAPES eShape, U8 nRampIndex );

/**@} EOF ShapeTable.h */

#endif  // _SHAPETABLE_H