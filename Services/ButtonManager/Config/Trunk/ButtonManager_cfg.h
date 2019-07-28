/******************************************************************************
 * @file ButtonManager_cfg.h
 *
 * @brief button manager configuration declarations
 *
 * This file declares the configuration for the button manager
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
 
// ensure only one instantiation
#ifndef _BUTTONMANAGR_CFG_H
#define _BUTTONMANAGR_CFG_H

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager_def.h"

// system includes ------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#include "FreeRtos.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define BTNMANAGER_PROCESS_EXEC_RATE    TASK_TIME_MSECS(( BTNMANAGER_PROCESS_RATE_MSECS ))
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// enumerations ---------------------------------------------------------------
/// enumerate the buttons
typedef enum BTNMNGRENUM
{
  // enumerate user buttons here

  // do not remove the below entries
  BTNMNGR_ENUM_MAX,
  BTNMNGR_ENUM_ILLEGAL
} BTNMNGRENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE BTNMNGRCFG tBtnMgrCfg;
extern  const CODE BTNMNGRDEF atBtnMgrDefs[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  ButtonManager_GetKeyStatus( U8 nKeyEnum );
extern  void  ButtonManager_LocalInitialize( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  ButtonManager_ProcessTask( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/**@} EOF ButtonManager_cfg.h */

#endif  // _BUTTONMANAGR_CFG_H