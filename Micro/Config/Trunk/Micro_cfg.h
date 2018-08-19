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
#include "Types/Types.h"

// global function prototypes
extern  BOOL  Micro_CheckTasksPending( void );
extern  void  Micro_EnterSleepMode( void );

// global parameter declarations -----------------------------------------------
/// declare the initialization -IRQ disabled structure
extern  const CODE PVMICROINITFUNC    apvInitIrqDsbFunctions[ ];

/// declare the initialization -IRQ enabled structure
extern  const CODE PVMICROINITFUNC    apvInitIrqEnbFunctions[ ];

/// declare the idle structure
extern  const CODE PVMICROIDLEFUNC    apvIdleFunctions[ ];

/// declare the shutdown structure
extern  const CODE PVMICROSHUTDNFUNC  apvShutdownFunctions[ ];

// global function declarations ---------------------------------------------------
extern  BOOL  Micro_CheckTasksPending( void );
extern  void  Micro_EnterSleepMode( void );
extern  void  Micro_LocalInitialize( void );

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  extern void vApplicationStackOverflowHook( xTaskHandle *pxTask,signed char *pcTaskName);
  extern void vApplicationIdleHook(void);
  extern void vApplicationTickHook(void);
  extern void vApplicationMallocFailedHook(void);
#endif

/**@} EOF Micro_cfg.c */

#endif // _MICRO_CFG_H
