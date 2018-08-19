/******************************************************************************
 * @file DigitalPot.h
 *
 * @brief digital potentiometer declarations 
 *
 * This file provides the declarations for the digital potentiometers module
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
 * \addtogroup DigitalPot
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DIGITALPOT_H
#define _DIGITALPOT_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DigitalPot/DigitalPot_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _DIGPOTERR
{
  DIGPOT_ERR_NONE = 0,      ///< no error
  DIGPOT_ERR_ILLENUM,       ///< illegal enumeration
  DIGPOT_ERR_ILLPOS,        ///< illegal position
} DIGPOTERR;

/// enumerate the direction
typedef enum _DIGPOTDIR
{
  DIGPOT_DIR_DN = 0,        ///< donw
  DIGPOT_DIR_UP             ///< up
} DIGPOTDIR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      DigitalPot_Initialize( void );
extern  DIGPOTERR DigitalPot_SetPosition( DIGPOTENUM eDigPotSel, U16 wPositon );
extern  DIGPOTERR DigitalPot_IncrementDecrement( DIGPOTENUM eDigPotSel, DIGPOTDIR eDigPotDir );
extern  DIGPOTERR DigitalPot_StoreValue( DIGPOTENUM eDigPotSel );

/**@} EOF DigitalPot.h */

#endif  // _DIGITALPOT_H