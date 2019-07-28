/*****************************************************************************
 * @file LedManager_cfg.c
 *
 * @brief LED manager configuration implementation 
 *
 * This file provides the implementation for the LED manager configuration
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
 * $Log: $
 * 
 *
 * \addtogroup LedManager
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "LedManager/LedManager.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
// OS includes
#include "FreeRtos.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define LED_ANIMATE_TASK_PRIORITY             ( tskIDLE_PRIORITY + 4 )
#define LED_SCAN_TASK_PRIORITY                ( tskIDLE_PRIORITY + 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static  void  AnimateTask( PVOID pvParameters );
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
static  void  ScanTask( PVOID pvParameters );
#endif  // MATRIX DEFS
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// constant parameter initializations -----------------------------------------
/// declare the led definitions
const CODE LEDDEF  g_atLedDefs[ LEDMANAGER_ENUM_MAX ] = 
{
  // populate using one or both of the helper macros
  // LEDDEF_DIRECT( pin )
  // LEDDEF_MATRIX( row, col )
  // LEDDEF_SPECIAL( func, col )
};

#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
const CODE LEDRGBDEF  g_atLedRgbDefs[ LEDMANAGER_RGBENUM_MAX ] =
{
  // popuate usinig the macro below
  // LEDMNGRRGBDEF( red, grn, blu )
};
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/// fill the matrix row columns 
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
const CODE GPIOPINENUM g_aeLedMatrixRows[ LEDMANAGER_MATRIX_MAX_NUM_ROWS ] =
{
};

const CODE GPIOPINENUM g_aeLedMatrixCols[ LEDMANAGER_MATRIX_MAX_NUM_COLS ] =
{
};
#endif  // MATRIX DEFS

/// define each sequence here
/// LEDMNGRDEFSTART( name )
/// LEDMNGRDEFSNGSENTRY,
/// LEDMNGRDEFSNGSTEP( action, led, duration, nextevent, option )
/// LEDMNGRDEFRGBSTEP( action, led, color, duration, nextevent, option )


/// declare the animation definitions
const CODE PLEDSEQENTRY g_apLedAnimationsDef[ LEDMNGR_ANIMATION_MAX ] =
{
  /// fill animations here using the below helper
  /// LEDMNGRANIMATIONDEF( name )]]
};

/******************************************************************************
 * @function LedManager_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any custom initialization
 *
 *****************************************************************************/
void LedManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the animation task
  xTaskCreate( AnimateTask, "LedAnimation", configMINIMAL_STACK_SIZE, NULL, LED_ANIMATE_TASK_PRIORITY, NULL );
  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
  xTaskCreate( ScanTask, "LedMatrixScan", configMINIMAL_STACK_SIZE, NULL, LED_SCAN_TASK_PRIORITY, NULL );
  #endif  // MATRIX DEFS
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}


/******************************************************************************
 * @function LedManager_NotifyDone
 *
 * @brief animation done notification
 *
 * This function will be called to notify when an animation is done
 *
 *****************************************************************************/
void LedManager_NotifyDone( void )
{
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function AnimateTask
 *
 * @brief animation task
 *
 * This function is the animation task
 *
 * @param[in]   pvParameters  not used
 *
 *****************************************************************************/
static void AnimateTask( PVOID pvParameters )
{
  // main loop
  FOREVER
  {
    // call the default handler
    LedManager_ProcessAnimation( );

    // sleep for the animate rate
    vTaskDelay( LEDMANAGER_ANIMATE_RATE_MSECS / portTICK_RATE_MS );
  }
}

#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
/******************************************************************************
 * @function ScanTask
 *
 * @brief scan task
 *
 * This function is the animation task
 *
 * @param[in]   pvParameters  not used
 *
 *****************************************************************************/
static void ScanTask( PVOID pvParameters )
{
  // main loop
  FOREVER
  {
    // call the default handler
    LedManager_ProcessScan( );

    // sleep for the animate rate
    vTaskDelay( LEDMANAGER_SCAN_RATE_MSEC / portTICK_RATE_MS );
  }
}
#endif  // MATRIX DEFS
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKAMANAGER )
/******************************************************************************
 * @function LedManager_AnimationTask
 *
 * @brief animation task
 *
 * This function will call the animation process
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL LedManager_AnimationTask( TASKARG xArg )
{
  // call the animation process
  LedManager_ProcessAnimation( );
  
  // return true
  return( TRUE );
}

#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
/******************************************************************************
 * @function LedManager_ScanTask
 *
 * @brief scan task
 *
 * This function will be call the scan task
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL LedManager_ScanTask( TASKARG xArg )
{
  // call the animation process
  LedManager_ProcessScan( );
  
  // return true
  return( TRUE );
}
#endif  // MATRIX DEFS
#endif  // SYSTEMDEFINE_OS_SELECTION

/**@} EOF LedManager_cfg.c */
