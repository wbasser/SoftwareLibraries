/******************************************************************************
 * @file ButtonManager_cfg.c
 *
 * @brief button manager configuration implementation
 *
 * This file provides the configuration for the button manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define BTNMANAGER_PROCESS_TASK_PRIORITY              ( tskIDLE_PRIORITY + 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// fill out the defaults for the config
const CODE BTNMNGRCFG tBtnMgrCfg = 
{
  // BTNMNGR_CFG_ENTRY( debnc, repdly, reprate, sh_time, mh_time, lh_time, stuck_time )
  BTNMNGR_CFG_ENTRY( 50, 500, 100, 1000, 2000, 3000, 7000 )
};

/// fill out the button defs
const CODE BTNMNGRDEF atBtnMgrDefs[ BTNMNGR_ENUM_MAX ]  =
{
  // BTNMNGR_DEFCB_ENTRY( keyenum,             rel_enb, prs_enb, rep_enb, shh_enb, mdh_enb, lng_enb, tgl_enb, getstatus, callback )
  // BTNMNGR_DEFEVENT_ENTRY( keyenum,          rel_enb, prs_enb, rep_enb, shh_enb, mdh_enb, lng_enb, tgl_enb, getstatus, task )
};

/******************************************************************************
 * @function ButtonManager_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any custom initialization
 *
 *****************************************************************************/
void ButtonManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the animation task
  xTaskCreate( ProcessTask, "ButtonProcess", configMINIMAL_STACK_SIZE, NULL, BTNMANAGER_PROCESS_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function ProcessTask
 *
 * @brief process task
 *
 * This function is the process task
 *
 * @param[in]   pvParameters  not used
 *
 *****************************************************************************/
static void ProcessTask( PVOID pvParameters )
{
  // main loop
  FOREVER
  {
    // call the default handler
    ButtonManager_Process( );

    // sleep for the animate rate
    vTaskDelay( LEDMANAGER_ANIMATE_RATE_MSECS / portTICK_RATE_MS );
  }
}
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKAMANAGER )
/******************************************************************************
 * @function BUttonManager_ProcessTask
 *
 * @brief process task
 *
 * This function will call the animation process
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL ButtonManager_ProcessTask( TASKARG xArg )
{
  // call the animation process
  ButtonManager_Process( );
  
  // return true
  return( TRUE );
}
#endif  // SYSTEMDEFINE_OS_SELECTION

/**@} EOF ButtonManager_cfg.c */
