/******************************************************************************
 * @file LedManager.h
 *
 * @brief sound manager declarations
 *
 * This file provides the declarations for the sound manager
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
 
// ensure only one instatiation
#ifndef _LEDMANAGER_H
#define _LEDMANAGER_H

// local includes -------------------------------------------------------------
#include "LedManager/LedManager_cfg.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
#define LEDMANAGER_ANIMATE_EXEC_RATE    ( TASK_TIME_MSECS( 25 ))
#define LEDMANAGER_ANIMATE_NUM_EVENTS   ( 4 )
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
#define LEDMANAGER_SCAN_EXEC_RATE       ( TASK_TIME_MSECS( 1 ))
#define LEDMANAGER_SCAN_NUM_EVENTS      ( 2 )
#endif  // MATRIX DEFS

// Enumerations ---------------------------------------------------------------
/// enumeratre the errors
typedef enum _LEDMANAGERERR
{
  LEDMANAGER_ERR_NONE = 0,      ///< no error
  LEDMANAGER_ERR_ILLLED,        ///< illegal LED enumeration
  LEDMANAGER_ERR_ILLACT,        ///< illegal LED action
  LEDMANAGER_ERR_ILLANI,        ///< illegal animation
  LEDMANAGER_ERR_ILLCLR,        ///< illegal color
} LEDMANAGERERR;

// global function prototypes --------------------------------------------------
extern 	void          LedManager_Initialize( void );
extern  LEDMANAGERERR LedManager_Control( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption );
extern	LEDMANAGERERR LedManager_PlayAnimation( LEDMNGRANIMENUM eAnimation, U16 wOption );
extern  BOOL          LedManager_AnimateTask( TASKARG xArg );
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
extern  BOOL          LedManager_ScanTask( TASKARG xArg );
#endif  // MATRIX DEFS
#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
extern  LEDMANAGERERR LedManager_RgbControl( LEDMNGRRGBENUM eLedSel, LEDACTION eAction, LEDRGBCOLOR eColor, U16 wOption );
#endif  // LEDMANAGER_RGB_LEDS_ENABLED


/**@} EOF LedManager.h */

#endif  // _LEDMANAGER_H