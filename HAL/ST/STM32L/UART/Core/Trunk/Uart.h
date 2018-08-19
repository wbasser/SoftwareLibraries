/******************************************************************************
 * @file Uart.h
 *
 * @brief UART declarations
 *
 * This file provides the declarations for the UART devices
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup UART
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_H
#define _UART_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _UARTERR
{
  UART_ERR_NONE = 0,        ///< no error
  UART_ERR_ILLDEV,          ///< illegal device
  UART_ERR_ILLPRM,          ///< illegal parameter
  UART_ERR_ILLPER,          ///< illegal peripheral
  UART_ERR_ILLACT           ///< illegal IOCTL action
} UARTERR;

/// enumerate the IOCTL actions
typedef enum _UARTACTION
{
  UART_ACTION_NONE = 0,     ///< no action
  UART_ACTION_SET_BAUDRATE, ///< set baudrate
  UART_ACTION_GET_BAUDRATE, ///< get baudrate
  UART_ACTION_SET_PARITY,   ///< set parity
  UART_ACTION_GET_PARITY,   ///< get parity
  UART_ACTION_SET_WRDLEN,   ///< set wordlength
  UART_ACTION_GET_WRDLEN,   ///< get wordlength
  UART_ACTION_SET_STOPBITS, ///< set stopbits
  UART_ACTION_GET_STOPBITS, ///< get stopbits
  UART_ACTION_MAX           ///< maximum action selection
} UARTACTION;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    Uart_Initialize( void );
extern  void    Uart_CloseAll( void );
extern  UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten );
extern  UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead );
extern  UARTERR Uart_Ioctl( UARTDEVENUM eDev, UARTACTION eAction, PVOID pvData );
extern  UARTERR Uart_Close( UARTDEVENUM eDev );

/**@} EOF Uart.h */

#endif  // _UART_H
