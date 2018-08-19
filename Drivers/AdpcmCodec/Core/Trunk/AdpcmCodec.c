/******************************************************************************
 * @file AdpcmCodec.c
 *
 * @brief ADPCM Codec implementation
 *
 * This file provides the implementation for the ADPCM codec
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AdpcmCodec
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AdpcmCodec/AdpcmCodec.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the size of the index table
#define	INDEX_TBL_SIZE	            ( 16 )

// define the size of the quantizier table
#define	QUANT_TBL_SIZE	            ( 89 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
// index table
static	const CODE S8	acIndexTable[ INDEX_TBL_SIZE ] =
{
	-1, -1, -1, -1, 2, 4, 6, 8, 
	-1, -1, -1, -1, 2, 4, 6, 8
};

// quantizier table
static	const CODE S16	aiQuantTable[ QUANT_TBL_SIZE ] =
{
	    7,     8,     9,    10,    11,    12,    13,    14,
	   16,    17,    19,    21,    23,    25,    28,    31,
       34,    37,    41,    45,    50,    55,    60,    66,
       73,    80,    88,    97,   107,   118,   130,   143,
      157,   173,   190,   209,   230,   253,   279,   307,
      337,   371,   408,   449,   494,   544,   598,   658,
      724,   796,   876,   963,  1060,  1166,  1282,  1411,
     1552,  1707,  1878,  2066,  2272,  2499,  2749,  3024,
     3327,  3660,  4026,  4428,  4871,  5358,  5894,  6484,
     7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794,
    32767  
};

/******************************************************************************
 * @function AdpcmCodec_Initilaize
 *
 * @brief initialization
 *
 * This function performs any required initialization
 *
 *****************************************************************************/
void AdpcmCodec_Initialize( void )
{
}

/******************************************************************************
 * @function AdpcmCodec_Encode
 *
 * @brief encode function
 *
 * This function will take a signed 16 PCM value and convert it to a
 * signed 4 bit ADPCM code
 *
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   iValue    PCM value
 *
 * @return      ADPCM code
 *
 *****************************************************************************/
C8 AdpcmCodec_Encode( PADPCMCTL ptCtl, S16 iValue )
{
	S32	lPredSample, lDiff, lStep, lTempStep;
	U8	nMask;
	C8	cCode, cIndex;
	
	// restore previous values
	lPredSample = ptCtl->lPrevSample;
	cIndex = ptCtl->cPrevIndex;
	
	// get the current step
	lStep = aiQuantTable[ cIndex ];
	
	// compute the difference between the acutal sample and the predicted sample
	lDiff = iValue - lPredSample;
	if ( lDiff >= 0 )
	{
		// set code to 0
		cCode = 0;
	}
	else
	{
		// set code to -1
		cCode = 8;
		lDiff = ~lDiff;
	}
	
	// set the mask/copy the step/quantitize the difference into 4 bit ADPCM code
	nMask = 4;
	lTempStep = lStep;
	for ( U8 nBit = 0; nBit < 3; nBit++ )
	{
		if ( lDiff >= lTempStep )
		{
			cCode |= nMask;
			lDiff -= lTempStep;
		}
		
		// shift the temp step size
		lTempStep >>= 1;
		nMask >>= 1;
	}
	
	// compute new sample estimate predicted sample
	lDiff = 0;
	if ( cCode & 4 )
	{
		// add the step
		lDiff += lStep;
	}
	
	if ( cCode & 2 )
	{
		// add the shifted step
		lDiff += ( lStep >> 1 );
	}
	
	if ( cCode & 1 )
	{
		// add the shifted step
		lDiff += ( lStep >> 2 );
	}
	
	lDiff += ( lStep >> 3 );
	
	// check for overflow
	if ( cCode & 8 )
	{
		// invert
		lDiff = ~lDiff;
	}
	
	// add it to the predicted sample
	lPredSample += lDiff;
	
	// check for overflow
	if ( lPredSample > 32767 )
	{
		// set it to max
		lPredSample = 32767;
	}
	else if ( lPredSample < -32768 )
	{
		// set it to min
		lPredSample = -32768;
	}
	
	// compute new stepsize
	cIndex += acIndexTable[ cCode ];
	
	// check for limits
	if ( cIndex < 0 )
	{
		// set it to minimum
		cIndex = 0;
	}
	else if ( cIndex > ( INDEX_TBL_SIZE - 1 ))
	{
		// set it to max
		cIndex = ( INDEX_TBL_SIZE - 1 ); 
	}
	
	// save the values
	ptCtl->lPrevSample = lPredSample;
	ptCtl->cPrevIndex = cIndex;
	
	// return the new code
	return( cCode & 0x0F );
}

/******************************************************************************
 * @function AdpcmCodec_Decode
 *
 * @brief Decode function
 *
 * This function will take a signed 4 bit ADPCM code and convert it to a
 * signed 16 bit PCM value
 *
 * @param[in]   ptCtl     pointer to the control structure
 * @param[in]   cValue    ADPCM value
 *
 * @return      PCM value
 *
 *****************************************************************************/
S16 AdpcmCodec_Decode( PADPCMCTL ptCtl, C8 cValue )
{
	S32	lPredSample, lDiff, lStep;
	C8	cCode, cIndex;
	
	// restore previous values
	lPredSample = ptCtl->lPrevSample;
	cIndex = ptCtl->cPrevIndex;
	
	// get the current step
	lStep = aiQuantTable[ cIndex ];
	
	// inverse quantitize
	lDiff = lStep >> 3;
	if ( cCode & 4 )
	{
		lDiff += lStep;
	}
	
	if ( cCode & 2 )
	{
		lDiff += ( lStep >> 1 );
	}
	
	if ( cCode & 1 )
	{
		lDiff += ( lStep >> 2 );
	}
	
	// check for sign
	if ( cCode & 8 )
	{
		// subtract it
		lPredSample -= lDiff;
	}
	else
	{
		// add it
		lPredSample += lDiff;
	}
	
	// check for overflow
	if ( lPredSample > 32767 )
	{
		// set it to max
		lPredSample = 32767;
	}
	else if ( lPredSample < -32768 )
	{
		// set it to min
		lPredSample = -32768;
	}
	
	// compute new stepsize
	cIndex += acIndexTable[ cCode ];
	
	// check for limits
	if ( cIndex < 0 )
	{
		// set it to minimum
		cIndex = 0;
	}
	else if ( cIndex > ( INDEX_TBL_SIZE - 1 ))
	{
		// set it to max
		cIndex = ( INDEX_TBL_SIZE - 1 ); 
	}
	
	// save the values
	ptCtl->lPrevSample = lPredSample;
	ptCtl->cPrevIndex = cIndex;
	
	// return the new value
	return( lPredSample );
}

/**@} EOF AdpcmCodec.c */

