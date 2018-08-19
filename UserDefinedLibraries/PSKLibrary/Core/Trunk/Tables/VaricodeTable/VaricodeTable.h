/******************************************************************************
 * @file VericodeTable.h
 *
 * @brief vericode table declarations
 *
 * This file provides the declarations for the varicode table
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
 * \addtogroup VericodeTable
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _VERICODETABLE_H
#define _VERICODETABLE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  U8  VeriCodeTable_CodeToAscii( U16 wCode );
extern  U16 VericodeTable_AsciiToCode( U8 nCode );

/**@} EOF VericodeTable.h */

#endif  // _VERICODETABLE_H