/******************************************************************************
 * @file BinaryCommandHandler_cfg.h
 *
 * @brief binary command handler configuration declarations
 *
 * This file provides the declarations for the binary command handler confiuration
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
 * \addtogroup BinaryCommandHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYCOMMANDHANDLER_CFG_H
#define _BINARYCOMMANDHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the number of defined protocols
typedef enum _BINCMDENUM
{
  // add binary command enumerations here
  
  // do not remove the entries below
  BINCMD_ENUM_MAX,
  BINCMD_ENUM_ILLEGAL = 0xFF
} BINCMDENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE  BINCMDDEF   atBinCmdDefs[ BINCMD_ENUM_MAX ];

/**@} EOF BinaryCommandHandler_cfg.h */

#endif  // _BINARYCOMMANDHANDLER_CFG_H