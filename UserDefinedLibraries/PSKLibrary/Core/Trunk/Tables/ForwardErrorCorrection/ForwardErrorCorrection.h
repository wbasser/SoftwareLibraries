/******************************************************************************
 * @file ForwardErrorCorrecton.h
 *
 * @brief Forward Error Correction Table declarations
 *
 * This file provides the forward error correction tables
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
 * \addtogroup ForwardErrorCorrecton
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FORWARDERRORCORRECTON_H
#define _FORWARDERRORCORRECTON_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the fec mask
#define	FWDERRCOR_VAL_MASK                ( 0x1F )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

// global parameter declarations
extern	U8	ForwardErrorCorrection_GetSymbol( U8 nSymbol );

/**@} EOF ForwardErrorCorrecton.h */

#endif  // _FORWARDERRORCORRECTON_H