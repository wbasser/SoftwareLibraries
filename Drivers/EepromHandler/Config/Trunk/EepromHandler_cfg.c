/******************************************************************************
 * @file EepromHandler_cfg.c
 *
 * @brief EEPROM handler configuration implementation
 *
 * This file provoides the implementation for the EEPROM handler
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
 * \addtogroup EepromHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "EepromHandler/EepromHandler.h"

// library includes -----------------------------------------------------------

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
// OS includes
#include "FreeRtos.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
#define EEPROM_BKGNDWRITE_TASK_PRIORITY       ( tskIDLE_PRIORITY + 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
static  void  BackGroundWriteTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function EepromHandler_LclInitialize
 *
 * @brief EEPROM handler initialization
 *
 * This function will initialize the EEPROM handler
 *
 *****************************************************************************/
void EepromHandler_LclInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the background write task
  xTaskCreate( BackGroundWriteTask, "EEPROM BKGND Write", configMINIMAL_STACK_SIZE, NULL, EEPROM_BKGNDWRITE_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}

#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function EepromHandler_ProcessBackgroundWrite
 *
 * @brief process the background  writ etask
 *
 * This function will call the base background write task
 *
 * @param[in]   xArg          task argument
 *
 * @return      TRUE to flush event
 *
 *****************************************************************************/
BOOL EepromHandler_ProcessBackgroundWrite( TASKARG xArg )
{
  // call the function
  EepromHandler_BlockWriteEvent( );  
  
  // return true
  return( TRUE );
}
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function BackGroundWriteTask
 *
 * @brief background write task
 *
 * This function calls the background write function
 *
 * @param[in]   pvParameters  not used
 *
 *****************************************************************************/
static void BackGroundWriteTask( PVOID pvParameters )
{
  // main loop
  FOREVER
  {
    // call the default handler
    EepromHandler_BlockWriteEvent( );

    // sleep for the animate rate
    vTaskDelay( EEPROMHANDLER_PAGE_WRITE_MSECS / portTICK_RATE_MS );
  }
}
#endif // SYSTEMDEFINE_OS_SELECTION
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

 
/**@} EOF EepromHandler_cfg.c */
