/******************************************************************************
 * @file CommandLineInterface.c
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
 * \addtogroup CommandLineInterface
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "CommandHandlers/CommandLineInterface/CommandLineInterface.h"

// system includes ------------------------------------------------------------
#include "ctype.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the special characters
#define	CH_CAN	                ( 0x18 )
#define	CH_ESC	                ( 0x1B )
#define CH_SPC                  ( 0x20 )
#define CH_TIL                  ( 0x7E )
#define CH_BKS                  ( 0x08 )
#define CH_FSL                  ( 0x2F )

/// define the number of elements per line for display block
#define NUM_ELEMS_LINE          ( 16 )

/// define the default prompt
#define DEFAULT_PROMPT          ( '>' )
#define DEFAULT_EOLCHAR         ( '\r' )

/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PCMDLINIF )handle )

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
typedef struct _CMDLINIF
{
  struct _CMDLINIF*   ptSignature;    ///< signature
  struct _CMDLINIF*   ptNextCmdLinIf; ///< pointer to the next command line interface
  U8                  nBufferSize;    ///< buffer size
  U8                  nMaxNumArgs;    ///< number of arguments
  U8                  nNumArgs;       ///< number of arguments
  BOOL                bEnableEcho;    ///< enable echo
  C8                  cEolChar;       ///< end of line character, PVCMDLINIFWRITEFUNC pvWriteFunc, PC8 pszDelims, PCMDLINIFCMDENTRY ptCmdTable
  C8                  cCurPrompt;     ///< set the prompt character
  U8                  nCurBufLen;        ///< current buffer length
  U8                  nPrvBufLen;     ///< previous buffer length
  PC8                 pcBuffer;       ///< pointer to the buffer
  PC8*                ppcArgs;        ///< pointer to the arguments
  PU32                puArgs;         ///< pointer to the numeric arguments
  PVCMDLINIFWRITEFUN  pvWriteFunc;    ///< pointer to the write function
  U8                  nMenuDepth;     ///< menu depth
  U8                  nStackIndex;    ///< stack index
  PTBLSTACK           ptStack;        ///< pointer to the menu stack
  PCMDLINIFCMDENTRY   ptCmdTable;     ///< pointer to the command table
} CMDLINIF, *PCMDLINIF;
#define CMDLINIF_SIZE       sizeof( CMDLINIF )

// global parameter declarations ----------------------------------------------
const CODE C8	g_szCmdLinIfErrStrn[ ]   = { "RERR,0x%02X:%3d\n\r" };
const CODE C8 g_szCmdLinIfNewLin[ ]    = { "\n\r" };
const CODE C8 g_szCmdLinIfFmtHxw[ ]    = { "%04X " };
const CODE C8 g_szCmdLinIfFmtHxb[ ]    = { "%02X " };
const CODE C8 g_szCmdLinIfFmtDcb[ ]    = { "%3d " };

// local parameter declarations -----------------------------------------------
static  PCMDLINIF   ptFirstCmdLinIf;          

// local function prototypes --------------------------------------------------
static  void            OutputString( const PC8 pszString );
static  void            ParseBuffer( PCMDLINIF ptCmdLinIf, U8 nCOmpareValue );
static  CMDLINIFCMDSTS  ParseCommand( PCMDLINIF ptCtl, U8 nCompareValue );
static  void            OutputError( CMDLINIFCMDSTS eError );
static  void            OutputPrompt( C8 nPrompt );

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
 * @function CommandLineInterace_Initialize
 *
 * @brief ASCII protocol initialization
 *
 * This function initializes all protocol in the config table
 *
 *****************************************************************************/
void CommandLineInterface_Initialize( void )
{
  // reset the first pointer
  ptFirstCmdLinIf = NULL;
}

