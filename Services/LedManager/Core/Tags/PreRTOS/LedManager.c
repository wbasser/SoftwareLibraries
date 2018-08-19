/******************************************************************************
 * @file  LedManager.c 
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
 * \addtogroup LedManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#ifdef __ATMEL_AVR__
#include <util/delay.h>
#endif

// local includes -------------------------------------------------------------
#include "LedManager/LedManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the play states
typedef enum _ANIMATIONSTATE
{
  ANIMATION_STATE_IDLE = 0,     ///< no animation in progress
  ANIMATION_STATE_START,        ///< start a sequence
  ANIMATION_STATE_EXEC,         ///< execute sequences
} ANIMATIONSTATE;

/// enumerate the current state
typedef enum _LEDSTATE
{
  LED_STATE_OFF = 0,          ///< led is off
  LED_STATE_ON,               ///< led is on
  LED_STATE_BLNKOFF,          ///< led is blink off
  LED_STATE_BLNKON,           ///< led is blink on
  LED_STATE_PULSE             ///< led is in pulse
} LEDSTATE;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LEDCTL
{
  LEDSTATE  eCurState;          ///< current state
  U16       wCounts;            ///< current counts for this led
  U16       wOption;            ///< option
  BOOL      bNewRequest;        ///< new request
  BOOL      bAllOffOnDisabled;  ///< disable all off/on
} LEDCTL, *PLEDCTL;
#define LEDCTL_SIZE     sizeof( LEDCTL )

/// define the stack entry structure
typedef struct _SEQSTACK
{
  LEDMNGRANIMENUM eCurrentAnimation;
  U8              nAnimationIdx;
} SEQSTACK, *PSEQSTACK;
#define SEQSTACK_SIZE   sizeof( SEQSTACK )

// local parameter declarations -----------------------------------------------
static  LEDCTL          atLedCtls[ LEDMANAGER_ENUM_MAX ];
static  ANIMATIONSTATE  eAnimationState;
static  LEDMNGRANIMENUM eCurrentAnimation;
static  U16             wAnimationOption;
static  U8              nCurAnimationIdx;
static  PLEDSEQENTRY    ptCurAnimation;
static  U16             wCurrentCount;
#if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
static  SEQSTACK        atSeqStack[ LEDMANAGER_ANIMATION_CALLSTACK_DEPTH ];
static  U8              nStackIndex;
#endif
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
static  U8              anColVals[ LEDMANAGER_MATRIX_MAX_NUM_ROWS ];
static  U8              nCurScanRow;
#endif  // MATRIX DEFS

// local function prototypes --------------------------------------------------
static  void  SetLedAction( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption );
static  void  ChangeLedState( PLEDDEF ptDef, BOOL bState );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LedManager_Initialize
 *
 * @brief sound manager initialization
 *
 * This function will initialize the sound manager
 *
 *****************************************************************************/
void LedManager_Initialize( void )
{
  // clear the control
  memset( &atLedCtls, 0, ( LEDCTL_SIZE * LEDMANAGER_ENUM_MAX ));

  // reset the stack index
  #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
  nStackIndex = 0;
  #endif
  
  // clear the animation state
  eAnimationState = ANIMATION_STATE_IDLE;  
}

/******************************************************************************
 * @function LedManager_Control
 *
 * @brief control a LED
 *
 * This function will issue a command to control an led
 *
 * @param[in]   eLedSel     LED selection
 * @param[in]   eAction     LED action
 * @param[in]   wOption     optional parameter 
 *
 * @return      an approriate error
 * 
 *****************************************************************************/
LEDMANAGERERR LedManager_Control( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption )
{
  LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;
  LEDACTION         eLclAction;
  LEDMANAGERSELENUM eLclLed;
  
  // check for all leds
  if ( eLedSel == LEDMANAGER_ENUM_ALL )
  {
    // now check action
    if (( eAction >= LED_ACTION_ALLOFF ) && ( eAction < LED_ACTION_ALLMAX ))
    {
      // now for each led
      for ( eLclLed = 0; eLclLed < LEDMANAGER_ENUM_MAX; eLclLed++ )
      {
        switch( eAction )
        {
          case LED_ACTION_ALLOFF :
            eLclAction = LED_ACTION_OFF;
            break;
        
          case LED_ACTION_ALLON :
            eLclAction = LED_ACTION_ON;
            break;
        
          case LED_ACTION_ALLBLINKSLOW :
            eLclAction = LED_ACTION_BLINKSLOW;
            break;
        
          case LED_ACTION_ALLBLINKFAST :
            eLclAction = LED_ACTION_BLINKFAST;
            break;
            
          default :
            eLclAction = LED_ACTION_OFF;
            break;
        }
          
        // set the action
        SetLedAction( eLclLed, eLclAction, wOption );
      }
    }
    else
    {
      // report the error
      eError = LEDMANAGER_ERR_ILLACT;
    }      
  }
  else if ( eLedSel < LEDMANAGER_ENUM_MAX )
  {
    // now check action
    if ( eAction  < LED_ACTION_MAX )
    {
      // just call set local action
      SetLedAction( eLedSel, eAction, wOption );
    }
    else
    {
      // report the error
      eError = LEDMANAGER_ERR_ILLACT;
    }
  }
  else
  {
    // set the error
    eError = LEDMANAGER_ERR_ILLLED;
  }
  
  // return the error
  return( eError );
}

