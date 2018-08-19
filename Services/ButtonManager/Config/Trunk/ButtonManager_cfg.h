/******************************************************************************
 * @file ButtonManager_cfg.h
 *
 * @brief button manager configuration declarations
 *
 * This file declares the configuration for the button manager
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
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BUTTONMANAGR_CFG_H
#define _BUTTONMANAGR_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the buttons
typedef enum BTNMNGRENUMS
{
  // enumerate user buttons here

  // do not remove the below entries
  BTNMNGR_ENUM_MAX,
  BTNMNGR_ENUM_ILLEGAL
} BTNMNGRENUMS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE BTNMNGRCFG tBtnMgrCfg;
extern  const CODE BTNMNGRDEF atBtnMgrDefs[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  ButtonManager_GetKeyStatus( U8 nKeyEnum );

/**@} EOF ButtonManager_cfg.h */

#endif  // _BUTTONMANAGR_CFG_H