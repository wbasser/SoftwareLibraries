/******************************************************************************
 * @file AsciiCommandHandler.c
 *
 * @brief ASCII protocol handler implementation
 *
 * This file provides the ASCII protocol handler impelementation
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
 * \addtogroup AsciiProtocolHandler
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler.h"

// system includes ------------------------------------------------------------
#include "ctype.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the special characters
#define	CH_CAN	                ( 0x18 )
#define	CH_ESC	                ( 0x1B )
#define CH_SPC                  ( 0x20 )
#define CH_TIL                  ( 0x7E )

/// define the number of elements per line for display block
#define NUM_ELEMS_LINE          ( 16 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the table stack structure
typedef struct _TBLSTACK
{
  PVASCCMDHANDLERFUNC  const *ptTable;  ///< maximum number of tables
  U8                          nIndex;   ///< current index
 } TBLSTACK, *PTBLSTACK;
#define TBLSTACK_SIZE         sizeof( TBLSTACK )

/// define the local control structure
typedef struct _LCLCTL
{
  U8        nBufIdx;        ///< buffer index
  U8        nNumArgs;       ///< number of arguments
  PC8       pszCmd;         ///< pointer to the command
  C8        cCurPrompt;     ///< set the prompt character
  TBLSTACK  atTableStack[ ASCIICOMMANDHANDLER_TABLE_STACK_DEPTH ];
  U8        nStackIndex;    ///< stack index
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------
const CODE C8	g_szAsciiErrStrn[ ]   = { "RERR,0x%02X:%3d\n\r" };
const CODE C8 g_szAsciiNewLin[ ]    = { "\n\r" };
const CODE C8 g_szAsciiFmtHxw[ ]    = { "%04X " };
const CODE C8 g_szAsciiFmtHxb[ ]    = { "%02X " };
const CODE C8 g_szAsciiFmtDcb[ ]    = { "%3d " };

// local parameter declarations -----------------------------------------------
static  LCLCTL          atLclCtl[ ASCCMD_ENUM_MAX ];
static  PVASCWRITEFUNC  pvCurWriteFunc;

// local function prototypes --------------------------------------------------
static  void      OutputString( const PC8 pszString );
static  ASCCMDSTS ParseCommand( ASCCMDENUM eProtEnum, PASCCMDDEF ptDef, PLCLCTL ptCtl, U8 nCompareValue );
static  void      OutputError( ASCCMDSTS eError );
static  void      OutputPrompt( C8 nPrompt );

// constant parameter initializations -----------------------------------------
static  const CODE C8 szErrBof[ ] = { "*** Command to long! ***\r" };
static  const CODE C8 szBkSpc[ ]  = { "\b \b" };
static  const CODE C8 szIllCmd[ ] = { "*** Illegal Command! ***\n\r" };
static  const CODE C8 szIllFlg[ ] = { "*** Illegal Mode! ***\n\r" };
static  const CODE C8 szIllArg[ ] = { "*** Illegal Number of Arguments! ***\n\r" };
static  const CODE C8 szIllVal[ ] = { "*** Illegal Value! ***\n\r" };
static  const CODE C8 szFmtSep[ ] = { " : " };
static  const CODE C8 szFmtPlc[ ] = { "   " };


/******************************************************************************
 * @function AscCommandHandlerInitialize
 *
 * @brief ASCII protocol initialization
 *
 * This function initializes all protocol in the config table
 *
 *****************************************************************************/
void AsciiCommandHandler_Initialize( void )
{
  ASCCMDENUM  eIdx;
  PASCCMDDEF  ptDef;
  
  // for each entry in the config table
  for ( eIdx = 0; eIdx < ASCCMD_ENUM_MAX; eIdx++ )
  {
    // get a pointer to the control/definition structures
    ptDef = ( PASCCMDDEF )&atAscCmdDefs[ eIdx ];

    // clear the indices/number of arguments/pointer to the command
    atLclCtl[ eIdx ].nBufIdx = 0;
    atLclCtl[ eIdx ].nNumArgs = 0;
    atLclCtl[ eIdx ].pszCmd = NULL;
    atLclCtl[ eIdx ].cCurPrompt = PGM_RDBYTE( ptDef->cPromptChar );
    
    // clear the stack index
    atLclCtl[ eIdx ].nStackIndex = 0;
  }
}

