/******************************************************************************
 * @file PCA9534.h
 *
 * @brief PCA9534 port expander delcrations
 *
 * This file provides the declarations for the module
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
 * \addtogroup PCA9534
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PCA9534_H
#define _PCA9534_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PCA9534/PCA9534_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _PCA9434ERR
{
  PCA9534_ERR_NONE = 0,         ///< no error
  PCA9534_ERR_ILLPIN = 0x80,    ///< illegal pin enumeration
  PCA9534_ERR_ILLDEV,           ///< illegal device enumeration
  PCA9534_ERR_ILLACT,           ///< illegal action, output on an input pin
  PCA9534_ERR_I2CXFR,           ///< error on I2C transfer
} PCA9434ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        PCA9534_Initialize( void );
extern  PCA9434ERR  PCA9534_SetOutput( PCA9534DEVENUM eDeviceEnum, PCA9534PINENUM ePinEnum, BOOL bState );
extern  PCA9434ERR  PCA9534_GetInput( PCA9534DEVENUM eDeviceEnum, PCA9534PINENUM ePinEnum, PBOOL pbState );
 
/**@} EOF PCA9534.h */

#endif  // _PCA9534_H