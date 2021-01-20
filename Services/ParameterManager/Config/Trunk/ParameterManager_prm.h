/******************************************************************************
 * @file ParameterManager_prm.h
 *
 * @brief Parameter Manager parameter declarations 
 *
 * This file provides the parameter declarations for the parameter manager
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
 * \addtogroup ParameterManager
 * @{
 *****************************************************************************/
// library includes
#include "SystemDefines/SystemDefines_prm.h"
#include "TaskManager/TaskManager.h"

// ensure only one instantiation
#ifndef _PARAMETERMANAGER_PRM_H
#define _PARAMETERMANAGER_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the base address for the eeprom location
#define PARAM_BASE_ADDRESS              ( 0 )

/// define the paramaeter argument size - valid values are 1, 2 or 4
#define PARAM_SIZE_BYTES                ( 1 )

/// define the macro to enable the use of names
#define PARAM_USE_NAMES                 ( 0 )

/// define the macro to enable CRC vs Checksum
#define PARAM_USE_CRC                   ( 1 )

/// define the size of the block for updates
#define PARAM_UPDATE_BLOCK_SIZE     ( 256 )

/// define the macro to enable notifications
#define	PARAM_ENABLE_NOTIFICATIONS      ( 1 )

/// define the macro to enable callbacks
#define	PARAM_ENABLE_CALLBACKS          ( 0 )

/// define the callback or the task
#if ( PARAM_ENABLE_NOTIFICATIONS == 0 )
  #if ( PARAM_ENABLE_CALLBACKS == 1 )
    #define	PARAM_DONE_CALLBACK( )  		( NULL )
  #else
    #define	PARAM_NOTIFICATION_TASK			( TASK_SCHD_SYSCTRLMANAGER_ENUM )
    #define	PARAM_DONE_EVENT						( SYSCTRLMNGR_EVENT_CONFIGDONE )
    #define PARAM_ERROR_EVENT           ( SYSCTRLMNGR_EVENT_ERRORDET )
  #endif	// PARAM_ENABLE_CALLBACKS
#endif	// PARAM_ENABLE_NOTIFICATIONS

/// define the enable debug commands macro
#define PARAM_ENABLE_DEBUG_COMMANDS     ( 1 )

/// define the mode to allow for writing and resetting parameters
#define PARAM_WRITE_RESET_SYSTEM_MODE   ( 0 )

/// define the password to allow resetting of the parameters
#define PARAM_RESET_DEFAULTS_PASSWORD   ( SYSTEMDEFINE_CONFIG_RESET_DEFAULT )

/// define the macro to enable config version reset
#define PARAM_ENABLE_CONFIGFVER             ( ON )

#if ( PARAM_ENABLE_CONFIGVER == ON )
  /// define the configuration version
  #define PARAM_CONFIG_VERMAJ               ( 2 )
  #define PARAM_CONFIG_VERMIN               ( 0 )
#endif


/**@} EOF ParameterManager_prm.h */

#endif  // _PARAMETERMANAGER_PRM_H
