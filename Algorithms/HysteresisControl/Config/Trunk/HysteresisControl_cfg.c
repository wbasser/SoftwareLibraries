/******************************************************************************
 * @file HysteresisControl_cfg.c
 *
 * @brief Hystereiss control configuration implementation 
 *
 * This file provides the configuration implementation for the Hysteresis
 * control
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
 * \addtogroup HysteresisControl
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HysteresisControl/HysteresisControl_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE HYSTCONTROLDEF atHystControlDefs[ HYST_CONTROL_ENUM_MAX ] =
{
  // define controls using the below helper macro
  // YSCONTROLDEFM( execrate, sensor, relay, poshyst, neghyst )
};
 
/**@} EOF HysteresisControl_cfg.c */
