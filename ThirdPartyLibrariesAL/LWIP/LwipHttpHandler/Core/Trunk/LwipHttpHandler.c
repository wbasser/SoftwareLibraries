/******************************************************************************
 * @file LwipHttpHandler.c
 *
 * @brief LWIP HTTP handler
 *
 * This file provides the HTTP functionality for the LWIP HTTP handler
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup LwipHttpHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "apps/httpserver_raw/httpd.h"
#include "LwipHttpHandler/LwipHttpHandler.h"
#include "apps/httpserver_raw/fs.h"
#include "apps/httpserver_raw/fsdata.h"
#include "HtmlFiles/HtmlFiles.h"
#include "HTMLPageDefs/HTMLPageDefs.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum size of the dynamic page buffer
#define DYN_PAGE_BUFFER_SIZE    8192

/// define the maximum number of open files
#ifndef LWIP_MAX_OPEN_FILES
#define LWIP_MAX_OPEN_FILES     10
#endif

// define the page colors
#define SET_PAG_BGR     RGB( 0xFF, 0xFF, 0xFF )
#define SET_PAG_TXT     RGB( 0x00, 0x00, 0x00 )
#define SET_PAG_LNK     RGB( 0x00, 0x00, 0xFF )
#define SET_PAG_VLK     RGB( 0x80, 0x00, 0x80 )
#define SET_PAG_ALK     RGB( 0xFF, 0x00, 0x00 )

// define the page font size
#define SET_PAG_FNTSIZE 4

// define the separator width/color
#define SET_SEP_WIDTH   5
#define SET_SEP_COLOR   RGB( 0x00, 0x80, 0x80 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the fs_table
typedef struct _FSTABLE
{
  struct fs_file  ptFile;
  BOOL            bInUse;
} FSTABLE;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  C8      acDynPageBuffer[ DYN_PAGE_BUFFER_SIZE ];
static  FSTABLE atFsTable[ LWIP_MAX_OPEN_FILES ];

// local function prototypes --------------------------------------------------
static  int             ProcessSsiTag( int iIndex, char* pcInsert, int iLength );
static	BOOL            CheckDecimalParam(const PC8 pcValue, PS32 plValue );
static	BOOL            IsValidHexDigit( const C8 cDigit );
static	U8              HexDigit( const C8 cDigit );
static	BOOL            DecodeHexEscape( const PC8 pcEncoded, PC8 pcDecoded );
static  struct fs_file* fs_malloc( void );
static  void            fs_free( struct fs_file *file );
static  U16             GenerateDynamicPage( LWIPHTTPDYNPAGE const *ptPage );
static  U16             SetGenerate( PC8 pcBuffer, U16 wMaxLength, U16 wCurLength );

// constant parameter initializations -----------------------------------------
static  const C8  szCgiErr[ ]       = { "??" };
static  const C8  szSetHtm[ ]       = { "/sysset.htm" };
static  const C8  szSetTitle[ ]     = { "System Settings" };
static  const C8  szSetTime[ ]      = { "10" };
static  const C8  szSetRtnLink[ ]   = { "./index.shtm" };
static  const C8  szSetRtnLabel[ ]  = { "Home" };

/// initialize the settings page
static const LWIPHTTPDYNPAGE tLwipHttpSetPage = LWIPHTTP_DYNPAGE( szSetHtm, szSetTitle, szSetTime, szSetRtnLabel, szSetRtnLink, NULL, SET_PAG_BGR, SET_PAG_TXT, SET_PAG_LNK, SET_PAG_VLK, SET_PAG_ALK, SET_PAG_FNTSIZE, SET_SEP_COLOR, SET_SEP_WIDTH, 0, 0, 0, 0, 0, 0, 0, SetGenerate, NULL );

/******************************************************************************
 * @function LwipHttpHandler_Initialize
 *
 * @brief LWIP HTTP handler initialization
 *
 * This function initializes the LWIP HTTP implementation.  It also provides
 * the file handling for local files
 *
 *****************************************************************************/
