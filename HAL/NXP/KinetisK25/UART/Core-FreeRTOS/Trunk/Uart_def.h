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
#define UART_DEF( chan, baudrate, parity, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp ) \
  { \
    .eChan = chan, \
    .uBaudRate = baudrate, \
    .eParity = parity, \
    .wRxBufSize = rxsize, \
    .wTxBufSize = txsize, \
    .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
    .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
    .pvCallback = ( PVUARTINTCALLBACK )callback, \
    .tEventFlags.bRxChar = rcvchr, \
    .tEventFlags.bRxErr = rcverr, \
    .tEventFlags.bTxEmp = txemp, \
    .tEventFlags.bTxCmp = txcmp, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the UART channels
typedef enum _UARTCHAN
{
  UART_CHAN_0 = 0,          ///< UART channel 0
  UART_CHAN_1,              ///< UART channel 1
  UART_CHAN_MAX
} UARTCHAN;

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
  UART_PARITY_EVEN = 0,   ///< even parity
  UART_PARITY_ODD,        ///< odd parity
  UART_PARITY_SPACE,      ///< space
  UART_PARITY_MARK,       ///< mark
  UART_PARITY_NONE,       ///< no parity
} UARTPARITY;

/// enumerate the UART events
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_NONE,    ///< no event
  UART_IRQ_EVENT_RXCHAR,  ///< receive character event
  UART_IRQ_EVENT_RXERR,   ///< receive error event
  UART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  UART_IRQ_EVENT_TXCMP,   ///< transmit complete event
} UARTIRQEVENTS;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUARTINTCALLBACK )( UARTIRQEVENTS, U8 );

/// define the structure to define a UART channel
typedef struct _UARTDEF
{
  UARTCHAN          eChan;        ///< UART channel
  U32               uBaudRate;    ///< baud rate
  UARTPARITY        eParity;      ///< parity
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
  } tEventFlags;
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE        sizeof( UARTDEF )

/**@} EOF Uart_def.h */

#endif  // _UART_DEF_H