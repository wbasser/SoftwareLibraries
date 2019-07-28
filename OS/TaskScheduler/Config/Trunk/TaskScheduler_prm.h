/******************************************************************************
 * @file TaskScheduler_prm.h
 *
 * @brief Task Scheduler parameter declarations
 *
 * This file provides the declarations for the task scheduler
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup TaskScheduler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TASKSCHEDULER_PRM_H
#define _TASKSCHEDULER_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the event size in bytes ( 1,2 or 4 )
#define TASKSCHEDULER_EVENT_SIZE_BYTES            ( 1 )

/// define the macro to enable large event queues
#define TASKSCHEDULER_ENABLE_LARGE_EVENT_QUEUES   ( 0 )

/**@} EOF TaskScheduler_prm.h */

#endif  // _TASKSCHEDULER_PRM_H