/******************************************************************************
 * @function AscCommandHandlerProcessChar
 *
 * @brief ASCII protocol chanacter process
 *
 * This function processes a character and takes appropriate action
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   nRcvChar    receive character
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_ProcessChar( ASCCMDENUM eProtEnum, C8 cRcvChar, U8 nCompareValue )
{
  ASCCMDSTS   eStatus = ASCCMD_STS_NONE;
  PASCCMDDEF  ptDef;
  PLCLCTL     ptCtl;
  PC8         pcBuffer, pcArg, pcDelims;
  PC8*        ppcArgs;
  PU32        puVals;
  U8          nMaxNumArgs, nIdx;
  BOOL        bEchoFlag, bRunFlag;
  #ifdef STRTOKR_P
  PC8         pcTemp;
  #endif
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
      // get a pointer to the control/definition structures
    ptCtl = &atLclCtl[ eProtEnum ];
    ptDef = ( PASCCMDDEF )&atAscCmdDefs[ eProtEnum ];
    
    // get the write character function/buffer pointer/echo flag
    pvCurWriteFunc = ( PVASCWRITEFUNC )PGM_RDWORD( ptDef->pvWriteFunc );
    pcBuffer = ( PC8 )PGM_RDWORD( ptDef->pcBuffer );
    bEchoFlag = PGM_RDBYTE( ptDef->bEchoChar );
    
    // test for echo character
    if( bEchoFlag )
    {
      // echo the character
      pvCurWriteFunc( &cRcvChar, 1 );
    }

    // check for a carriage return
    if( cRcvChar == PGM_RDBYTE( ptDef->cEOLChar ))
    {
      // send a new line
      if ( bEchoFlag )
      {
        cRcvChar = '\n';
        pvCurWriteFunc( &cRcvChar, 1 );
      }

      // terminate the buffer
      *( pcBuffer + ptCtl->nBufIdx ) = '\0';

      // check for zero length
      if( ptCtl->nBufIdx != 0 )
      {
        // get the values from the definition structure
        ppcArgs = ( PC8* )PGM_RDWORD( ptDef->ppcArgs );
        puVals = ( PU32 )PGM_RDWORD( ptDef->puVals );
        nMaxNumArgs = PGM_RDBYTE( ptDef->nNumArgs );

        // get the delimitires
        pcDelims = ( PC8 )PGM_RDWORD( ptDef->pszDelims );

        // get the command
        #ifdef STRTOKR_P
        pcArg = ( PC8 )STRTOKR_P(( char* )pcBuffer, ( char const *)pcDelims, ( char** )&pcTemp );
        #else
        pcArg = ( PC8 )STRTOK_P(( char* )pcBuffer, ( char const *)pcDelims );
        #endif

        // clear the number of arguments
        ptCtl->nNumArgs = 0;

        // copy to command
        ptCtl->pszCmd = pcArg;

        // now loop if not null
        bRunFlag = TRUE;
        while( bRunFlag )
        {
          // store the next token
          #ifdef STRTOKR_P
          pcArg = ( PC8 )STRTOKR_P( NULL, ( char const *)pcDelims, ( char** )&pcTemp );
          #else
          pcArg = ( PC8 )STRTOK_P( NULL, ( char const *)pcDelims );
          #endif

          // check for a null argument/ or too many argument
          if(( pcArg == NULL ) || ( ptCtl->nNumArgs >= nMaxNumArgs ))
          {
            // clear run flag
            bRunFlag = FALSE;
          }
          else
          {
            // store the argument
            *( ppcArgs + ptCtl->nNumArgs++ ) = pcArg;
          }
        }

        // for each argument convert to numbers
        for ( nIdx = 0; nIdx < ptCtl->nNumArgs; nIdx++ )
        {
          // convert to a long
          *( puVals + nIdx ) = strtol( *( ppcArgs + nIdx ), NULL, 0 );
        }

        // call the command parser
        eStatus = ParseCommand( eProtEnum, ptDef, ptCtl, nCompareValue );
        switch( eStatus )
        {
          case ASCCMD_STS_ILLCOMMAND :
          case ASCCMD_STS_FLAGMISMATCH :
          case ASCCMD_STS_ILLNUMARGS :
          case ASCCMD_STS_ILLVALUE :
            // display the error
            OutputError( eStatus );
            break;

          case ASCCMD_STS_OUTPUTBUFFER :
            // output the buffer
            pcBuffer = ( PC8 )PGM_RDWORD( ptDef->pcBuffer );
            pvCurWriteFunc( pcBuffer, strlen( pcBuffer ));
            break;

          default :
            break;
        }

        // set the good flag
        eStatus = ASCCMD_STS_MSGPROCESSED;

        // display the prompt
        OutputPrompt( PGM_RDBYTE( ptCtl->cCurPrompt ));

        // clear the length
        ptCtl->nBufIdx = 0;
      }
    }
    else if( cRcvChar == '\b' ) // check for a backspae
    {
      // if buffer not zero
      if( ptCtl->nBufIdx > 0 )
      {
        // decrement the index
        ptCtl->nBufIdx--;

        // output a backspace/space/backspace
        nIdx = ( bEchoFlag ) ? 1 : 0;
        OutputString(( const PC8 )&szBkSpc[ nIdx ] );
      }
    }
    else if(( cRcvChar == CH_CAN ) || ( cRcvChar == CH_ESC ))
    {
      // flush buffer
      ptCtl->nBufIdx = 0;

      // output a new line
        OutputString(( const PC8 )g_szAsciiNewLin );
    }
    else
    {
      // check for valid ASCII characters
      if (( cRcvChar >= CH_SPC ) && ( cRcvChar <= CH_TIL ))
      {
        // is there room in buffer
        if( ptCtl->nBufIdx < PGM_RDBYTE( ptDef->nBufSize ))
        {
          // convert to upper case/stuff in buffer
          *( pcBuffer + ptCtl->nBufIdx++ ) = ( C8 )( toupper(( U8 )cRcvChar ));
        }
        else
        {
          // display error
          OutputString(( const PC8 )szErrBof );
        }
      }
    }
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandlerGetCommand
 *
 * @brief get the current command
 *
 * This function returns the current command
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   ppnCommand  pointer to the command pointer
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_GetCommand( ASCCMDENUM eProtEnum, PC8* ppcCommand )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // set the command pointer
    *( ppcCommand ) = atLclCtl[ eProtEnum ].pszCmd;
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandlerGetNumArgs
 *
 * @brief gets the number of arguments
 *
 * This function returns the number of arguments
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnNumArgs   pointer to the number of arguments storage
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_GetNumArgs( ASCCMDENUM eProtEnum, PU8 pnNumArgs )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // set the number of arguments
    *( pnNumArgs ) = atLclCtl[ eProtEnum ].nNumArgs;
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandlerGetArg
 *
 * @brief gets an entered argument
 *
 * This function returns a pointer to the entered argument
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnArg       pointer to the argument storage
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_GetArg( ASCCMDENUM  eProtEnum, U8 nArgIdx, PC8* ppcArg )
{
  ASCCMDSTS  eStatus = ASCCMD_STS_NONE;
  PC8*        ppcArgs;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // check for valid argument
    if ( nArgIdx < atLclCtl[ eProtEnum ].nNumArgs )
    {
      // get the pointer to the argument array
      ppcArgs = ( PC8* )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].ppcArgs );
      *ppcArg = *( ppcArgs + nArgIdx );
    }
    else
    {
      // illgal argument index
      eStatus = ASCCMD_STS_ILLARGINDEX;
    }
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AsciiCommandHandler_GetValue
 *
 * @brief gets a value
 *
 * This function checks the index and gets a value from the arugments array
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   puValue     pointer to the value storage
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_GetValue( ASCCMDENUM eProtEnum, U8 nArgIdx, PU32 puValue )
{
  ASCCMDSTS  eStatus = ASCCMD_STS_NONE;
  PU32        puVals;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // check for valid argument
    if ( nArgIdx < atLclCtl[ eProtEnum ].nNumArgs )
    {
      // get the pointer to the argument array
      puVals = ( PU32 )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].puVals );
      *puValue = *( puVals + nArgIdx );
    }
    else
    {
      // illgal argument index
      eStatus = ASCCMD_STS_ILLARGINDEX;
    }
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandlerGetBuffer
 *
 * @brief get a pointer to the buffer
 *
 * This function returns a pointer to the buffer
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[io]   pnBuffer    pointer to the buffer
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_GetBuffer( ASCCMDENUM eProtEnum, PC8* ppcBuffer )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;
  
  // clear the return
  *( ppcBuffer ) = NULL;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // return the pointer
    *( ppcBuffer ) = ( PC8 )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].pcBuffer );
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandler_OutputBuffer
 *
 * @brief output a string
 *
 * This function ouputs a string to the write pointer
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   pcString    pointer to the string to output
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_OutputBuffer( ASCCMDENUM eProtEnum )
{
  ASCCMDSTS       eStatus = ASCCMD_STS_NONE;
  PVASCWRITEFUNC  pvWriteFunc;
  PC8             pcBuffer;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // get the buffer
    pcBuffer = ( PC8 )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].pcBuffer );

    // get the write routine/output the string
    pvWriteFunc = ( PVASCWRITEFUNC )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].pvWriteFunc );
    pvWriteFunc( pcBuffer, strlen( pcBuffer ));
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandlerOutputString
 *
 * @brief output a string
 *
 * This function ouputs a string to the write pointer
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   pcString    pointer to the string to output
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_OutputString( ASCCMDENUM eProtEnum, PC8 pcString )
{
  ASCCMDSTS       eStatus = ASCCMD_STS_NONE;
  PVASCWRITEFUNC  pvWriteFunc;
  
  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // get the write routine/output the string
    pvWriteFunc = ( PVASCWRITEFUNC )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].pvWriteFunc );

    #ifdef __ATMEL_AVR__
    PC8 pcBuffer;
    U8  nStrLen;
      
    // get the length
    nStrLen = STRLEN_P( pcString );

    // allocate space
    if (( pcBuffer = ( PC8 )malloc( nStrLen )) != NULL )
    {
      // copy to the local buffer
      STRCPY_P( pcBuffer, pcString );
      
      // now output it
      pvWriteFunc( pcBuffer, strlen( pcBuffer ));
      
      // free the buffer
      free( pcBuffer );
    }
    #else
    pvWriteFunc( pcString, strlen( pcString ));
    #endif  // __ATMEL_AVR__
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandler_DisplayBLock
 *
 * @brief dumps a block of data to the screen
 *
 * This function displays a block of data in HEX/ASCII
 *
 * @param[in]   eProtEnum       protocol enumeration
 * @param[in]   wBaseAddr       base address
 * @param[in]   pnBlockData     pointer to the data
 * @param[in]   wLength           length of data to display
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_DisplayBlock( ASCCMDENUM eProtEnum, U16 wBaseAddr, PU8 pnBlockData, U16 wLength )
{
  ASCCMDSTS       eStatus = ASCCMD_STS_NONE;
  PVASCWRITEFUNC  pvWriteFunc;
  PC8             pcBuffer;
  U16             wBlockIndex, wBufIndex;
  U8              nElemIndex;
  C8              cChar;

  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // get the write routine/pointer to the buffer
    pvWriteFunc = ( PVASCWRITEFUNC )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].pvWriteFunc );
    pcBuffer = ( PC8 )PGM_RDWORD( atAscCmdDefs[ eProtEnum ].pcBuffer );

    // for each byte in the block
    for ( wBlockIndex = 0; wBlockIndex < wLength; wBlockIndex += NUM_ELEMS_LINE )
    {
      // set the current base/output the address
      wBufIndex = SPRINTF_P( pcBuffer, ( char const* )g_szAsciiFmtHxw, wBaseAddr );
      
      // add the separator
      wBufIndex += SPRINTF_P(( pcBuffer + wBufIndex ), ( char const* )szFmtSep );

      // now for each element in the line
      for ( nElemIndex = 0; nElemIndex < NUM_ELEMS_LINE; nElemIndex++ )
      {
        // check for valid address
        if (( wBlockIndex + nElemIndex ) >= wLength )
        {
          // fill with spaces
          wBufIndex += SPRINTF_P( ( pcBuffer + wBufIndex ), ( char const* )szFmtPlc );
        }
        else
        {
          // output the element/add a separator
          wBufIndex += SPRINTF_P( ( pcBuffer + wBufIndex ), ( char const* )g_szAsciiFmtHxb, *( pnBlockData + wBlockIndex + nElemIndex ));
        }
      }

      // now add the ascii
      for ( nElemIndex = 0; nElemIndex < NUM_ELEMS_LINE; nElemIndex++ )
      {
        // check for valid address
        if (( wBlockIndex + nElemIndex ) >= wLength )
        {
          // stuff space
          *( pcBuffer + wBufIndex++ ) = ' ';
        }
        else
        {
          // get the character
          cChar = *( pnBlockData + wBlockIndex + nElemIndex );
          // output the element as ASCII
          if ( isprint(( int ) cChar ))
          {
            // stuff character
            *( pcBuffer + wBufIndex++ ) = cChar;
          }
          else
          {
            // stuff default charac
            *( pcBuffer + wBufIndex++ ) = '.';
          }
        }
      }

      // now add the end of line/output the buffer
      wBufIndex += SPRINTF_P(( pcBuffer + wBufIndex ), ( char const* )g_szAsciiNewLin );
      pvWriteFunc( pcBuffer, wBufIndex );

      // adjust the address
      wBaseAddr += NUM_ELEMS_LINE;
    }  

    // new line
    SPRINTF_P( pcBuffer, ( char const* )g_szAsciiNewLin );
    pvWriteFunc( pcBuffer, strlen( pcBuffer ));
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandler_SetPromptCharacter
 *
 * @brief sets the current prompt character
 *
 * This function will set the current prompt control to the passed value,
 * if a negative value is entered, the prompt will be restored to the default
 *
 * @param[in]   eProtEnum       protocol enumeration
 * @param[in]   cPrompt         prompt character
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_SetPromptCharacter( ASCCMDENUM eProtEnum, C8 cPrompt )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;

  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // set the prompt
    atLclCtl[ eProtEnum ].cCurPrompt =  ( cPrompt > 0 ) ? cPrompt : PGM_RDBYTE( atAscCmdDefs[ eProtEnum ].cPromptChar );
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function AscCommandHandler_output a prompt character
 *
 * @brief outputs the prompt 
 *
 * This function will output the current prompt character
 *
 * @param[in]   eProtEnum       protocol enumeration
 *
 * @return      appropriate status
 *
 *****************************************************************************/