/******************************************************************************
 * @function CommandLineInterace_ProcessChar
 *
 * @brief create a command line interface
 *
 * This function will create a command line interface, allocate memory and
 * initilaize the control structure
 *
 * @param[in]   nBufferSize     size of the command buffer 
 * @param[in]   nNumArgs        number of arguments allowed in the command line
 * @param[in]   bEnableEcho     enable the echo of incoming characters
 * @param[in]   cPrompt         prompt character
 * @param[in]   CEndOfLine      end of line character
 * @param[in]   nMenuDepth      depth of the menu stack
 * @param[in]   pvWriteFunc     pointer to the write function
 * @param[in]   pszDelims       pointer to a valid string of delimiters
 * @param[in]   ptCmdTable      pointer to the command table
 *
 * @return      command line interface handle or NULL if failure
 *
 *****************************************************************************/
PVCMDLINFHANDLE CommandLineInterface_Create( U8 nBufferSize, U8 nNumArgs, BOOL bEnableEcho, C8 cPrompt, C8 cEndOfLine, U8 nMenuDepth, PVCMDLINIFWRITEFUNC pvWriteFunc, PC8 pszDelims, PCMDLINIFCMDENTRY ptCmdTable )
{
  PCMDLINIF ptNewCmdLinIf, ptCurCmdLinIf, ptLstCmdLinIf;
  
  // allocate space for new task
  if (( ptNewCmdLinIf = malloc( CMDLINIF_SIZE )) != NULL )
  {
    // now allocate space for the BUFFER
    if (( ptNewCmdLinIf->pcBuffer = malloc( nBufferSize )) != NULL )
    {
    ``// alloate the command arguement space
      if (( ptNewCmdLinIf->ppcArgs = malloc( sizeof( PC8 ) * nNumArgs )) != NULL )
      {
        // allocate the argument space
        if (( ptNewCmdLinIf->puArgs = malloc( sizeof( U32 ) * nNumArgs )) != NULL )
        {
          // now allocate space for the menu
          if ( ptNewCmdLinIf->ptStack = malloc( TABLSTACK_SIZE * nMenuDepth )) != NULL )
          {
            // now initialize the control structure
            ptNewCmdLinIf->ptSignature    = ptNewCmdLinIf;
            ptNewCmdLinIf->ptNextCmdLinIf = NULL;
            ptNewCmdLinIf->nBufferSize    = nBufferSize;
            ptNewCmdLinIf->nNMaxNumArgs   = nNumArgs;
            ptNewCmdLinIf->nNumArgs       = 0;
            ptNewCmdLinIf->bEnableEcho    = bEnableEcho;
            ptNewCmdLinIf->cEolChar       = cEndOfLine;
            ptNewCmdLinIf->cCurPrompt     = cPrompt;
            ptNewCmdLinIf->nCurBufLen        = 0;
            ptNewCmdLinIf->pvWriteFunc    = pvWriteFunc;
            ptNewCmdLinIf->nMenuDepth     = nMenuDepth;
            ptNewCmdLinIf->nStackIndex    = 0;
            ptNewCmdLinIf->ptCmdTable     = ptCmdTable;
            
            // search for the entry point
            for ( ptCurCmdLinIf = ptFirstCmdLinIf, ptLstCmdLinIf = NULL; ptCurCmdLinIf != NULL; ptLstCmdLinIf = ptCurCmdLinIf, ptCurCmdLinIf = ptCurCmdLinIf->ptNextCmdLinIf );

            // add at end
            ptLstCmdLinIf->ptNextCmdLinIf = ptNewCmdLinIf;
          }
          else
          {
            // error - not enough room
            free( ptNewCmdLinIf->puArgs );
            free( ptNewCmdLinIf->ppcArgs );
            free( ptNewCmdLinIf->pcBuffer );
            free( ptNewCmdLinIf );
            ptNewCmdLinIf = NULL;
          }
        }
        else
        {
          // error - not enough room
          free( ptNewCmdLinIf->ppcArgs );
          free( ptNewCmdLinIf->pcBuffer );
          free( ptNewCmdLinIf );
          ptNewCmdLinIf = NULL;
        }
      }
      else
      {
          // error - not enough room
          free( ptNewCmdLinIf->pcBuffer );
          free( ptNewCmdLinIf );
          ptNewCmdLinIf = NULL;
      }
    }
    else
    {
      // error - not enough room
      free( ptNewCmdLinIf );
      ptNewCmdLinIf = NULL;
    }
  }
  
  // return the handle
  return(( PTASKSCHEDULERHANDLE )ptNewCmdLinIf );
}

