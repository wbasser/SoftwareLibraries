/******************************************************************************
 * @file SystemControlManager_cfg.h
 *
 * @brief system control manager configuration declarations
 *
 * This file declares the system control manager configurations
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
 
// ensure only one instantiation
#ifndef _SYSTEMCONTROLMANAGER_CFG_H
#define _SYSTEMCONTROLMANAGER_CFG_H

// local includes -------------------------------------------------------------
#include "SystemControlManager/SystemControlManager_def.h"

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
extern  QueueHandle_t g_xSystemControlManagerQueue;
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
#include "StateExecutionEngine/StateExecutionEngine.h"

// macro and defines ------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// define the process event  size
#define SYSCTRLMNGR_TASK_NUM_EVENTS               ( 4 )
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------
/// enumerate the local modes
typedef enum _SYSCTRLMGRLCLMODE
{
  SYSCTRLMNGR_LCLMODE_INITIALIZE      = SYSCTRLMNGR_MODE_00_INITIALIZE,
  SYSCTRLMNGR_LCLMODE_CONFIGURATION   = SYSCTRLMNGR_MODE_01_CONFIGURATION,
  SYSCTRLMNGR_LCLMODE_POWERRECOVERY   = SYSCTRLMNGR_MODE_02_POWERRECOVERY,
  SYSCTRLMNGR_LCLMODE_STANDBY         = SYSCTRLMNGR_MODE_03_STANDBY,
  SYSCTRLMNGR_LCLMODE_IDLE            = SYSCTRLMNGR_MODE_04_IDLE,
  SYSCTRLMNGR_LCLMODE_RUN             = SYSCTRLMNGR_MODE_05_RUN,
  SYSCTRLMNGR_LCLMODE_POWERLOSS       = SYSCTRLMNGR_MODE_06_POWERLOSS,
  SYSCTRLMNGR_LCLMODE_ERROR           = SYSCTRLMNGR_MODE_07_ERROR,
  SYSCTRLMNGR_LCLMODE_DIAGNOSTICS     = SYSCTRLMNGR_MODE_14_DIAGNOSTICS,
  SYSCTRLMNGR_LCLMODE_MANUFACTURING   = SYSCTRLMNGR_MODE_15_MANUFACTURING
} SYSCTRLMGRLCLMODE;

// global parameter declarations -----------------------------------------------
extern  const CODE SYSCTRLMNGRSCHDDEF g_atSysCtrlMngrSchdDefs[ ];
#if ( TASK_TICK_ENABLE == 1 )
extern  const CODE SYSCTRLMNGRTICKDEF g_atSysCtrlMngrTickDefs[ ];
#endif // TASK_TICK_ENABLE
extern  const CODE SYSCTRLMGRENTCHK   g_atSysCtrlMngrEntChkFuncs[ ];

// global function prototypes --------------------------------------------------
extern  void  SystemControlManager_LocalInitialize( void );
extern  U8    SystemControlManager_GetNumberSchdDefs( void );
#if ( TASK_TICK_ENABLE == 1 )
extern  U8    SystemControlManager_GetNumberTickDefs( void );
#endif // TASK_TICK_ENABLE

/**@} EOF SystemControlManager_cfg.h */

#endif  // _SYSTEMCONTROLMANAGER_CFG_H