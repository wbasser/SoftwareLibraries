/******************************************************************************
 * @file KeyHandler.h
 *
 * @brief Key handler declarations 
 *
 * This file 
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
 * \addtogroup KeyHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _KEYHANDLER_H
#define _KEYHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "KeyHandler/KeyHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error
typedef enum _KEYHANDERR
{
  KEYHAND_ERR_NONE = 0,         ///< no error
  KEYHAND_ERR_ILLENUM = 0xF0,   ///< illegal key enum
} KEYHANDERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        KeyHandler_Initialize( void );
extern  KEYHANDERR  KeyHandler_GetKeyState( KEYHANDENUM eKeyEnum, PBOOL pbState );

/**@} EOF KeyHandler.h */

#endif  // _KEYHANDLER_H