#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
/******************************************************************************
 * @function LedManager_RgbControl
 *
 * @brief control a RGB LED
 *
 * This function will issue a command to control an RGB led
 *
 * @param[in]   eLedSel     LED selection
 * @param[in]   eAction     LED action
 * @param[in]   eColor      LED color
 * @param[in]   wOption     optional parameter 
 *
 * @return      an approriate error
 * 
 *****************************************************************************/
LEDMANAGERERR LedManager_RgbControl( LEDMNGRRGBENUM eLedSel, LEDACTION eAction, LEDRGBCOLOR eColor, U16 wOption )
{
  LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;
  LEDRGBDEF         tRgbDef;

  if ( eLedSel < LEDMNGR_ENUM_RGB_MAX )
  {
    // now check action
    if ( eAction  < LED_ACTION_MAX )
    {
      // now check for color
      if ( eColor < LED_RGBCOLOR_MAX )
      {
        // get the def structure for this RGB LED
        MEMCPY_P( &tRgbDef, &atLedRgbsDef[ eLedSel ], LEDRGBDEF_SIZE );

        // now for each LED - set the action
        switch( eColor )
        {
          case LED_RGBCOLOR_BLK :
            // just call set local action
            SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
            break;

          case LED_RGBCOLOR_BLU :
            // just call set local action
            SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
            break;

          case LED_RGBCOLOR_GRN :
            // just call set local action
            SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
            SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
            break;

          case LED_RGBCOLOR_CYN :
            SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, wOption );
            SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
            SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
            break;

          case LED_RGBCOLOR_RED :
            SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
            SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
            break;

          case LED_RGBCOLOR_VIO :
            SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
            SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
            SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
            break;

          case LED_RGBCOLOR_ORN :
            SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
            SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
            SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
            break;

          case LED_RGBCOLOR_WHT :
            SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
            SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
            SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
            break;

          default :
            break;
        }
      }
      else
      {
        // report the error
        eError = LEDMANAGER_ERR_ILLCLR;
      }
    }
    else
    {
      // report the error
      eError = LEDMANAGER_ERR_ILLACT;
    }
  }
  else
  {
    // set the error
    eError = LEDMANAGER_ERR_ILLLED;
  }
  
  // return the error
  return( eError );
}
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/******************************************************************************
 * @function LedManager_PlayAnimation
 *
 * @brief play an animation
 *
 * This function will play a led animation
 *
 * @param[in]   eLedSeq   led animation
 *
 * @return      an approriate error
 * 
 *****************************************************************************/
