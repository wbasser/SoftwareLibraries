/******************************************************************************
 * @file AfeLMP9100X_cfg.h
 *
 * @brief Analog Front End LMP9100X configuration declarations 
 *
 * This file provides the declarations for the configuration of the Analog
 * Front End LMP9100X
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
 * \addtogroup AfeLMP9100X
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AFELMP9100X_CFG_H
#define _AFELMP9100X_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AfeLMP9100X/AfeLMP9100X_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the devices
typedef enum _AFELMP9100ENUM
{
  // enumerate user devices here
  
  // do not remove these entries
  AFELMP9100_ENUM_MAX,
  AFELMP9100_ENUM_ILLEGAL = 0xFF
} AFELMP9100ENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE AFELMLP9100XCTLDEF atAfeLMP9100XDefs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF AfeLMP9100X_cfg.h */

#endif  // _AFELMP9100X_CFG_H