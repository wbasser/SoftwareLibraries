/******************************************************************************
 * @file 
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2014 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup 
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
  I2C_ERR_BLKING,           ///< device is blocked
  I2C_ERR_ILLDEV = 0xE0,    ///< illegal device
  I2C_ERR_ILLPRM,           ///< illegal parameter
  I2C_ERR_ILLACT,           ///< illegal IOCTL action
  I2C_ERR_TIMEOUT,          ///< timeout
  I2C_ERR_BUSBUSY,          ///< bus busy
  I2C_ERR_BUSFAULT,         ///< bus fault
  I2C_ERR_ARBLOST,          ///< arbitration lost
  I2C_ERR_SLVNAK,           ///< slave NAK
} I2CERR;

/// enumerate the IOCTL actions
typedef enum _I2CACTION
{
  I2C_ACTION_NONE = 0,      ///< no action
  I2C_ACTION_POLL_DEVICE,   ///< poll device - check busy
  I2C_ACTION_SET_DEVADDR,   ///< set device address
  I2C_ACTION_GET_DEVADDR,   ///< get device address
  I2C_ACTION_MAX            ///< maximum action selection
} I2CACTION;

// structures -----------------------------------------------------------------
/// define the transfer control structure
typedef struct _I2CXFRCTL
{
  U8    nDevAddr;           ///< device address
  U8    nAddrLen;           ///< address length
  U32UN tAddress;           ///< address
  PU8   pnData;             ///< pointer to the data
  U16   wDataLen;           ///< data length
  U32   uTimeout;           ///< timeout in MSEC
} I2CXFRCTL, *PI2CXFRCTL;
#define I2CXFRCTL_SIZE        sizeof( I2CXFRCTL )

/// define the check for busy structure
typedef struct _I2CCHKBSY
{
  U8    nDevAddr;           ///< address
  BOOL  bReadMode;          ///< read mode
} I2CCHKBSY, *PI2CCHKBSY;
#define I2CCHKBSY_SIZE        sizeof( I2CCHKBSY )

// global function prototypes --------------------------------------------------
extern  void    I2c_Initialize( void );
extern  void    I2c_CloseAll( void );
extern  I2CERR  I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl );
extern  I2CERR  I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl );
extern  I2CERR  I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData );
extern  I2CERR  I2c_Close( I2CDEVENUM eDev );

/**@} EOF I2c.h */

#endif  // _I2C_H