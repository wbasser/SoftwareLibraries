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

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  #include "ManufInfo/ManufInfo.h"
#endif // SYSTEMDEFINE_OS_SELECTION

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
  // CONFIGMGRFIXBLKDEF( size, getdefault, getactual )
  // CONFIGMGRVARBLKDEF( getsize, getdefault, getactual )
};

// local parameter declarations -----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  static  xQueueHandle  xConfigUpdateQueue;
  static  xTaskHandle   xConfigTaskHandle;
#elif (SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
  static  PTASKMINIMALHANDLE  ptUpdateHandle;
#endif // SYSTEMDEFINE_OS_SELECTION

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  static  void  UpdateTask( PVOID pvParameters );
#elif (SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
  static  BOOL  UpdateTask( TASKMINIMALEVENT xEvent );
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
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
    ptUpdateHandle = TaskMinimal_Create( UpdateTask, 0, 2, TRUE );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function ConfigManager_GetVerMajor
 *
 * @brief get the software version major
 *
 * This function will return the software version major
 *
 * @return        version major
 *
 *****************************************************************************/
U8 ConfigManager_GetVerMajor( void )
{
  U8  nValue = 0;
  
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    #if ( CONFIGMGR_ENABLE_CONFIGVER == ON )
      nValue = CONFIGMGR_CONFIG_VERMAJ;
    #else
      nValue = ManufInfo_GetSfwMajor( );
    #endif //CONFIGMGR_ENABLE_CONFIGVER
  #endif // SYSTEMDEFINE_OS_SELECTION
  
  // return the software major
  return( nValue );
}

/******************************************************************************
 * @function ConfigManager_GetVerMajor
 *
 * @brief get the software version major
 *
 * This function will return the software version major
 *
 * @return        version major
 *
 *****************************************************************************/
U8  ConfigManager_GetVerMinor( void )
{
  U8  nValue = 0;
  
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    #if ( CONFIGMGR_ENABLE_CONFIGVER == ON )
      nValue = CONFIGMGR_CONFIG_VERMIN;
    #else
      nValue = ManufInfo_GetSfwMinor( );
    #endif //CONFIGMGR_ENABLE_CONFIGVER
  #endif // SYSTEMDEFINE_OS_SELECTION
  
  // return the software major
  return( nValue );
}

/******************************************************************************
 * @function ConfigManager_RdWord
 *
 * @brief read a word from the eeprom
 *
 * This function will return read a word from the EEPROM
 *
 * @param[in]     wAddress        address to read
 * @param[io]     pwValue         pointer to store the value read
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ConfigManager_RdWord( U16 wAddress, PU16 pwValue )
{
  BOOL bError = FALSE;

  // return the error
  return( bError );
}

/******************************************************************************
 * @function ConfigManager_RdBlock
 *
 * @brief read a block from the eeprom
 *
 * This function will read a block from the EEPROM
 *
 * @param[in]     wAddress        address to read
 * @param[in]     wLength         length to read
 * @param[io]     pnValue         pointer to store the value read
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ConfigManager_RdBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  BOOL bError = FALSE;

  // return the error
  return( bError );
}

/******************************************************************************
 * @function ConfigManager_WrWord
 *
 * @brief write a word from the eeprom
 *
 * This function will return write a word to the EEPROM
 *
 * @param[in]     wAddress        address to write
 * @param[io]     wValue          the value to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ConfigManager_WrWord( U16 wAddress, U16 wValue )
{
  BOOL bError = FALSE;

  // return the error
  return( bError );
}

/******************************************************************************
 * @function ConfigManager_WrBlock
 *
 * @brief write a block to the eeprom
 *
 * This function will wriute a block to the EEPROM
 *
 * @param[in]     wAddress        address to write
 * @param[in]     wLength         length to write
 * @param[io]     pnValue         pointer to store the value write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL ConfigManager_WrBlock( U16 wAddress, U16 wLength, PU8 pnData )
{
  BOOL bError = FALSE;

  // return the error
  return( bError );
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
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function UpdateTask
 *
 * @brief call the update task
 *
 * This function will call the config update function
 *
 * @param[in]     pvParmaeters    pointer to the parmaeters
 *
 *****************************************************************************/
static void UpdateTask( PVOID pvParameters )
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
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
/******************************************************************************
 * @function UpdateTask
 *
 * @brief call the update task
 *
 * This function will call the config update function
 *
 * @param[in]     xEvent      event
 *
 *****************************************************************************/
static BOOL UpdateTask( TASKMINIMALEVENT xEvent )
{
  // call the update
  ConfigManager_UpdateConfig(( CONFIGTYPE )xEvent );

  // always return true
  return( TRUE );
}
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF ConfigManager_cfg.c */
