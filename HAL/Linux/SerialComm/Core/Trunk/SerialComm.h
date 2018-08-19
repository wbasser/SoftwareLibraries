/******************************************************************************
 * @file SerialComm.h
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup SerialComm
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SERIALCOMM_H
#define _SERIALCOMM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SerialComm/SerialComm_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _SERIALCOMMERR
{
  SERIALCOMM_ERR_NONE = 0,        ///< no error
  SERIALCOMM_ERR_ILLDEV,          ///< illegal device
  SERIALCOMM_ERR_ILLPRM,          ///< illegal parameter
  SERIALCOMM_ERR_ILLPER,          ///< illegal peripheral
  SERIALCOMM_ERR_ILLACT,          ///< illegal IOCTL action
  SERIALCOMM_ERR_CLOSEFAIL,      ///< error on close
} SERIALCOMMERR;

/// enumerate the IOCTL actions
typedef enum _SERIALCOMMACTION
{
  SERIALCOMM_ACTION_NONE = 0,     ///< no action
  SERIALCOMM_ACTION_SET_BAUDRATE, ///< set baudrate
  SERIALCOMM_ACTION_GET_BAUDRATE, ///< get baudrate
  SERIALCOMM_ACTION_SET_PARITY,   ///< set parity
  SERIALCOMM_ACTION_GET_PARITY,   ///< get parity
  SERIALCOMM_ACTION_SET_WRDLEN,   ///< set wordlength
  SERIALCOMM_ACTION_GET_WRDLEN,   ///< get wordlength
  SERIALCOMM_ACTION_SET_STOPBITS, ///< set stopbits
  SERIALCOMM_ACTION_GET_STOPBITS, ///< get stopbits
  SERIALCOMM_ACTION_MAX           ///< maximum action selection
} SERIALCOMMACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          SerialComm_Initialize( void );
extern  SERIALCOMMERR SerialComm_Close( SERIALCOMMDEVENUM eDev );
extern  SERIALCOMMERR SerialComm_Write( SERIALCOMMDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten );
extern  SERIALCOMMERR SerialComm_Read( SERIALCOMMDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead );
extern  SERIALCOMMERR SerialComm_Ioctl( SERIALCOMMDEVENUM eDev, SERIALCOMMACTION eAction, PVOID pvData );

/**@} EOF SerialComm.h */

#endif  // _SERIALCOMM_H