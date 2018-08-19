/******************************************************************************
 * @file DigitalPot_cfg.h
 *
 * @brief digital pot configuration declarations
 *
 * This file provides the declarations for the digital pots
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
#ifndef _DIGITALPOT_CFG_H
#define _DIGITALPOT_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DigitalPot/DigitalPot_def.h"

// library includes -----------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the digital pots
typedef enum _DIGPOTENUM
{
  // add user definitions here
  
  // do not remove these entries
  DIGPOT_ENUM_MAX,
  DIGPOT_ENUM_ILLEGAL = 0xFF
} DIGPOTENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE DIGPOTDEF  atDigPotDefs[ ];

/**@} EOF DigitalPot_cfg.h */

#endif  // _DIGITALPOT_CFG_H