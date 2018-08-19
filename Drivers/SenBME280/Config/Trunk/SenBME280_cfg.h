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

// local includes -------------------------------------------------------------
#include "SenBME280/SenBME280_prm.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
// global function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
#define the number of events and execution rate for this task
#define SENBME280_NUM_EVENTS                    ( 2 )
#define SENBME280_EXEC_RATE                     ( TASK_TIME_MSECS( SENBME280_SCAN_RATE_MSECS ))
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SenBME280_LocalInitialize( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
extern  BOOL  SenBME280_ProcessScanTask( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SenBME280_cfg.h */

#endif  // _SENBME280_CFG_H