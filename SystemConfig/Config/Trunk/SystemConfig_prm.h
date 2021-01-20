/******************************************************************************
 * @file SystemConfig.h
 *
 * @brief System coniguration declarations
 *
 * This file provides the declarations for the the system configuration
 * including pin assignments, device assignments, tick rates.
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
 * \addtogroup SystemConfig
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SYSTEMCONFIG_H
#define _SYSTEMCONFIG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "UART/Uart.h"

// Macros and Defines ---------------------------------------------------------
/// define the system tick rate
#define SYSTEM_CONFIG_TICK_RATE_HZ              ( 20 )

/// define the number of flash wait states
#define SYSTEM_CONFIG_NUM_FLASH_WAIT_STATES     ( 1 )

/// define the software revision
#define SYSTEM_CONFIG_SFWREV_MAJ                ( 0x01 )
#define SYSTEM_CONFIG_SFWREV_MIN                ( 0x00 )
#define SYSTEM_CONFIG_SFWREV_TST                ( 0x00 )

/// define the hardware revision
#define SYSTEM_CONFIG_HDWREV_MAJ                ( 0xDE )
#define SYSTEM_CONFIG_HDWREV_MIN                ( 0xAD )

/**@} EOF SystemConfig.h */

#endif  // _SYSTEMCONFIG_H