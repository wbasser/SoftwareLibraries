/******************************************************************************
 * @file AscBinDirector_def.h
 *
 * @brief Communications Director declarations 
 *
 * This file provides the declarations for the communications director
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
 * \addtogroup AscBinDirector
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ASCBINDIRECTOR_DEF_H
#define _ASCBINDIRECTOR_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#include "BinaryCommandHandler/BinaryCommandHandler.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the protocol def macro
#define ASCBINENTRY( ascprot, binprot ) \
  { \
    .eAscProtEnum = ascprot, \
    .eBinProtEnum = binprot, \
  }

// enumerations ---------------------------------------------------------------
typedef enum _ASCBINSEL
{
  ASCBIN_SEL_ASC = 0,
  ASCBIN_SEL_BIN,
  ASCBIN_SEL_MAX
} ASCBINSEL;

// structures -----------------------------------------------------------------
/// define the structure to define the protocol enumerations
typedef struct _ASCBINDEF
{
  ASCCMDENUM  eAscProtEnum;
  BINCMDENUM  eBinProtEnum;
} ASCBINDEF, *PASCBINDEF;
#define ASCBINDEF_SIZE    sizeof( ASCBINDEF )

/**@} EOF AscBinDirector_def.h */

#endif  // _ASCBINDIRECTOR_DEF_H