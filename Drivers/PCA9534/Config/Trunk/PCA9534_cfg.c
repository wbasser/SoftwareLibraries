/******************************************************************************
 * @file PCA9534_cfg.c
 *
 * @brief PCA9534 port expander configuration implementation 
 *
 * This file provides the configuration for the PCA9534 port expander module
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PCA9534/PCA9534_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE PCA9534DEVDEF  atPca9534Devs[ PCA9534_DEV_ENUM_NAX ] =
{
  // configure a device and then configure the number of pins
  // DEFPCA9534DEV( i2cdev, addr, dir0, inv0, init0, dir1, inv1, init1, dir2, inv2, init2, dir3, inv3, init3, dir4, inv4, init4, dir5, inv5, init5, dir6, inv6, init6, dir7, inv7, init7 )
};

/**@} EOF PCA9534_cfg.c */
