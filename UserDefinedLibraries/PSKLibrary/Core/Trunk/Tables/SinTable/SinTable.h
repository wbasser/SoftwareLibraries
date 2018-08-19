/******************************************************************************
 * @file SinTable
 *
 * @brief SIN lookup table declarations
 *
 * This file provides the declarations for the sin look up table
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
 * \addtogroup SinTable
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SINTABLE_H
#define _SINTABLE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern S16  SinTable_Lookup( U16 wPhase );

/**@} EOF SinTable.h */

#endif  // _SINTABLE_H