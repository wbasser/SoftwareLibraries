/******************************************************************************
 * @file Uart_def.h
 *
 * @brief UART definitions
 *
 * This file contains the UART configuration definitions
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
 * \addtogroup UART
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _UART_DEF_H
#define _UART_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the helper macro for creating storage
#define UART_BUFFER( name, rxsize, txsize ) \
 static U8 an ## name ## RxBuf[ rxsize ]; \
 static U8 an ## name ## TxBuf[ txsize ];
                     
/// define the helper macro for creating a UART device
#define UART_DEVICE( chan, baudrate, parity, wlen, sbits, imux, rxsize, txsize, name, callback, rcvchr, rcverr, txemp, txcmp, brkdet ) \
 { \
   .eChan = chan, \
   .uBaudRate = baudrate, \
   .eParity = parity, \
   .eWrdLen = wlen, \
   .eStop  = sbits, \
   .eInputMux = imux, \
   .nRxBufSize = rxsize, \
   .nTxBufSize = txsize, \
   .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
   .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
   .pvCallBack = ( PVUARTINTCALLBACK )callback, \
   .tEventFlags.bRxChar = rcvchr, \
   .tEventFlags.bRxErr = rcverr, \
   .tEventFlags.bTxEmp = txemp, \
   .tEventFlags.bTxCmp = txcmp, \
   .tEventFlags.bBrkDet = brkdet \
 }

// enumerations ---------------------------------------------------------------
/// enumerate the uart channels
typedef enum _UARTCHAN
{
  UART_CHAN_SC00 = 0,
  UART_CHAN_SC01,
  UART_CHAN_MAX
} UARTCHAN;

/// enumerate the parity selections
typedef enum _UARTPAR
{
  UART_PAR_NONE = 0,        ///< no parity
  UART_PAR_ODD,             ///< odd parity
  UART_PAR_EVEN,            ///< event parity
  UART_PAR_MAX              ///< maximum parity selection
} UARTPAR;

/// enumerate the word length selections
typedef enum _UARTWRDLEN
{	
  UART_WRDLEN_FIVE = 5,     ///< word length 5
  UART_WRDLEN_SIX,          ///< word length 6
  UART_WRDLEN_SEVEN,        ///< word length 7
  UART_WRDLEN_EIGHT,        ///< word length 8
  UART_WRDLEN_MAX           ///< maximum word length selection
} UARTWRDLEN;

/// enumerate the stop bit selections
typedef enum _UARTSTOP
{
  UART_STOP_ONE = 0,        ///< one stop bit
  UART_STOP_TWO,            ///< two stop bits
  UART_STOP_MAX             ///< maximum stop bit selection
} UARTSTOP;

/// enumerate the input mux
typedef enum _UARTINMUX
{
  UART_INMUX_DX0A = 0,    ///< DX0A
  UART_INMUX_DX0B,        ///< DX0B
  UART_INMUX_DX0C,        ///< DX0C
  UART_INMUX_DX0D,        ///< DX0D
  UART_INMUX_DX0E,        ///< DX0E
  UART_INMUX_DX0F,        ///< DX0F
  UART_INMUX_DX0G,        ///< DX0G
} UARTINMUX;

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

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVUARTINTCALLBACK )( UARTIRQEVENTS eEvent, U8 nOption );

/// define the UART definition structure
typedef struct _UARTDEF
{
  UARTCHAN          eChan;          ///< UARTR channel
  U32               uBaudRate;      ///< baud rate
  UARTPAR           eParity;        ///< parity
  UARTWRDLEN        eWrdLen;        ///< word length
  UARTSTOP          eStop;          ///< stop bits
  UARTINMUX         eInputMux;      ///< input channel mux (DX0)
  U8                nRxBufSize;     ///< RX buffer size
  U8                nTxBufSize;     ///< RX buffer size
  PU8               pnRxBuffer;     ///< RX buffer pointer
  PU8               pnTxBuffer;     ///< TX buffer pointer
  PVUARTINTCALLBACK pvCallBack;     ///< callback function
  struct
  {
    BOOL            bRxChar : 1;    ///< event on receive character
    BOOL            bRxErr  : 1;    ///< event on receive error
    BOOL            bTxEmp  : 1;    ///< event on transmit empty
    BOOL            bTxCmp  : 1;    ///< event on transmit shift register empty
    BOOL            bBrkDet : 1;    ///< event on break detect
  } tEventFlags;
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE sizeof( UARTDEF )

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H
