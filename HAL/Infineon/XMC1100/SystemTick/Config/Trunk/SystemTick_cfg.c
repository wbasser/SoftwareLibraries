/******************************************************************************
 * @file SystemTick_cfg.c
 *
 * @brief System Tick configuration structures
 *
 * This file conatins the initilization structures
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
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