/******************************************************************************
 * @function CommandLineInterace_ProcessChar
 *
 * @brief ASCII protocol chanacter process
 *
 * This function processes a character and takes appropriate action
 *
 * @param[in]   ptHandle      pointer to the handle
 * @param[in]   nRcvChar      receive character
 * @param[in]   nCompareValue compare value
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFERR CommandLineInterface_ProcessChar( PVCMDLINFHANDLE pvHandle, C8 cRcvChar, U8 nCompareValue );
{
  CMDLINIFERR     eError = CMDLINIF_ERR_NONE;
  PCMDLINIF       ptCmdLinIf;
  CMDLINIFCMDSTS  eCmdStatus;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
    // check for echo
    if ( ptCmdLinIf->bEnableEcho )
    {
      // echo character
      ptCmdLinIf->pvWriteFunc( &cRcvChar, 1 );
    }
    
    // now look for special characters
    switch( cRcvChar )
    {
      case CH_FLS :
        if ( ptCmdLinIf->nCurBufLen == 0 ) && ( ptCmdLinIf->nPrvBufLen != 0 )
        {
          // restore previous buffer length/force the end of line character
          ptCmdLinIf->nCurBufLen = ptCmdLinIf->nPrvBufLen );
          
        }
        break;
        
      case CH_BKS :
        // if buffer not zero
        if( ptCmdLinIf->nCurBufLen > 0 )
        {
          // decrement the index
          ptCmdLinIf->nCurBufLen--;

          // output a backspace/space/backspace
          nIdx = ( ptCmdLinIf->bEchoFlag ) ? 1 : 0;
          OutputString(( const PC8 )&szBkSpc[ nIdx ] );
        }
        break;
        
      case CH_CAN :
      case CH_ESC :
        // flush buffer
        ptCmdLinIf->nCurBufLen = 0;

        // output a new line
        OutputString(( const PC8 )g_szCmdLinIfNewLin );
        break;
        
      default :
        if ( cRcvChar == ptCmdLinIf->cEolChar )
        {
          // process the buffer
          ParseBuffer( ptCmdLinIf, nCompareValue );
        }
        else if (( cRcvChar >= CH_SPC ) && ( cRcvChar <= CH_TIL ))
        {
          // is there room in buffer
          if( ptCmdLinIf->nCurBufLen < ptCmdLinIf->nBufSize )
          {
            // convert to upper case/stuff in buffer
            *( ptCmdLinIf->pcBuffer + ptCmdLinIf->nCurBufLen++ ) = ( C8 )( toupper(( U8 )cRcvChar ));
          }
          else
          {
            // display error
            OutputString(( const PC8 )szErrBof );
          }
        }
        break;
      }
    }
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function CommandLineInterace_GetCommand
 *
 * @brief get the current command
 *
 * This function returns the current command
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[io]   ppnCommand  pointer to the command pointer
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_GetCommand( PVCMDLINFHANDLE ptHandle, PC8* ppcCommand )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
    // set the command pointer
    *( ppcCommand ) = atLclCtl[ eProtEnum ].pszCmd;
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function CommandLineInterace_GetNumArgs
 *
 * @brief gets the number of arguments
 *
 * This function returns the number of arguments
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[io]   pnNumArgs   pointer to the number of arguments storage
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_GetNumArgs( PVCMDLINFHANDLE ptHandle, PU8 pnNumArgs )
{
  CMDLINIFCMDSTS eStatus = ASCCMD_STS_NONE;
  
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
 * @function CommandLineInterace_GetArg
 *
 * @brief gets an entered argument
 *
 * This function returns a pointer to the entered argument
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[io]   pnArg       pointer to the argument storage
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_GetArg( PVCMDLINFHANDLE ptHandle, U8 nArgIdx, PC8* ppcArg )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
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
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function CommandLineInterface_GetValue
 *
 * @brief gets a value
 *
 * This function checks the index and gets a value from the arugments array
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[io]   puValue     pointer to the value storage
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_GetValue( PVCMDLINFHANDLE ptHandle, U8 nArgIdx, PU32 puValue )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS  eStatus = ASCCMD_STS_NONE;
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
 * @function CommandLineInterace_GetBuffer
 *
 * @brief get a pointer to the buffer
 *
 * This function returns a pointer to the buffer
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[io]   pnBuffer    pointer to the buffer
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_GetBuffer( PVCMDLINFHANDLE ptHandle, PC8* ppcBuffer )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS eStatus = ASCCMD_STS_NONE;
  
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
 * @function CommandLineInterace__OutputBuffer
 *
 * @brief output a string
 *
 * This function ouputs a string to the write pointer
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[in]   pcString    pointer to the string to output
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_OutputBuffer( PVCMDLINFHANDLE ptHandle )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS       eStatus = ASCCMD_STS_NONE;
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
 * @function CommandLineInterace_OutputString
 *
 * @brief output a string
 *
 * This function ouputs a string to the write pointer
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[in]   pcString    pointer to the string to output
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_OutputString( PVCMDLINFHANDLE ptHandle, PC8 pcString )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS       eStatus = ASCCMD_STS_NONE;
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
 * @function CommandLineInterace__DisplayBLock
 *
 * @brief dumps a block of data to the screen
 *
 * This function displays a block of data in HEX/ASCII
 *
 * @param[in]   ptHandle      pointer to the handle
 * @param[in]   wBaseAddr     base address
 * @param[in]   pnBlockData   pointer to the data
 * @param[in]   wLength       length of data to display
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_DisplayBlock( PVCMDLINFHANDLE ptHandle, U16 wBaseAddr, PU8 pnBlockData, U16 wLength )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS       eStatus = ASCCMD_STS_NONE;
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
      wBufIndex = SPRINTF_P( pcBuffer, ( char const* )g_szCmdLinIfFmtHxw, wBaseAddr );
      
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
          wBufIndex += SPRINTF_P( ( pcBuffer + wBufIndex ), ( char const* )g_szCmdLinIfFmtHxb, *( pnBlockData + wBlockIndex + nElemIndex ));
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
      wBufIndex += SPRINTF_P(( pcBuffer + wBufIndex ), ( char const* )g_szCmdLinIfNewLin );
      pvWriteFunc( pcBuffer, wBufIndex );

      // adjust the address
      wBaseAddr += NUM_ELEMS_LINE;
    }  

    // new line
    SPRINTF_P( pcBuffer, ( char const* )g_szCmdLinIfNewLin );
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
 * @function CommandLineInterace__SetPromptCharacter
 *
 * @brief sets the current prompt character
 *
 * This function will set the current prompt control to the passed value,
 * if a negative value is entered, the prompt will be restored to the default
 *
 * @param[in]   ptHandle    pointer to the handle
 * @param[in]   cPrompt     prompt character
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_SetPromptCharacter( PVCMDLINFHANDLE ptHandle, C8 cPrompt )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS eStatus = ASCCMD_STS_NONE;

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
 * @function CommandLineInterace__output a prompt character
 *
 * @brief outputs the prompt 
 *
 * This function will output the current prompt character
 *
 * @param[in]   ptHandle    pointer to the handle
 *
 * @return      appropriate status
 *
 *****************************************************************************/
