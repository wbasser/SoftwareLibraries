/******************************************************************************
 * @file StellarisLwipHandler_cfg.h
 *
 * @brief Stellaris LWIP handler configuration declarations
 *
 * This file provides the declarations for the ethernet handler configuration
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
 * \addtogroup StellarisLwipHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _STELLARISLWIPHANDLER_CFG_H
#define _STELLARISLWIPHANDLER_CFG_H

// local includes -------------------------------------------------------------
#include "StellarisLwipHandler/StellarisLwipHandler_def.h"

// enumerations ---------------------------------------------------------------
/// enumerate the list of lwip protocols
typedef enum _LWIPPROTS
{
  // enumerate user protocols here
  
  // do not remove entries below
  LWIP_PROT_MAX
} LWIPPROTS;

// global parameter declarations -----------------------------------------------
extern  const LWIPPROTDEF atLwipProtDefs[ ];

/**@} EOF StellarisLwipHandler_cfg.h */

#endif  // _STELLARISLWIPHANDLER_CFG_H