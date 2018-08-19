/******************************************************************************
 * @file SenSI7210A20.h
 *
 * @brief Silicon Labs SI7210A20 temperature/humidity sensor declarations
 *
 * This file provides the declarations for Silicon Labs S7210A20 temperature/
 * humidity sensor driver
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
 * \addtogroup SenSI7210A20
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENSI7210A20_H
#define _SENSI7210A20_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the value
typedef enum _SENSI7210A20TYPE
{
  SENSI7210A20_TYPE_HUMID = 0,         ///< humidity
  SENSI7210A20_TYPE_TEMP,              ///< temperature
  SENSI7210A20_TYPE_MAX
} SENSI7210A20TYPE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SenSI7210A20_Initialize( void );
extern  void  SenSI7210A20_ProcessConversion( void );
extern  FLOAT SenSI7210A20_GetValue( SENSI7210A20TYPE eType );

/**@} EOF SenSI7210A20.h */

#endif  // _SENSI7210A20_H