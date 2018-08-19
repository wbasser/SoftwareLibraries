/******************************************************************************
 * @file LwipHttpHandler.h
 *
 * @brief Stellaris LWIP HTTP handler declarations
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
#ifndef _LWIPHTTPHANDLER_H
#define _LWIPHTTPHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LwipHttpHandler/LwipHttpHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LwipHttpHandler_Initialize( void );
extern	S16		LwipHttpHandler_CgiFindParameter( const PC8 pcToFind, PC8 pcParam[], U16 wNumParams);
extern	U16		LwipHttpHandler_CgiEncodeFormString( const PC8 pcDecoded, PC8 pcEncoded, U16 wLength );
extern	U16		LwipHttpHandler_CgiDecodeFormString( const PC8 pcEncoded, PC8 pcDecoded, U16 wLength );
extern	S32		LwipHttpHandler_CgiGetParam( const PC8 pcName, PC8 pcParams[], PC8 pcValue[], U16 wNumParams, BOOL* pbError );

/**@} EOF LwipHttpHandler.h */

#endif  // _LWIPHTTPHANDLER_H