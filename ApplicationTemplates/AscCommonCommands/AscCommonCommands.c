/******************************************************************************
 * @file AscCommonCommands.c
 *
 * @brief 
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
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup AscCommonCommands
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AscCommonCommands/AscCommonCommands.h"

// library includes -----------------------------------------------------------
#include "ManufInfo/ManufInfo.h"
#include "DebugManager/DebugManager.h"
#include "EepromHandler/EepromHandler.h"
#include "SystemControlManager/SystemControlManager.h"
#include "LogHandler/LogHandler.h"
#include "RTCManager/RTCManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/// command handlers

// constant parameter initializations -----------------------------------------
/// declare the command strings

/// initialize the command table
const CODE ASCCMDENTRY atAscCommonCommandsTable[ ] =
{
  // add the external tables here
  #if ( MANUFINFO_ENABLE_DEBUGCOMMANDS == 1 )
  ASCCMD_EXTTBL( atManufInfoCmdHandlerTable ),
  #endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

  #if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  ASCCMD_EXTTBL( atEepromDbgHandlerTable ),
  #endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

  #if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
  ASCCMD_EXTTBL( atSysCtrlMngrCmdHandlerTable ),
  #endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGOMMANDS

  #if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  ASCCMD_EXTTBL( atDebugManagerCmdHandlerTable ),
  #endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

  #if ( LOGHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
  ASCCMD_EXTTBL( atLogHandlerCmdHandlerTable ),
  #endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS
  
  #if ( RTCMANAGER_ENABLE_DEBUG_COMMANDS == 1)
  ASCCMD_EXTTBL( atRtcManagerCmdHandlerTable ),
  #endif // RTCMANAGER_ENABLE_DEBUG_COMMANDS

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/// define the response strings

/**@} EOF AscCommonCommands.c */