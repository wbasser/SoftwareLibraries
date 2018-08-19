/******************************************************************************
 * @file LwipHttpHandler_cfg.c
 *
 * @brief LWIP HTTP handler configuration implementation
 *
 * This file provides the HTTP configuration implementation
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

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LwipHttpHandler/LwipHttpHandler_cfg.h"
#include "LwipHttpHandler/LwipHttpHandler_def.h"

// macros/defines -------------------------------------------------------------
#define MAX_BUF_SIZE  80

// global parameter declarations ----------------------------------------------

// local paraameter declarations ----------------------------------------------
static  C8  acBuffer[ MAX_BUF_SIZE ];

// local function prototypes

// global constant declarations -----------------------------------------------
/// initialize the SSI tags

/// initialize the SSI tags
const LWIPHTTPSSITAG  atLwipHttpSsiTags[ ] =
{
 // LWIPHTTP_SSITAG( tag, handler )
};

/// initialize the CGI function handlers
//const tCGI atLwipHttpCgiFuncs[ ] =
const LWIPHTTPCGIFUNC atLwipHttpCgiFuncs[ ] =
{
  // LWIPHTTP_CGIFUNC( cgi, handler )
  //  LWIPHTTP_CFGFUNC( szLwpSetCgi, LwipSettings_ProcessCgi ),
};

/// initialize the dynamic web page generators
const LWIPHTTPDYNPAGE atLwipHttpDynPages[ ] =
{
  // LWIPHTTP_DYNPAGE( page, title, time, rtnlabel, rtnlink, command, pbckclr, txtclr, lnkclr, vlnkclr, alnkclr, fontsize, sepcolor, sepwidth, colspan, colwidth, numcols, brdwidth, cellspace, cellwidth, tbckclr, genfunc, btnlabel )
};

/******************************************************************************
 * @function LwipHttpHandler_GetNumSsiTags
 *
 * @brief get the SSI tag size
 *
 * This function returns the size of the the SSI tags
 *
 * @return      
 *
 *****************************************************************************/
U32 LwipHttpHandler_GetNumSsiTags( void )
{
  return( sizeof( atLwipHttpSsiTags ) / LWIPHTTPSSITAG_SIZE );
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
U32 LwipHttpHandler_GetCgiFuncSize( void )
{
  return( sizeof( atLwipHttpCgiFuncs ) / LWIPHTTPCGIFUNC_SIZE );
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
U32 LwipHttpHandler_GetDynPageSize( void )
{
  return( sizeof( atLwipHttpDynPages ) / LWIPHTTPDYNPAGE_SIZE );
}

/**@} EOF LwipHttpHandler_cfg.c */
