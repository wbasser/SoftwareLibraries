/******************************************************************************
 * @file ButtonManager.c
 *
 * @brief button manager implementation
 *
 * This file provides the implementation for the button magnager
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// test for task arugment size a minium of 2 bytes
#if (( TASK_TSKARG_SIZE_BYTES == 1 ) && ( BTNMNGR_ENUM_MAX > 31 ))
{
  #error Task Argument size must be at least 2 bytes!
}
#endif

// enumerations ---------------------------------------------------------------
// define the button states
typedef enum
{
  BTN_STATE_RELEASED = 0,   ///< button is released
  BTN_STATE_DEBOUNCE,       ///< button is being debounce
  BTN_STATE_PRESSED,        ///< button has been pressed
  BTN_STATE_STUCK,          ///< button is stuck
} BTNSTATE;

/// enumerate the event indices
typedef enum _BTNMNGREVNINDICES
{
  BTNMNGR_EVNINDICES_EVENT = LE_U16_MSB_IDX,
  BTNMNGR_EVNINDICES_BUTTON = LE_U16_LSB_IDX
} BTNMNGREVNINDICES;

// structures -----------------------------------------------------------------
// define the control structure
typedef struct
{
  BTNSTATE  eState;         ///< button state
  U16       wDelayCounts;   ///< delay counts
  U16       wHoldCounts;    ///< hold count
  BOOL      bOffOn;         ///< button on/off
} BTNCTL, *ptBtnCtl;
#define BTNCTL_SIZE   sizeof( BTNCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BTNMNGRCFG  tConfig;
static  BTNCTL      atBtnCtls[ BTNMNGR_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static  void  PostEvent( BTNMNGRENUM eKey, BTNMNGREVENTS eEvent, PBTNMNGRDEF ptDef );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ButtonManager_Initialize
 *
 * @brief initialize the button manager
 *
 * This function initializes the control structure for the button manager
 *
 *****************************************************************************/
void ButtonManager_Initialize( void )
{
  // get a copy of the config block
  MEMCPY_P( &tConfig, &tBtnMgrCfg, BTNMNGRCFG_SIZE );
  
  // adjust the times
  tConfig.wDebounceTimeMsecs /= BTNMNGR_PROCESS_RATE_MSECS;
  tConfig.wRepeatDelayMsecs /= BTNMNGR_PROCESS_RATE_MSECS;
  tConfig.wRepeatRateMsecs /= BTNMNGR_PROCESS_RATE_MSECS;
  tConfig.wShortHoldTimeMsecs /= BTNMNGR_PROCESS_RATE_MSECS;
  tConfig.wLongHoldTimeMsecs /= BTNMNGR_PROCESS_RATE_MSECS;
  tConfig.wStuckTimeMsecs /= BTNMNGR_PROCESS_RATE_MSECS;
  
  // clear the control structure
  memset( atBtnCtls, 0, sizeof( atBtnCtls ));
}

/******************************************************************************
 * @function ButtonManager_ResetAllStates
 *
 * @brief reset all states
 *
 * This function clears all states to released
 *
 *****************************************************************************/
void ButtonManager_ResetAllStates( void )
{
  // clear the control structure
  memset( atBtnCtls, 0, sizeof( atBtnCtls ));
}

/******************************************************************************
 * @function ButtonManager_Process
 *
 * @brief button manager process
 *
 * This function processes all defined buttons and determines there current statees
 *
 * @param[in]   xArg    task argument
 *  
 * @return      always return true
 *
 *****************************************************************************/
