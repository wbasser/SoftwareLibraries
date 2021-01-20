/******************************************************************************
 * @file EepromHandler.h
 *
 * @brief EEPROM handler impelemntation declarations
 *
 * This file provides the declarations for the EEPROM handler implementation
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
#ifndef _EEPROMHANDLER_H
#define _EEPROMHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "EepromHandler/Eepromhandler_cfg.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#if ( EEPROMHANDLER_ENABLE_EMULATION == 0 )
#include "I2C/I2c.h"
#endif // EEPROMHANDLER_ENABLE_EMULATION
#if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

// Macros and Defines ---------------------------------------------------------
#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
#include "TaskManager/TaskManager.h"
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

// Macros and Defines ---------------------------------------------------------
#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES

// enumerations ---------------------------------------------------------------
/// enumerate the errors from the EEPROM
typedef enum _EEPROMERR
{
  EEPROM_ERR_NONE = 0,        ///< no error
  EEPROM_ERR_ILLADDR = 0xB0,  ///< illegal address
  EEPROM_ERR_DEVBUSY,
#if ( EEPROMHANDLER_ENABLE_EMULATION == 0 )
  EEPROM_ERR_I2CERR = I2C_ERR_ILLDEV,   ///< I2C error
#else
  EEPROM_ERR_I2CERR,
#endif // EEPROMHANDLER_ENABLE_EMULATION
} EEPROMERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
#if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == 1 )
extern  const CODE ASCCMDENTRY g_atEepromDbgHandlerTable[ ];
#endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

// global function prototypes --------------------------------------------------
extern  void       EepromHandler_Initialize( void );
#if ( EEPROMHANDLER_ENABLE_BACKGROUND_WRITES == 1 )
extern  void       EepromHandler_BlockWriteEvent( void );
#endif // EEPROMHANDLER_ENABLE_BACKGROUND_WRITES
extern  EEPROMERR  EepromHandler_RdByte( U16 wAddress, PU8 pnData );
extern  EEPROMERR  EepromHandler_RdWord( U16 wAddress, PU16 pwData );
extern  EEPROMERR  EepromHandler_RdLong( U16 wAddress, PU32 puData );
extern  EEPROMERR  EepromHandler_RdBlock( U16 wAddress, U16 wLength, PU8 pnData );
extern  EEPROMERR  EepromHandler_WrByte( U16 wAddress, U8 nData );
extern  EEPROMERR  EepromHandler_WrWord( U16 wAddress, U16 wData );
extern  EEPROMERR  EepromHandler_WrLong( U16 wAddress, U32 uData );
extern  EEPROMERR  EepromHandler_WrBlock( U16 wAddress, U16 wLength, PU8 pnData );

/**@} EOF EepromHandler.h */

#endif  // _EEPROMHANDLER_H
