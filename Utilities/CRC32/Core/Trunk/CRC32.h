/******************************************************************************
 * @file CRC32.h	
 *
 * @brief CRC32 calculation declarations
 *
 * This file provides the declarations for the CRC32 calculation module
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
 * \addtogroup CRC32
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CRC32_H
#define _CRC32_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	U32	CRC32_GetInitialValue( void );
extern	U32	CRC32_CalculateByte( U32 uOldCrc, U8 nData );
extern	U32	CRC32_CalculateBlock( PU8 pnData, U32 uLength );

/**@} EOF CRC32.h */

#endif  // _CRC32_H