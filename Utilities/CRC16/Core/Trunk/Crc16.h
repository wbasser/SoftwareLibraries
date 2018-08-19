/******************************************************************************
 * @file CRC16.h	
 *
 * @brief CRC16 calculation declarations
 *
 * This file provides the declarations for the CRC16 calculation module
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
 * $Log: $
 * 
 *
 * \addtogroup CRC16
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CRC16_H
#define _CRC16_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	U16	CRC16_GetInitialValue( void );
extern	U16	CRC16_CalculateByte( U16 wOldCrc, U8 nData );
extern	U16	CRC16_CalculateBlock( PU8 pnData, U16 wLength );

/**@} EOF CRC16.h */

#endif  // _CRC16_H