CMDLINIFCMDSTS CommandLineInterface_OutputPrompt( PVCMDLINFHANDLE ptHandle )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  
  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( pvHandle );
  
  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the error
  return( eError );
  CMDLINIFCMDSTS eStatus = ASCCMD_STS_NONE;

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
 * @function ParseBuffer
 *
 * @brief command parser process routine
 *
 * This function initializes the diagnostic bus interfaace
 *
 * @param[in] ptCmdLinIf  pointer to the control structure
 *
 *****************************************************************************/
static void ParseBuffer( PCMDLINIF ptCmdLinIf, nCompareValue )
{
  U8          nIdx;
  #ifdef STRTOKR_P
    PC8         pcTemp;
  #endif
  PC8         pcArg;
  
  // terminate the buffer
  *( ptCmdLinIf->pcBuffer + ptCmdLinIf->nCurBufLen ) = '\0';
  
  // check for buffer not being empty
  if ( ptCmdLinIf->nCurBufLen != 0 )
  {
    // get the command
    #ifdef STRTOKR_P
      pcArg = ( PC8 )STRTOKR_P(( char* )ptCmdLinIf->pcBuffer, ( char const *)ptCmdLinIf->pcDelims, ( char** )&pcTemp );
    #else
      pcArg = ( PC8 )STRTOK_P(( char* )ptCmdLinIf->pcBuffer, ( char const *)ptCmdLinIf->pcDelims );
    #endif

    // clear the number of arguments
    ptCmdLinIf->nNumArgs = 0;
    ptCmdLinIf->pszCmd = pcArg;

    // now loop if not null
    while(( pcArg != NULL ) && ( ptCmdLinIf->nNumArgs < ptCmdLinIf->nMaxNumArgs ))
    {
      // store the next token
      #ifdef STRTOKR_P
        pcArg = ( PC8 )STRTOKR_P( NULL, ( char const *)ptCmdLinIf->pcDelims, ( char** )&pcTemp );
      #else
        pcArg = ( PC8 )STRTOK_P( NULL, ( char const *)ptCmdLinIf->pcDelims );
      #endif

        // store the argument
        if ( pcArg != NULL )
        {
          // store it
          *( ptCmdLinIf->ppcArgs + ptCmdLinIf->nNumArgs++ ) = pcArg;
        }
      }
    }
    
    // for each argument convert to numbers
    for ( nIdx = 0; nIdx < ptCmdLinIf->nNumArgs; nIdx++ )
    {
      // convert to a long
      *( ptCmdLinIf->puVals + nIdx ) = strtol( *( ptCmdLinIf->ppcArgs + nIdx ), NULL, 0 );
    }
  }

  // now process the command table
  switch(( eCmdStatus = ParseCommand( ptCmdLinIf, nCompareValue )))
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
      ptCmdLinIf->pvCurWriteFunc( ptCmdLinIf->pcBuffer, strlen( ptCmdLinIf->pcBuffer ));
      break;

    default :
      break;
  }

  // display the prompt
  OutputPrompt( ptCmdLinIf->cCurPrompt );

  // save the length/clear the length
  ptCmdLinIf->nPrvBufLen = ptCmdLinIf->nCurBufLen;
  ptCmdLinIf->nCurBufLen = 0;
}

