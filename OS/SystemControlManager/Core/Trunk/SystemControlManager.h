/******************************************************************************
 * @file SystemControlManager.h
 *
 * @brief System control manager declarations
 *
 * This file provides the system control manager declarations
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
#ifndef _SYSTEMCONTROLMANAGER_H
#define _SYSTEMCONTROLMANAGER_H

// local includes -------------------------------------------------------------
#include "SystemControlManager/SystemControlManager_cfg.h"

// library includes -------------------------------------------------------------
#include "Types/Types.h"
#if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS

// Macros and Defines ---------------------------------------------------------
#include "Types/Types.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// define the process event  size
#define SYSCTRLMNGR_TASK_EVENT_SIZE   ( 4 )
#endif // SYSTEMDEFINE_OS_SELECTION

// global function prototypes --------------------------------------------------
/// initialization and task process
extern  void            SystemControlManager_Initialize( void );
extern  void            SystemControlManager_ProcessEvent( STATEEXECENGARG xEvent );

#if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
extern  const CODE ASCCMDENTRY atSysCtrlMngrCmdHandlerTable[ ];
#endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS

/// user callable functions
extern  SYSCTRLMGRERR   SystemControlManager_SetMode( SYSCTRLMGRMODE eMode );
extern  SYSCTRLMGRMODE  SystemControlManager_GetMode( void );

/**@} EOF SystemControlManager.h */

#endif  // _SYSTEMCONTROLMANAGER_H