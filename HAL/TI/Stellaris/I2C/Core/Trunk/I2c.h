/******************************************************************************
 * @file Uart.h
 *
 * @brief I2C declarations
 *
 * This file provides the declarations for the I2C devices
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup I2C
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _I2C_H
#define _I2C_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _I2CERR
{
  I2C_ERR_NONE = 0,         ///< no error
  I2C_ERR_ILLDEV = 0xB0,    ///< B0 - illegal device
  I2C_ERR_ILLPRM,           ///< B1 - illegal parameter
  I2C_ERR_ILLPER,           ///< B2 - illegal peripheral
  I2C_ERR_ILLACT,           ///< B3 - illegal IOCTL action
  I2C_ERR_TIMEOUT,          ///< B4 - timeout
  I2C_ERR_BLKING,           ///< B5 - device is blocked
  I2C_ERR_BUSBUSY,          ///< B6 - bus busy
  I2C_ERR_IF_BUS,           ///< B7 - bus fault
  I2C_ERR_SLA_NAK,          ///< B8 - slave NAK
  I2C_ERR_DAT_NAK,          ///< B9 - slave data NAK
} I2CERR;

/// enumerate the IOCTL actions
typedef enum _I2CACTION
{
  I2C_ACTION_NONE = 0,      ///< no action
  I2C_ACTION_POLL_DEVICE,   ///< poll device
  I2C_ACTION_SET_DEVADDR,   ///< set device address
  I2C_ACTION_GET_DEVADDR,   ///< get device address
  I2C_ACTION_MAX            ///< maximum action selection
} I2CACTION;

// structures -----------------------------------------------------------------
/// define the control structure for the I2C read/write
typedef struct _I2CXFRCTL
{
  U8    nDevAddr;           ///< slave address
  U32UN tAddress;           ///< command/address union
  U8    nAddrLen;           ///< command address length
  PU8   pnData;             ///< pointer to the data for both read/write
  U16   wDataLen;           ///< read/write length
  U32   uTimeout;           ///< timeout value
} I2CXFRCTL, *PI2CXFRCTL;

#define I2CXFRCTL_SIZE   sizeof( I2CXFRCTL )
/// define the check for busy structure
typedef struct _I2CCHKBSY
{
  U8    nDevAddr;           ///< address
  BOOL  bReadMode;          ///< read mode
} I2CCHKBSY, *PI2CCHKBSY;
#define I2CCHKBSY_SIZE        sizeof( I2CCHKBSY )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    I2c_Initialize( void );
extern  I2CERR  I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptI2cXfrCtl );
extern  I2CERR  I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptI2cXfrCtl );
extern  I2CERR  I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData );
extern  I2CERR  I2c_Close( I2CDEVENUM eDev );

/**@} EOF I2c.h */

#endif  // _I2C_H