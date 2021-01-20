/******************************************************************************
 * @file QueueManager_cfg.h
 *
 * @brief queue manager configuration declarations
 *
 * This file declares the queue configuration structure
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
 * \addtogroup QueueManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _QUEUEMANAGER_CFG_H
#define _QUEUEMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "QueueManager/QueueManager_def.h"

// library includes -----------------------------------------------------------
/// define the macro for enabling large queue entry size
#define QUEUEMAMAGER_ENABLE_LARGE_ENTRY_SIZE            ( 1 )

/// define the macro for enabling large number of queues
#define QUEUEMANAGER_ENABLE_LARGE_NUMBER_ENTRIES        ( 0 )

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate each queue
typedef enum _QUEUENUM
{
  // add user defined queues here
  
  // do not remove the below entries
  QUEUE_ENUM_MAX,
  QUEUE_ENUM_ILLEGAL = 0xFF
} QUEUEENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE QUEUEDEF  g_atQueueDefs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF QueueManager_cfg.h */

#endif  // _QUEUEMANAGER_CFG_H