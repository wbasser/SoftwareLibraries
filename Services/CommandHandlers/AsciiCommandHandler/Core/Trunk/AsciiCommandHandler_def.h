/******************************************************************************
 * @file AsciiCommandHandler_def.h
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ASCIICOMMANDHANDLER_DEF_H
#define _ASCIICOMMANDHANDLER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the command length used to indicate end of table
#define CMD_LENGTH_ENDOFTABLE             ( 0 )

/// define the command length used to indicate external table
#define CMD_LENGTH_EXTTABLE               ( 0xFF )

/// define the helper macro for creating storage
#define ASCCMD_BUFFER( name, bufsize, numargs ) \
  static C8  ac ## name ## Buffer[ bufsize ]; \
  static PC8 pc ## name ## Args[ numargs ]; \
  static U32 au ## name ## Vals[ numargs ];

#define ASCCMD_DEF( bufsize, numargs, name, enbecho, prompt, eolchar, writefunc, delims, cmdtbl ) \
  { \
    .nBufSize = bufsize, \
    .nNumArgs = numargs, \
    .pcBuffer = ( PC8 )&ac ## name ## Buffer, \
    .ppcArgs = ( PC8*)&pc ## name ## Args, \
    .puVals = ( PU32 )&au ## name ## Vals, \
    .bEchoChar = enbecho, \
    .cPromptChar = prompt, \
    .cEOLChar = eolchar, \
    .pvWriteFunc = writefunc, \
    .pszDelims = ( PC8 )delims, \
    .pvCmdTable = cmdtbl, \
  }

/// helper macro to create entries
#define ASCCMD_ENTRY( cmd, len, args, flg, compare, handler ) \
  { \
    .pszCommand = ( PC8 )&cmd, \
    .nCmpLen = len, \
    .nNumArgs = args, \
    .eFlagCompare = flg, \
    .nCmpValue = compare, \
    .tPointers.pvCmdHandler = handler \
  }

/// define the helper macro to create the table end
#define ASCCMD_ENDTBL( ) \
  { \
    .nCmpLen = CMD_LENGTH_ENDOFTABLE \
  }

/// define the helper macro to create the external table length
#define ASCCMD_EXTTBL( cmdtbl ) \
  { \
    .nCmpLen = CMD_LENGTH_EXTTABLE, \
    .tPointers.pvExtTable = ( PVOID )cmdtbl, \
  }

// enumerations ---------------------------------------------------------------
/// define the flag compare types
typedef enum _ASCFLAGCOMPARE
{
  ASCFLAG_COMPARE_NONE = 0,     // do nothing
  ASCFLAG_COMPARE_EQ,           // test for both being equal
  ASCFLAG_COMPARE_NE,           // test for both being not equal
  ASCFLAG_COMPARE_GT,           // test for flag being greater than entry in table
  ASCFLAG_COMPARE_LT,            // test for flag being less than entry in table
  ASCFLAG_COMPARE_GE,           // test for flag being greater than or equal to entry in table
  ASCFLAG_COMPARE_LE,           // test for flag being less than or equal to the entry in table
} ASCFLAGCOMPARE;

/// enumerate the status
typedef enum _ASCCMDSTS
{
  ASCCMD_STS_NONE,            ///< no errors
  ASCCMD_STS_ILLPROTENUM,     ///< illegal protocol enum
  ASCCMD_STS_ILLARGINDEX,     ///< illegal argument index
  ASSCMD_STS_NOCMDTABLE,      ///< on command table defined
  ASCCMD_STS_ILLCOMMAND,      ///< illegal command
  ASCCMD_STS_FLAGMISMATCH,    ///< flag mismatch
  ASCCMD_STS_ILLNUMARGS,      ///< illegal number of arguments
  ASCCMD_STS_ILLVALUE,        ///< illegal value
  ASCCMD_STS_OUTPUTBUFFER,    ///< output the buffer
  ASCCMD_STS_MSGPROCESSED,    ///< message processed
  ASCCMD_STS_TOOMANYTBLS,     ///< too many tables
  ASCCMD_STS_ILLPASSWORD,     ///< illegal password
} ASCCMDSTS;

// structures -----------------------------------------------------------------
/// define the write char function type
typedef void ( *PVASCWRITEFUNC )( PC8, U16 );

/// define the command handler function
typedef ASCCMDSTS  ( *PVASCCMDHANDLERFUNC )( U8 );

/// define the command structure
typedef struct _ASCCMDENTRY
{
  PC8                 pszCommand;   ///< pointer to the command
  U8                  nCmpLen;      ///< compare length
  U8                  nNumArgs;     ///< number of arguments
  ASCFLAGCOMPARE      eFlagCompare; ///< flag compare type
  U8                  nCmpValue;    ///< flag compare value
  union
  {
    PVASCCMDHANDLERFUNC pvCmdHandler; ///< command handler
    PVOID               pvExtTable;   ///< pointer to the command table
  } tPointers;
} ASCCMDENTRY, *PASCCMDENTRY;
#define ASCCMDENTRY_SIZE    sizeof( ASCCMDENTRY )

/// define the ASCII protocol structure
typedef struct _ASCCMDDEF
{
  U8                  nBufSize;     ///< buffer size
  U8                  nNumArgs;     ///< number of arguments to parse
  PC8                 pcBuffer;     ///< pointer to the buffer
  PC8*                ppcArgs;      ///< pointer to the arguments array
  PU32                puVals;       ///< pointer to the values array
  BOOL                bEchoChar;    ///< set to true if character echo is required
  C8                  cPromptChar;  ///< set this equal to the prompt character, 0 = no prompt
  C8                  cEOLChar;     ///< end of line character
  PVASCWRITEFUNC      pvWriteFunc;  ///< pointer to a function that will data to a device
  PC8                 pszDelims;    ///< pointer to the delimiter string
  ASCCMDENTRY const * pvCmdTable;   ///< pointer to the command table
} ASCCMDDEF, *PASCCMDDEF;
#define ASCCMDDEF_SIZE      sizeof( ASCCMDDEF )

/**@} EOF AsciiCommandHandler_def.h */

#endif  // _ASCIICOMMANDHANDLER_DEF_H