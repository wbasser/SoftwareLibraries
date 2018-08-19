/******************************************************************************
 * @file KeyHandler_cfg.h
 *
 * @brief key handler configuration declarations 
 *
 * This file provides the declarations for the key handler configuration
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup KeyHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _KEYHANDLER_CFG_H
#define _KEYHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "KeyHandler/KeyHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the keys here
typedef enum _KEYHANDENUM
{
  // add user defined keys here
  
  // do not remove these enumerations
  KEYHAND_ENUM_MAX,
  KEYHAND_ENUM_ILLEGAL = 0xFF
} KEYHANDENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE KEYHANDDEF atKeyHandDefs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF KeyHandler_cfg.h */

#endif  // _KEYHANDLER_CFG_H