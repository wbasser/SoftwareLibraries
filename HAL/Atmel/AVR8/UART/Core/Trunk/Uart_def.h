/******************************************************************************
 * @file Uart_def.h
 *
 * @brief UART definition file
 *
 * This file the enumerations and structures used to define a UART channel
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
 * \addtogroup Uart
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_DEF_H
#define _UART_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define UART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro to create a ASYNC UART
#define UART_ASYNC_DEF( uchan, baud, parity, len, stop, name, rxsize, txsize, callback ) \
 { \
  .eMode = UART_MODE_ASYNC, \
  .eChan = uchan, \
  .uBaudRate = baud, \
  .eParity = parity, \
  .eWordLen = len, \
  .eStopBits = stop, \
  .wRxBufSize = rxsize, \
  .wTxBufSize = txsize, \
  .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
  .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
  .pvCallback = ( PVUARTIRQCALLBACK )callback \
 }
 
// enumerations ---------------------------------------------------------------
/// enumerate the UART channels
typedef enum _UARTCHAN
{
  UART_CHAN_0 = 0,
  UART_CHAN_1,
  UART_CHAN_2,
  UART_CHAN_MAX
} UARTCHAN  ;

/// enumerate the callback events
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_RXCHAR,  ///< receive character event
  UART_IRQ_EVENT_RXERR,   ///< receive error event
  UART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  UART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  UART_IRQ_EVENT_BRKDET   ///< break detect event
} UARTIRQEVENTS;

/// enumerate the modes
typedef enum _UARTMODE
{
  UART_MODE_ASYNC = 0,
  UART_MODE_SYNC, 
  UART_MODE_MSTSPI = 3
} UARTMODE;

/// enumerate the word lengths
typedef enum _UARTWLEN
{
  UART_WLEN_5 = 0,
  UART_WLEN_6,
  UART_WLEN_7,
  UART_WLEN_8,
  UART_WLEN_9 = 7
} UARTWLEN;

/// enumerate the stop bits
typedef enum _UARTSTOP
{
  UART_STOP_1 = 0,
  UART_STOP_2
} UARTSTOP;

/// enumerate the parity
typedef enum _UARTPARITY
{
  UART_PARITY_NONE = 0,
  UART_PARITY_EVEN = 2,
  UART_PARITY_ODD
} UARTPARITY;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUARTIRQCALLBACK )( UARTIRQEVENTS, U8 );

/// define the UART control structure
typedef struct _UARTDEF
{
  UARTMODE          eMode;        ///< USART mode
  UARTCHAN          eChan;        ///< usart channel
  U32               uBaudRate;    ///< baud rate
  UARTWLEN          eWordLen;     ///< word length
  UARTPARITY        eParity;      ///< parity
  UARTSTOP          eStopBits;    ///< stop bits
  U16               wRxBufSize;   ///< RX buffer size
  U16               wTxBufSize;   ///< RX buffer size
  PU8               pnRxBuffer;   ///< RX buffer pointer
  PU8               pnTxBuffer;   ///< TX buffer pointer
  PVUARTIRQCALLBACK pvCallback;   ///< event callback handler
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE    sizeof( UARTDEF )

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H