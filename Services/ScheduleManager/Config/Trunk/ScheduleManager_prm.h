/**************************************************************************//**
 * @file SchedulerManager_prm.h
 *
 * @brief  schedule manager parmaeter declarations
 *
 * This file provides the scheduler manager
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
 * \addtogroup schedulerManager 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SCHEDULERMANAGER_PRM_H
#define _SCHEDULERMANAGER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the enumeration for the config manager
#define SCHDMNGR_CONFIGCHANGE_ENUM          ( TASK_SCHD_ENUM_CFGMGR )

/// define the system control manager fast mode
#define SCHDMNGR_SYSCTRLMNGR_FASTMODE_ENUM  ( SYSCTRLMNGR_MODE_ILLEGAL )

/**@} EOF SchedulerManager_prm.h */

#endif  // _SCHEDULERMANAGER_PRM_H