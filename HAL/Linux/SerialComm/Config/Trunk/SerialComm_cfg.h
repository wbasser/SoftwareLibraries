/******************************************************************************
 * @file SerialComm_cfg.h
 *
 * @brief SERIALCOMM configuration declarations
 *
 * This file contains the UARt configuration declarations
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
 * $Rev: $
 * 
 *
 * \addtogroup SERIALCOMM
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _SERIALCOMM_CFG_H
#define _SERIALCOMM_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SerialComm/SerialComm_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the SERIALCOMM enuemrations
typedef enum  _SERIALCOMMDEVENUM
{
  // enumerate devices below
  
  // do not remove these entries
  SERIALCOMM_DEV_ENUM_MAX,
  SERIALCOMM_DEV_ENUM_ILLEGAL = 0xFF
} SERIALCOMMDEVENUM;

// global function prototypes --------------------------------------------------
extern  const SERIALCOMMDEF atSerialCommDefs[ ];

/**@} EOF SerialComm_cfg.h */

#endif  // _SERIALCOMM_CFG_H
