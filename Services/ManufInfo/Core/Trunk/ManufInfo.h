/******************************************************************************
 * @file ManufInfo.h
 *
 * @brief Manufacturing Info  Delcarations
 *
 * This file provides the manufacturing info decarlations
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
 * \addtogroup ManufInfo
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MANINFO_H
#define _MANINFO_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"
#include "ManufInfo/ManufInfo_prm.h"

// library includes -----------------------------------------------------------
#if ( MANUFINFO_ENABLE_DEBUGCOMMANDS  == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
#if ( MANUFINFO_ENABLE_DEBUGCOMMANDS  == 1 )
extern  const CODE ASCCMDENTRY atManufInfoCmdHandlerTable[ ];
#endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

// global function prototypes --------------------------------------------------
extern  U8        ManufInfo_GetSfwMajor( void );
extern  U8        ManufInfo_GetSfwMinor( void );
extern  U8        ManufInfo_GetHdwMajor( void );
extern  U8        ManufInfo_GetHdwMinor( void );
extern  const PC8 ManufInfo_GetTitle( void );
extern  const PC8 ManufInfo_GetSerNum( void );
extern  const PC8 ManufInfo_GetPartNum1( void );
extern  const PC8 ManufInfo_GetPartNum2( void );

/**@} EOF ManInfo_def.h */

#endif  // _MANINFO_DEF_H