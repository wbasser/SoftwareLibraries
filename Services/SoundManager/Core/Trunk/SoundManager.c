/******************************************************************************
 * @file  SoundManager.c 
 *
 * @brief sound mananger implementation
 *
 * This file provides the implementation of the sound manager
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SoundManager/SoundManager.h"
#include "SoundManager/SoundManager_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the play states
typedef enum _PLAYSTATE
{
  PLAYSTATE_IDLE = 0,     ///< no sound in progress
  PLAYSTATE_START,        ///< start a sequence
  PLAYSTATE_EXEC,         ///< execute the sequence
  PLAYSTATE_WAIT,         ///< wait for a sequence to be done
} PLAYSTATE;

// structures -----------------------------------------------------------------

// local parameter declarations -----------------------------------------------
static  SNDMNGRANIMENUM eCurSeq;
static  U8              nCurSeqIdx;
static  PSOUNDSEQENTRY  ptCurSequence;
static  SOUNDSEQENTRY   tCurSeqEntry;
static  PLAYSTATE       ePlayState;
static  U16             wCurCounts;
static  S16             sCurStep, sCurFrequency;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SoundManager_Initialize
 *
 * @brief sound manager initialization
 *
 * This function will initialize the sound manager
 *
 *****************************************************************************/
void SoundManager_Initialize( void )
{
  // clear the sound sequence in progress flag
  ePlayState = PLAYSTATE_IDLE;
}

/******************************************************************************
 * @function SoundManager_PlaySequence
 *
 * @brief play a sequence
 *
 * This function will play a sequence 
 *
 * @param[in]   eSoundSeq   sound to play
 *
 *****************************************************************************/
SOUNDMANAGERERR SoundManager_PlaySequence( SNDMNGRANIMENUM eSoundSeq )
{
  SOUNDMANAGERERR eError = SOUNDMANAGER_ERR_NONE;
  
  // check for a valid sequence
  if ( eSoundSeq < SNDMNGR_ANIMATION_ENUM_MAX )
  {
    // check to see if a sequence is in progress
    if ( ePlayState != PLAYSTATE_IDLE )
    {
      // turn off the sequence
      SoundManager_StopSequence( );
    }

    // reset the index
    nCurSeqIdx = 0;
    eCurSeq = eSoundSeq;
    
    // start the sequence
    ePlayState = PLAYSTATE_START;
  }
  else
  {
    // set the error
    eError = SOUNDMANAGER_ERR_ILLSEQ;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function SoundManager_StopSequence
 *
 * @brief stop the current sequence
 *
 * This function will stop the current sound sequence
 *
 *****************************************************************************/
void SoundManager_StopSequence( void )
{
  // stop the timer
  SoundManager_ToneControl( 0, OFF );
  
  // reset the state
  ePlayState = PLAYSTATE_IDLE;
}

/******************************************************************************
 * @function SoundManager_PlayTask
 *
 * @brief sound manager task handler
 *
 * This function processes the sound playing task
 *
 *****************************************************************************/
void SoundManager_PlayTask( void )
{
  // process the current state
  switch ( ePlayState )
  {
    case PLAYSTATE_IDLE :
      break;

    case PLAYSTATE_START :
      // get the current sequence
      ptCurSequence = ( PSOUNDSEQENTRY)PGM_RDWORD( aptSoundSeqs[ eCurSeq ] );

      // reset the current index
      nCurSeqIdx = 0;
      wCurCounts = 0;

      // go to execute state
      ePlayState = PLAYSTATE_EXEC;
      break;
      
    case PLAYSTATE_EXEC :
      // get the sequence entry
      MEMCPY_P( &tCurSeqEntry, &ptCurSequence[ nCurSeqIdx ], SOUNDSEQENTRY_SIZE );

      // generate a sound
      SoundManager_ToneControl( tCurSeqEntry.wFrequency, ON );

      // convert the time into counts
      wCurCounts = tCurSeqEntry.wDurationMsecs / SOUNDMANAGER_PLAY_TIME_MSECS;
      wCurCounts = MAX( wCurCounts, 1 );

      // now determine the next state
      if ( tCurSeqEntry.eEvent == SOUND_SEQEVENT_SWEEP )
      {
        // set sweep state/copy the frequency/calculate the step/set count to 1
        sCurFrequency = tCurSeqEntry.wFrequency;
        sCurStep = ( tCurSeqEntry.wOption - sCurFrequency ) / wCurCounts;
        wCurCounts = 1;
      }

      // just go to wait
      ePlayState = PLAYSTATE_WAIT;
      break;
      
    case PLAYSTATE_WAIT :
      // decrement the time
      if ( --wCurCounts ==  0 )
      {
        // process the next event
        switch( tCurSeqEntry.eEvent )
        {
          case SOUND_SEQEVENT_NEXT :
            // increment the index
            nCurSeqIdx++;
            ePlayState = PLAYSTATE_EXEC;
            break;
            
          case SOUND_SEQEVENT_JMPBEG :
            // reset the index
            nCurSeqIdx = 0;
            ePlayState = PLAYSTATE_EXEC;
            break;
            
          case SOUND_SEQEVENT_JUMPSEQ :
            // set the sequence to the option'
            nCurSeqIdx = tCurSeqEntry.wOption;
            ePlayState = PLAYSTATE_EXEC;
            break;
            
          case SOUND_SEQEVENT_REPEAT :
            // decrement the option count 
            if ( --tCurSeqEntry.wOption == 0 )
            {
              // increment the index
              nCurSeqIdx++;
            }
            ePlayState = PLAYSTATE_EXEC;
            break;

          case SOUND_SEQEVENT_SWEEP :
            // test for done
            if ( sCurFrequency < tCurSeqEntry.wOption )
            {
              // apply the step/update the tone
              sCurFrequency += sCurStep;
              SoundManager_ToneControl( sCurFrequency, ON );

              // reset the count
              wCurCounts = 1;
            }
            else
            {
              // increment the index back to execute
              nCurSeqIdx++;
              ePlayState = PLAYSTATE_EXEC;
            }
            break;

          case SOUND_SEQEVENT_DONE :
          default :
            // stop playing sound, error
            SoundManager_ToneControl( 0, OFF );
            
            // reset state back to idle
            ePlayState = PLAYSTATE_IDLE;
            break;
        }
      }
      break;

    default :
      // should never happen
      ePlayState = PLAYSTATE_IDLE;
      break;
  }
}

/**@} EOF SoundManager.c */
