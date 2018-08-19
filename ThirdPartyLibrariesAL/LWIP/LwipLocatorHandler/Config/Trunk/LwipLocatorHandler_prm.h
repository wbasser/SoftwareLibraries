/******************************************************************************
 * @file LwipLocatorHandler_prm.h
 *
 * @brief LWIP locator handler prameters 
 *
 * This file provides the parameters for the LWIP locator handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LwipLocatorHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPLOCATORHANDLER_PRM_H
#define _LWIPLOCATORHANDLER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION
#include "ConfigManager/ConfigManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the configuration manager task
#define LWIPLOCATORHANDLER_CONFIGUPDATE_TASK    ( TASK_SCHD_ILLEGAL )

/// define the configuration enumeration
#define LWIPLOCATORHANDLER_CONFIG_ENUM          ( CONFIG_TYPE_ILLEGAL )

/**@} EOF LwipLocatorHandler_prm.h */

#endif  // _LWIPLOCATORHANDLER_PRM_H