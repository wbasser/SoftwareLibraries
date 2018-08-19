/******************************************************************************
 * @file I2c_def.h
 *
 * @brief I2C definition declarations
 *
 * This file provides the definitions for the configuration structure for
 * the I2C module
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _I2C_DEF_H
#define _I2C_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for defining an I2C instantiation
#define I2CCFGDEF_ENTRY( mode, baudrate, slaveaddr, callback ) \
  .eMode = mode, \
  .uBaudRate = baudrate, \
  .nSlaveAddr = slaveaddr, \
  .pvCallback = callback

// enumerations ---------------------------------------------------------------
/// enumerate the mode
typedef enum  _I2CMODE
{
  I2C_MODE_MASTER = 0,
  I2C_MODE_SLAVE,
  I2C_MODE_MULTI_MASTER,
} I2CMODE;

/// enumerate the status for the callbacks
typedef enum  _I2CCALLBACKSTATUS
{
  I2C_CALLBACK_NO_ERROR = 0,   ///< no error status
  I2C_CALLBACK_BUF_FULL,       ///< receiver buffer full
  I2C_CALLBACK_LAST_XMT_BYTE,  ///< last transmit byte
} I2CCALLBACKSTATUS;

/// enumerate the events for the callback
typedef enum _I2CCALLBACKEVENTS
{
  I2C_CALLBACK_SLVADR = 0,    ///< slave address received
  I2C_CALLBACK_SLVRCV,        ///< slave data byte received
  I2C_CALLBACK_SLVXMT,        ///< slave data byte transmit request
  I2C_CALLBACK_SLVSTP,        ///< slave stop received
  I2C_CALLBACK_MSTDONE,       ///< master transmit/receive done
} I2CCALLBACKEVENTS;


// structures -----------------------------------------------------------------
/// define the I2C callback routine
typedef I2CCALLBACKSTATUS ( *PVI2CCALLBACK )( I2CCALLBACKEVENTS, PU8 );

/// define the I2C definition structure
typedef struct _I2CCFGDEF
{
  I2CMODE       eMode;        ///< mode
  U32           uBaudRate;    ///< baudrate
  U8            nSlaveAddr;   ///< slave address
  PVI2CCALLBACK pvCallback;   ///< callback
} I2CCFGDEF, *PI2CCFGDEF;
#define I2CCFGDEF_SIZE      sizeof( I2CCFGDEF )

/**@} EOF I2c_def.h */

#endif  // _I2C_DEF_H