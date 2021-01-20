/******************************************************************************
 * @file SenBME280_cfg.h
 *
 * @brief BME280 sensor configuration declarations
 *
 * This file provides the declarations for the configuration of the BME280
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
 * \addtogroup SenBME280
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENBME280_CFG_H
#define _SENBME280_CFG_H

// system includes ------------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// local includes -------------------------------------------------------------
#include "SenBME280/SenBME280.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
/// define the scan rate for the sensor
#define SENBME280_SCAN_RATE_MSECS               ( 500 )

/// define the macro to allow for compensation at sensor measurement
#define SENBME280_COMPENSATE_AT_MEASURE         ( TRUE )

// global function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #define the number of events and execution rate for this task
  #define SENBME280_NUM_EVENTS                    ( 2 )
  #define SENBME280_EXEC_RATE                     ( TASK_TIME_MSECS( SENBME280_SCAN_RATE_MSECS ))
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SenBME280_LocalInitialize( void );
extern  BOOL    SenBME280_WriteRegister( U8 nRegister, U8 nData );
extern  BOOL    SenBME280_ReadRegisters( U8 nRegister, PU8 pnData, U8 nLength );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  extern  BOOL  SenBME280_ProcessScanTask( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SenBME280_cfg.h */

#endif  // _SENBME280_CFG_H