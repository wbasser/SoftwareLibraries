/******************************************************************************
 * @file I2C.h
 *
 * @brief I2C sercom declarations
 *
 * This file provides the declarations for the I2C sercom module
 *
 * @copyright Copyright (c) 2014 CyberIntegration
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
#ifndef _I2C_H
#define _I2C_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
#define I2C_AUTORDWR_ENABLED      ( 1 )

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
  I2C_ACTION_POLL_DEVICE,   ///< poll device
  I2C_ACTION_SET_DEVADDR,   ///< set device address
  I2C_ACTION_GET_DEVADDR,   ///< get device address
  I2C_ACTION_MAX            ///< maximum action selection
} I2CACTION;

/// enumerate the I2C channels
typedef enum _I2CCHAN
{
  I2C_CHAN_0 = 0,          ///< SERCOM channel 0
  I2C_CHAN_1,              ///< SERCOM channel 1
  #ifdef SERCOM2
  I2C_CHAN_2,              ///< SERCOM channel 2
  #endif // SERCOM2
  #ifdef SERCOM3
  I2C_CHAN_3,              ///< SERCOM channel 3
  #endif // SERCOM3
  #ifdef SERCOM4
  I2C_CHAN_4,              ///< SERCOM channel 4
  #endif // SERCOM4
  #ifdef SERCOM5
  I2C_CHAN_5,              ///< SERCOM channel 5
  #endif // SERCOM5
  I2C_CHAN_MAX
} I2CCHAN;

// structures -----------------------------------------------------------------
/// define the task handle
typedef PVOID   PVI2CHANDLE;

/// define the structure to define a UART channel
typedef struct _I2CDEF
{
  I2CCHAN           eChan;        ///< UART channel
  BOOL              bRunStandby;  ///< run in standby
  BOOL              bFastSpeed;   ///< fast speed
  GPIOPORT          eDevPort;     ///< device port
  GPIOFUNCMUX       eDevMux;      ///< device mux
  U8                nSdaPin;      ///< SDA pin
  U8                nSclPin;      ///< SCL pin
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE        sizeof( I2CDEF )

/// define the transfer control
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
extern  PVI2CHANDLE I2c_Configure( PI2CDEF ptI2cDef );
extern  I2CERR      I2c_Write( PVI2CHANDLE pvI2cHandle, PI2CXFRCTL ptXfrCtl );
extern  I2CERR      I2c_Read( PVI2CHANDLE pvI2cHandle, PI2CXFRCTL ptXfrCtl );
extern  I2CERR      I2c_Ioctl( PVI2CHANDLE pvI2cHandle, I2CACTION eAction, PVOID pvData );
extern  I2CERR      I2c_Close( PVI2CHANDLE pvI2cHandle );

/**@} EOF I2c.h */

#endif  // _I2C_H