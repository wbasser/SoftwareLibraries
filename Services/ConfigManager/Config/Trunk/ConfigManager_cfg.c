/******************************************************************************
 * @file ConfigManager_cfg.c
 *
 * @brief configuration manager config implementation
 *
 * This file provides the custimazation implementation
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
 *
 * \addtogroup ConfigManager_cfg.c
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ConfigManager/ConfigManager.h"
#include "ConfigManager/ConfigManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define CONFIGMANAGER_UPDATE_TASK_PRIORITY                ( tskIDLE_PRIORITY + 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
/// config block definitions 
const CODE CONFIGMGRBLKDEF atConfigDefs[ CONFIG_TYPE_MAX ] =
{
  /// create definitions here wih the below macro
  // CONFIGMGRBLKDEF( size, getdefault, getactual )
};

// local parameter declarations -----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static  xQueueHandle  xConfigUpdateQueue;
static  xTaskHandle   xConfigTaskHandle;
#endif // SYSTEMDEFINE_OS_SELECTION

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static  void ConfigManager_UpdateTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION

/******************************************************************************
 * @function ConfigManager_LocalInitialize
 *
 * @brief initialize the configuration manager
 *
 * This function will pdrform any system dependent initialization
 *
 *****************************************************************************/
void ConfigManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the task/queue for the update of a config block
  xConfigUpdateQueue = xQueueCreate( CONFIG_TYPE_MAX, sizeof( CONFIGTYPE ));
  xTaskCreate( &ConfigManager_UpdateTask, "Config Manager Update Task", configMINIMAL_STACK_SIZE, NULL, CONFIGMANAGER_UPDATE_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function ConfigManager_ProcessUpdate
 *
 * @brief call the update task
 *
 * This function will call the config update function
 *
 * @param[in]     xArg      configuration block
 *
 * @return        TRUE
 *
 *****************************************************************************/
BOOL ConfigManager_ProcessUpdate( TASKARG xArg )
{
  // call the update function
  ConfigManager_UpdateConfig(( CONFIGTYPE )xArg );
  
  // always return true
  return( TRUE );
}
#endif // SYSTEMDEFINE_OS_SELECTION

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function ConfigManager_UpdateTask
 *
 * @brief call the update task
 *
 * This function will call the config update function
 *
 * @param[in]     pvParmaeters    pointer to the parmaeters
 *
 *****************************************************************************/
static void ConfigManager_UpdateTask( PVOID pvParameters )
{
  CONFIGTYPE eConfigType;
  
  FOREVER
  {
    // get the value from the queue
    xQueueReceive( xConfigUpdateQueue, &eConfigType, portMAX_DELAY );
    
    // call the task
    ConfigManager_UpdateConfig( eConfigType );
  }
}
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF ConfigManager_cfg.c */
