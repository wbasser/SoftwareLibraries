/******************************************************************************
 * @file Uart_def.h
 *
 * @brief UART definitions
 *
 * This file contains the UART configuration definitions
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
 * \addtogroup UART
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _UART_DEF_H
#define _UART_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// stellarisware library includes
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define UART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 
                     
/// define the helper macro for creating a UART device
#define UART_DEVICE( devper, gpioper, devbase, gpiobase, rxpin, txpin, rxfunc, txfunc, rxsize, txsize, name, parity, wrdlen, stop, baudrate, callback ) \
                   { .uDevPeriph = devper, \
                     .uGpioPeriph = gpioper, \
                     .uDevBase = devbase, \
                     .uGpioBase = gpiobase, \
                     .nRxPin = rxpin, \
                     .nTxPin = txpin, \
                     .uRxPinFunc = rxfunc, \
                     .uTxPinFunc = txfunc, \
                     .wRxBufSize = rxsize, \
                     .wTxBufSize = txsize, \
                     .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
                     .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
                     .eParity = parity, \
                     .eWrdLength = wrdlen, \
                     .eStop = stop, \
                     .uBaudRate = baudrate, \
                     .pvCallBack = ( PVUARTINTCALLBACK )callback \
                   }

// enumerations ---------------------------------------------------------------
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_RXCHAR,  ///< receive character event
  UART_IRQ_EVENT_RXERR,   ///< receive error event
  UART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  UART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  UART_IRQ_EVENT_BRKDET   ///< break detect event
} UARTIRQEVENTS;

/// enumerate the parity selections
typedef enum _UARTPAR
{
  UART_PAR_NONE = UART_CONFIG_PAR_NONE, ///< no oparity
  UART_PAR_EVEN	= UART_CONFIG_PAR_EVEN, ///< event parity
  UART_PAR_ODD	= UART_CONFIG_PAR_ODD,  ///< odd parity
  UART_PAR_MARK	= UART_CONFIG_PAR_ONE,  ///< mark parity
  UART_PAR_MAX                          ///< maximum parity selection
} UARTPAR;

/// enumerate the word length selections
typedef enum _UARTWRDLEN
{	
  UART_WRDLEN_FIVE	= UART_CONFIG_WLEN_5, ///< word length 5
  UART_WRDLEN_SIX		= UART_CONFIG_WLEN_6, ///< word length 6
  UART_WRDLEN_SEVEN	= UART_CONFIG_WLEN_7, ///< word length 7
  UART_WRDLEN_EIGHT = UART_CONFIG_WLEN_8, ///< word length 8
  UART_WRDLEN_MAX                         ///< maximum word length selection
} UARTWRDLEN;

/// enumerate the stop bit selections
typedef enum _UARTSTOP
{
  UART_STOP_ONE = UART_CONFIG_STOP_ONE, ///< one stop bit
  UART_STOP_TWO	= UART_CONFIG_STOP_TWO, ///< two stop bits
  UART_STOP_MAX                         ///< maximum stop bit selection
} UARTSTOP;

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVUARTINTCALLBACK )( UARTIRQEVENTS eEvent, U8 nData );

/// define the UART definition structure
typedef struct _UARTDEF
{
  U32               uDevPeriph;     ///< device peripheral
  U32               uGpioPeriph;    ///< GPIO peripheral
  U32               uDevBase;       ///< base address of the device
  U32               uGpioBase;      ///< base address of the GPIO
  U8                nRxPin;         ///< RX pin
  U8                nTxPin;         ///< TX pin
  U32               uRxPinFunc;     ///< RX pin function
  U32               uTxPinFunc;     ///< RX pin function
  U16               wRxBufSize;     ///< RX buffer size
  U16               wTxBufSize;     ///< RX buffer size
  PU8               pnRxBuffer;     ///< RX buffer pointer
  PU8               pnTxBuffer;     ///< TX buffer pointer
  UARTPAR           eParity;        ///< parity
  UARTWRDLEN        eWrdLength;     ///< wordlength
  UARTSTOP          eStop;          ///< stop bits
  U32               uBaudRate;      ///< baud rate
  PVUARTINTCALLBACK pvCallBack;     ///< callback function
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE sizeof( UARTDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H