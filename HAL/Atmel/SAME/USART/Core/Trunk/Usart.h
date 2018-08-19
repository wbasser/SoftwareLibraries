/******************************************************************************
 * @file Uart.h
 *
 * @brief USART declarations
 *
 * This file provides the declarations for the USART devices
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup USART
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USART_H
#define _USART_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USART/Usart_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _USARTERR
{
  USART_ERR_NONE = 0,        ///< no error
  USART_ERR_ILLDEV,          ///< illegal device
  USART_ERR_ILLPRM,          ///< illegal parameter
  USART_ERR_ILLPER,          ///< illegal peripheral
  USART_ERR_ILLACT           ///< illegal IOCTL action
} USARTERR;

/// enumerate the IOCTL actions
typedef enum _USARTACTION
{
  USART_ACTION_NONE = 0,     ///< no action
  USART_ACTION_SET_BAUDRATE, ///< set baudrate
  USART_ACTION_GET_BAUDRATE, ///< get baudrate
  USART_ACTION_SET_PARITY,   ///< set parity
  USART_ACTION_GET_PARITY,   ///< get parity
  USART_ACTION_SET_WRDLEN,   ///< set wordlength
  USART_ACTION_GET_WRDLEN,   ///< get wordlength
  USART_ACTION_SET_STOPBITS, ///< set stopbits
  USART_ACTION_GET_STOPBITS, ///< get stopbits
  USART_ACTION_MAX           ///< maximum action selection
} USARTACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void      Usart_Initialize( void );
extern  void      Usart_CloseAll( void );
extern  USARTERR  Usart_Write( USARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten );
extern  USARTERR  Usart_Read( USARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead );
extern  USARTERR  Usart_Ioctl( USARTDEVENUM eDev, USARTACTION eAction, PVOID pvData );
extern  USARTERR  Usart_Close( USARTDEVENUM eDev );

/**@} EOF Usart.h */

#endif  // _USART_H
