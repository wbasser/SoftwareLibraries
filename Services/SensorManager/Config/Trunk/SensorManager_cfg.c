/******************************************************************************
 * @file SensorManager_cfg.c
 *
 * @brief sensor manager configuration implementation
 *
 * This file provides the sensor manager configuration implementation
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/SensorManager.h"

// library includes -----------------------------------------------------------

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
// OS includes ----------------------------------------------------------------
#include "FreeRTOS/FreeRTOSInclude.h"

// Macros and Defines ---------------------------------------------------------
/// define the priorities for the sensor manager
#define SENSORMANAGER_SCAN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
/// declare all the average filter buffers here
// SENMANAVGFILTSTORAGE( bufname, size )

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static void ScanTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION

// constant parameter initializations -----------------------------------------
const CODE SENMANDEF  atSensorMngrDefs[ SENMAN_ENUM_MAX ] =
{
  // define sensors using one of the below macros
  // SENMANINTNOFILT( chan, samprate, minraw, maxraw, minegu, maxegu )
  // SENMANINTNOFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, linfunc )
  // SENMANINTAVGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname )
  // SENMANINTAVGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, linfunc )
  // SENMANINTLDLGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc )
  // SENMANINTLDLGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, linfunc )

  // SENMANEXTNOFILT( chan, samprate, minraw, maxraw, minegu, maxegu, getfunc )
  // SENMANEXTNOFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, linfunc )
  // SENMANEXTAVGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc )
  // SENMANEXTAVGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc, linfunc )
  // SENMANEXTLDLGFILT( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc )
  // SENMANEXTLDLGFILTLIN( chan, samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, linfunc )

  // SENMANSPCNOFILT( samprate, minraw, maxraw, minegu, maxegu, getfunc )
  // SENMANSPCNOFILTLIN( samprate, minraw, maxraw, minegu, maxegu, getfunc, linfunc )
  // SENMANSPCAVGFILT( samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc )
  // SENMANSPCAVGFILTLIN( samprate, minraw, maxraw, minegu, maxegu, numsamps, bufname, getfunc, linfunc )
  // SENMANSPCLDLGFILT( samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, getfunc )
  // SENMANSPCLDLGFILTLIN( samprate, minraw, maxraw, minegu, maxegu, lagtc, leadtc, getfunc, linfunc )
  SENMANSPCNOFILT( BME280_SCAN_RATE_MSECS, 0, 0, 0, 0, SenBME280_GetPressure ),
  SENMANSPCNOFILT( BME280_SCAN_RATE_MSECS, 0, 0, 0, 0, SenBME280_GetTemperature ),
  SENMANSPCNOFILT( BME280_SCAN_RATE_MSECS, 0, 0, 0, 0, SenBME280_GetHumidity ),
};

/******************************************************************************
 * @function SensorManager_LocalInitialize
 *
 * @brief sensor manager initialization
 *
 * This function will initialize the sensor manager
 *
 *****************************************************************************/
void SensorManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the animation task
  xTaskCreate( ScanTask, "SensorScan", configMINIMAL_STACK_SIZE, NULL, SENSORMANAGER_SCAN_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
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
    SensorManager_ProcessScan( );
    
    // sleep for the required period
    vTaskDelay( SENSORMANAGER_EXECUTION_RATE / portTICK_RATE_MS );
  }
}
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function SensorManager_ScanTask
 *
 * @brief process the scan task
 *
 * This function will call the scan task to process all AtoD's
 *
 * @return    TRUE    flush all events
 *
 *****************************************************************************/
BOOL SensorManager_ScanTask( TASKARG xArg )
{
  // call the process scan task
  SensorManager_ProcessScan( );
  
  // always return true
  return( TRUE );
}
#endif // SYSTEMDEFINE_OS_SELECTION

/******************************************************************************
 * @function SensorManager_InternalAdcConvertChannel
 *
 * @brief convert an internal AtoD value
 *
 * This function will call the an internal AtoD adn return the value
 *
 * @param[in]     nChannel      channel to convert
 *
 * @return    the converted value
 *
 *****************************************************************************/
U16 SensorManager_InternalAdcConvertChannel( U8 nChannel )
{
  U16   wValue = 0;

  // return it
  return( wValue );
}

/**@} EOF SemsprMamager_cfg.c */
