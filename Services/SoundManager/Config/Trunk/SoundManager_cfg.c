/******************************************************************************
 * @file SoundManager_cfg.c
 *
 * @brief sound manager configuration implementation
 *
 * This file provides the sound manager configuration implementation
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

// local includes -------------------------------------------------------------
#include "SoundManager/SoundManager.h"

// library includes -----------------------------------------------------------
#include "SoundControlHandler/SoundControlHandler.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
// OS includes
#include "FreeRtos.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define SOUNDMANAGER_PLAY_TASK_PRIORITY             ( tskIDLE_PRIORITY + 4 )
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
static  void  PlayTask( PVOID pvParameters );
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// constant parameter initializations -----------------------------------------
/// define each sequence here
/// SNDMNGRDEFSTART( name )
/// SNDMNGRDEFSTEP( frequency, duration, nextevent, option ) 
/// SNDMNGRDEFSTOP

/// alternate 800/1000 at 250MSEC
SNDMNGRDEFSTART( Tone800ALT1000AT250 )
SNDMNGRDEFSTEP(  800, 250, SOUND_SEQEVENT_NEXT,   0 )
SNDMNGRDEFSTEP( 1000, 250, SOUND_SEQEVENT_JMPBEG, 1 )
SNDMNGRDEFSTOP

/// swepp 800/1000 at 200MSEC
SNDMNGRDEFSTART( Tone800SWP1000AT200 )
SNDMNGRDEFSTEP(  800, 100, SOUND_SEQEVENT_SWEEP,  1000 )
SNDMNGRDEFSTEP(    0,   0, SOUND_SEQEVENT_JMPBEG,    1 )
SNDMNGRDEFSTOP

/// sweep 600/1200 at 1 second
SNDMNGRDEFSTART( Tone600SWP1200AT1000 )
SNDMNGRDEFSTEP( 600, 1000, SOUND_SEQEVENT_SWEEP, 1200 )
SNDMNGRDEFSTEP(   0,    0, SOUND_SEQEVENT_JMPBEG,   1 )
SNDMNGRDEFSTOP

/// fill in the sound sequences entries here
const CODE PSOUNDSEQENTRY aptSoundSeqs[ SNDMNGR_ANIMATION_ENUM_MAX ] =
{
  /// using the seq names above
  /// SNDMNGRANIMATIONDEF( name )
  SNDMNGRANIMATIONDEF( Tone800ALT1000AT250 ),
  SNDMNGRANIMATIONDEF( Tone800SWP1000AT200 ),
  SNDMNGRANIMATIONDEF( Tone600SWP1200AT1000 ),
};

/******************************************************************************
 * @function SoundManager_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any custom initialization
 *
 *****************************************************************************/
void SoundManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // create the animation task
  xTaskCreate( PlaTask, "SountPlay", configMINIMAL_STACK_SIZE, NULL, SOUNDMANAGER_PLAY_TASK_PRIORITY, NULL );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS
}

/******************************************************************************
 * @function SoundManager_ToneCOntrol
 *
 * @brief tone control
 *
 * This function will turn on/off the tone generation
 *
 * @param[in]   wFrequency      frequency of tone
 * @param[in]   bState          desired on/off state
 *
 *****************************************************************************/
void SoundManager_ToneControl( U16 wFrequency, BOOL bState )
{
  // turn off/on the sound
  SoundControlHandler_SetFrequency( wFrequency );
  SoundControlHandler_Control( bState );
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
/******************************************************************************
 * @function PlayTask
 *
 * @brief play task
 *
 * This function is the sound play task
 *
 * @param[in]   pvParameters  not used
 *
 *****************************************************************************/
static void PlayTask( PVOID pvParameters )
{
  // main loop
  FOREVER
  {
    // call the default handler
    SoundManager_PlayTask( );

    // sleep for the animate rate
    vTaskDelay( SOUNDMANAGER_PLAY_RATE_MSECS / portTICK_RATE_MS );
  }
}
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKAMANAGER )
/******************************************************************************
 * @function SoundManager_PlayTask
 *
 * @brief play task
 *
 * This function will call the play process
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL SoundManager_ProcessPlay( TASKARG xArg )
{
  // call the animation process
  SoundManager_PlayTask( );
  
  // return true
  return( TRUE );
}

#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SoundManager_cfg.c */
