/******************************************************************************
 * @file BootConfig_prm.h
 *
 * @brief Boot Configuraiton parameter declarations
 *
 * This file provides the boot configuration parameter declarations
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
 * $Log: $
 * 
 *
 * \addtogroup BootConfig
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef BOOTCONFIG_PRM
#define BOOTCONFIG_PRM

// local includes -------------------------------------------------------------
#include "inc/hw_flash.h"

// Macros and Defines ---------------------------------------------------------
/// define the boot port
#define BOOT_CONFIG_PORT    0

/// define the boot port pin
#define BOOT_CONFIG_PIN     0

/// define the boot enable polarity
#define BOOT_CONFIG_POL     0

/// define the boot enable  0
#define BOOT_CONFIG_ENAB    0

/// define the number of ticks per usec
BOOT_CONFIG_NUMTICKS_USEC
/**@} EOF BootConfig_prm.h */

#endif  // BOOTCONFIG_PRM