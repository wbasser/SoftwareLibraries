/******************************************************************************
 * @file SoundManager.h
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
 * \addtogroup SoundManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

// local includes -------------------------------------------------------------
#include "SoundManager/SoundManager_cfg.h"

// Macros and Defines ---------------------------------------------------------

// Enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _SOUNDMANAGERERR
{
  SOUNDMANAGER_ERR_NONE = 0,      ///< no error
  SOUNDMANAGER_ERR_ILLSEQ,        ///< illegal sound sequence
} SOUNDMANAGERERR;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void  (*PVSNDMNGRCBFNC)(void);

// global function prototypes --------------------------------------------------
extern 	void	            SoundManager_Initialize( void );
extern	SOUNDMANAGERERR 	SoundManager_PlaySequence( SNDMNGRANIMENUM eSoundSeq, PVSNDMNGRCBFNC pvCallbackFunc );
extern  void              SoundManager_StopSequence( void );
extern  void              SoundManager_PlayTask( void );
extern  BOOL              SoundManager_IsPlayingSequence( void );

/**@} EOF SoundManager.h */

#endif  // _SOUNDMANAGER_H