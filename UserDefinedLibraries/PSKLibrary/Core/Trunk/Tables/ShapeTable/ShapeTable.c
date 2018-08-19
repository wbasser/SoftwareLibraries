/******************************************************************************
 * @file ShapeTable.c
 *
 * @brief shape table implementation
 *
 * This file provides the implementation of the shape table
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
 * \addtogroup shape table
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ShapeTable/ShapeTable.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// instantiate the cosine lookup table
// cosine table initialization
static	const CODE FLOAT	afCosine[ ] =
{
	1.0,			    0.999924702,	0.999698819,	0.999322385,
	0.998795456,	0.998118113,	0.997290457,	0.996312612,
	0.995184727,	0.99390697,		0.992479535,	0.990902635,
	0.98917651,		0.987301418,	0.985277642,	0.983105487,
	0.98078528,		0.978317371,	0.97570213,		0.972939952,
	0.970031253,	0.966976471,	0.963776066,	0.960430519,
	0.956940336,	0.95330604,		0.949528181,	0.945607325,
	0.941544065,	0.937339012,	0.932992799,	0.92850608,
	0.923879533,	0.919113852,	0.914209756,	0.909167983,
	0.903989293,	0.898674466,	0.893224301,	0.88763962,
	0.881921264,	0.876070094,	0.870086991,	0.863972856,
	0.85772861,		0.851355193,	0.844853565,	0.838224706,
	0.831469612,	0.824589303,	0.817584813,	0.810457198,
	0.803207531,	0.795836905,	0.788346428,	0.780737229,
	0.773010453,	0.765167266,	0.757208847,	0.749136395,
	0.740951125,	0.732654272,	0.724247083,	0.715730825,
	0.707106781,	0.698376249,	0.689540545,	0.680600998,
	0.671558955,	0.662415778,	0.653172843,	0.643831543,
	0.634393284,	0.624859488,	0.615231591,	0.605511041,
	0.595699304,	0.585797857,	0.575808191,	0.565731811,
	0.555570233,	0.545324988,	0.53499762,		0.524589683,
	0.514102744,	0.503538384,	0.492898192,	0.482183772,
	0.471396737,	0.460538711,	0.44961133,		0.438616239,
	0.427555093,	0.41642956,		0.405241314,	0.39399204,
	0.382683432,	0.371317194,	0.359895037,	0.34841868,
	0.336889853,	0.325310292,	0.31368174,		0.302005949,
	0.290284677,	0.278519689,	0.266712757,	0.25486566,
	0.24298018,		0.231058108,	0.21910124,		0.207111376,
	0.195090322,	0.183039888,	0.170961889,	0.158858143,
	0.146730474,	0.134580709,	0.122410675,	0.110222207,
	0.09801714,		0.085797312,	0.073564564,	0.061320736,
	0.049067674,	0.036807223,	0.024541229,	0.012271538,
	0.0
};	

/******************************************************************************
 * @function Shape_GetValue
 *
 * @brief get the desired value
 *
 * This function will calculate and return the desired cosine shape
 *
 * @param[in]   eShpe       current shape
 * @param[in]   nRampIndex  ramp index
 *
 * @return      current shape cosine value
 *
 *****************************************************************************/
FLOAT	Shape_GetValue( SHAPES eShape, U8 nRampIndex )
{
  FLOAT fValue;

  	// determine which shape
	switch ( eShape ) {
		case SHAPE_ZZ :
			fValue = 0.0;	
			break;
			
		case SHAPE_PP :
			fValue = 1.0;	
			break;
			
		case SHAPE_MM :
			fValue = -1.0;
			break;
			
		case SHAPE_ZP :
			fValue = ( nRampIndex < 128 ) ? 0.0 : afCosine[ 256 - nRampIndex ];
			break;
			
		case SHAPE_PZ :
			fValue = ( nRampIndex < 128 ) ? afCosine[ nRampIndex ] : 0.0;
			break;
			
		case SHAPE_MZ :
			fValue = ( nRampIndex < 128 ) ? -1.0 * afCosine[ nRampIndex ] : 0.0;
			break;
			
		case SHAPE_PM :
			fValue = ( nRampIndex < 128 ) ? afCosine[ nRampIndex ] : -1.0 * afCosine[ 256 - nRampIndex ];
			break;
			
		case SHAPE_MP :
			fValue = ( nRampIndex < 128 ) ? -1.0 * afCosine[ nRampIndex ] : afCosine[ 256 - nRampIndex ];
			break;
			
		default :
			fValue = 0.0;
			break;
	}	

  // return the value
  return( fValue );
}
 
/**@} EOF ShapeTable.c */
