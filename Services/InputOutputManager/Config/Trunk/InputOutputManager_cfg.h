/******************************************************************************
 * @file InputOutputManager_cfg.h
 *
 * @brief input/output manager configuration declarations
 *
 * This file provides the declarations for input/output manager configurations
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
 * \addtogroup InputOutputManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _INPUTOUTPUTMANAGER_CFG_H
#define _INPUTOUTPUTMANAGER_CFG_H

// local includes -------------------------------------------------------------
#include "InputOutputManager/InputOutputManager_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// define the I/O enumerations here
typedef enum _IOMNGRCHNENUM
{
  // declare the user defined I/Os here

  // do not remove these entries
  IOMNGR_CHN_ENUM_MAX,
  IOMNGR_CHN_ENUM_ILLEGAL = 0xFF
} IOMNGRCHNENUM;


/// define the relay matrix enumerations here
typedef enum _IOMNGRMTXENUM
{
  // declare the user define matrices here
  
  // do not remove these entries
  IOMNGR_MTX_ENUM_MAX,
  IOMNGR_MTX_ENUM_ILLEGL = 0xFF
} IOMNGRMTXENUM;

// global parameter declarations -----------------------------------------------
extern  const CODE IOMNGRCHNDEF   atIOMngrChnDefs[ ];
extern  const CODE IOMNGRMTXDEF   atIOMngrMtxDefs[ ];

/**@} EOF InputOutputManager_cfg.h */

#endif  // _INPUTOUTPUTMANAGER_CFG_H