BOOL ButtonManager_Process( TASKARG xArg )
{
  BTNMNGRENUM   eBtn;
  BTNCTL*       ptBtnCtl;
  BTNMNGRDEF    tBtnDef;
  BOOL          bKeyState;
  
  // for each button
  for ( eBtn = 0; eBtn < BTNMNGR_ENUM_MAX; eBtn++ )
  {
    // get pointer to control
    ptBtnCtl = &atBtnCtls[ eBtn ];
    
    // copy the def
    MEMCPY_P( &tBtnDef, &atBtnMgrDefs[ eBtn ], BTNMNGRDEF_SIZE );

    // get the current state
    bKeyState = ButtonManager_GetKeyStatus( tBtnDef.nKeyEnum );
    
    // deterime which state this button is in
    switch( ptBtnCtl->eState )
    {
      // key is released
      case BTN_STATE_RELEASED :
        // is this button pressed
        if ( bKeyState )
        {
          // set the debounce time/goto debounce state
          ptBtnCtl->wDelayCounts = tConfig.wDebounceTimeMsecs;
          ptBtnCtl->eState = BTN_STATE_DEBOUNCE;
        }
        break;
        
      // key is being debounced
      case BTN_STATE_DEBOUNCE :
        // check to see if key is still pressed
        if ( bKeyState )
        {
          // set the delay to repeat delay/set state to pressed/generate an event
          ptBtnCtl->wDelayCounts = tConfig.wRepeatDelayMsecs;
          ptBtnCtl->eState = BTN_STATE_PRESSED;
          if ( tBtnDef.tEventFlags.bToggleEnable )
          {
            // toggle the button off/on state/post the toggle event
            ptBtnCtl->bOffOn ^= TRUE;
            PostEvent( eBtn, ( ptBtnCtl->bOffOn ) ? BTNMNGR_EVENT_BTNON : BTNMNGR_EVENT_BTNOFF, &tBtnDef );
          }
          else
          {
            // post a pressed event
            PostEvent( eBtn, BTNMNGR_EVENT_PRESSED, &tBtnDef );
          }
        }
        else
        {
          // just go back to released
          ptBtnCtl->eState = BTN_STATE_RELEASED;
        }
        break;
        
      // key is pressed
      case BTN_STATE_PRESSED :
        // check to see if key is still pressed
        if ( bKeyState )
        {
          if ( --ptBtnCtl->wDelayCounts == 0 )
          {
            // set delay time for repeat delay/goto press
            ptBtnCtl->wDelayCounts = tConfig.wRepeatRateMsecs;
            PostEvent( eBtn, BTNMNGR_EVENT_REPEAT, &tBtnDef );
          }
          
          // increment the hold time
          ptBtnCtl->wHoldCounts++;

          // check for short hold
          if ( ptBtnCtl->wHoldCounts >= tConfig.wShortHoldTimeMsecs )
          {
            // generate an event
            PostEvent( eBtn, BTNMNGR_EVENT_SHORTHOLD, &tBtnDef );
          }
          
          // check for long hold
          if ( ptBtnCtl->wHoldCounts >= tConfig.wLongHoldTimeMsecs )
          {
            // generate an event
            PostEvent( eBtn, BTNMNGR_EVENT_LONGHOLD, &tBtnDef );
          }
          
          // check for stuck key
          if ( ptBtnCtl->wHoldCounts >= tConfig.wStuckTimeMsecs )
          {
            // generate an event
            PostEvent( eBtn, BTNMNGR_EVENT_STUCK, &tBtnDef );
            
            // goto stuck key state
            ptBtnCtl->eState = BTN_STATE_STUCK;
          }
        }
        else
        {
          // back to released
          ptBtnCtl->eState = BTN_STATE_RELEASED;
          
          // generate an event/reset the hold counts
          PostEvent( eBtn, BTNMNGR_EVENT_RELEASED, &tBtnDef );
          ptBtnCtl->wHoldCounts = 0;
        }
        break;
        
      // key is stuck
      case BTN_STATE_STUCK :
        // check for key released
        if ( bKeyState )
        {
          // generate a release event
          PostEvent( eBtn, BTNMNGR_EVENT_RELEASED, &tBtnDef );
          
          // goto released state
          ptBtnCtl->eState = BTN_STATE_RELEASED;
        }
        break;
        
      default :
        // should never happen - reset back to released
        ptBtnCtl->eState = BTN_STATE_RELEASED;
        break;
    }
  }

  // return true
  return( TRUE );
}

/******************************************************************************
 * @function PostEvent
 *
 * @brief post event or call the callback function
 *
 * This function either post and event or call the callback if defined
 *
 * @param[in]   eKey      key
 * @param[in]   eEvent    event
 * @param[in]   ptDef     pointer to the def structure
 *  
 *****************************************************************************/
static void PostEvent( BTNMNGRENUM eKey, BTNMNGREVENTS eEvent, PBTNMNGRDEF ptDef )
{
  U16UN tEvent;
  
  // determine if there the event is enabled
  if (( ptDef->tEventFlags.bReleaseEnable   && ( eEvent == BTNMNGR_EVENT_RELEASED ))  ||
      ( ptDef->tEventFlags.bPressEnable     && ( eEvent == BTNMNGR_EVENT_PRESSED ))   ||
      ( ptDef->tEventFlags.bRepeatEnable    && ( eEvent == BTNMNGR_EVENT_REPEAT ))    ||
      ( ptDef->tEventFlags.bShortHoldEnable && ( eEvent == BTNMNGR_EVENT_SHORTHOLD )) ||
      ( ptDef->tEventFlags.bToggleEnable    && ( eEvent == BTNMNGR_EVENT_BTNON ))     ||
      ( ptDef->tEventFlags.bToggleEnable    && ( eEvent == BTNMNGR_EVENT_BTNOFF ))    ||
      ( ptDef->tEventFlags.bLongHoldEnable  && ( eEvent == BTNMNGR_EVENT_LONGHOLD )))
  {
    // determine the reporting method
    switch( ptDef->eRptMethod )
    {
      case BTNMNGR_RPTMETHOD_CB :
        // event is enabled - process
        if ( ptDef->pvCallback != NULL )
        {
          // call the cllback
          ptDef->pvCallback( eKey, eEvent );
        }
        break;
        
      case BTNMNGR_RPTMETHOD_EVENT :
        // post it
        #if ( TASKARG_SIZE_BYTES == 1 )
        tEvent.anValue[ BTNMNGR_EVNINDICES_BUTTON ] = ( eEvent < EVENT_SHIFT_CNT );
        tEvent.anValue[ BTNMNGR_EVNINDICES_BUTTON ] |= eKey;
        #else
        tEvent.anValue[ BTNMNGR_EVNINDICES_EVENT ] = eEvent;
        tEvent.anValue[ BTNMNGR_EVNINDICES_BUTTON ] = eKey;
        #endif

        // post it
        TaskManager_PostEvent( ptDef->eTaskEnum, tEvent.wValue );
        break;
        
      default :
        break;
    }
  }
}

/**@} EOF ButtonManager.c */
