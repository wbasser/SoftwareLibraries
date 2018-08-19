/******************************************************************************
 * @file MenuHandler.c
 *
 * @brief menu handler implementation 
 *
 * This file provides the implementation for the menu handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup MenuHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MenuHandler/MenuHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the edit function calling prototype
typedef BOOL  *( PVEDITFUNC )( U8 );

/// define the menu item stack entry
typedef struct _STACKITEM
{
  U8          nCurMenuIndex;
  PMENUENTRY  ptCurMenu;
} STACKITEM, *PSTACKITEM;
#define STACKITEM_SIZE              sizeof( STACKITEM )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8              nCurMenuIndex;
static  U8              nMaxMenuIndex;
static  U8              nCurStackIndex;
static  U8              nPromptRow;
static  U8              nPromptCol;
static  U8              nValueRow;
static  U8              nValueCol;
static  U32             uCurValue;
static  PMENUENTRY      ptCurMenu;
static  PMENUITEMENTRY  ptBaseMenuItems;
static  PVEDITFUNC      pvCurEditFunc;
static  STACKITEM       atMenuStack[ MENUHANDLER_MAXIMUM_DEPTH ];

// local function prototypes --------------------------------------------------
static  void    DisplayPrompts( BOOL vDisplayPrompt );
static  BOOL    ProcessCommandKey( U8 nKey );
static  void    PushMenu( void );
static  void    PopMenu( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function MenuHandler_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the menu handler
 *
 *****************************************************************************/
void MenuHandler_Initialize( void )
{
  // set up the control variables based on top level menu
  ptCurMenu = &tTopLevelMenu;
  nCurMenuIndex = 0;
  nMaxMenuIndex = PGM_RDBYTE( ptCurMenu->nNumberOfItems );
  nCurStackIndex = 0;
  
  // set the row/col for prompt/value
  nPromptRow = nPromptCol = nValueRow = nValueCol = 0;
  if ( MENUHANDLER_MULTIROW_ENABLE == 1 )
  {
    nValueRow = nValueCol = 1
  }
  
  // set the default edit function
  pvCurEditFunc = ProcessCommandKey;
  
  // display the prompts
  DisplayPrompts( TRUE );
}

/******************************************************************************
 * @function MenuHandler_ProcessKey
 *
 * @brief process valid key strokes
 *
 * This function will process valid key strokes
 *
 * @param[in]   nKey      key entered
 *
 * @return        TRUE if exit is required
 *
 *****************************************************************************/
BOOL MenuHandler_ProcessKey( U8 nKey )
{
  // call the current edit function
  return( pvCurEditFunc( nKey ));
}

/******************************************************************************
 * @function DisplayPrompts
 *
 * @brief display the prompt/first value
 *
 * This function will display the prompt and first value
 *
 * @param[in]   bDisplayPrompt
 *
 *****************************************************************************/
static void DisplayPrompts( BOOL bDisplayPrompt )
{
  // test for display of the prompt
  if (( MENUHANDLER_MULTIROW_ENABLE == 1 ) && ( bDisplayPrompt == TRUE ))
  {
    // display the prompt
    MenuHandler_DisplayLine( nPromptRow, nPromptCol, ptCurMenu->pszTitle };
  }
  
  // now display the first menu item
  ptBaseMenuItems = PGM_RDWORD( ptCurMenu->ptMenuItems );
  MenuHandler_DisplayLine( nValueRow, nValueCol, ( ptBaseMenuItems + nCurMenuIndex )->pszPrompt );
}

/******************************************************************************
 * @function ProcessCommandKey
 *
 * @brief process command keys
 *
 * This function will process the initial keys that allow manipulation
 * of the menu
 *
 * @param[in]     nkey    key entered  
 *
 * @return        TRUE if exit is required
 *
 *****************************************************************************/
