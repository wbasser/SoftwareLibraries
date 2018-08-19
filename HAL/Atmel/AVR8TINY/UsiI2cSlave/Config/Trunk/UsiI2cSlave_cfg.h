/******************************************************************************
 * @file UsiI2cSlave_cfg.h
 *
 * @brief USI I2C slave congifuration 
 *
 * This file provides the configuration for the I2C
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
 * \addtogroup USII2cSlave
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USII2CSLAVE_CFG_H
#define _USII2CSLAVE_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsiI2cSlave/UsiI2cSlave_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern const CODE PVUSIINTCALLBACK  pvUsiCallBack;

// global function prototypes --------------------------------------------------

/**@} EOF UsiI2cSlave_cfg.h */

#endif  // _USII2CSLAVE_CFG_H