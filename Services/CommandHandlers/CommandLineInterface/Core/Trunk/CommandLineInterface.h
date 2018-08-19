/******************************************************************************
 * @file CommandLineInterface.h
 *
 * @brief command line interface declarations 
 *
 * This file provides the declarations for the command line interface
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup CommandLineInterface
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _COMMANDLINEINTERFACE_H
#define _COMMANDLINEINTERFACE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskScheduler/TaskScheduler.h"

// Macros and Defines ---------------------------------------------------------
/// helper macro to create entries
#define CDMLINIFCMD_ENTRY( cmd, len, args, flg, compare, handler ) \
  { \
    .pszCommand = ( PC8 )&cmd, \
    .nCmpLen = len, \
    .nNumArgs = args, \
    .eFlagCompare = flg, \
    .nCmpValue = compare, \
    .tPointers.pvCmdHandler = handler \
  }

/// define the helper macro to create the table end
#define CMDLINIFCMD_ENDTBL( ) \
  { \
    .nCmpLen = CMD_LENGTH_ENDOFTABLE \
  }

/// define the helper macro to create the external table length
#define CMDLINIFCMD_EXTTBL( cmdtbl ) \
  { \
    .nCmpLen = CMD_LENGTH_EXTTABLE, \
    .tPointers.pvExtTable = ( PVOID )cmdtbl, \
  }
// enumerations ---------------------------------------------------------------
/// define the flag compare types
typedef enum _CMDLINIFFLAGCOMPARE
{
  CMDLINIFFLAG_COMPARE_NONE = 0,    // do nothing
  CMDLINIFFLAG_COMPARE_EQ,          // test for both being equal
  CMDLINIFFLAG_COMPARE_NE,          // test for both being not equal
  CMDLINIFFLAG_COMPARE_GT,          // test for flag being greater than entry in table
  CMDLINIFFLAG_COMPARE_LT,          // test for flag being less than entry in table
  CMDLINIFFLAG_COMPARE_GE,          // test for flag being greater than or equal to entry in table
  CMDLINIFFLAG_COMPARE_LE,          // test for flag being less than or equal to the entry in table
} CMDLINIFFLAGCOMPARE;

/// enumerate the errors
typedef enum _CMDLINIFERR
{
  CMDLINIF_ERR_NONE = 0,            ///< no error
  CMDLINIF_ERR_ILLHANDLE,           ///< illegal handler
  CMDLINIF_ERR_ILLARGINDEX,         ///< illegal argument index
} CMDLINIFERR;

/// enumerate the return statuss
typedef enum _CMDLINIFCMDSTS
{
  CMDLINIFCMD_STS_NONE,             ///< no errors
  CMDLINIFCMD_STS_ILLPROTENUM,      ///< illegal protocol enum
  CMDLINIFCMD_STS_ILLARGINDEX,      ///< illegal argument index
  CMDLINIFCMD_STS_NOCMDTABLE,       ///< on command table defined
  CMDLINIFCMD_STS_ILLCOMMAND,       ///< illegal command
  CMDLINIFCMD_STS_FLAGMISMATCH,     ///< flag mismatch
  CMDLINIFCMD_STS_ILLNUMARGS,       ///< illegal number of arguments
  CMDLINIFCMD_STS_ILLVALUE,         ///< illegal value
  CMDLINIFCMD_STS_OUTPUTBUFFER,     ///< output the buffer
  CMDLINIFCMD_STS_MSGPROCESSED,     ///< message processed
  CMDLINIFCMD_STS_TOOMANYTBLS,      ///< too many tables
  CMDLINIFCMD_STS_ILLPASSWORD,      ///< illegal password
} CMDLINIFCMDSTS;

// structures -----------------------------------------------------------------
/// define the Command line interface handle
typedef PVOID           ( PVCMDLINFHANDLE )

/// define the write char function type
typedef void ( *PVCMDLINIFWRITEFUNC )( PC8, U16 );

/// define the command handler function
typedef CMDLINIFCMDSTS  ( *PVCMDLINIFCMDHANDLERFUNC )( U8 );

/// define the command structure
typedef struct _CMDLINIFCMDENTRY
{
  PC8                         pszCommand;     ///< pointer to the command
  U8                          nCmpLen;        ///< compare length
  U8                          nNumArgs;       ///< number of arguments
  CMDLINIFFLAGCOMPARE         eFlagCompare;   ///< flag compare type
  U8                          nCmpValue;      ///< flag compare value
  union
  {
    PVCMDLINIFCMDHANDLERFUNC  pvCmdHandler;   ///< command handler
    PVOID                     pvExtTable;     ///< pointer to the command table
  } tPointers;
} CMDLINIFCMDENTRY, *PCMDLINIFCMDENTRY;
#define CMDLINIFCMDENTRY_SIZE    sizeof( CMDLINIFCMDENTRY )

// global parameter declarations -----------------------------------------------
extern	const CODE C8	g_szCmdLinIfErrStrn[ ];
extern  const CODE C8 g_szCmdLinIfNewLin[ ];
extern  const CODE C8 g_szCmdLinIfFmtHxw[ ];
extern  const CODE C8 g_szCmdLinIfFmtHxb[ ];
extern  const CODE C8 g_szCmdLinIfFmtDcb[ ];

// global function prototypes --------------------------------------------------
extern  void            CommandLineInterface_Initialize( void );
extern  PVCMDLINFHANDLE CommandLineInterface_Create( U8 nBufferSize, U8 nNumArgs, BOOL bEnableEcho, C8 cPrompt, C8 cEndOfLine, U8 nMenuDepth, PVCMDLINIFWRITEFUNC pvWriteFunc, PC8 pszDelims, PCMDLINIFCMDENTRY ptCmdTable );
extern  CMDLINIFERR     CommandLineInterface_ProcessChar( PVCMDLINFHANDLE ptHandle, C8 cRcvChar, U8 nCompareValue );
extern  CMDLINIFERR     CommandLineInterface_GetCommand( PVCMDLINFHANDLE ptHandle, PC8* ppCCommand );
extern  CMDLINIFERR     CommandLineInterface_GetNumArgs( PVCMDLINFHANDLE ptHandle, PU8 pnNumArgs );
extern  CMDLINIFERR     CommandLineInterface_GetArg( PVCMDLINFHANDLE ptHandle, U8 nArgIdx, PC8* ppcArg );
extern  CMDLINIFERR     CommandLineInterface_GetValue( PVCMDLINFHANDLE ptHandle, U8 nArgIdx, PU32 puValue );
extern  CMDLINIFERR     CommandLineInterface_GetBuffer( PVCMDLINFHANDLE pvHandle, PC8* ppcBuffer );
extern  CMDLINIFERR     CommandLineInterface_OutputBuffer( PVCMDLINFHANDLE ptHandle );
extern  CMDLINIFERR     CommandLineInterface_OutputString( PVCMDLINFHANDLE ptHandle, PC8 pcString );
extern  CMDLINIFERR     CommandLineInterface_DisplayBlock( PVCMDLINFHANDLE ptHandle, U16 wBaseAddr, PU8 pnBlockData, U16 wLength );
extern  CMDLINIFERR     CommandLineInterface_SetPromptCharacter( PVCMDLINFHANDLE ptHandle, C8 cPrompt );
extern  CMDLINIFERR     CommandLineInterface_OutputPrompt( PVCMDLINFHANDLE ptHandle );

/**@} EOF CommandLineInterface.h */

#endif  // _COMMANDLINEINTERFACE_H