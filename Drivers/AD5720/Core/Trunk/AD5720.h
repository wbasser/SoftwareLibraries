/******************************************************************************
 * @file AD5720.h
 *
 * @brief AD5720 programmable resistor declarationss
 *
 * This file provides the declarations for the AD5720 programmable resistor
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
 * \addtogroup AD5720
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AD5720_H
#define _AD5720_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  AD5720_Initializle( void );
extern  void  AD5720_SetResistance( U16 wResistance );

/**@} EOF AD5720.h */

#endif  // _AD5720_H