/******************************************************************************
 * @function ParseCommand
 *
 * @brief command parser process routine
 *
 * This function initializes the diagnostic bus interfaace
 *
 * @param[in] ptCtl         pointer to the control structure
 * @param[in] nCompareValue value to use in compare
 *
 * @return   apropriate error status
 *
 *****************************************************************************/
static CMDLINIFCMDSTS ParseCommand( PCMDLINIF ptCtl, U8 nCompareValue )
{
  CMDLINIFCMDSTS      eStatus = ASCCMD_STS_NONE;
  U8                  nIdx, nTblNumArgs, nTblCmpVal;
  ASCFLAGCOMPARE      eFlagCompare;
  PVASCCMDHANDLERFUNC pvFunction;
  BOOL                bCompareFlag, bRunFlag;
  S16                 sCmpResult;
  PC8                 pszTblCmd;
  
  // reset index/set flag
  bRunFlag = TRUE;
  nIdx = 0;
  
  // determine if a valid command table exists
  if ( ptCtl->ptCmdTable != NULL )
  {
    // check for a valid command
    while( bRunFlag )
    {
      // test for end of table
      if ( ptCtl->ptCmdTable->nCmpLen == CMD_LENGTH_ENDOFTABLE )
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
static void OutputError( CMDLINIFCMDSTS eError )
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


/**@} EOF CommandLineInterface.c */
  