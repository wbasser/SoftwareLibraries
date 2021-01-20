/******************************************************************************
 * @file AsciiCommandHandler_cfg.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
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
#ifndef _ASCIICOMMANDHANDLER_CFG_H
#define _ASCIICOMMANDHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the number of defined protocols
typedef enum _ASCCMDENUM
{
  // add enumerations below
  
  // do not remove the items below
  ASCCMD_ENUM_MAX,
  ASCCMD_ENUM_ILLEGAL = 0xF
} ASCCMDENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE  ASCCMDDEF   g_atAscCmdDefs[ ASCCMD_ENUM_MAX ];


/**@} EOF AsciiCommandHandler_cfg.h */

#endif  // _ASCIICOMMANDHANDLER_CFG_H