/******************************************************************************
 * @file AsciiCommandHandler.h
 *
 * @brief ASCII protocol Handler declarations
 *
 * This file provides the ASCII protocol handler declarations
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
 * \addtogroup AsciiCommandHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ASCIICOMMANDHANDLER_H
#define _ASCIICOMMANDHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern	const CODE C8	g_szAsciiErrStrn[ ];
extern  const CODE C8 g_szAsciiNewLin[ ];
extern  const CODE C8 g_szAsciiFmtHxw[ ];
extern  const CODE C8 g_szAsciiFmtHxb[ ];
extern  const CODE C8 g_szAsciiFmtDcb[ ];

// global function prototypes --------------------------------------------------
extern  void        AsciiCommandHandler_Initialize( void );
extern  ASCCMDSTS   AsciiCommandHandler_ProcessChar( ASCCMDENUM eProtEnum, C8 cRcvChar, U8 nCompareValue );
extern  ASCCMDSTS   AsciiCommandHandler_GetCommand( ASCCMDENUM eProtEnum, PC8* ppCCommand );
extern  ASCCMDSTS   AsciiCommandHandler_GetNumArgs( ASCCMDENUM eProtEnum, PU8 pnNumArgs );
extern  ASCCMDSTS   AsciiCommandHandler_GetArg( ASCCMDENUM  eProtEnum, U8 nArgIdx, PC8* ppcArg );
extern  ASCCMDSTS   AsciiCommandHandler_GetValue( ASCCMDENUM eProtEnum, U8 nArgIdx, PU32 puValue );
extern  ASCCMDSTS   AsciiCommandHandler_GetBuffer( ASCCMDENUM eProtEnum, PC8* ppcBuffer );
extern  ASCCMDSTS   AsciiCommandHandler_OutputBuffer( ASCCMDENUM eProtEnum );
extern  ASCCMDSTS   AsciiCommandHandler_OutputString( ASCCMDENUM eProtEnum, PC8 pcString );
extern  ASCCMDSTS   AsciiCommandHandler_DisplayBlock( ASCCMDENUM eProtEnum, U16 wBaseAddr, PU8 pnBlockData, U16 wLength );
extern  ASCCMDSTS   AsciiCommandHandler_SetPromptCharacter( ASCCMDENUM eProtEnum, C8 cPrompt );
extern  ASCCMDSTS   AsciiCommandHandler_OutputPrompt( ASCCMDENUM eProtEnum );

/**@} EOF AsciiCommandHandler.h */

#endif  // _ASCIICOMMANDHANDLER_H