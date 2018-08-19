/******************************************************************************
 * @file BinaryProtocolHandler_cfg.h
 *
 * @brief Binary protocol handler configuration declarations 
 *
 * This file provides the declarations for the binary protocol handler
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
 * $Rev: $
 * 
 *
 * \addtogroup BinaryProtocolHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYPROTOCOLHANDLER_CFG_H
#define _BINARYPROTOCOLHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinaryProtocolHandler/BinaryProtocolHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// enumerate the number of defined protocols
typedef enum _BINPROTENUM
{
  // add binary command enumerations here
  
  // do not remove the entries below
  BINPROT_ENUM_MAX,
  BINPROT_ENUM_ILLEGAL = 0xFF
} BINPROTENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE  BINPROTDEF   atBinProtDefs[ BINPROT_ENUM_MAX ];

/**@} EOF BinaryProtocolHandler_cfg.h */

#endif  // _BINARYPROTOCOLHANDLER_CFG_H