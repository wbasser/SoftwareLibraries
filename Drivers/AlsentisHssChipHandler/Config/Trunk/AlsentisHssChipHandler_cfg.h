/******************************************************************************
 * @file AlsentisHssChipHandler_cfg.h
 *
 * @brief PCA9534 configuration declarations
 *
 * This file provides the declarations for the PCA9534 module configuration
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup AlsentsChipandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALSENTISCHIPHANDLER_H
#define _ALSENTISCHIPHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AlsentisHssChipHandler/AlsentisHssChipHandler_def.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the devices
typedef enum _ALENTISHSSDCHIPEVENUM
{
  // add user enuemrations below
  
  // do not remove the entries below
  ALSENTISHSSCHIP_DEV_ENUM_NAX,
  ALSENTISHSSCHIP_DEV_ENUM_ILLEGAL = 255
} ALENTISHSSDCHIPEVENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE ALSENTISHSSCHIPDEF  atAlsentisHssChipDevs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF AlsentisHssChipHandler.h */

#endif  // _ALSENTISCHIPHANDLER_H