void LwipHttpHandler_Initialize( void )
{
  // iniitlize the HTTP server
  httpd_init( );

  // compute number of entries in the  SSI tagle/intialize the handler
  http_set_ssi_handler( ProcessSsiTag, apszSsiTags, LwipHttpHandler_GetNumSsiTags( ));

  // compute number of entries in the CGI table/initialize the handler
  http_set_cgi_handlers( atLwipHttpCgiFuncs, LwipHttpHandler_GetCgiFuncSize(  ));
}

/******************************************************************************
 * @function LwipHttpHandler_CgiFindParameter
 *
 * @brief find a parameter 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
S16 LwipHttpHandler_CgiFindParameter( const PC8 pcToFind, PC8 pcParam[], U16 wNumParams)
{
  S16	iFound = -1;

  // for all item in the loop
  for( U16 wIndex = 0; wIndex < wNumParams; wIndex++ ) 
  {
      // compare name to entry
      if( strcmp( pcToFind, pcParam[ wIndex ] ) == 0 )
      {
        // set found/break
        iFound = wIndex;
        break;
      }
  }
  
  // return status
  return( iFound );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
U16 LwipHttpHandler_CgiEncodeFormString( const PC8 pcDecoded, PC8 pcEncoded, U16 wLength )
{
  U16	wIndex;
  U16	wCount = 0;

  // Make sure we were not passed a tiny buffer.
  if( wLength >= 1 )
  {
    // for each character
    for( wIndex = 0; ( pcDecoded[ wIndex ] != '\0' ) && ( wCount < ( wLength - 1 )); wIndex++ )
    {
      switch( pcDecoded[ wCount ] )
      {
        case '\'':
          wCount += snprintf( &pcEncoded[ wCount ], ( wLength - wCount ), "&#39;" );
          break;

        default:
          pcEncoded[ wCount++ ] = pcDecoded[ wIndex ];
          break;
      }
    }
  }

  // return the count
  return( wCount );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
U16 LwipHttpHandler_CgiDecodeFormString( const PC8 pcEncoded, PC8 pcDecoded, U16 wLength )
{
  U16		wIndex = 0;
  U16		wCount = 0;
  BOOL	bValid = FALSE;
  BOOL	bRunFlag = TRUE;

  // while there is inputs chars and we still have room
  while( pcEncoded[ wIndex ] && ( wCount < ( wLength - 1 )) && bRunFlag )
  {
    switch( pcEncoded[ wIndex ] )
    {
      // '+' in the encoded data is decoded as a space.
      case '+':
        pcDecoded[ wCount++ ] = ' ';
        wIndex++;
        break;

      // '%' in the encoded data indicates that the following 2
      // characters indicate the hex ASCII code of the decoded character.
      case '%':
        if(( pcEncoded[ wIndex + 1 ] != '\0' )&& ( pcEncoded[ wIndex + 2 ] != '\0' ))
        {
          // Decode the escape sequence.
          bValid = DecodeHexEscape( &pcEncoded[ wIndex ], &pcDecoded[ wCount ] );

          // If the escape sequence was valid, move to the next output character.
          wCount += ( bValid ) ? 1 : 0;

          // Skip past the escape sequence in the encoded string.
          wIndex += 3;
        }
        else
        {
          // clear the run flag to force exit
          bRunFlag = FALSE;
        }
        break;

      // For all other characters, just copy the input to the output.
      default:
        pcDecoded[ wCount++ ] = pcEncoded[ wIndex++ ];
        break;
    }
  }

  // Terminate the string and return the number of characters we decoded.
  pcDecoded[ wCount ] = '\0';
  return( wCount );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
S32 LwipHttpHandler_CgiGetParam( const PC8 pcName, PC8 pcParams[], PC8 pcValue[], U16 wNumParams, BOOL* pbError )
{
  S16		sParam;
  S32		lValue = 0;

  // set error for now
  *pbError = TRUE;

  // Is the parameter we are looking for in the list?
  if (( sParam = LwipHttpHandler_CgiFindParameter( pcName, pcParams, wNumParams )) != -1 )
  {
    // We found the parameter so now get its value.
    if ( CheckDecimalParam( pcValue[ sParam ], &lValue )) 
    {
      // set good
      *pbError = FALSE;
    }
  }

  // return the value
  return( lValue );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
struct fs_file* fs_open( char *pszName )
{
  const struct fsdata_file* ptTree;
  struct fs_file*           ptFile = NULL;
  U8                        nIndex, nNumDynPages;
  PC8                       pszCmd;
  LWIPHTTPDYNPAGE const *   ptPage;

  // allocate memory for the file system structure.
  ptFile = fs_malloc(  );
  if ( ptFile != NULL )
  {
    // is this a dynamic web page
    ptPage = NULL;

    // check to see if this is the settings page
    if ( strcmp( pszName, szSetHtm ) == 0 )
    {
      // page found - generate it
      ptPage = &tLwipHttpSetPage;
    }
    else
    {
      // compute the number of dynamic web pages
      nNumDynPages = LwipHttpHandler_GetDynPageSize( );

      // for each entry
      for( nIndex = 0; nIndex < nNumDynPages; nIndex++ )
      {
        // is this our web page
        if ( strcmp( pszName, atLwipHttpDynPages[ nIndex ].pcPage ) == 0 )
        {
          // page found - generate it
          ptPage = &atLwipHttpDynPages[ nIndex ];

          // exit the loop
          break;
        }
      }
    }

    // was a dynamic page found
    if ( ptPage != NULL )
    {
      // generate the page
      ptFile->len = GenerateDynamicPage( ptPage );
      ptFile->data = acDynPageBuffer;
      ptFile->index = ptFile->len;
      ptFile->pextension = NULL;
    }
    else
    {
      // process the file system/set the pointer to the root
      ptTree = HtmlFiles_GetRoot( );

      // while there are entries in the linked list
      while( ptTree != NULL )
      {
        // is this our file entry
        if ( strncmp( pszName, ( char* )ptTree->name, ptTree->len ) == 0 )
        {
          // fill in the data pointer/length/index/clear extension
          ptFile->data = ( char * )ptTree->data;
          ptFile->len = ptTree->len;
          ptFile->index = ptTree->len;
          ptFile->pextension = NULL;

          // exit loop
          break;
        }
        else
        {
          // advance to the next node in the linked list
          ptTree = ptTree->next;
        }
      }

      // if we did not find the entry/return a null
      if ( ptTree == NULL )
      {
        // free the memory/set the pointer to NULL
        fs_free( ptFile );
        ptFile = NULL;
      }
    }
  }

  // return the pointer to the file
  return( ptFile );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void fs_close( struct fs_file* file )
{
  // free the memory
  fs_free( file );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
int fs_read( struct fs_file* file, char* buffer, int count )
{
  S32 lAvailable = -1;

  // check to see if a comand
  if ( file->pextension == ( void* )1 )
  {
    // nothing to do
    file->pextension = NULL;
  }
  // check for end of file
  else if ( file->len != file->index )
  {
    // determine how much data we can copy
    lAvailable = file->len - file->index;
    lAvailable = MIN( lAvailable, count );

    // copy the data/adjust the file index
    memcpy( buffer, file->data + lAvailable, lAvailable );
    file->index += lAvailable;
  }

  // return the number of bytes read
  return( lAvailable );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static U16 GenerateDynamicPage( LWIPHTTPDYNPAGE const *ptPage )
{
  U16 wNumBytes = 0;

  // clear the dynamic page
  memset( acDynPageBuffer, 0, DYN_PAGE_BUFFER_SIZE );

  // generate the page header
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageHtb );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageHdb );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szMetaBeg );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szMetaExp );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szMetaCnt );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, ptPage->pcTime );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szMetaEnd );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageTtb );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, ptPage->pcTitle );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageTte );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageHde );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageBdb );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFmtStrn, g_szPageSty );

  // generate the title
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageClr, ptPage->uPageBackColor, ptPage->uPageLinkColor, ptPage->uPageVlnkColor, ptPage->uPageAlnkColor );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szHtmlLbk );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageBfn, ptPage->nFontSize, ptPage->uPageTextColor, g_szFntTahoma );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageH1b );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, ptPage->pcTitle );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageH1e );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageHrw, ptPage->nSepWidth, ptPage->uSepColor );

  // generate the table header
  if ( ptPage->pcCommand != NULL )
  {
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFormAct, ptPage->pcCommand, g_szFormGet );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablBeg );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablPrm, ptPage->nBorderWidth, ptPage->nCellSpacing, ptPage->uBackColor, ptPage->wCellWidth );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablCgb );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szClgrPrm, ptPage->nColSpan, ptPage->wColWidth );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablCge );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablHdr, 10, ptPage->nNumCols, ptPage->pcTitle );
  }

  // now print the contents
  if ( ptPage->pvTableGen != NULL )
  {
    // do it
    wNumBytes += ptPage->pvTableGen( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, wNumBytes );
  }

  // only populate the submit button if label is not null
  if ( ptPage->pcLabel != NULL )
  {
    // generate the submit button
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablRwb );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablTdb );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szBtnsSub, ptPage->pcLabel );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablTde );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablRwe );
  }

  // generate the table header
  if ( ptPage->pcCommand != NULL )
  {
    // generate the table footer
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szTablEnd );
    wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szFormEnd );
  }

  // generate the return link
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageHrw, ptPage->nSepWidth, ptPage->uSepColor );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szHtmlLnk, ptPage->pcReturnLink, ptPage->pcReturnLabel );

  // generate the bottom of page
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageBde );
  wNumBytes += snprintf( &acDynPageBuffer[ wNumBytes ], DYN_PAGE_BUFFER_SIZE, g_szPageHte );

  // return the number of bytes
  return( wNumBytes );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static U16 SetGenerate( PC8 pcBuffer, U16 wMaxLength, U16 wCurLength )
{
  U16                     wNumBytes;
  U8                      nIndex, nNumDynPages;
  LWIPHTTPDYNPAGE const * ptPage;

  // set the current length
  wNumBytes = wCurLength;

  // compute the number of dynamic web pages
  nNumDynPages = LwipHttpHandler_GetDynPageSize( );

  // for each entry
  for( nIndex = 0; nIndex < nNumDynPages; nIndex++ )
  {
    // get the pointer to the entry
    ptPage = &atLwipHttpDynPages[ nIndex ];

    // print the entry
    wNumBytes += snprintf(( pcBuffer + wNumBytes ), wMaxLength, g_szHtmlLnk, ptPage->pcPage, ptPage->pcTitle );
  }

  // return the number of bytes
  return( wNumBytes );
}


/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static int ProcessSsiTag( int iIndex, char* pcInsert, int iLength )
{
  PVGETTAGVALUE pvGetTagValue;

  // determine if the index is within range
  if ( iIndex < LwipHttpHandler_GetNumSsiTags( ))
  {
    // get the function
    pvGetTagValue = atLwipHttpSsiTags[ iIndex ].pvGetTagValue;

    // print the return string
    snprintf( pcInsert, iLength, g_szFmtStrn, pvGetTagValue( ));
  }
  else
  {
    // print an error
    snprintf( pcInsert, iLength, szCgiErr );
  }

  // return the length of the returned string
  return( strlen( pcInsert ));
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static BOOL CheckDecimalParam(const PC8 pcValue, PS32 plValue )
{
  U16		wIndex;
  BOOL	bStarted, bFinished, bNegative, bRunning, bValid;
  S32		lAccum;

  // Check that the string is a valid decimal number.
  bStarted = bFinished = bNegative = FALSE;
  bValid = TRUE;

  // clear the indices/value
  wIndex = 0;
  lAccum = 0;

  // while not end of string and run flag
  bRunning = TRUE;
  while(( pcValue[ wIndex ] != '\0' ) && bRunning )
  {
    // Ignore whitespace before the string.
    if( !bStarted )
    {
      if(( pcValue[ wIndex ] == ' ' ) || ( pcValue[ wIndex ] == '\t' ))
      {
        wIndex++;
        continue;
      }

      // Ignore a + or - character as long as we have not started.
      if(( pcValue[ wIndex ] == '+' ) || ( pcValue[ wIndex ] == '-' ))
      {
        // If the string starts with a '-', remember to negate the result.
        bNegative = ( pcValue[ wIndex ] == '-' ) ? TRUE : FALSE;
        bStarted = TRUE;
        wIndex++;
      }
      else
      {
        // We found something other than whitespace or a sign character so we start looking for numerals now.
        bStarted = TRUE;
      }
    }

    if( bStarted )
    {
      if( !bFinished )
      {
        // We expect to see nothing other than valid digit characters here.
        if(( pcValue[ wIndex ] >= '0' ) && ( pcValue[ wIndex ] <= '9' ))
        {
          // add to accumulator
          lAccum = ( lAccum * 10 ) + ( pcValue[ wIndex ] - '0' );
        }
        else
        {
          // Have we hit whitespace?  If so, check for no more characters until the end of the string.
          if(( pcValue[ wIndex ] == ' ' ) || ( pcValue[ wIndex ] == '\t' ))
          {
            // set the finished flag
            bFinished = TRUE;
          }
          else
          {
            // something other, exit with false
            bRunning = FALSE;
            bValid = FALSE;
          }
        }
      }
      else
      {
        // We are scanning for whitespace until the end of the string.
        if(( pcValue[ wIndex ] != ' ' ) && ( pcValue[ wIndex ] != '\t' ))
        {
          // We found something other than whitespace so the string is not valid
          bRunning = FALSE;
          bValid = FALSE;
        }
      }

      // Move on to the next character in the string.
      wIndex++;
    }
 }

  // test for valid
  if ( bValid )
  {
    *plValue =  bNegative ? -lAccum : lAccum;
  }

  return( bValid );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static BOOL IsValidHexDigit( const C8 cDigit )
{
  BOOL bReturn = FALSE;

  if((( cDigit >= '0' ) && ( cDigit <= '9' )) ||
     (( cDigit >= 'a' ) && ( cDigit <= 'f' )) ||
     (( cDigit >= 'A' ) && ( cDigit <= 'F' )))
  {
      bReturn = TRUE;
  }

  // return the value
  return( bReturn );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static U8 HexDigit( const C8 cDigit )
{
  U8	nValue = 0xFF;

  // if char is not a hex
  if(( cDigit >= '0' ) && ( cDigit <= '9' ))
  {
    // subtract the ascii zero
    nValue = cDigit - '0';
  }
  else
  {
    // if a hex char
    if(( cDigit >= 'a' ) && ( cDigit <= 'f' ))
    {
      // adjust it
      nValue = ( cDigit - 'a' ) + 10;
    }
    else
    {
      // if a hex char
      if(( cDigit >= 'A' ) && ( cDigit <= 'F' ))
      {
      // adjust it
        nValue = ( cDigit - 'A' ) + 10;
      }
    }
  }

  // return the value
  return( nValue );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static BOOL DecodeHexEscape( const PC8 pcEncoded, PC8 pcDecoded )
{
  BOOL bStatus = FALSE;

  // valid escape sequence
  if(( pcEncoded[ 0 ] == '%' ) && ( IsValidHexDigit( pcEncoded[ 1 ] )) && ( IsValidHexDigit( pcEncoded[ 2 ] )))
  {
    // decode the hex two byte string and convert to an 8 bit decimal
    *pcDecoded = (( HexDigit( pcEncoded[ 1 ] ) * 16 ) + HexDigit( pcEncoded[ 2 ]));
    bStatus = TRUE;
  }

  // return the status
  return( bStatus );
}

/*-----------------------------------------------------------------------------------*/
static struct fs_file* fs_malloc( void )
{
  int i;
  struct fs_file* ptFile = NULL;

  // for each entry in the table
  for( i = 0; i < LWIP_MAX_OPEN_FILES; i++ ) 
  {
    // is this entry in use
    if( atFsTable[ i ].bInUse == FALSE )
    {
      // set in use/set the pointer
      atFsTable[ i ].bInUse = TRUE;
      ptFile = &atFsTable[ i ].ptFile;
    }
  }

  // return the pointer
  return( ptFile );
}

/*-----------------------------------------------------------------------------------*/
static void fs_free( struct fs_file *file )
{
  int i;
  
  // for each entry in the array
  for( i = 0; i < LWIP_MAX_OPEN_FILES; i++ )
  {
    if( &atFsTable[ i ].ptFile == file )
    {
      // mark is as unused/exit loop
      atFsTable[ i ].bInUse = FALSE;
      break;
    }
  }
}

/**@} EOF LwipHttpHandler.c */
