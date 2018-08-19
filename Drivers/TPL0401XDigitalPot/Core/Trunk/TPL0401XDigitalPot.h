/******************************************************************************
 * @file TPL0401XDigitalPot.h
 *
 * @brief TPL0401X Digital pot declarations
 *
 * This file provides the declarations for the TPL0401X Digital Potentiometer
 * device driver
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
 * \addtogroup TPL0401XDigitalPot
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TPL0401XDIGITALPOT_H
#define _TPL0401XDIGITALPOT_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the device
typedef enum _TPL0401XDIGPOTDEV
{
  TPL0401X_DIGPOT_DEV_A = 0,
  TPL0401X_DIGPOT_DEV_B,
  TPL0401X_DIGPOT_DEV_MAX  
} TPL0401XDIGPOTDEV;

/// enumerate the errors
typedef enum _TPL0401DIGPOTERR
{
  TPL0401X_DIGPOT_ERR_NONE = 0,
  TPL0401X_DIGPOT_ERR_ILLDEV = 0xF7,  
} TPL0401DIGPOTERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void              TPL0401XDigitalPot_Initialize( void );
extern  TPL0401DIGPOTERR  TPL0401XDigitalPot_Write( TPL0401XDIGPOTDEV eDevice, U8 nValue );
extern  TPL0401DIGPOTERR  TPL0401XDigitalPot_Read( TPL0401XDIGPOTDEV eDevice, PU8 pnValue );

/**@} EOF TPL0401XDigitalPot.h */

#endif  // _x_H