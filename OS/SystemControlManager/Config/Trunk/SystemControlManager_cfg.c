/******************************************************************************
 * @file SystemControlManager_cfg.c
 *
 * @brief system control manager configuration implementation
 *
 * This file provides the implementation for the system control manager configuration
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
 * $Rev: $
 * 
 *
 * \addtogroup SystemControlManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemControlManager/SystemControlManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define SYSCTRL_TASK_PRIORITY             ( tskIDLE_PRIORITY + 4 )
#define SYSCTRL_QUEUE_SIZE                ( 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static  void  SystemControlTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
static  void  LclInitEntry( void );
static  void  LclConfigEntry( void );
static  void  LclPowRcvEntry( void );
static  void  LclStandbyEntry( void );
static  void  LclIdleEntry( void );
static  void  LclRunEntry( void );
static  void  LclErrorEntry( void );

// constant parameter initializations -----------------------------------------
/// declare the control manager definitions
const CODE  SYSCTRLMNGRDEF atSysCtrlMngrDefs[ ] = 
{
  // SYSCNTRLMNGR_DEF( task, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15, mode16, func ) 
};

/// declare the functions for each mode
const CODE  SYSCTRLMGRENTCHK atSysCtrlMngrEntChkFuncs[ SYSCTRLMNGR_MODE_MAX ] = 
{
  SYSCTRLMNGGR_ENTCHK( LclInitEntry,    NULL            ),   ///< initialization
  SYSCTRLMNGGR_ENTCHK( LclConfigEntry,  NULL            ),   ///< configuration
  SYSCTRLMNGGR_ENTCHK( LclPowRcvEntry,  NULL            ),   ///< power recovery
  SYSCTRLMNGGR_ENTCHK( LclStandbyEntry, NULL            ),   ///< standby
  NULL,   ///< idle
  NULL,   ///< run
  NULL,   ///< power loss
  NULL,   ///< error
  NULL,   ///< undefined
  NULL,   ///< undefined
  NULL,   ///< undefined
  NULL,   ///< undefined
  NULL,   ///< undefined
  NULL,   ///< undefined
  NULL,   ///< undefined
  NULL,   ///< undefined
};

/******************************************************************************
 * @function SystemControlManager_LocalInitialize
 *
 * @brief initilize the system control manager
 *
 * This function will perform the initialization of the system control manager
 * and enter the initialization state
 *
 *****************************************************************************/
void SystemControlManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the task and queue for the system control manager
  xTaskCreate( SystemControlTask, "SystemControl", configMINIMAL_STACK_SIZE, NULL, SYSCTRL_TASK_PRIORITY, NULL );
  xSystemControlManagerQueue = xQueueCreate( SYSCTRL_QUEUE_SIZE, sizeof( STATEEXECENGARG ));
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function SystemControlManage_GetNumberDefs
 *
 * @brief get then number of entries
 *
 * This function will return the number of entries in the def table
 *
 *****************************************************************************/
U8 SystemControlManager_GetNumberDefs( void )
{
  // return the number of entrys
  return( sizeof( atSysCtrlMngrDefs ) / SYSCTRLMNGRDEF_SIZE );
}

/******************************************************************************
 * @function LclInitEntry
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
static void LclInitEntry( void )
{
  STATEEXECENGARG xStateArg;
  
  // set the event
  xStateArg = SYSCTRLMNGR_EVENT_INITDONE;
  
  // mark local initialization done
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xQueueSendToBack( xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
  #else
  TaskManager_PostEvent( TASK_SCHD_ENUM_SYSCTL, xStateArg );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LclConfigEntry
 *
 * @brief LclConfigEntry
 *
 * This function 
 *
 *****************************************************************************/
static void LclConfigEntry( void )
{
  STATEEXECENGARG xStateArg;

  // set the event
  xStateArg = SYSCTRLMNGR_EVENT_CONFIGDONE;
  
  // enable the log handler
  LogHandler_Initialize( );

  // initialize the config/parameter managers
  //ParameterManager_Initialize( );
  //ConfigManager_Initialize( );
  
  // post a config done
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xQueueSendToBack( xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
  #else
  TaskManager_PostEvent( TASK_SCHD_ENUM_SYSCTL, xStateArg );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LclPowRcvEntry
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
static void LclPowRcvEntry( void )
{
  STATEEXECENGARG xStateArg;

  // set the event
  xStateArg = SYSCTRLMNGR_EVENT_GOIDLE;
  
  // move to idle
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  xQueueSendToBack( xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
  #else
  TaskManager_PostEvent( TASK_SCHD_ENUM_SYSCTL, xStateArg );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LclStandbyEntry
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
static void LclStandbyEntry( void )
{
}

/******************************************************************************
 * @function LclSpcExit
 *
 * @brief exit special modes
 *
 * This function will test for a valid mode and return true if ok
 *
 * @param[in]     eNewMode      newmode
 *
 * @return        TRUE if valid mode, FALSE if not
 *
 *****************************************************************************/
static BOOL LclSpcExit( SYSCTRLMGRMODE eNewMode )
{
  BOOL bStatus = FALSE;

  // test for valid mode
  if ( eNewMode == SYSCTRLMNGR_MODE_04_IDLE )
  {
    // set status OK
    bStatus = TRUE;
  }

  // return the status
  return( bStatus );
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function SystemControlTask
 *
 * @brief system control task
 *
 * This function is the system control task
 *
 * @param[in]   pvParameters  not used
 *
 *****************************************************************************/
static void SystemControlTask( PVOID pvParameters )
{
  STATEEXECENGARG xStateArg;
  
  // main loop
  FOREVER
  {
    // wait for an queue post
    xQueueReceive( xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
    
    // now call the event handler with the event
    SystemControlManager_ProcessEvent( xStateArg );
  }
}
#endif  // SYSTEMDEFINE_OS_SELECTION

/**@} EOF LedManager_cfg.c */

/**@} EOF SystemControlManager_cfg.c */
