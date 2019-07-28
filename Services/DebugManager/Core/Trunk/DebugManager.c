/******************************************************************************
 * @file DebugManager.c
 *
 * @brief debug handler implementation
 *
 * This file provides the implementation of the debug handler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DebugManager/DebugManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the elements per line
#if ( DEBUGMANAGER_ARGSIZE_BYTES == 1 )
  #define ELEMENTS_PER_LINE_HEX   ( 8 )
  #define ELEMENTS_PER_LINE_DEC   ( 8 )
#elif ( DEBUGMANAGER_ARGSIZE_BYTES == 2 )
  #define ELEMENTS_PER_LINE_HEX   ( 8 )
  #define ELEMENTS_PER_LINE_DEC   ( 4 )
#elif ( DEBUGMANAGER_ARGSIZE_BYTES == 4 )
  #define ELEMENTS_PER_LINE_HEX   ( 4 )
  #define ELEMENTS_PER_LINE_DEC   ( 2 )
#else
#endif

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the storage structure
typedef struct _DBGENTRY
{
  DBGARG  xArg1;        ///< argument #1
  DBGARG  xArg2;        ///< argument #2
} DBGENTRY, *PDBGENTRY;
#define DBGENTRY_SIZE   sizeof( DBGENTRY )

// local parameter declarations -----------------------------------------------
static  DBGENTRY  atDbgEntries[ DEBUGMANAGER_NUM_ENTRIES ];
static  U16       wIndex;

// local function prototypes --------------------------------------------------

/// command handlers
#if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
static  ASCCMDSTS CmdDmpDbg( U8 nCmdEnum );
#endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

// constant parameter initializations -----------------------------------------
#if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
/// define the string constants
static  const CODE C8 szNewLine[ ]  = { "\n\r" };
#if ( DEBUGMANAGER_ARGSIZE_BYTES == 1 )
  static  const CODE C8 szFmtHex[ ]   = { "%02X:%02X "};
  static  const CODE C8 szFmtDec[ ]   = { "%3d:%3d " };
#elif ( DEBUGMANAGER_ARGSIZE_BYTES == 2 )
  static  const CODE C8 szFmtHex[ ]   = { "%04X:%04X "};
  static  const CODE C8 szFmtDec[ ]   = { "%5u:%5u " };
#elif ( DEBUGMANAGER_ARGSIZE_BYTES == 4 )
  static  const CODE C8 szFmtHex[ ]   = { "%08X:%08X "};
  static  const CODE C8 szFmtDec[ ]   = { "%10lu:%10lu " };
#else
  static  const CODE C8 szFmtHex[ ]   = { "%02X:%02X "};
  static  const CODE C8 szFmtDec[ ]   = { "%3d:%3d " };
#endif

/// declare the command strings
static  const CODE C8 szDmpDbg[ ]   = { "DDBG" };

/// initialize the command table
const CODE ASCCMDENTRY g_atDebugManagerCmdHandlerTable[ ] =
{
  ASCCMD_ENTRY( szDmpDbg, 4, 1, ASCFLAG_COMPARE_NONE, 0, CmdDmpDbg ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};
#endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

/******************************************************************************
 * @function DebugManager_Initialize
 *
 * @brief debug handler inializations
 *
 * This function will initialize the debug handler
 *
 *****************************************************************************/
void DebugManager_Initialize( void )
{
  // reset/clear
  wIndex = 0;
}

/******************************************************************************
 * @function DebugManager_AddElement
 *
 * @brief add an element
 *
 * This function will add an element to the array and adjust the indices
 *
 * @param[in]   xArg1     argument #1
 * @param[in]   xArg2     argument #2
 *
 *****************************************************************************/
void DebugManager_AddElement( DBGARG xArg1, DBGARG xArg2 )
{
  #if (( DEBUGMANAGER_ENABLE_TIMESTAMP == 1) && ( DEBUGMANAGER_ARGSIZE_BYTES != 1 ))
  U32UN tTimeStamp;
  tTimeStamp.uValue = DEBUGMANAGER_GETTIME_MSECS( );

  // check for room
  if ( wIndex < DEBUGMANAGER_NUM_ENTRIES )
  {
    // store the values
    atDbgEntries[ wIndex ].xArg1 = tTimeStamp.awValue[ LE_U32_MSW_IDX ] & 0x7FFF;
    atDbgEntries[ wIndex ].xArg2 = tTimeStamp.awValue[ LE_U32_LSW_IDX ];
  
    // increment the index
    wIndex++;
  }
  #endif

  // check for room
  if ( wIndex < DEBUGMANAGER_NUM_ENTRIES )
  {
    // store the values
    atDbgEntries[ wIndex ].xArg1 = xArg1;
    atDbgEntries[ wIndex ].xArg2 = xArg2;
  
    // increment the index
    wIndex++;
  }
}


#if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
/******************************************************************************
 * @function CmdDmpDbg
 *
 * @brief dump debug command handler
 *
 * This function dumps the debug 
 *
 * @return  A
 *****************************************************************************/
static ASCCMDSTS CmdDmpDbg( U8 nCmdEnum )
{
  U32UN tTemp;
  U8    nElementCount, nElementSize;
  U16   wBufIdx;
  PC8   pszFormat, pcBuffer;

  // get the argument
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );

  // dump the debug data
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );
    
  // determine the mode
  switch( tTemp.anValue[ LE_U32_LSB_IDX ] )
  {
    case DEBUG_DUMPMODE_DEC :
      // decimal format
      pszFormat = ( PC8 )( szFmtDec );
      nElementSize = ELEMENTS_PER_LINE_DEC;
      break;
      
    case DEBUG_DUMPMODE_HEX :
      // decimal format
      pszFormat = ( PC8 )( szFmtHex );
      nElementSize = ELEMENTS_PER_LINE_HEX;
      break;
      
    default :
      nElementSize = 0;
      pszFormat = NULL;
      break;
  }

  // loop
  nElementCount = 0;
  for ( wBufIdx = 0; wBufIdx < wIndex; wBufIdx++ )
  {
    // output the buffer   
    SPRINTF_P( pcBuffer, pszFormat, atDbgEntries[ wBufIdx ].xArg1, atDbgEntries[ wBufIdx ].xArg2 );
    AsciiCommandHandler_OutputBuffer( nCmdEnum );

    // check for a new line
    if ( ++nElementCount == nElementSize )
    {
      // reset count/output a new line
      nElementCount = 0;
      AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szNewLine );
    }
  }

  // new line
  AsciiCommandHandler_OutputString( nCmdEnum, ( PC8 )szNewLine );
  
  // clear the index
  wIndex = 0;
    
  // return no error
  return( ASCCMD_STS_NONE );	
}
#endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

/**@} EOF DebugManager.c */
