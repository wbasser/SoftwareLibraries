/******************************************************************************
 * @file FreeRTOSInclude.h
 *
 * @brief Free RTOS abstraction definitions
 *
 * This file provides the Free RTOS abstraction definitions
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
 * $Rev: $
 * 
 *
 * \addtogroup 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FREERTOSINCLUDE_H
#define _FREERTOSINCLUDE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "FreeRTOS.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF FreeRTOSInclude.h */

#endif  // _FREERTOSINCLUDE_H