/******************************************************************************
 * @file SenSI7210A20_cfg.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup SenSI7210A20_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenSI7210A20/SenSI7210A20.h"

// library includes -----------------------------------------------------------

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_FREERTOS )
// OS includes ----------------------------------------------------------------
#include "FreeRTOS/FreeRTOSInclude.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_FREERTOS )
#define SENSI7210A20_SCAN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 7 )
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_FREERTOS )
#endif // SYSTEMDEFINE_OS_SELECTION

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SenSI7210A20_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed local initialization
 *
*****************************************************************************/
void SenSI7210A20_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_FREERTOS )
  // create the animation task
  xTaskCreate( ScanTask, "SenSI7210A20Scan", configMINIMAL_STACK_SIZE, NULL, SENSI7210A20_SCAN_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}
 
 #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_TASKMANAGER )
/******************************************************************************
 * @function SenSI7210A20_ProcessScanTask
 *
 * @brief scan task
 *
 * This function provides the handler for scanning the sensor
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE flush event
 *
 *****************************************************************************/
BOOL SenSI7210A20_ProcessScanTask( TASKARG xArg )
{
  // call the local scan task
  SenSI7210A20_ProcessScan( );

  // always return ture
  return( TRUE );
}
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_USE_FREERTOS )
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
    SenSI7210A20_ProcessScan_ProcessScan( );
    
    // sleep for the required period
    vTaskDelay( SENSI7210A20_SCAN_RATE_MSECS / portTICK_RATE_MS );
  }
}
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SenSI7210A20_cfg.c */