static BOOL ProcessCommandKey( U8 nKey )
{
  BOOL          bExit = FALSE;
  MENUITEMTYPE  eType;
  
  
  // process the key
  switch( nKey )
  {
    case MENUHANDLER_KEY_ENT :
      // get the menu item type
      eType = PGM_RDBYTE(( ptBaseMenuItems + nCurMenuIndex )->eType );
      
      // process it
      switch( eType )
      {
        case MENUITEM_TYPE_ENTRY : 
          // push the current menu/display the new prompt/item
          PushMenu( );
          DisplayPrompts( TRUE );
          break;
          
        case MENUITEM_TYPE_VALUEDEC :
          break;
          
        case MENUITEM_TYPE_VALUESEL :
          break;
          
        case MENUITEM_TYPE_PARAMDEC :
          break;
          
        case MENUITEM_TYPE_PARAMSEL :
          break;
          
        case MENUITEM_TYPE_VALUEONOFF :
          // get the current value
          uCurValue = 
          break;
          
        case MENUITEM_TYPE_PARAMONOFF :
          break;
          
        case MENUITEM_TYPE_PRVMENU :
          // pop the last menu/display the new prompt/item
          PopMenu( );
          DisplayPrompts( TRUE );
          break;
          
        case MENUITEM_TYPE_EXITMENU :
          // clear the display/set the exit flag
          MenuHandler_DisplayClear( );
          bExit = TRUE;
          break;
          
        default :
          break;
      }
      break;

    case MENUHANDLER_KEY_CLR :
      break;

    case MENUHANDLER_KEY_DN :
      // decrement the current index
      if ( nCurMenuIndex != 0 )
      {
        // decrement it
        nCurMenuIndex--;
      }
      else
      {
        // set it to max - 1
        nCurMenuIndex = nMaxMenuIndex - 1;
      }
      
      // update the display
      DisplayPrompts( );
      break;

    case MENUHANDLER_KEY_UP :
      // increment the current index
      nCurMenuIndex++;
      nCurMenuIndex %= nMaxMenuIndex;
      
      // update the display
      DisplayPrompts( FALSE );
      break;
      
    default :
      break;
  }
  
  // return the exit state
  return( bExit );
}

/******************************************************************************
 * @function PushMenu
 *
 * @brief push menu
 *
 * This function will push the current menu item onto the stack if room
 *
 *****************************************************************************/
static void PushMenu( void )
{
  PMENUITEMENTRY  ptCurItem;
  
  // is there room
  if ( nCurStackIndex < MENUHANDLER_MAXIMUM_DEPTH )
  {
    // store the current menu/index at current location
    atMenuStack[ nCurStackIndex ].nCurMenuIndex = nCurMenuIndex;
    atMenuStack[ nCurStackIndex ].ptCurMenu = ptCurMenu;
    
    // increment the stack index
    nCurStackIndex++;
    
 		// get the new menu/get the count/reset the index
    ptCurMenu = PGM_RDWORD(( PMENUENTRY )( ptBaseMenuItems + nCurMenuIndex )->pvFuncMenu );
    nCurMenuIndex = 0;
    nMaxMenuIndex = PGM_RDBYTE( ptCurMenu->nNumberOfItems );
  }
}

/******************************************************************************
 * @function PopMenu
 *
 * @brief pop menu
 *
 * This function will pop a menu off of the stack if one is available
 *
 *****************************************************************************/
static void PopMenu( void )
{
  // is there anything on stack
  if ( nCurStackIndex != 0 )
  {
    // decrement the stack index
    nCurStateIndex--;
    
    // pop the saved menu
    ptCurMenu = atMenuStack[ nCurStackIndex ].ptCurMenu;
    nCurMenuIndex = atMenuStack[ nCurStackIndex ].nCurMenuIndex;
    nMaxMenuIndex = PGM_RDBYTE( ptCurMenu->nNumberOfItems );
  }
}

/**@} EOF MenuHandler.c */
