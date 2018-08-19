/******************************************************************************
 * @file DebugManager.h
 *
 * @brief Debug handler declarations
 *
 * This file provides the declarations for the debug handler
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
 * \addtogroup DebugManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DEBUGMANAGER_H
#define _DEBUGMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DebugManager/DebugManager_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the dump mode
typedef enum _DEBUGDUMPMODE
{
  DEBUG_DUMPMODE_HEX = 0,     ///< hex mode
  DEBUG_DUMPMODE_DEC          ///< decimal mode
} DEBUGDUMPMODE;

// structures -----------------------------------------------------------------
/// determine the debug argument size argument size
#if ( DEBUGMANAGER_ARGSIZE_BYTES == 1 )
	typedef U8		DBGARG;
	typedef	PU8		PDBGARG;
#elif ( DEBUGMANAGER_ARGSIZE_BYTES == 2 )
	typedef U16		DBGARG;
	typedef PU16	PDBGARG;
#elif ( DEBUGMANAGER_ARGSIZE_BYTES == 4 )
	typedef U32		DBGARG;
	typedef	PU32	PDBGARG;
#else
	typedef	U8		DBGARG;
	typedef	PU8		PDBGARG;
#endif

// global parameter declarations -----------------------------------------------
#if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
extern  const CODE ASCCMDENTRY atDebugManagerCmdHandlerTable[ ];
#endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

// global function prototypes --------------------------------------------------
extern  void  DebugManager_Initialize( void );
extern  void  DebugManager_AddElement( DBGARG xArg1, DBGARG xArg2 );

/**@} EOF DebugManager.h */

#endif  // _DEBUGMANAGER_H