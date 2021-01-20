/******************************************************************************
 * @file SoundManager_def.h
 *
 * @brief definitions for the sound managers
 *
 * This file defines the macros, structures and parameters for the sound manager
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
#ifndef _SOUNDMANAGER_DEF_H
#define _SOUNDMANAGER_DEF_H

// local includes -------------------------------------------------------------
#include "SoundManager/SoundManager_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating an animation step
#define SNDMNGRDEFSTEP( frequency, duration, nextevent, option ) \
  { \
    .wFrequency = frequency, \
    .wDurationMsecs = duration, \
    .eEvent = nextevent, \
    .wOption = option \
  },
  
/// define the helper macro for starting an animation sequence
#define SNDMNGRDEFSTART( name ) \
  static  const CODE  SOUNDSEQENTRY at ## name ## Defs[ ] = { \

/// define the helper macro for ending an animation sequence
#define SNDMNGRDEFSTOP \
  };
  
/// define the helpermacro for createing an entry
#define SNDMNGRANIMATIONDEF( name ) \
    ( const CODE PSOUNDSEQENTRY )&at ## name ## Defs \
  

// enumerations ---------------------------------------------------------------
/// enumerate the sound sequence events
typedef enum _SOUNDSEQEVENT
{
  SOUND_SEQEVENT_DONE = 0,    ///< sequence done
  SOUND_SEQEVENT_NEXT,        ///< next sequence
  SOUND_SEQEVENT_JMPBEG,      ///< jump to the beginning
  SOUND_SEQEVENT_JMPSEQ,     ///< jump to a step within this sequence
  SOUND_SEQEVENT_REPEAT,      ///< repeat till count is zero
  SOUND_SEQEVENT_SWEEP,       ///< sweep frequency, target is option
  SOUND_SEQEVENT_CONTINUOUS,  ///< continuous output
  SOUND_SEQEVENT_MAX    
} SOUNDSEQEVENT;

// structures -----------------------------------------------------------------
/// define the structure for a sequence entry
typedef struct _SOUNDSEQENTRY
{
  U16             wFrequency;     ///< led enumeration
  U16             wDurationMsecs; ///< duration in msecs
  SOUNDSEQEVENT   eEvent;         ///< action to execute when this tone is done
  U16             wOption;        ///< option used with jump or repeat events
} SOUNDSEQENTRY, *PSOUNDSEQENTRY;
#define SOUNDSEQENTRY_SIZE  sizeof( SOUNDSEQENTRY )

/**@} EOF SoundManager_def.h */

#endif  // _SOUNDMANAGER_DEF_H