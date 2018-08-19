/******************************************************************************
 * @file LedManager_cfg.h
 *
 * @brief LED manager configuration declarations
 *
 * This file provides the configuration declarations for the LED manager
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
#ifndef _LEDMANAGER_CFG_H
#define _LEDMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedManager/LedManager_prm.h"
#include "LedManager/LedManager_def.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define LEDMANAGER_ANIMATE_EXEC_RATE    TASK_TIME_MSECS(( LEDMANAGER_ANIMATE_RATE_MSECS ))
#define LEDMANAGER_ANIMATE_NUM_EVENTS   ( 4 )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
#define LEDMANAGER_SCAN_EXEC_RATE       ( TASK_TIME_MSECS( 1 ))
#define LEDMANAGER_SCAN_NUM_EVENTS      ( 2 )
#endif  // MATRIX DEFS


// enumerations ---------------------------------------------------------------
/// enumerate each led
typedef enum _LEDMANAGERSELENUM
{
  // enumerate user LEDS here
  
  // do not remove the below entry
  LEDMANAGER_ENUM_MAX,
  LEDMANAGER_ENUM_ALL = 0xFF
} LEDMANAGERSELENUM;

#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
/// enumerate the RGB leds
typedef enum _LEDMANAGERRGBENUM
{
  // enumerate user RGBs here
  
  // do not remove the below entries
  LEDMANAGER_RGBENUM_MAX,
  LEDMANAGER_RGBENUM_ALL = 0xFF
} LEDMANAGERRGBENUM;
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/// enumerate each animation
typedef enum _LEDMNGRANIMENUM
{
  // enumerate user animations here
  
  // do not remove these below
  LEDMNGR_ANIMATION_MAX,
  LEDMNGR_ANIMATION_STOP = 0xFF
} LEDMNGRANIMENUM;

// global parameter declarations -----------------------------------------------
/// declare the led definitions
extern  const CODE LEDDEF  atLedDefs[ ];
#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
extern  const CODE LEDRGBDEF  atLedRgbDefs[ ];
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/// declare the animation enumeration
extern  const CODE PLEDSEQENTRY apLedAnimationsDef[ ];
/// declare the LED matrix rows/cols
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
extern  const CODE GPIOPINENUM aeLedMatrixRows[ ];
extern  const CODE GPIOPINENUM aeLedMatrixCols[ ];
#endif  // MATRIX DEFS
#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
extern  const CODE LEDRGBDEF  atLedsRgbDef[ ];
#endif // LEDMANAGER_RGB_LEDS_ENABLED

// global function prototypes --------------------------------------------------
extern  void  LedManager_LocalInitialize( void );
extern  void  LedManager_NotifyDone( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  LedManager_AnimationTask( TASKARG xArg );
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
extern  BOOL  LedManager_ScanTask( TASKARG xArg );
#endif  // MATRIX DEFS
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/**@} EOF LedManager_cfg.h */

#endif  // _LEDMANAGER_CFG_H