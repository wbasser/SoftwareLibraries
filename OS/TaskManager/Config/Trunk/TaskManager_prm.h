/******************************************************************************
 * @file TaskManager_prm.h
 *
 * @brief task manage parameters
 *
 * This file supplies the task manager parameters
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
 * \addtogroup TaskManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _TASKMANAGER_PRM_H
#define _TASKMANAGER_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the task argument size - valid values are 1, 2 or 4
#define TASK_TSKARG_SIZE_BYTES	          ( 2 )

/// define the macro to enable tick tasks - set to 1 to enable
#define TASK_TICK_ENABLE                  ( 0 )

/// define the maximum number of priority events
#define TASK_MAXNUM_PRI_EVENTS            ( 2 )

/// define the macro to enable large event queues
#define TASK_ENABLE_LARGE_EVENT_QUEUES    ( 0 )

/// define the macro to disable tick task processing 
#define TASK_DISABLE_TICK_PROCESSING      ( 0 )
 
/**@} EOF TaskManager_prm.h */

#endif  // _TASKMANAGER_PRM_H
