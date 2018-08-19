/******************************************************************************
 * @file ForwardErrorCorrection.c
 *
 * @brief forward error correction table implementation
 *
 * This file provides the implementation for the forward error correction
 * table
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
 * \addtogroup ForwardErrorCorrection
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ForwardErrorCorrection/ForwardErrorCorrection.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
// For the QPSK modulator/demodulator, rate 1/2 constraint length 5 
// convolutional FEC coding is used.
// The generator polynomials used are:
//  g1(x) = x^4 + x^3           + 1		= 0x19
//  g0(x) = x^4	+       x^2 + x + 1		= 0x17
// 
//                                g1(x)
//              /----+--------+--------------------------+
//            /      |        |                          |
// symbol msb       ---      ---      ---      ---      ---
//                 | b4|<---| b3|<---| b2|<---| b1|<---| b0| <-- inverted data in
// symbol lsb       ---      ---      ---      ---      ---
//            \      |                 |        |        |
//              \----+-----------------+--------+--------+
//                                g0(x)

// global parameter declaration/initialization
static	const U8	anFecTable[ ] =
{
	2, 1, 3, 0, 3, 0, 2, 1,
	0, 3, 1, 2, 1, 2, 0, 3,
	1, 2, 0, 3, 0, 3, 1, 2,
	3, 0, 2, 1, 2, 1, 3, 0 
};

/******************************************************************************
 * @function ForwardErrorCorrection_GetSymbol
 *
 * @brief forward error correction symbol
 *
 * This function will return the forward error corrected symbol 
 *
 * @param[in]   nSymbol       symbol to correct
 *
 * @return      corrected symbol
 *
 *****************************************************************************/
U8 ForwardErrorCorrection_GetSymbol( U8 nSymbol )
{
	// return the new symbol
	return(( nSymbol < sizeof( anFecTable )) ? anFecTable[ nSymbol ] : 0 );
}

/**@} EOF ForwardErrorCorrection.c */
