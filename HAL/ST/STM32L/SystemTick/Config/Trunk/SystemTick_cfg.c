/******************************************************************************
 * @file SystemTick_cfg.c
 *
 * @brief System Tick configuration structures
 *
 * This file conatins the initilization structures
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick_cfg.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
const CODE PVSYSTEMTICKFUNC apvSystemTickFunctions[ ] = 
{
  TaskManager_TickProcess,
  NULL
};

/**@} EOF SystemTick_cfg.c */
