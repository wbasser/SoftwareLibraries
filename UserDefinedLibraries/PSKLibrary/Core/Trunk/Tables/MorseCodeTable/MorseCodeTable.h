/******************************************************************************
 * @file MorseCodeTable.h
 *
 * @brief Morse code table declarations
 *
 * This file provides the declarations for the morse code table
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
 * \addtogroup MorseCodeTable
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MORSECODETABLE_H
#define _MORSECODETABLE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  U8  MorseCodeTable_CodeToAscii( U16 wCode );
extern  U16 MorseCodeTable_AsciiToCode( U8 nCode );

/**@} EOF MorseCodeTable.h */

#endif  // _MORSECODETABLE_H