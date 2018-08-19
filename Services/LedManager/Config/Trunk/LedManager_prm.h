/******************************************************************************
 * @file LedManager_prm.h
 *
 * @brief LED manager parameter declarations
 *
 * This file defines the parameters that control the LED manager
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
 * \addtogroup LedManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMANAGER_PRM_H
#define _LEDMANAGER_PRM_H

// system -includes -----------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum number of columns
#define LEDMANAGER_MATRIX_MAX_NUM_COLS          ( 0 )

/// define the maximum number of rows
#define LEDMANAGER_MATRIX_MAX_NUM_ROWS          ( 0 )

/// define the state of the row for active
#define LEDMANAGER_MATRIX_ROW_ACTIVE_LEVEL      ( HI )

/// define the state of the column for active
#define LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL      ( HI )

/// define the LED flash slow on/off times
#define LEDMANAGER_FLASH_SLOW_TIME              ( 500 )

/// define the LED flash fast on/off times
#define LEDMANAGER_FLASH_FAST_TIME              ( 200 )

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// define the animation done task enum
#define LEDMANAGER_ANIMATION_DONE_TASK          ( TASK_SCHD_ILLEGAL )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/// define the animation done event value
#define LEDMANAGER_ANIMATION_DONE_EVENT         ( 0 )

/// define the animation call stack depth
#define LEDMANAGER_ANIMATION_CALLSTACK_DEPTH    ( 8 )

/// define the macro to enable RGB diodes
#define LEDMANAGER_RGB_LEDS_ENABLED             ( 0 )

/// define the animation rate in milliseconds 
#define LEDMANAGER_ANIMATE_RATE_MSECS           ( 25 )

/**@} EOF LedManager_prm.h */

#endif  // _LEDMANAGER_PRM_H