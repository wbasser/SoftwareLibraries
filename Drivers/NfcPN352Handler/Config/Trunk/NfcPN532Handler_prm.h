/******************************************************************************
 * @file NfcPN532Handler_prm.h
 *
 * @brief NFC PN532 handler parameter declarations
 *
 * This file provides the parameter declaratons for the PN532 NFC chip
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
 * \addtogroup NfcPN532Handler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NFCPN532HANDLER_PRM_H
#define _NFCPN532HANDLER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the enumerations for the I2C
#define NFCPN532_I2C_DEV_ENUM                   ( I2C_DEV_ENUM_LCLBUS )

/// define the communications task
#define NFCPN532_TASK_ENUM                      ( TASK_SCHD_ILLEGAL )

/**@} EOF NfcPN532Handler_prm.h */

#endif  // _NFCPN532HANDLER_PRM_H