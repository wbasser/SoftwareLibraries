/******************************************************************************
 * @file SystemMonitor_cfg.h
 *
 * @brief System monitor configuration declarations
 *
 * This file provides the declarations for the system monitor configurations
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
 * \addtogroup SystemMonitor
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SYSTEMMONITOR_CFG_H
#define _SYSTEMMONITOR_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemMonitor/SystemMonitor_def.h"
#include "SystemMonitor/SystemMonitor_prm.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

// macros/defines -------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define SYSTEMMONITOR_TRANSMIT_NUM_EVENTS               ( 2 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

// enumerations ---------------------------------------------------------------
/// enumerate the system monitor variables
typedef enum  _SYSMONENUM
{
  // add your enumerations below
  
  // do not remove these entries
  SYSMON_ENUM_MAX,
  SYSMON_ENUM_ILLEGAL
} SYSMONENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE SYSMONENTDEF atSysMonDefs[ ];

// global function prototypes --------------------------------------------------
extern  void  SystemMonitor_LocalInitialize( void );
extern  void  SystemMonitor_EnableDisableTask( BOOL bState );
extern  void  SystemMonitor_SetScanTaskTime( U16 wTaskTime );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  SystemMonitor_ScanTask( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SystemMonitor_cfg.h */

#endif  // _SYSTEMMONITOR_CFG_H