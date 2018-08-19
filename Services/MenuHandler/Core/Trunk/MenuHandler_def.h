/******************************************************************************
 * @file MenuHandler_def.h
 *
 * @brief Menu Handler definitions 
 *
 * This file provides the definitions for the menu handler
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
 
// ensure only one instantiation
#ifndef _MENUHANDLER_DEF_H
#define _MENUHANDLER_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the menu item types
typedef enum _MENUITEMTYPE
{
  MENUITEM_TYPE_ENTRY = 0,            ///< entry 
  MENUITEM_TYPE_VALUEDEC,             ///< value - decimal number
  MENUITEM_TYPE_VALUESEL,             ///< value - selection
  MENUITEM_TYPE_PARAMDEC,             ///< parameter - decimal number
  MENUITEM_TYPE_PARAMSEL,             ///< parameter - selection
  MENUITEM_TYPE_VALUEONOFF,           ///< value - off/on
  MENUITEM_TYPE_PARAMONOFF,           ///< parameter - off/on
  MENUITEM_TYPE_PRVMENU,              ///< previous menu
  MENUITEM_TYPE_EXITMENU,             ///< exit menu
  MENUITEM_TYPE_MAX
} MENUITEMTYPE;

/// enumerate the menu keys
typedef enum _MENUKEYENUM
{
  MENU_KEY_ENUM_0 = 0,                ///< 0
  MENY_KEY_ENUM_1,                    ///< 1
  MENY_KEY_ENUM_2,                    ///< 2
  MENY_KEY_ENUM_3,                    ///< 3
  MENY_KEY_ENUM_4,                    ///< 4
  MENY_KEY_ENUM_5,                    ///< 5
  MENY_KEY_ENUM_6,                    ///< 6
  MENY_KEY_ENUM_7,                    ///< 7
  MENY_KEY_ENUM_8,                    ///< 8
  MENY_KEY_ENUM_9,                    ///< 9
  MENU_KEY_ENUM_ENT,                  ///< enter
  MENU_KEY_ENUM_CLR,                  ///< clear
  MENU_KEY_ENUM_LEFT,                 ///< left
  MENU_KEY_ENUM_RIGHT,                ///< right
  MENU_KEY_ENUM_DOWN,                 ///< down
  MENY_KEY_ENUM_UP,                   ///< up
  MENU_KEY_ENUM_MAX
} MENUKEYENUM;

// structures -----------------------------------------------------------------
/// define the function call to get a value
typedef U32 (* PVFUNCMENU )( void );

/// define a menu item entry structure
typedef struct _MENUITEMENTRY
{
  MENUITEMTYPE    eType;            ///< type
  PC8             pszPrompt;        ///< menu prompt
  U8              nOption;          ///< option
  PVOID           pvFuncMenu;       ///< special handling function/next menu
} MENUITEMENTRY, *PMENUITEMENTRY;
#define MENUITEMENTRY_SIZE            sizeof( MENUITEMENTRY )

/// define the menu structure
typedef struct _MENUENTRY
{
  U8              nNumberOfItems;   ///< number of items
  PC8             pszTitle;         ///< menu title
  PMENUITEMENTRY  ptMenuItems;      ///< pointer to menu items
} MENUENTRY, *PMENUENTRY;
#define MENUENTRY_SIZE                sizeof( MENUENTRY )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF MenuHandler_def.h */

#endif  // _MENUHANDLER_DEF_H