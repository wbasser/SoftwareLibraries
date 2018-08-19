/******************************************************************************
 * @file SensorManager_cfg.h
 *
 * @brief sensor manager configuration declarations
 *
 * This file provides the configuration declarations for the sensor manager
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
#ifndef _SENSORMANAGER_CFG_H
#define _SENSORMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/SensorManager_prm.h"
#include "SensorManager/SensorManager_def.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// define the number of arguments for the scan process
#define SENSORMANAGER_SCAN_NUM_EVENTS        ( 4 )
#define SENSORMANAGER_SCAN_EXEC_RATE         ( TASK_TIME_MSECS( SENSORMANAGER_EXECUTION_RATE ))
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

// enumerations ---------------------------------------------------------------
/// enumerate each sensor
typedef enum _SENMANENUM
{
  // enumerate user defined sensor entries here
  
  // do not remove the below entries
  SENMAN_ENUM_MAX,
  SENMAN_ENUM_ILLEGAL = 0xFF,
} SENMANENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE SENMANDEF  atSensorMngrDefs[ ];

// global function prototypes --------------------------------------------------
extern  void  SensorManager_LocalInitialize( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  SensorManager_ScanTask( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER
extern  U16   SensorManager_InternalAdcConvertChannel( U8 nChannel );

/**@} EOF SensorManager_cfg.h */

#endif  // _SENSORMANAGER_CFG_H