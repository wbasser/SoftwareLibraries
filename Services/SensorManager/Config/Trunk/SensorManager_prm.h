/******************************************************************************
 * @file SensorManager_prm.h
 *
 * @brief Sensor Manager parameter declarations
 *
 * This file declares any customization for the syensor manager
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
 * \addtogroup SensorManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENSORMANAGER_PRM_H
#define _SENSORMANAGER_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the sensor manager execution rate in MSEC
#define SENSORMANAGER_EXECUTION_RATE_MSEC       ( 5 )

/// define the filter enable flags
#define SENSORMANAGER_AVERAGE_FILTER_ENABLE     ( 0 )
#define SENSORMANAGER_LEADLAG_FILTER_ENABLE     ( 0 )
#define SENSORMANAGER_FIR_FILTER_ENABLE         ( 0 )
#define SENSORMANAGER_IIR_FILTER_ENABLE         ( 0 )
#define SENSORMANAGER_USR_FILTER_ENABLE         ( 0 )

/// define the argument types
#define SENSORMANAGER_TYPE_SIGNED16             ( 0 )
#define SENSORMANAGER_TYPE_SIGNED32             ( 1 )
#define SENSORMANAGER_TYPE_FLOAT                ( 2 )

/// define the sensor manager argument type using on eof the three above defines
#define SENSORMANAGER_ARGUMENT_TYPE             ( SENSORMANAGER_TYPE_SIGNED16 )

/**@} EOF SensorManager_prm.h */

#endif  // _SENSORMANAGER_PRM_H