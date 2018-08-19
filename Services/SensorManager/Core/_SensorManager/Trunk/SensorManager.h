/******************************************************************************
 * @file SensorManager.h
 *
 * @brief Sensor Manager declarations
 *
 * This file provides the declarations for the sensor manager service
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup SensorManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENSORMANAGER_H
#define _SENSORMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/SensorManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// define the errors
typedef enum _SENMANERROR
{
  SENMAN_ERROR_NONE = 0,        ///< no error
  SENMAN_ERROR_ILLENUM,         ///< illegal enumeration
  SENMAN_ERROR_VALNOTVALID,     ///< value is not a valid because of sampling
} SENMANERROR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        SensorManager_Initialize( void );
extern  void        SensorManager_ProcessScan( void );
extern  SENMANERROR SensorManager_GetValue( SENMANENUM eSenEnum, PSENMANARG pxValue );

/**@} EOF .h */

#endif  // _x_H