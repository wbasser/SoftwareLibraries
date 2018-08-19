/******************************************************************************
 * @file I2C.h
 *
 * @brief I2C declarations
 *
 * This file provides the declarations for the I2C devices
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
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

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _I2CERR
{
  I2C_ERR_NONE = 0,         ///< no error
  I2C_ERR_ILLDEV,           ///< illegal device
  I2C_ERR_ILLPRM,           ///< illegal parameter
  I2C_ERR_ILLPER,           ///< illegal peripheral
  I2C_ERR_ILLACT,           ///< illegal IOCTL action
  I2C_ERR_TIMEOUT,          ///< timeout
  I2C_ERR_BLKING,           ///< device is blocked
  I2C_ERR_SYSERR,						///< system error
  I2C_ERR_ARBLOST,					///< arbitration lost
  I2C_ERR_BUSERR,						///< bus error
  I2C_ERR_BUSNAK,           ///< bus NAK
} I2CERR;

/// enumerate the IOCTL actions
typedef enum _I2CACTION
{
  I2C_ACTION_NONE = 0,      ///< no action
  I2C_ACTION_SET_DEVADDR,   ///< set device address
  I2C_ACTION_GET_DEVADDR,   ///< get device address
  I2C_ACTION_MAX            ///< maximum action selection
} I2CACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void   I2c_Initialize( void  );
extern  I2CERR I2c_Write( I2CDEVENUM eDev, U8 nSlvAddr, PU8 pnData, U8 nXmtlen, U32 uTimeout );
extern  I2CERR I2c_Read( I2CDEVENUM eDev, U8 nSlvAddr, PU8 pnData, U8 nRcvLen, U32 uTimeout );
extern  I2CERR I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData );
extern  I2CERR I2c_Close( I2CDEVENUM eDev );

/**@} EOF I2c.h */

#endif  // _I2C_H
