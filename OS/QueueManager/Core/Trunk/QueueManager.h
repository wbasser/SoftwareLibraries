/******************************************************************************
 * @file QueueManager.h
 *
 * @brief quueue manager declarations
 *
 * This file declares the API's for the queue manager
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
 * \addtogroup 
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _QUEUEMANAGER_H
#define _QUEUEMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "QueueManager/QueueManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the queue errors
typedef enum _QUEUESTATUS
{
  QUEUE_STATUS_NONE = 0,     // no error
  QUEUE_STATUS_ILLQUE,       // illegal queue
  QUEUE_STATUS_QUEFUL,       // queue full
  QUEUE_STATUS_QUEEMP,       // queue empty
} QUEUESTATUS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        QueueManager_Initialize( void );
extern  QUEUESTATUS QueueManager_PutHead( QUEUEENUM eQueue, PU8 pnEntry );
extern  QUEUESTATUS QueueManager_PutTail( QUEUEENUM eQueue, PU8 pnEntry );
extern  QUEUESTATUS QueueManager_Get( QUEUEENUM eQueue, PU8 pnEntry );
extern  QUEUESTATUS QueueManager_Peek( QUEUEENUM eQueue, PU8 pnEntry );
extern  QUEUESTATUS QueueManager_Pop( QUEUEENUM eQueue );
extern  QUEUESTATUS QueueManager_Flush( QUEUEENUM eQueue );
extern  QUEUESTATUS QueueManager_GetRemaining( QUEUEENUM eQueue, PU8 pnRemaining );
extern  QUEUESTATUS QueueManager_GetStatus( QUEUEENUM eQueue );  

/**@} EOF QueueManager.h */

#endif  // _QUEUEMANAGER_H