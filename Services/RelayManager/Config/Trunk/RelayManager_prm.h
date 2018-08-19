/******************************************************************************
 * @file RelayManager_prm.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup RelayManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RELAYMANAGER_PRM_H
#define _RELAYMANAGER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the drive active time
#define RELAY_MANAGER_DRIVE_ACTIVE_TIME       ( 50 )

/// define the scan time
#define RELAY_MANAGER_SCAN_TIME               ( 2 )

/// define the 50/60 hz flag
#define RELAY_MANAGER_50HZ_SELECTION          ( 0 )

/// define the enable zero cross slot generation
#define RELAY_MANAGER_ENABLE_ZEROCROSS        ( 0 )

/// define the external driver macro
#define RELAY_MANAGER_ENABLE_EXTDRIVER        ( 0 )

/// define the dynamic configuration of on/off times
#define RELAY_MANAGER_ENABLE_DYNONOFTIMES     ( 0 )

/// define the enable charge pump operation
#define RELAY_MANAGER_ENABLE_CHARGEPUMP       ( 0 )

/// define the enable keep alive operation
#define RELAY_MANAGER_ENABLE_KEEPALIVE        ( 0 )

/**@} EOF RelayManager_prm.h */

#endif  // _RELAYMANAGER_PRM_H