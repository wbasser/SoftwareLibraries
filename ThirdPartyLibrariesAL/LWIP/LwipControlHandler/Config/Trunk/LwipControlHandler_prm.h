/******************************************************************************
 * @file LwipControlHandler_prm.h
 *
 * @brief LWIP handler parameter declarations
 *
 * This file provides the declarations for the ethernet handler configuration
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup LwipControlHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LWIPCONTROHANDLER_PRM_H
#define _LWIPCONTROHANDLER_PRM_H

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

/// define the config update task
#define LWIPCONTROLHANDLER_CONFIGUPDATE_TASK          ( TASK_SCHD_ILLEGAL )

/// define the configuration enumeration
#define LWIPCONTROLHANDLER_CONFIG_ENUM                ( CONFIG_TYPE_ILLEGAL )

/**@} EOF LwipControlHandler_prm.h */

#endif  // _LWIPCONTROHANDLER_PRM_H