LEDMANAGERERR LedManager_PlayAnimation( LEDMNGRANIMENUM eAnimation, U16 wOption )
{
  LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;

  // check to see if this is a stop request
  if ( eAnimation == LEDMNGR_ANIMATION_STOP )
  {
    // stop it
    eAnimationState = ANIMATION_STATE_IDLE;

    // clear all leds
    LedManager_Control( LEDMANAGER_ENUM_ALL, LED_ACTION_ALLOFF, 0 );
  }
  else 
  {
    // check for a valid animation
    if ( eAnimation < LEDMNGR_ANIMATION_MAX )
    {
      // if an animation is currently running, turn it off
      if ( eAnimationState != ANIMATION_STATE_IDLE )
      {
        // stop it
        eAnimationState = ANIMATION_STATE_IDLE;
      }
    
      // reset the index/set the current animation/store the option
      eCurrentAnimation = eAnimation;
      wAnimationOption = wOption;
    
      // start the sequence
      eAnimationState = ANIMATION_STATE_START;
    }
    else
    {
      // set the error
      eError = LEDMANAGER_ERR_ILLANI;
    }
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function LedManager_AnimateTask
 *
 * @brief sound manager task handler
 *
 * This function processes the led animiation task
 *
 * @param[in]   xArg    event argument
 * @return      TRUE    pop the event of the stack
 *
 *****************************************************************************/
BOOL LedManager_AnimateTask( TASKARG xArg )
{
  BOOL              bStatus = TRUE;
  LEDMANAGERSELENUM eLed;
  PLEDCTL           ptCtl;
  LEDDEF            tDef;
  LEDSEQENTRY       tSequence;
  
  // check to see if there is an animation running
  switch( eAnimationState )
  {
	case ANIMATION_STATE_IDLE :
		break;
		
    case ANIMATION_STATE_START :
      // get the current entry
      ptCurAnimation = ( PLEDSEQENTRY )PGM_RDWORD( apLedAnimationsDef[ eCurrentAnimation ] );

      // process the first event/switch to execution state/force a return by not popping the event off the queue
      eAnimationState = ANIMATION_STATE_EXEC;
      nCurAnimationIdx = 0;
      bStatus = FALSE;
      wCurrentCount = 1;
      break;
      
    case ANIMATION_STATE_EXEC :
      // decrement the count and test for next state
      if ( --wCurrentCount == 0 )
      {
        // get and act on the first entry
        MEMCPY_P( &tSequence, &ptCurAnimation[ nCurAnimationIdx++ ], LEDSEQENTRY_SIZE );
		    if ( tSequence.eEvent != LED_SEQEVENT_CALL )
		    {
			    LedManager_Control( tSequence.nLedEnum, tSequence.eAction, 0 );
		    }
		
        // determine the next state
        switch( tSequence.eEvent )
        {
          case LED_SEQEVENT_JMPBEG :
            // set the index back to 0
            nCurAnimationIdx = 0;

            // force a reloop
            wCurrentCount = 1;
            bStatus = FALSE;
            break;
            
          case LED_SEQEVENT_JMPSEQ :
            // set the index to the passed option
            nCurAnimationIdx = tSequence.nOption;

            // force a reloop
            wCurrentCount = 1;
            bStatus = FALSE;
            break;
            
          case LED_SEQEVENT_DONE :
            // test for a return from a call
            #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
            if ( nStackIndex != 0 )
            {
              // get the value from the stack
              eCurrentAnimation = atSeqStack[ --nStackIndex ].eCurrentAnimation;
              nCurAnimationIdx = atSeqStack[ nStackIndex ].nAnimationIdx;

              // get the current entry
              ptCurAnimation = ( PLEDSEQENTRY )PGM_RDWORD( apLedAnimationsDef[ eCurrentAnimation ] );

              // force a reloop
              wCurrentCount = 1;
              bStatus = FALSE;
            } 
            else
            {
              // turn off the animation
              eAnimationState = ANIMATION_STATE_IDLE;
            
              // post an event
              TaskManager_PostEvent( LEDMANAGER_ANIMATION_DONE_TASK, LEDMANAGER_ANIMATION_DONE_EVENT );
            }
            #else
            // turn off the animation
            eAnimationState = ANIMATION_STATE_IDLE;
            
            // post an event
            TaskManager_PostEvent( LEDMANAGER_ANIMATION_DONE_TASK, LEDMANAGER_ANIMATION_DONE_EVENT );
            #endif
            break;
                        
          case LED_SEQEVENT_WAIT :
            // now set the delay - goto to wait
            if ( wAnimationOption != 0 )
            {
              // use passed time
              wCurrentCount = TASK_TIME_MSECS( wAnimationOption ) / LEDMANAGER_ANIMATE_EXEC_RATE;
            }
            else
            {
              // use default time
              wCurrentCount = TASK_TIME_MSECS( tSequence.wDurationMsecs ) / LEDMANAGER_ANIMATE_EXEC_RATE;
            }
            break;
            
          case LED_SEQEVENT_NEXT :
            // force a reloop
            wCurrentCount = 1;
            bStatus = FALSE;
            break;
            
          #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
          case LED_SEQEVENT_CALL :
            // test for room on stack
            if ( nStackIndex < LEDMANAGER_ANIMATION_CALLSTACK_DEPTH )
            {
              // stuff the current index
              atSeqStack[ nStackIndex ].eCurrentAnimation = eCurrentAnimation;
              atSeqStack[ nStackIndex++ ].nAnimationIdx = nCurAnimationIdx;
            }
            
            // set the new animation
            eCurrentAnimation = tSequence.nOption;
            nCurAnimationIdx = 0;

            // get the current entry
            ptCurAnimation = ( PLEDSEQENTRY )PGM_RDWORD( apLedAnimationsDef[ eCurrentAnimation ] );
           
            // force a reloop
            wCurrentCount = 1;
            bStatus = FALSE;
            break;
          #endif
       
          default :
            break;
        }
      }
      break;
      
    default :
      break;
  }
  
  // process the led's based on their action
  for ( eLed = 0; eLed < LEDMANAGER_ENUM_MAX; eLed++ )
  {
    // get a pointer to the control
    ptCtl = &atLedCtls[ eLed ];
    
    // copy the control structure
    MEMCPY_P( &tDef, &atLedDefs[ eLed ], LEDDEF_SIZE );

    // process the state
    switch( ptCtl->eCurState )
    {
      case LED_STATE_OFF :
        // turn off the GPIO
        ChangeLedState( &tDef, OFF );
        break;
        
      case LED_STATE_ON :
        // turn on the GPIO
        ChangeLedState( &tDef, ON );
        break;
        
      case LED_STATE_BLNKOFF :
        // decrement count if not zero
        if ( ptCtl->wCounts == 0 )
        {
          // set the state to on
          ChangeLedState( &tDef, ON );
          ptCtl->wCounts = ptCtl->wOption;
          ptCtl->eCurState = LED_STATE_BLNKON;
        }
        else
        {
          // decrement the count
          ptCtl->wCounts--;
        }
        break;
        
      case LED_STATE_BLNKON :
        // decrement count if not zero
        if ( ptCtl->wCounts == 0 )
        {
          // set the state to off
          ChangeLedState( &tDef, OFF );
          ptCtl->wCounts = ptCtl->wOption;
          ptCtl->eCurState = LED_STATE_BLNKOFF;
        }
        else
        {
          // decrement the count
          ptCtl->wCounts--;
        }
        break;
        
      case LED_STATE_PULSE :
        // decrement count and test for zero
        if ( --ptCtl->wCounts == 0 )
        {
          // turn off the led/set the state to off
          ChangeLedState( &tDef, OFF );
          ptCtl->eCurState = LED_STATE_OFF;
        }
        else
        {
          // refresh it to on
          ChangeLedState( &tDef, ON );
        }
        break;
        
      default :
        break;
    }
  }
  
  // return the current status
  return( bStatus );
}

#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
/******************************************************************************
 * @function LedManager_ScanTask
 *
 * @brief led manager scan task handler
 *
 * This function processes scanning the LED matrix
 *
 * @param[in]   xArg    event argument
 * @return      TRUE    pop the event of the stack
 *
 *****************************************************************************/
BOOL LedManager_ScanTask( TASKARG xArg )
{
  GPIOPINENUM ePin;
  U8          nIdx;
  
  // now turn off all columns in this row
  for ( nIdx = 0; nIdx < LEDMANAGER_MATRIX_MAX_NUM_COLS; nIdx++ )
  {
    // get the pin
    ePin = PGM_RDBYTE( aeLedMatrixCols[ nIdx ] );
    Gpio_Set( ePin, !LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL );
  }
  
  // delay for a while
  #ifdef __ATMEL_AVR__
  _delay_us( 25 );
  #endif

  // turn off the current row index
  ePin = PGM_RDBYTE( aeLedMatrixRows[ nCurScanRow ] );
  Gpio_Set( ePin, !LEDMANAGER_MATRIX_ROW_ACTIVE_LEVEL );
  
  // increment the row
  nCurScanRow++;
  nCurScanRow %= LEDMANAGER_MATRIX_MAX_NUM_ROWS;
  
  // now set the columns for this row
  for ( nIdx = 0; nIdx < LEDMANAGER_MATRIX_MAX_NUM_COLS; nIdx++ )
  {
    // is this column on
    if ( anColVals[ nCurScanRow ] & BIT( nIdx ))
    {
      // turn on this column
      ePin = PGM_RDBYTE( aeLedMatrixCols[ nIdx ] );
      Gpio_Set( ePin, LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL );
    }
  }
  
  // now turn on the row
  ePin = PGM_RDBYTE( aeLedMatrixRows[ nCurScanRow ] );
  Gpio_Set( ePin, LEDMANAGER_MATRIX_ROW_ACTIVE_LEVEL );
  
  // always return true
  return( TRUE );
}
#endif  // MATRIX DEFS

/******************************************************************************
 * @function SetLedAction
 *
 * @brief set the led action
 *
 * This function will set the action for a given led
 *
 * @param[in]   eLedSel     LED selection
 * @param[in]   eAction     LED action
 * @param[in]   wOption     optional parameter
 *
 *****************************************************************************/
static void SetLedAction( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption )
{
  PLEDCTL       ptCtl;
  
  // get a pointer to the control
  ptCtl = &atLedCtls[ eLedSel ];
    
  // process the action
  switch( eAction )
  {
    case LED_ACTION_OFF :
      if ( ptCtl->bAllOffOnDisabled == FALSE )
      {
        // set the state off
        ptCtl->eCurState = LED_STATE_OFF;
      }
      break;
      
    case LED_ACTION_ON :
      if ( ptCtl->bAllOffOnDisabled == FALSE )
      {
        // set the state to on
        ptCtl->eCurState = LED_STATE_ON;
      }
      break;
      
    case LED_ACTION_BLINKSLOW :
      // set the state to blink off
      ptCtl->eCurState = LED_STATE_BLNKOFF;
      ptCtl->wOption = TASK_TIME_MSECS( LEDMANAGER_FLASH_SLOW_TIME ) / LEDMANAGER_ANIMATE_EXEC_RATE;
      ptCtl->wCounts = 0;
      ptCtl->bAllOffOnDisabled = FALSE;
      break;
      
    case LED_ACTION_BLINKFAST :
      // set the state to blink on
      ptCtl->eCurState = LED_STATE_BLNKOFF;
      ptCtl->wOption = TASK_TIME_MSECS( LEDMANAGER_FLASH_FAST_TIME ) / LEDMANAGER_ANIMATE_EXEC_RATE;
      ptCtl->wCounts = 0;
      ptCtl->bAllOffOnDisabled = FALSE;
      break;
      
    case LED_ACTION_PULSE :
      // set the state to pulse
      ptCtl->eCurState = LED_STATE_PULSE;
      ptCtl->wCounts = TASK_TIME_MSECS( wOption ) / LEDMANAGER_ANIMATE_EXEC_RATE;
      ptCtl->bAllOffOnDisabled = FALSE;
      break;

    case LED_ACTION_BLINKSLOW_LOCK :
      // set the state to blink off
      ptCtl->eCurState = LED_STATE_BLNKOFF;
      ptCtl->wOption = TASK_TIME_MSECS( LEDMANAGER_FLASH_SLOW_TIME ) / LEDMANAGER_ANIMATE_EXEC_RATE;
      ptCtl->wCounts = 0;
      ptCtl->bAllOffOnDisabled = TRUE;
      break;
      
    case LED_ACTION_BLINKFAST_LOCK :
      // set the state to blink on
      ptCtl->eCurState = LED_STATE_BLNKOFF;
      ptCtl->wOption = TASK_TIME_MSECS( LEDMANAGER_FLASH_FAST_TIME ) / LEDMANAGER_ANIMATE_EXEC_RATE;
      ptCtl->wCounts = 0;
      ptCtl->bAllOffOnDisabled = TRUE;
      break;
      
    case LED_ACTION_PULSE_LOCK :
      // set the state to pulse
      ptCtl->eCurState = LED_STATE_PULSE;
      ptCtl->wCounts = TASK_TIME_MSECS( wOption ) / LEDMANAGER_ANIMATE_EXEC_RATE;
      ptCtl->bAllOffOnDisabled = TRUE;
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function ChangeLedState
 *
 * @brief change the state of an led
 *
 * This function will change the state of the LED
 *
 * @param[in]   ptDef   pointer to the definition structure
 * @param[in]   bState  desired state of the LED
 *
 *****************************************************************************/
static void ChangeLedState( PLEDDEF ptDef, BOOL bState )
{
  // determine led type
  switch( ptDef->eDriveType )
  {
    case LED_DRIVETYPE_DIRECT :
      Gpio_Set( ptDef->tRowDrive.eDrivePin, bState );
      break;
      
    #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    case LED_DRIVETYPE_MATRIX :
      // determine the action
      if (( bState ) ^ ( LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL == HI ))
      {
        // clear the bit
        anColVals[ ptDef->tRowDrive.nRowIndex ] &= ~BIT( ptDef->nColIndex );
      }
      else
      {
        // set the bit
        anColVals[ ptDef->tRowDrive.nRowIndex ] |= BIT( ptDef->nColIndex );
      }
      break;
    #endif
    
    case LED_DRIVETYPE_SPECIAL :
      ptDef->tRowDrive.pvSpclFunc( ptDef->nColIndex, bState );
      break;
      break;
    
    default :
      break;
  }
}

/**@} EOF LedManager.c */
