/******************************************************************************
 * @file Micro_cfg.h
 *
 * @brief configuration file declarations
 *
 * This file declares the initialization structures for main
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
 * \addtogroup Micro
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _MICRO_CFG_H
#define _MICRO_CFG_H

// local includes -------------------------------------------------------------
#include "Micro/Micro_def.h"

// library includes -----------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// macro declarations ---------------------------------------------------------
#define MICRO_ENABLE_SYSTIMECMDS                          ( 0 )

// global parameter declarations -----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  /// declare the initialization -IRQ disabled structure
  extern  const CODE PVMICROINITFUNC    g_apvInitIrqDsbFunctions[ ];

  /// declare the initialization -IRQ enabled structure
  extern  const CODE PVMICROINITFUNC    g_apvInitIrqEnbFunctions[ ];

  /// declare the idle structure
  extern  const CODE PVMICROIDLEFUNC    g_apvIdleFunctions[ ];

  /// declare the shutdown structure
  extern  const CODE PVMICROSHUTDNFUNC  g_apvShutdownFunctions[ ];
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// global function declarations ---------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  extern  BOOL  Micro_CheckTasksPending( void );
  extern  void  Micro_EnterSleepMode( void );
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
extern  void  Micro_LocalInitialize( void );
extern  void  Micro_LocalIrqInitialize( void );
extern  void  Micro_LocalIdle(void);

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  extern void vApplicationStackOverflowHook( xTaskHandle *pxTask,signed char *pcTaskName);
  extern void vApplicationIdleHook(void);
  extern void vApplicationTickHook(void);
  extern void vApplicationMallocFailedHook(void);
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

/**@} EOF Micro_cfg.c */

#endif // _MICRO_CFG_H