ASCCMDSTS AsciiCommandHandler_OutputPrompt( ASCCMDENUM eProtEnum )
{
  ASCCMDSTS eStatus = ASCCMD_STS_NONE;

  // check for valid protocol
  if ( eProtEnum < ASCCMD_ENUM_MAX )
  {
    // get the prompt character
    OutputPrompt( PGM_RDBYTE( atLclCtl[ eProtEnum ].cCurPrompt ));
  }
  else
  {
    // set the error status
    eStatus = ASCCMD_STS_ILLPROTENUM;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function OutputPrompt
 *
 * @brief output a prompt
 *
 * This function outputs a prompt
 *
 * @return      appropriate status
 *
 *****************************************************************************/
static void OutputPrompt( C8 cPrompt )
{
  // get the write routine/prompt character
  if ( cPrompt != '\0' )
  {
    pvCurWriteFunc( &cPrompt, 1 );
  }
}

/******************************************************************************
 * @function OutputString
 *
 * @brief output a string locally
 *
 * This function will output a string locally
 *
 * @param[in]   eProtEnum   protocol enumeration
 * @param[in]   pszString   pointer to a string
 *
 * @return      appropriate status
 *
 *****************************************************************************/
static void OutputString( const PC8 pszString )
{
  #ifdef __ATMEL_AVR__
  U8  nStrLen;
  PC8 pcBuffer;
  
  // get the length
  nStrLen = STRLEN_P( pszString );

  // allocate space
  if (( pcBuffer = ( PC8 )malloc( nStrLen )) != NULL )
  {
    // copy to the local buffer
    STRCPY_P( pcBuffer, pszString );
    
    // now output it
    pvCurWriteFunc( pcBuffer, nStrLen );
    
    // free the buffer
    free( pcBuffer );
  }
  #else
  // output it
  pvCurWriteFunc(( PC8 )pszString, strlen( pszString ));
  #endif  // __ATMEL_AVR__
}

/******************************************************************************
 * @function ParseCommand
 *
 * @brief command parser process routine
 *
 * This function initializes the diagnostic bus interfaace
 *
 * @param[in] ptDef         pointer to the definition structure
 * @param[in] ptCtl         pointer to the control structure
 *
 * @return   apropriate error status
 *
 *****************************************************************************/
static ASCCMDSTS ParseCommand( ASCCMDENUM eProtEnum, PASCCMDDEF ptDef, PLCLCTL ptCtl, U8 nCompareValue )
{
  ASCCMDSTS           eStatus = ASCCMD_STS_NONE;
  U8                  nIdx, nCmpLen, nTblNumArgs, nTblCmpVal;
  ASCFLAGCOMPARE      eFlagCompare;
  PVASCCMDHANDLERFUNC pvFunction;
  BOOL                bCompareFlag, bRunFlag;
  S16                 sCmpResult;
  PC8                 pszTblCmd;
  ASCCMDENTRY const * ptCmdTable;
  
  // reset index/set flag
  bRunFlag = TRUE;
  nIdx = 0;
  
  // determine if a valid command table exists
  if (( ptCmdTable = ( ASCCMDENTRY const * )PGM_RDWORD( ptDef->pvCmdTable )) != NULL )
  {
    // check for a valid command
    while( bRunFlag )
    {
      // get the compare length from the table
      nCmpLen = PGM_RDBYTE(( ptCmdTable + nIdx )->nCmpLen );
    
      // test for end of table
      if ( nCmpLen == CMD_LENGTH_ENDOFTABLE )
      {
        // is this the last table
        if ( ptCtl->nStackIndex == 0 )
        {
          // report the error as an illegal command/clear the run flag
          eStatus = ASCCMD_STS_ILLCOMMAND;
          bRunFlag = FALSE;
        }
        else
        {
          // restore the command table pointer
          ptCtl->nStackIndex--;
          nIdx = ptCtl->atTableStack[ ptCtl->nStackIndex ].nIndex;
          ptCmdTable = ( ASCCMDENTRY const * )ptCtl->atTableStack[ ptCtl->nStackIndex ].ptTable;
        }
      }
      else if ( nCmpLen == CMD_LENGTH_EXTTABLE )
      {
        // check for room on stack
        if ( ptCtl->nStackIndex < ASCIICOMMANDHANDLER_TABLE_STACK_DEPTH )
        {
          // save the current table pointer/get the next table/reset index
          ptCtl->atTableStack[ ptCtl->nStackIndex ].ptTable = ( PVASCCMDHANDLERFUNC const * )ptCmdTable;
          ptCmdTable = ( ASCCMDENTRY const * )PGM_RDWORD(( ptCmdTable + nIdx )->tPointers.pvExtTable );
          ptCtl->atTableStack[ ptCtl->nStackIndex ].nIndex = nIdx + 1;
          ptCtl->nStackIndex++;
          nIdx = 0;

          // test for a null table
          if ( ptCmdTable == NULL )
          {
            // set the error/exit
            eStatus = ASCCMD_STS_ILLCOMMAND;
            bRunFlag = FALSE;
          }
        }
        else
        {
          // set the error/exit
          eStatus = ASCCMD_STS_TOOMANYTBLS;
          bRunFlag = FALSE;
        }
      }
      else
      {
        // test for end of table
        if( nCmpLen != 0 )
        {
          // get the pointer to the command from the table
          pszTblCmd = ( PC8 )PGM_RDWORD(( ptCmdTable + nIdx )->pszCommand );

          // compare
          sCmpResult = STRNCMP_P(( char const* )ptCtl->pszCmd, ( char const* )pszTblCmd, nCmpLen );

          // is this our command
          if( sCmpResult == 0 )
          {
            // now check for right number of arguments
            nTblNumArgs = PGM_RDBYTE(( ptCmdTable + nIdx )->nNumArgs );
            if(( nTblNumArgs == 0 ) || ( nTblNumArgs == ptCtl->nNumArgs ))
            {
              // check for special flag
              eFlagCompare = PGM_RDBYTE(( ptCmdTable + nIdx )->eFlagCompare );
              nTblCmpVal = PGM_RDBYTE(( ptCmdTable + nIdx )->nCmpValue );
              switch( eFlagCompare )
              {
                case ASCFLAG_COMPARE_EQ :
                  bCompareFlag = ( nTblCmpVal == nCompareValue ) ? TRUE : FALSE;
                  break;

                case ASCFLAG_COMPARE_NE :
                  bCompareFlag = ( nTblCmpVal != nCompareValue ) ? TRUE : FALSE;
                  break;

                case ASCFLAG_COMPARE_GT :
                  bCompareFlag = ( nCompareValue > nTblCmpVal ) ? TRUE : FALSE;
                  break;

                case ASCFLAG_COMPARE_LT :
                  bCompareFlag = ( nCompareValue < nTblCmpVal ) ? TRUE : FALSE;
                  break;

                case ASCFLAG_COMPARE_GE :
                  bCompareFlag = ( nCompareValue >= nTblCmpVal ) ? TRUE : FALSE;
                  break;

                case ASCFLAG_COMPARE_LE :
                  bCompareFlag = ( nCompareValue <= nTblCmpVal ) ? TRUE : FALSE;
                  break;

                default :
                  bCompareFlag = TRUE;
                  break;
              }
            
              // chek for a flag compare
              if ( !bCompareFlag )
              {
                // error - flags don't match
                eStatus = ASCCMD_STS_FLAGMISMATCH;
                bRunFlag = FALSE;
              }
              else
              {
                // execute it/exit/clear the run flag
                pvFunction = ( PVASCCMDHANDLERFUNC )PGM_RDWORD(( ptCmdTable + nIdx )->tPointers.pvCmdHandler );
                eStatus = ( pvFunction )( eProtEnum );
                bRunFlag = FALSE;
              }
            }
            else
            {
              // indiate wrong number of arguments
              eStatus = ASCCMD_STS_ILLNUMARGS;
              bRunFlag = FALSE;
            }
          }

          // increment the index
          nIdx++;
        }
      }
    }
  }
  else
  {
    // return the no command table error
    eStatus = ASSCMD_STS_NOCMDTABLE;
  }

  // cleanup stack
  ptCtl->nStackIndex = 0;

  // return the error
  return( eStatus );
}

/******************************************************************************
 * @function OutputError
 *
 * @brief output an error
 *
 * This function prints an error message
 *
 * @param[in] eError    error value
 *
 *****************************************************************************/
static void OutputError( ASCCMDSTS eError )
{
  PC8 pszString;
  
  // determine the wtring
  switch( eError )
  {
    case ASCCMD_STS_ILLCOMMAND :
      pszString = ( PC8 )&szIllCmd;
      break;
      
    case ASCCMD_STS_FLAGMISMATCH :
      pszString = ( PC8 )&szIllFlg;
      break;
      
    case ASCCMD_STS_ILLNUMARGS :
      pszString = ( PC8 )&szIllArg;
      break;
      
    case ASCCMD_STS_ILLVALUE :
      pszString = ( PC8 )&szIllVal;
      break;
      
    default :
      pszString = NULL;
      break;
  }
  
  // output the string
  OutputString( pszString );
}

/**@} EOF AsciiCommandHandler.c */
