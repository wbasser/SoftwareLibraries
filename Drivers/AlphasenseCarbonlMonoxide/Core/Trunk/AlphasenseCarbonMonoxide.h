/******************************************************************************
 * @file AlphasenseCarbonMonoxide.h
 *
 * @brief Alphasense carbon monoxide sensor declarations
 *
 * This file provides the declarations for the Alphasense carbon monoxide sensor
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
 * \addtogroup AlphasenseCarbonMonoxide
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALPHASENSECARBONMONOXIDE_H
#define _ALPHASENSECARBONMONOXIDE_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  AlphasenseCarbonMonoxide_Initialize( void );
extern  FLOAT AlphasenseCarbonMonoxide_GetValuePpm( void );

/**@} EOF AlphasenseCarbonMonoxide.h */

#endif  // _ALPHASENSECARBONMONOXIDE_H