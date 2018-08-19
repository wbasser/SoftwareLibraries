/******************************************************************************
 * @file HysteresisControl_def.h
 *
 * @brief Hysteresis control configuration definitions 
 *
 * This file provides the defnitions for configuring the hysteresis control
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
#ifndef _HYSTERESISCONTROL_DEF_H
#define _HYSTERESISCONTROL_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "SensorManager/SensorManager.h"
#include "RelayManager/RelayManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining a hysteresis control
#define HYSCONTROLDEFM( execrate, sensor, relay, poshyst, neghyst ) \
  { \
    .uExecutionRate = execrate, \
    .eSensorEnum = sensor, \
    .eRelayEnum = relay, \
    .iPosHysteresis = poshyst, \
    .iNegHysteresis = neghyst, \
  }
  
// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the definition structure
typedef struct _HYSTCONTROLDEF
{
  U32           uExecutionRate;
  SENMANENUM    eSensorEnum;
  RELAYENUM     eRelayEnum;
  S16           iPosHysteresis;
  S16           iNegHysteresis;
} HYSTCONTROLDEF, *PHYSTCONTROLDEF;
#define HYSTCONTROLDEF_SIZE           sizeof( HYSTCONTROLDEF )

/**@} EOF HysteresisControl_def.h */

#endif  // _HYSTERESISCONTROL_DEF_H