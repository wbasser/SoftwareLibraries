/******************************************************************************
 * @file DebugCommandsAsc.c
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 *CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup DebugCommandsAsc
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DebugCommandsAsc/DebugCommandsAsc.h"

// library includes -----------------------------------------------------------
#include "UART/Uart.h"
#include "ManufInfo/ManufInfo.h"
#include "SystemControlManager/SystemControlManager.h"
#include "EepromHandler/EepromHandler.h"
#include "DebugManager/DebugManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PC8   pcBuffer;

// local function prototypes --------------------------------------------------

/// command handlers

// constant parameter initializations -----------------------------------------

/// initialize the command table
const CODE ASCCMDENTRY atDebugCommandsAscTable[ ] =
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

  #if ( DEBUGMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
  ASCCMD_EXTTBL( atDebugManagerCmdHandlerTable ),
  #endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/// define the strings

/******************************************************************************
 * @function DebugCommandsAsc_Initialize
 *
 * @brief debug commands initilization
 *
 * This function will initialize the debug command handler
 *
 *****************************************************************************/
void DebugCommandsAsc_Initialize( void )
{
  // get a pointer to the buffer
  AsciiCommandHandler_GetBuffer( ASCCMD_ENUM_DBGCOMMANDS, &pcBuffer );
}


/******************************************************************************
 * @function DebugCommandsAsc_ProcessEvent
 *
 * @brief process an event
 *
 * This function will process an event
 *
 * @param[in]     xArg  event argument
 *
 * @return        TRUE to flush event
 *
 *****************************************************************************/
BOOL DebugCommandsAsc_ProcessEvent( TASKARG xArg )
{
  // process the character
  AsciiCommandHandler_ProcessChar( ASCCMD_ENUM_DBGCOMMANDS, ( C8 )xArg, SystemControlManager_GetMode( ));

  // retrn true
  return( TRUE );
}

/******************************************************************************
 * @function DebugCommandsAsc_PutChar
 *
 * @brief outputs a character
 *
 * This function will write a character to the appropriate device
 *
 * @param[in]     nChar     character to send
 *
 *****************************************************************************/
void DebugCommandsAsc_PutChar( U8 nChar )
{
  U16 wBytesWritten;

  // write the data
  Uart_Write( UART_DEV_ENUM_DBG, &nChar, 1, &wBytesWritten );
}

/******************************************************************************
 * @function DebugCommandsAsc_Write
 *
 * @brief output a block of characters
 *
 * This function will output a block of characters to the approrpiate device
 *
 * @param[in]    pcData     pointer to the data to send
 * @param[in]   wLength     length of the data
 *
 *****************************************************************************/
void DebugCommandsAsc_Write( PC8 pcData, U16 wLength )
{
  U16 wBytesWritten = 0;

  // write a block of data to the device
  do
  {
    // write a block to the UART
    Uart_Write( UART_DEV_ENUM_DBG, ( PU8)pcData, wLength, &wBytesWritten );
    wLength -= wBytesWritten;
    pcData += wBytesWritten;
  } while( wLength != 0 ); 
}

/**@} EOF DebugCommandsAsc.c */