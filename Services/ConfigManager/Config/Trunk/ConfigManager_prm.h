/******************************************************************************
 * @file ConfigManager_prm.h
 *
 * @brief Config Manager parameter declarations
 *
 * This file declares any customization for the configr manager
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
 * \addtogroup ConfigManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CONFIGMANAGER_PRM_H
#define _CONFIGMANAGER_PRM_H

// system includes ------------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the log event enable macro
#define CONFIGMANAGER_ENABLE_LOGEVENTS          ( 0 )

/// define the macro to enable notifications
#define	CONFIGMANAGER_ENABLE_NOTIFICATIONS      ( 0 )

/// define the macro to enable callbacks
#define	CONFIGMANAGER_ENABLE_CALLBACKS          ( 0 )

/// define the callback or the task
#if ( CONFIGMANAGER_ENABLE_NOTIFICATIONS == 1 )
  #if ( CONFIGMANAGER_ENABLE_CALLBACKS == 1 )
    #define	CONFIGMANAGER_DONE_CALLBACK( )  		( NULL )
  #else
    #define	CONFIGMANAGER_NOTIFICATION_TASK			( 0 )
    #define	CONFIGMANAGER_DONE_EVENT						( 0 )
    #define CONFIGMANAGER_ERROR_EVENT           ( 0 )
  #endif	// CONFIGMANAGER_ENABLE_CALLBACKS
#endif	// CONFIGMANAGER_ENABLE_NOTIFICATIONS

/**@} EOF ConfigManager_prm.h */

#endif  // _CONFIGMANAGER_PRM_H