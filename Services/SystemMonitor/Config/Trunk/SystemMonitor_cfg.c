/******************************************************************************
 * @file SystemMonitor_cfg.c
 *
 * @brief System Monitor configuration implemenation
 *
 * This file provides the implementations for the configuratin of the system
 * monitor
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemMonitor/SystemMonitor_cfg.h"
#include "SystemMonitor/SystemMonitor.h"

// library includes -----------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
// OS includes ----------------------------------------------------------------
#include "FreeRTOS/FreeRTOSInclude.h"

// Macros and Defines ---------------------------------------------------------
/// define the priorities for the SystemMonitor manager
#define SYSTEMMONITOR_SCAN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 3 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// local function delarations -------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static void ScanTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// local parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static U16 wPollTimeMsecs;
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// global parameter declarations ----------------------------------------------
const CODE SYSMONENTDEF atSysMonDefs[ SYSMON_ENUM_MAX ] =
{
  // add entries here using one of the macros provided
  // SYSMONTYPEU8( get, put )
  // SYSMONTYPES8( get, put )
  // SYSMONTYPEU16( get, put )
  // SYSMONTYPES16( get, put )
  // SYSMONTYPEU32( get, put )
  // SYSMONTYPES32( get, put )
  // SYSMONTYPEUFLOAT( get, put )
  // SYSMONTYPEOPTU8( get, put, option )
  // SYSMONTYPEOPTS8( get, put, option )
  // SYSMONTYPEOPTU16( get, put, option )
  // SYSMONTYPEOPTS16( get, put, option )
  // SYSMONTYPEOPTU32( get, put, option )
  // SYSMONTYPEOPTS32( get, put, option )
  // SYSMONTYPEUOPTFLOAT( get, put, option )
};

/******************************************************************************
 * @function SystemMonitor_LocalInitialize
 *
 * @brief sensor manager initialization
 *
 * This function will initialize the sensor manager
 *
 *****************************************************************************/
void SystemMonitor_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the animation task
  xTaskCreate( ScanTask, "SensorScan", configMINIMAL_STACK_SIZE, NULL, SYSTEMMONITOR_SCAN_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}

/******************************************************************************
 * @function SystemMonitor_EnableDisableTask
 *
 * @brief enable/disable task
 *
 * This function will enable and disable the task
 *
 * @param[in]   bState      desired state
 *
 *****************************************************************************/
void SystemMonitor_EnableDisableTask( BOOL bState )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    TaskManager_EnableDisable( SYSTEMMONITOR_TRANSMIT_TASK_ENUM, bState );
  #endif
}

/******************************************************************************
 * @function SystemMonitor_LocalInitialize
 *
 * @brief sensor manager initialization
 *
 * This function will initialize the sensor manager
 *
 *****************************************************************************/
void SystemMonitor_SetScanTaskTime( U16 wTaskTime )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    TaskManager_StartTimer( SYSTEMMONITOR_TRANSMIT_TASK_ENUM, wTaskTime );
  #endif
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function ScanTask
 *
 * @brief process the scan task
 *
 * This function will call the scan task to process all AtoD's
 *
 *****************************************************************************/
static void ScanTask( PVOID pvParameters )
{
  FOREVER
  {
    // call the process scan task
    SystemMonitor_ProcessScan( );
    
    // sleep for the required period
    vTaskDelay( SYSTEMMONITOR_EXECUTION_RATE / portTICK_RATE_MS );
  }
}
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function SystemMonitor_ScanTask
 *
 * @brief process the scan task
 *
 * This function will call the scan task to process all AtoD's
 *
 * @return    TRUE    flush all events
 *
 *****************************************************************************/
BOOL SystemMonitor_ScanTask( TASKARG xArg )
{
  // call the process scan task
  SystemMonitor_ProcessTransmit( );
  
  // always return true
  return( TRUE );
}
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SensorMamager_cfg.c */
