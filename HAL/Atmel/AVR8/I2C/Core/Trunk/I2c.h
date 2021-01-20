/******************************************************************************
 * @file I2c.h
 *
 * @brief I2C declarations
 *
 * This file provides the declarations for the I2C software module
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _I2C_H
#define _I2C_H

// lOCAL includes   -----------------------------------------------------------
#include "I2C/I2c_cfg.h"

// Macros and Defines ---------------------------------------------------------
#define I2C_AUTORDWR_ENABLED      ( 1 )

// enumerations ---------------------------------------------------------------
/// enumerate the single instantiation
typedef enum _I2CDEVENUM
{
  I2C_DEV_ENUM_LCLBUS = 0
} I2CDEVENUM;

/// enumerate the IOCTL acton
typedef enum _I2CACTION
{
  I2C_ACTION_SET_BAUDRATE = 0,
  I2C_ACTION_GET_BAUDRATE,
  I2C_ACTION_SET_SLVADDR,
  I2C_ACTION_GET_SLVADDR,
  I2C_ACTION_SET_SLVMASK,
  I2C_ACTION_GET_SLVMASK,
  I2C_ACTION_GET_BUS_BUSY,
  I2C_ACTION_RESET_DEVICE,
  I2C_ACTION_MAX
} I2CACTION;

/// enumerate the errors
typedef enum _I2CERROR
{
  I2C_ERROR_NONE = 0,         ///< 
  I2C_ERROR_BUS_BUSY = 0xD0,
  I2C_ERROR_ARB_LOST,
  I2C_ERROR_SLA_NACK,
  I2C_ERROR_DATA_NACK,
  I2C_ERROR_PARTIAL_XFR,
  I2C_ERROR_BUS_ERROR,
  I2C_ERROR_TIMEOUT,
  I2C_ERROR_ILL_MODE,
  I2C_ERROR_ILL_ACTION,
  I2C_ERROR_ILL_PARAMETER
} I2CERROR;

// structures -----------------------------------------------------------------
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

// global function prototypes --------------------------------------------------
extern  void      I2c_Initialize( void );
extern  I2CERROR  I2c_Write( I2CDEVENUM eDevEnum, PI2CXFRCTL ptXfrCtl );
extern  I2CERROR  I2c_Read( I2CDEVENUM eDevEnum, PI2CXFRCTL ptXfrCtl );
extern  I2CERROR  I2c_Ioctl( I2CDEVENUM eDevEnum, I2CACTION eAction, PVOID pvParam );
extern  void      I2c_Close( void );

/**@} EOF I2c.h */

#endif  // _I2C_H