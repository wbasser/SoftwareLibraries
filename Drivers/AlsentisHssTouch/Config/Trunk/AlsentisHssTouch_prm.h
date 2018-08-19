/******************************************************************************
 * @file AlsentisHssTouch_prm.h
 *
 * @brief parameter for the Alsentis HSS touch
 *
 * This file provides the parameter declarations
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AlsentisHssTouch
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALSENTISHSSTOUCH_PRM_H
#define _ALSENTISHSSTOUCH_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"
#include "QueueManager/QueueManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum number of sensors
#define ALSENTISHSSTOUCH_NUM_SENSORS              ( 4 )

/// define the configuration options
#define ALSENTISHSSTOUCH_PARAMMGR_SEL             ( 0 )
#define ALSENTISHSSTOUCH_CONFIGMGR_SEL            ( 1 )

/// define the config method using one of the above selections
#define ALSENTISHSSTOUCH_CONFIG_METHOD            ( ALSENTISHSSTOUCH_PARAMMGR_SEL )

/// define the operation options
#define ALSENTISHSSTOUCH_OPER_I2CPERIPH           ( 0 )
#define ALSENTISHSSTOUCH_OPER_EMBEDDED            ( 1 )

/// define the operation method
#define ALSENTISHSSTOUCH_OPER_METHOD              ( ALSENTISHSSTOUCH_OPER_I2CPERIPH )

/// define the task handler for the touch controller
#define ALSENTISHSSTOUCH_TASK_ENUM                ( TASK_SCHD_ILLEGAL )

/// define the queue for parameter updates if config method is parameter manager
#define ALSENTISHSSTOUCH_UPDATE_QUEUE_ENUM        ( QUEUE_ENUM_ILLEGAL )

/**@} EOF AlsentisHssTouch_prm.h */

#endif  // _ALSENTISHSSTOUCH_PRM_H