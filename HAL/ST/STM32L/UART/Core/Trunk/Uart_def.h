/******************************************************************************
 * @file Uart_def.h
 *
 * @brief UART definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * UART channels
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup Uart_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_DEF_H
#define _UART_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define UART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro for creating a UART channel
#define UART_ASYNC_DEF( chan, baudrate, parity, wrdlen, stopbits, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, brkdet ) \
  { \
    .eChan        = chan, \
    .eMode        = UART_MODE_ASYNC, \
    .uBaudRate    = baudrate, \
    .eSamp        = UART_SAMP_16X, \
    .eDataOrder   = UART_DATAORDER_LSB, \
    .eParity      = parity, \
    .eWrdLen      = wrdlen, \
    .eStopBit     = stopbits, \
    .wRxBufSize   = rxsize, \
    .wTxBufSize   = txsize, \
    .pnRxBuffer   = ( PU8 )&an ## name ## RxBuf, \
    .pnTxBuffer   = ( PU8 )&an ## name ## TxBuf, \
    .pvCallback   = ( PVUARTINTCALLBACK )callback, \
    .tEventFlags  = \
    { \
      .bRxChar    = rcvchr, \
      .bRxErr     = rcverr, \
      .bTxEmp     = txemp, \
      .bTxCmp     = txcmp, \
      .bBrkDet    = brkdet \
    } \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the UART channels
typedef enum _UARTCHAN
{
  UART_CHAN_1,              ///< UART channel 1
  UART_CHAN_2,              ///< UART channel 2
  UART_CHAN_3,              ///< UART channel 3
  UART_CHAN_4,              ///< UART channel 4
  UART_CHAN_5,              ///< UART channel 5
  UART_CHAN_MAX
} UARTCHAN;

/// enumerate the parity selection
typedef enum _UARTPARITY
{
  UART_PARITY_NONE = 0,   ///< no parity
  UART_PARITY_EVEN = 2,   ///< even parity
  UART_PARITY_ODD,        ///< odd parity
} UARTPARITY;

/// enumerate the word length
typedef enum _UARTWRDLEN
{
  UART_WRDLEN_7 = 0,          ///< 7 bits
  UART_WRDLEN_8,          ///< 8 bits
  UART_WRDLEN_9,          ///< 9 bits
  UART_WRDLEN_MAX
} UARTWRDLEN;

/// enumerate the stop bits
typedef enum _UARTSTOPBIT
{
  UART_STOPBIT_1 = 0,     ///< 1 bit
  UART_STOPBIT_0_5,       ///< half a bit
  UART_STOPBIT_2,         ///< 2 bits
  UART_STOPBIT_1_5,        ///< 1.5 bits
  UART_STOPBIT_MAX
} UARTSTOPBIT;

/// enumerate the modes
typedef enum _UARTMODE
{
  UART_MODE_ASYNC = 0,    ///< Async mode, external clock
  UART_MODE_SYNC,         ///< sync mode
  UART_MODE_LIN,          ///< LIN mode
  UART_MODE_SMCARD,       ///< smart card mode
  UART_MODE_MAX
} UARTMODE;

/// enumerate the sample rate
typedef enum _UARTSAMPLE
{
  UART_SAMP_16X = 0,       ///< sample 16x
  UART_SAMP_8X,            ///< sampe 8X
  UART_SAMP_MAX
} UARTSAMP;

/// enumerate the UART events
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_NONE,    ///< no event
  UART_IRQ_EVENT_RXCHAR,  ///< receive character event
  UART_IRQ_EVENT_RXERR,   ///< receive error event
  UART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  UART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  UART_IRQ_EVENT_BRKDET   ///< break detect event
} UARTIRQEVENTS;

/// enumerate the data order
typedef enum _UARTDATAORDER
{
  UART_DATAORDER_LSB = 0,   ///< data sent/rcvd with LSB first
  UART_DATAORDER_MSB,       ///< data sent/rcvd with MSB first
  UART_DATAORDER_MAX,
} UARTDATAORDER;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUARTINTCALLBACK )( UARTIRQEVENTS, U8 );

/// define the structure to define a UART channel
typedef struct _UARTDEF
{
  UARTCHAN          eChan;        ///< UART channel
  UARTMODE          eMode;        ///< UART clock mode
  U32               uBaudRate;    ///< baud rate
  UARTSAMP          eSamp;        ///< sampling
  UARTPARITY        eParity;      ///< parity
  UARTWRDLEN        eWrdLen;      ///< word length
  UARTSTOPBIT       eStopBit;     ///< number of stop bits
  UARTDATAORDER     eDataOrder;   ///< data order
  U16               wRxBufSize;   ///< RX buffer size
  U16               wTxBufSize;   ///< RX buffer size
  PU8               pnRxBuffer;   ///< RX buffer pointer
  PU8               pnTxBuffer;   ///< TX buffer pointer
  PVUARTINTCALLBACK pvCallback;   ///< callback
  struct
  {
    BOOL            bRxChar : 1;    ///< event on receive character
    BOOL            bRxErr  : 1;    ///< event on receive error
    BOOL            bTxEmp  : 1;    ///< event on transmit empty
    BOOL            bTxCmp  : 1;    ///< event on transmit shift register empty
    BOOL            bBrkDet : 1;    ///< event on break detect
  } tEventFlags;
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE        sizeof( UARTDEF )

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H