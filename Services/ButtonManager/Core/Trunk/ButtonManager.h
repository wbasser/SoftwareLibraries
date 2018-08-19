/******************************************************************************
 * @file ButtonManager.h
 *
 * @brief Button Manager declarations
 *
 * This file provides the declarations for the button manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BUTTONMANAGER_H
#define _BUTTONMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the process rate/number events
#define BTNMNGR_PROCESS_RATE        ( TASK_TIME_MSECS( BTNMNGR_PROCESS_RATE_MSECS ))
#define BTNMNGR_PROCESS_NUMEVENTS   ( 4 )

// define the extraction macros for getting key/event
#if ( TASKARG_SIZE_BYTES == 1 )
#define BTNMNGR_GET_EVENT( taskarg )  ( taskarg >> EVENT_SHIFT_CNT )
#define BTNMNGR_GET_KEY( taskarg )    ( taskarg & EVENT_KEY_MASK ) 
#else
#define BTNMNGR_GET_EVENT( taskarg )  ( taskarg >> 8 )
#define BTNMNGR_GET_KEY( taskarg )    ( taskarg & 0xFF )
#endif

// enumerations ---------------------------------------------------------------
/// enumerate the events
typedef enum _BTNMNGREVENTS
{
  BTNMNGR_EVENT_RELEASED = 0,     ///< 0 - button released
  BTNMNGR_EVENT_PRESSED,          ///< 1 - button pressed
  BTNMNGR_EVENT_REPEAT,           ///< 2 - button repeat
  BTNMNGR_EVENT_SHORTHOLD,        ///< 3 - button short hold
  BTNMNGR_EVENT_LONGHOLD,         ///< 4 - button long hold
  BTNMNGR_EVENT_STUCK,            ///< 5 - button stuck
  BTNMNGR_EVENT_BTNOFF,           ///< 6 - button toggle off
  BTNMNGR_EVENT_BTNON             ///< 7 - button toggle on
} BTNMNGREVENTS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  ButtonManager_Initialize( void );
extern  BOOL  ButtonManager_Process( TASKARG xArg );
extern  void  ButtonManager_ResetAllStates( void );

/**@} EOF ButtonManager.h */

#endif  // _BUTTONMANAGER_H