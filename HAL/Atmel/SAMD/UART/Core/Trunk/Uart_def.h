/******************************************************************************
 * @file Uart_def.h
 *
 * @brief UART definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * UART channels
 *
 * @copyright Copyright (c) 2012 CyberIntegration
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
 * \addtogroup Uart_def
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_DEF_H
#define _UART_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define UART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro for creating a UART channel
#define UART_ASYNC_DEF( chan, txdpad, rxdpad, baudrate, parity, wrdlen, stopbits, standby, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, txenb, brkdet ) \
  { \
    .eChan = chan, \
    .eMode = UART_MODE_ASYNCINT, \
    .eRxdPad = rxdpad, \
    .eTxdPad = txdpad, \
    .uBaudRate = baudrate, \
    .eSamp = UART_SAMP_16XARITH, \
    .eParity = parity, \
    .eWrdLen = wrdlen, \
    .eStopBit = stopbits, \
    .bRunStandby = standby, \
    .wRxBufSize = rxsize, \
    .wTxBufSize = txsize, \
    .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
    .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
    .pvCallback = ( PVUARTINTCALLBACK )callback, \
    .tEventFlags.bRxChar = rcvchr, \
    .tEventFlags.bRxErr = rcverr, \
    .tEventFlags.bTxEmp = txemp, \
    .tEventFlags.bTxCmp = txcmp, \
    .tEventFlags.bTxEnb = txenb, \
    .tEventFlags.bBrkDet = brkdet \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the UART channels
typedef enum _UARTCHAN
{
  UART_CHAN_0 = 0,          ///< SERCOM channel 0
  UART_CHAN_1,              ///< SERCOM channel 1
  #ifdef SERCOM2
  UART_CHAN_2,              ///< SERCOM channel 2
  #endif // SERCOM2
  #ifdef SERCOM3
  UART_CHAN_3,              ///< SERCOM channel 3
  #endif // SERCOM3
  #ifdef SERCOM4
  UART_CHAN_4,              ///< SERCOM channel 4
  #endif // SERCOM4
  #ifdef SERCOM5
  UART_CHAN_5,              ///< SERCOM channel 5
  #endif // SERCOM5
  UART_CHAN_MAX
} UARTCHAN;

/// enumerate the RXD pads
typedef enum _UARTRXDPAD
{
  UART_RXDPAD_0 = 0,        ///< SERCOM PAD0
  UART_RXDPAD_1,            ///< SERCOM PAD1
  UART_RXDPAD_2,            ///< SERCOM PAD2
  UART_RXDPAD_3,            ///< SERCOM PAD3
  UART_RXDPAD_MAX
} UARTRXDPAD;

/// enumerate the TXD pads
typedef enum _UARTTXDPAD
{
  UART_TXDPAD_0 = 0,        ///< SERCOM PAD0
  UART_TXDPAD_2,            ///< SERCOM PAD2
  UART_TXDPAD_MAX
} UARTTXDPAD;

/// enumerate the parity selection
typedef enum _UARTPARITY
{
  UART_PARITY_NONE = 0,   ///< no parity
  UART_PARITY_EVEN,       ///< even parity
  UART_PARITY_ODD,        ///< odd parity
} UARTPARITY;

/// enumerate the word length
typedef enum _UARTWRDLEN
{
  UART_WRDLEN_5 = 0,      ///< 5 bits
  UART_WRDLEN_6,          ///< 6 bits
  UART_WRDLEN_7,          ///< 7 bits
  UART_WRDLEN_8,          ///< 8 bits
  UART_WRDLEN_9,          ///< 9 bits
  UART_WRDLEN_MAX
} UARTWRDLEN;

/// enumerate the stop bits
typedef enum _UARTSTOPBIT
{
  UART_STOPBIT_1 = 0,     ///< 1 bit
  UART_STOPBIT_2,         ///< 2 bits
  UART_STOPBIT_MAX
} UARTSTOPBIT;

/// enumerate the modes
typedef enum _UARTMODE
{
  UART_MODE_ASYNCEXT = 0, ///< Async mode, external clock
  UART_MODE_ASYNCINT,     ///< async mode, internal clock
  UART_MODE_SYNCEXT,      ///< sync mode external clock
  UART_MODE_SYNCINT,      ///< sync mode internal clock
  UART_MODE_MAX
} UARTMODE;

/// enumerate the sample rate
typedef enum _UARTSAMPLE
{
  UART_SAMP_16XARITH = 0, ///< 16X oversampling, arithmatic baud rate
  UART_SAMP_16XFRAC,      ///< 16X oversampling, fractional baud rate
  UART_SAMP_8XARITH,      ///< 8X oversampling, arithmatic baud rate
  UART_SAMP_8XFRAC,       ///< 8X oversampling, fracitonal baud rate
  UART_SAMP_3XARITH,      ///< 3X oversampling, arithmatic baud rate
} UARTSAMP;

/// enumerate the UART events
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_NONE,    ///< no event
  UART_IRQ_EVENT_RXCHAR,  ///< receive character event
  UART_IRQ_EVENT_RXERR,   ///< receive error event
  UART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  UART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  UART_IRQ_EVENT_TXENB,   ///< transmit enable
  UART_IRQ_EVENT_BRKDET   ///< break detect event
} UARTIRQEVENTS;

/// enumerate the data order
typedef enum _UARTDATAORDER
{
  UART_DATAORDER_MSB = 0,
  UART_DATAORDER_LSB,
} UARTDATAORDER;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUARTINTCALLBACK )( UARTIRQEVENTS, U8 );

/// define the structure to define a UART channel
typedef struct _UARTDEF
{
  UARTCHAN          eChan;        ///< UART channel
  UARTMODE          eMode;        ///< UART clock mode
  UARTRXDPAD        eRxdPad;      ///< RXD pad
  UARTTXDPAD        eTxdPad;      ///< TAD pad
  U32               uBaudRate;    ///< baud rate
  UARTSAMP          eSamp;        ///< sampling
  UARTPARITY        eParity;      ///< parity
  UARTWRDLEN        eWrdLen;      ///< word length
  UARTSTOPBIT       eStopBit;     ///< number of stop bits
  BOOL              bRunStandby;  ///< run in standby
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
    BOOL            bTxEnb  : 1;    ///< trnasmit enable
    BOOL            bBrkDet : 1;    ///< event on break detect
  } tEventFlags;
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE        sizeof( UARTDEF )

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H