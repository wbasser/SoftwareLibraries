/******************************************************************************
 * @file DebugManager_prm.h
 *
 * @brief debug manager parameters
 *
 * This file supplies the debug manager parameters
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
 * \addtogroup DebugManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _DEBUGMANAGER_PRM_H
#define _DEBUGMANAGER_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the task argument size - valid values are 1, 2 or 4
#define DEBUGMANAGER_ARGSIZE_BYTES	    ( 1 )

/// define the maximum number of debug entries
#define DEBUGMANAGER_NUM_ENTRIES        ( 128 )

/// define the macro to enable time stamps
#define DEBUGMANAGER_ENABLE_TIMESTAMP   ( 0 )

/// define the macro to enable debug commands
#define DEBUGMANAGER_ENABLE_DEBUG_COMMANDS    ( 0 )

/// define the macro to get system time
#if ( DEBUGMANAGER_ENABLE_TIMESTAMP == 1 )
#define DEBUGMANAGER_GETTIME_MSECS            ( )
#endif // DEBUGMANAGER_ENABLE_TIMESTAMP

/**@} EOF DebugManager_prm.h */

#endif  // _DEBUGMANAGER_PRM_H
