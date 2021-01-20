/******************************************************************************
 * @file EepromHandler_prm.h
 *
 * @brief EEPROM handler parameter declarations
 *
 * This file provides the parameter declarations
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
 * \addtogroup EepromHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _EEPROMHANDLER_PRM_H
#define _EEPROMHANDLER_PRM_H

// local includes -------------------------------------------------------------

// libary includes -------------------------------------------------------------
#include "I2C/I2c.h"
#include "SystemControlManager/SystemControlManager.h"
#include "SystemTick/SystemTick.h"

// define ---------------------------------------------------------------------
/// define the slave address
#define	EEPROMHANDLER_DEV_ADDR                      ( 0x50 )

/// define the size of the device
#define	EEPROMHANDLER_DEV_SIZE                      ( 1024 )

/// define the size of the block in page write
#define EEPROMHANDLER_BLK_SIZE                      ( 8 )

/// define the size of the address
#define EEPROMHANDLER_ADR_SIZE                      ( 1 )

/// define the device enum
#define EEPROMHANDLER_DEVICE                        ( 0 )

/// define the base address for the config block
#define EEPROMHANDLER_CFGBLOCK_BASE_ADDR            ( 0 )

/// define the base address for the parameter block
#define EEPROMHANDLER_PRMBLOCK_BASE_ADDR            ( 0 )

/// define the base address for the log block
#define EEPROMHANDLER_LOGBLOCK_BASE_ADDR            ( 128 )

/// define the enumeration for the I2C
#define EEPROMHANDLER_I2C_ENUM                      ( I2C_DEV_ENUM_LCLBUS )

/// define the operation for busy polling( 0 - write, 1 - read )
#define EEPROMHANDLER_I2C_POLL_MODE                 ( 0 )

/// define the macro to enable debug commands
#define EEPROMHANDLER_ENABLE_DEBUGCOMMANDS          ( 1 )

#if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
/// define the system mode to allow debug commands in
#define EEPROMHANDLER_DIAGMODE_ENUM                 ( SYSCTRLMNGR_LCLMODE_DIAGNOSTICS )
#endif

/// define the macro to enable EEPROM emulation
#define EEPROMHANDLER_ENABLE_EMULATION              ( 0 )

/// define the macro to enable background writes
#define EEPROMHANDLER_ENABLE_BACKGROUND_WRITES      ( 0 )

#if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER ) && ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 ))
/// define the background write task enum
#define EEPROM_HANDLER_BACKGROUND_TASK_ENUM         ( TASK_SCHD_ILLEGAL )
#endif

/// define the page write time
#define EEPROMHANDLER_PAGE_WRITE_MSECS              ( 10 )

#endif  // _EEPROMHANDLER_PRM_H

/**@} EOF EepromHandler_prm.h */

