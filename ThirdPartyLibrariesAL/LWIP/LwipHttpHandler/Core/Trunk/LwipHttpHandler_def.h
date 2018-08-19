/******************************************************************************
 * @file LwipHttpHandler_def.h
 *
 * @brief Stellaris LWIP HTTP handler definition declarations
 *
 * This file provides the declarations for the LWIP HTTP handler
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
 
// ensure only one instantiation
#ifndef _LWIPHTTPHANDLER_DEF_H
#define _LWIPHTTPHANDLER_DEF_H

// system includes ------------------------------------------------------------
#include "apps/httpserver_raw/httpd.h"

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for SSI TAG entries
//#define LWIPHTTP_SSITAG( tag, handler )
#define LWIPHTTP_SSITAG( handler ) \
  { \
    .pszTag = tag, \
    .pvGetTagValue = handler \
  }

/// define the helper macro for CGI function entries
#define LWIPHTTP_CGIFUNC( cgi, handler ) \
  { \
    .pcCGIName = ( const char * )cgi, \
    .pfnCGIHandler = (tCGIHandler)handler \
  }

/// define the helper macro for the dynamic web page entries
#define LWIPHTTP_DYNPAGE( page, title, time, rtnlabel, rtnlink, command, pbckclr, txtclr, lnkclr, vlnkclr, alnkclr, fontsize, sepcolor, sepwidth, colspan, colwidth, numcols, brdwidth, cellspace, cellwidth, tbckclr, genfunc, btnlabel ) \
  { \
    .pcPage = ( PC8 )page, \
    .pcTitle = ( PC8 )title, \
    .pcTime = ( PC8 )time, \
    .pcReturnLabel = ( PC8 )rtnlabel, \
    .pcReturnLink = ( PC8 )rtnlink, \
    .pcCommand = ( PC8 )command, \
    .uPageBackColor = pbckclr, \
    .uPageTextColor = txtclr, \
    .uPageLinkColor = lnkclr, \
    .uPageVlnkColor = vlnkclr, \
    .uPageAlnkColor = alnkclr, \
    .nFontSize = fontsize, \
    .uSepColor = sepcolor, \
    .nSepWidth = sepwidth, \
    .nColSpan = colspan, \
    .wColWidth = colwidth, \
    .nNumCols = numcols, \
    .nBorderWidth = brdwidth, \
    .nCellSpacing = cellspace, \
    .wCellWidth = cellwidth, \
    .uBackColor = tbckclr, \
    .pvTableGen = genfunc, \
    .pcLabel = ( PC8 )btnlabel, \
  }

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the SSI tag fetch function 
typedef PC8 ( *PVGETTAGVALUE )( void );

/// define the SSI tag structure
typedef struct _LWIPHTTPSSITAG
{
  const PC8     pszTag;         ///< tag
  PVGETTAGVALUE pvGetTagValue;  ///< function to get the tag
} LWIPHTTPSSITAG, *PLWIPHTTPSSITAG;
#define LWIPHTTPSSITAG_SIZE   sizeof( LWIPHTTPSSITAG )

/// re-define the CGI tag handler
typedef tCGI  LWIPHTTPCGIFUNC;
#define LWIPHTTPCGIFUNC_SIZE  sizeof( LWIPHTTPCGIFUNC )

/// define the dynamic page generation function type
typedef U16 ( *PVDYNTABLEGEN )( PC8, U16, U16 );

/// define the dynamic web page entry
typedef struct _LWIPHTTPDYNPAGE
{
  const PC8     pcPage;         ///< page name
  const PC8     pcTitle;        ///< title
  const PC8     pcTime;         ///< expire/refresh time
  const PC8     pcReturnLabel;  ///< return label
  const PC8     pcReturnLink;   ///< return link
  const PC8     pcCommand;      ///< CGI command
  U32           uPageBackColor; ///< background color
  U32           uPageTextColor; ///< text color
  U32           uPageLinkColor; ///< link color
  U32           uPageVlnkColor; ///< virtual link color
  U32           uPageAlnkColor; ///< actual link color
  U8            nFontSize;      ///< title font size
  U32           uSepColor;      ///< separator color
  U8            nSepWidth;      ///< separator width
  U8            nColSpan;       ///< column span
  U16           wColWidth;      ///< column width
  U8            nNumCols;       ///< number of columns
  U8            nBorderWidth;   ///< border width
  U8            nCellSpacing;   ///< cell spacing
  U16           wCellWidth;     ///< cell width
  U32           uBackColor;     ///< background color
  PVDYNTABLEGEN pvTableGen;     ///< table content generator function
  const PC8     pcLabel;        ///< button label
} LWIPHTTPDYNPAGE, *PLWIPHTTPDYNPAGE;
#define LWIPHTTPDYNPAGE_SIZE  sizeof( LWIPHTTPDYNPAGE )

// global parameter declarations -----------------------------------------------

/**@} EOF LwipHttpHandler_def.h */

#endif  // _LWIPHTTPHANDLER_DEF_H