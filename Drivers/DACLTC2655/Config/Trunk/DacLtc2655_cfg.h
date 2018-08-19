/******************************************************************************
 * @file DacLtc2655_cfg.h
 *
 * @brief DAC LTC 2655 driver comfiguration declarations
 *
 * This file provides the interface to the DAC LTC2655 
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DACLTC2655
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DACLTC2655_CFG_H
#define _DACLTC2655_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DacLtc2655/DacLtc2655_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the DAC devices
typedef enum _DACLTC2655DEVENUM
{
  // enumerate user devices here
  
  // do not remove the declarations below
  DACLTC2655_DEV_ENUM_MAX,
  DACLTC2655_DEV_ENUM_ILLEGAL = 255
} DACLTC2655DEVENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const DACLTC2655DEF atDacLtc2655Defs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF DAcLtc2655_cfg.h */

#endif  // _DACLTC2655_CFG_H