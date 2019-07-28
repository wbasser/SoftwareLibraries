/******************************************************************************
 * @file Micro_cfg.c
 *
 * @brief Micro configuration structures
 *
 * This file conatins the initilization structures
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
 * \addtogroup Micro
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "MicroConfig/Micro_cfg.h"

// library includes -------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

// include HAL files

// include driver files

// include OS files

// include protocol file

// include Services files

// include application files

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
/// initialization function - interrupts disabled
const CODE PVMICROINITFUNC g_apvInitIrqDsbFunctions[ ] = 
{
  /// base HAL initializations here

  /// add HAL initializlations here
  
  /// add OS initializations here
  
  /// add driver initializations here
  
  /// add protocol initialization here

  /// add services initializations here

  /// add application initializations here

  // do not remove this entry
  NULL
};

/// initialization function - interrupts enabled
const CODE PVMICROINITFUNC g_apvInitIrqEnbFunctions[ ] =
{
  // add initialization funtions that required interrupts to be enable eher

  // do not remove this entry
  NULL
};

/// idle functions
const CODE PVMICROIDLEFUNC g_apvIdleFunctions[ ] = 
{
  // add idle functions here
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    TaskManager_IdleProcess,
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

  // do not remove this entry
  NULL
};

/// shutdown functions
const CODE PVMICROSHUTDNFUNC  g_apvShutdownFunctions[ ] =
{
  // add shutdown functions here
 
  // do not remove this entry
  NULL
};

/******************************************************************************
 * @function MicroCheckTasksPendingf
 *
 * @brief tests for tasks pending
 *
 * This function checks to see if tasks are pending
 *
 * @return  TRUE if tasks are pending
 *
 *****************************************************************************/
BOOL Micro_CheckTasksPending( void )
{
  BOOL bResult = TRUE;
  
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    bResult = TaskManager_CheckTasksPending( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER

  // return the result
  return( bResult );
}

/******************************************************************************
 * @function Micro_EnterSleepMode
 *
 * @brief go to sleep
 *
 * This function will call he sleep manager in the HAL and put the micro processor
 * to sleep if low power mode is desired
 *
 *****************************************************************************/
void Micro_EnterSleepMode( void )
{
}
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

/******************************************************************************
 * @function Micro_LocalInitialize
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void  Micro_LocalInitialize( void )
{
}  

/******************************************************************************
 * @function Micro_LocalIrqInitialize
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void  Micro_LocalIrqInitialize( void )
{
}  

/******************************************************************************
 * @function Micro_LocalIdle
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void Micro_LocalIdle(void)
{
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed char *pcTaskName)
  {
    while(1);
  }
  
  void vApplicationIdleHook(void)
  {
    while(1);
  }
  
  void vApplicationTickHook(void)
  {
    while(1);
  }
  
  void vApplicationMallocFailedHook(void)
  {
    while(1);
  }
  
#endif

/**@} EOF Micro_cfg.c */
