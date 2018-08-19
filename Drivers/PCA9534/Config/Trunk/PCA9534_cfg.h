/******************************************************************************
 * @file PCA9534_cfg.h
 *
 * @brief PCA9534 configuration declarations
 *
 * This file provides the declarations for the PCA9534 module configuration
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup PCA9534
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PCA9534_CFG_H
#define _PCA9534_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PCA9534/PCA9534_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the devices
typedef enum _PCA9534DEVENUM
{
  // add user enuemrations below
  
  // do not remove the entries below
  PCA9534_DEV_ENUM_NAX,
  PCA9534_DEV_ENUM_ILLEGAL = 255
} PCA9534DEVENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE PCA9534DEVDEF  atPca9534Devs[ ];

/**@} EOF PCA9534_cfg.h */

#endif  // _PCA9534_CFG_H