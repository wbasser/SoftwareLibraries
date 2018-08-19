/******************************************************************************
 * @file SenSI7210A20_cfg.h
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
 * \addtogroup SenSI7210A20
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENSI7210A20_CFG_H
#define _SENSI7210A20_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenSI7210A20/SenSI7210A20_prm.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
// global function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
#define the number of events and execution rate for this task
#define SENSI7210A20_NUM_EVENTS                    ( 2 )
#define SENSI7210A20_EXEC_RATE                     ( TASK_TIME_MSECS( SENSI7210A20_SCAN_RATE_MSECS ))
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SenSI7210A20_LocalInitialize( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
extern  BOOL  SenSI7210A20_ProcessScanTask( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SenSI7210A20_cfg.h */

#endif  // _SENSI7210A20_CFG_H