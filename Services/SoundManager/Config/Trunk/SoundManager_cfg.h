/******************************************************************************
 * @file SoundManager_cfg.h
 *
 * @brief sound manager configuration declarations
 *
 * This file provides the declarations for the sound manager configurations
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
 * \addtogroup SoundManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SOUNDMANGER_CFG_H
#define _SOUNDMANGER_CFG_H

// local includes -------------------------------------------------------------
#include "SoundManager/SoundManager_def.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define SOUNDMANAGER_PLAY_EXEC_RATE    TASK_TIME_MSECS(( SOUNDMANAGER_PLAY_TIME_MSECS ))
#define SOUNDMANAGER_PLAY_NUM_EVENTS   ( 4 )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// enumerations ---------------------------------------------------------------
/// enumerate each animation
typedef enum _SNDMNGRANIMENUM
{
  // enumerate user animations here
  SNDMNGR_ANIMATION_ENUM_800ALT1000AT250 = 0,
  SNDMNGR_ANIMATION_ENUM_800SWP1000AT200,
  SNDMNGR_ANIMATION_ENUM_600SWP1200AT1000,
  
  // do not remove these below
  SNDMNGR_ANIMATION_ENUM_MAX,
  SNDMNGR_ANIMATION_ENUM_STOP = 0xFF
} SNDMNGRANIMENUM;

// global parameter declarations -----------------------------------------------
/// declare the sound sequence defintions
extern  const CODE PSOUNDSEQENTRY aptSoundSeqs[ ];

// global function prototypes --------------------------------------------------
extern  void  SoundManager_LocalInitialize( void );
extern  void  SoundManager_ToneControl( U16 wFrequency, BOOL bState );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  SoundManager_ProcessPlay( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/**@} EOF SoundManger_cfg.h */

#endif  // _SOUNDMANGER_CFG_H