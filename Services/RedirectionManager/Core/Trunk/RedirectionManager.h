/******************************************************************************
 * @file RedirectionManager
 *
 * @brief redirection manager declarations
 *
 * This file provides the definitions for the redirection manager
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
 * $Rev: $
 * 
 *
 * \addtogroup RedirectionManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _REDIRECTIONMANAGER_H
#define _REDIRECTIONMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RedirectionManager/RedirectionManager_cfg.h"

// library includes -----------------------------------------------------------
#if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  #include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif  // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
#if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  extern  const CODE ASCCMDENTRY g_atRedirectionManagerDbgCmdTable[ ];
#endif  // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

// global function prototypes --------------------------------------------------
extern  void  RedirectionManager_Initialize( void );
extern  BOOL  RedirectionManager_SetRedirect( REDIRECTENUM eSrc, REDIRECTENUM eDst );
extern  BOOL  RedirectionManager_RedirectChar( REDIRECTENUM eSrc, U8 nChar );

/**@} EOF RedirectionManager.h */

#endif  // _REDIRECTIONMANAGER_H