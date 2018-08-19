/******************************************************************************
 * @file TaskManager_cfg.h
 *
 * @brief task manager configuration delcarations
 *
 * This file provides the enumeration of the tasks 
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
 * \addtogroup TaskManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _TASKMANAGE_CFG_H
#define _TASKMANAGE_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TaskManager/TaskManager_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate each scheduled task
typedef enum _TASKSCHDENUMS
{
  // add enumerations here
  
  // do not remove these declarations
  TASK_SCHD_MAX,
  TASK_SCHD_ILLEGAL
} TASKSCHDENUMS;

#if ( TASK_TICK_ENABLE == 1 )
/// enumerate each tick task
typedef enum _TASKSTICKENUMS
{
  // add enumerations here
  
  // do not remove these declarations
  TASK_TICK_MAX,
  TASK_TICK_ILLEGAL
} TASKTICKENUMS;
#endif // TASK_TICK_ENABLE

// global parameter declarations -----------------------------------------------
extern  const CODE TASKSCHDDEF  atTaskSchdDefs[ ];

#if ( TASK_TICK_ENABLE == 1 )
extern  const CODE TASKTICKDEF  atTaskTickDefs[ ];
#endif  // TASK_TICK_ENABLE

/**@} EOF TaskManager_cfg.h */

#endif  // _TASKMANAGE_CFG_H