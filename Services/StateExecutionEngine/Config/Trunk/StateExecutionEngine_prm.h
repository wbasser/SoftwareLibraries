/******************************************************************************
 * @file StateExecutionEngine_prm.h
 *
 * @brief  state execution engine parameter delcarations
 *
 * This file provides the parameters for customizing the state execution engine
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
 * \addtogroup StateExecutionEngine
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _STATEEXECUTIONENGINE_PRM_H
#define _STATEEXECUTIONENGINE_PRM_H

// library include ------------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif  // SYSTEMDEFINE_OS_SELECTION       

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
// use the task manager size
#define STATEEXECENG_ARG_SIZE_BYTES            ( TASK_TSKARG_SIZE_BYTES )
#else
/// define the macro for setting the arg size/must 1, 2 or 4
#define STATEEXECENG_ARG_SIZE_BYTES             ( 2 )
#endif  // SYSTEMDEFINE_OS_SELECTION       

/**@} EOF StateExecutionEngine_prm.h */

#endif  // _STATEEXECUTIONENGINE_PRM_H