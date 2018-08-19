/******************************************************************************
 * @file HysteresisControl_cfg.h
 *
 * @brief   hysteresis control configuration declarations
 *
 * This file provides the declarations for the configuration of the hysteresis
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
 
// ensure only one instantiation
#ifndef _HYSTERESISCONTROL_CFG_H
#define _HYSTERESISCONTROL_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HysteresisControl/HysteresisControl_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the hysteresis controls
typedef enum _HYSTCONTROLENUM
{
  // enumerate user controls here
  
  // do not remove the below entries
  HYST_CONTROL_ENUM_MAX,
  HYST_CONTROL_ENUM_ILLEGAL = 255
} HYSTCONTROLENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE HYSTCONTROLDEF atHystControlDefs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF HysteresisControl_cfg.h */

#endif  // _HYSTERESISCONTROL_CFG_H