/******************************************************************************
 * @file Usart_def.h
 *
 * @brief USART definition declarations
 *
 * This file provides the definition declarations for use in configuring
 * USART channels
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
 * \addtogroup USART
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USART_DEF_H
#define _USART_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
#define USART_BUFFER( name, rxsize, txsize ) \
                     static U8 an ## name ## RxBuf[ rxsize ]; \
                     static U8 an ## name ## TxBuf[ txsize ]; 

/// define the helper macro for creating a USART channel
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define USART_DEFNRM( chan, baudrate, wrdlen, stop, parity, clksrc, name, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, txenb ) \
  { \
    .eChan = chan, \
    .uBaudRate = baudrate, \
    .eMode = USART_MODE_NORMAL, \
    .eWrdLen = wrdlen, \
    .eStopBits = stop, \
    .eParity = parity, \
    .eClkSource = clksrc, \
    .wRxBufSize = rxsize, \
    .wTxBufSize = txsize, \
    .pnRxBuffer = ( PU8 )&an ## name ## RxBuf, \
    .pnTxBuffer = ( PU8 )&an ## name ## TxBuf, \
    .pvCallback = ( PVUSARTINTCALLBACK )callback, \
    .tEventFlags.bRxChar = rcvchr, \
    .tEventFlags.bRxErr = rcverr, \
    .tEventFlags.bTxEmp = txemp, \
    .tEventFlags.bTxCmp = txcmp, \
    .tEventFlags.bTxEnb = txenb, \
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define USART_DEFNRM( chan, baudrate, wrdlen, stop, parity, clksrc, rxsize, txsize, callback, rcvchr, rcverr, txemp, txcmp, txenb ) \
  { \
    .eChan = chan, \
    .eMode = USART_MODE_NORMAL, \
    .uBaudRate = baudrate, \
    .eWrdLen = wrdlen, \
    .eStopBits = stop, \
    .eParity = parity, \
    .eClkSource = clksrc, \
    .wRxBufSize = rxsize, \
    .wTxBufSize = txsize, \
    .pvCallback = ( PVUSARTINTCALLBACK )callback, \
    .tEventFlags.bRxChar = rcvchr, \
    .tEventFlags.bRxErr = rcverr, \
    .tEventFlags.bTxEmp = txemp, \
    .tEventFlags.bTxCmp = txcmp, \
    .tEventFlags.bTxEnb = txenb, \
  }
#endif // SYSTEMDEFINE_OS_SELECTION

/// define the macro for creating a SPI 
#define USART_DEFSPI( chan, baudrate, clkpol, clkpha, clksrc ) \
  { \
    .eChan = chan, \
    .eMode = USART_MODE_SPI, \
    .uBaudRate = baudrate, \
    .eClkPolarity = clkpol, \
    .eClkPhase = clkpha, \
    .eClkSource = clksrc, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the USART channels
typedef enum _USARTCHAN
{
  USART_CHAN_0 = 0,          ///< USART channel 0
  USART_CHAN_1,              ///< USART channel 1
  USART_CHAN_2,              ///< USART channel 2
  USART_CHAN_MAX
} USARTCHAN;

/// enumerate the USART Modes
typedef enum _USARTMODE
{
  USART_MODE_NORMAL = 0,      ///< normal
  USART_MODE_RS485,           ///< RS485
  USART_MODE_HWHANDSHAKE,     ///< hardware handlshaking
  USART_MODE_MODEM,           ///< modem
  USART_MODE_ISO7816T0,       ///< ISO7816-T0
  USART_MODE_ISO7816T1,       ///< ISO7816-T1
  USART_MODE_IRDA,            ///< IRDA
  USART_MODE_LON,             ///< LON
  USART_MODE_SPI_MASTER,      ///< SPI master
  USART_MODE_SPI_SLAVE,       ///< SPI slave
  USART_MODE_MAX
} USARTMODE;

/// enumerate the parity selection
typedef enum _USARTPARITY
{
  USART_PARITY_EVEN = 0,   ///< even parity
  USART_PARITY_ODD,        ///< odd parity
  USART_PARITY_SPACE,      ///< space
  USART_PARITY_MARK,       ///< mark
  USART_PARITY_NONE,       ///< no parity
  USART_PARITY_MAX,
} USARTPARITY;

/// enumerate the word lengtht
typedef enum _USARTWRDLEN
{
  USART_WRDLEN_5 = 0,
  USART_WRDLEN_6,
  USART_WRDLEN_7,
  USART_WRDLEN_8,
  USART_WRDLEN_MAX
} USARTWRDLEN;

/// enumerate the stop bits
typedef enum _USARTSTOP
{
  USART_STOP_1 = 0,
  USART_STOP_15,
  USART_STOP_2,
  USART_STOP_MAX,
} USARTSTOP;

/// enumerate the clock source
typedef enum _USARTCLKSRC
{
  USART_CLKSRC_MCK = 0,
  USART_CLKSRC_DIV8,
  USART_CLKSRC_PCK,
  USART_CLKSRC_SCK,
  USART_CLKSRC_MAX  
} USARTCLKSRC;

/// enumerate the clock polarity
typedef enum _USARTCLKPOL
{
  USART_CLKPOL_RE = 0,
  USART_CLKPOL_FE,
  USART_CLKPOL_MAX 
} USARTCLKPOL;

/// enumerate the clock phase
typedef enum _USARTCLKPHA
{
  USART_CLKPHA_TE = 0,
  USART_CLKPHA_LE,
  USART_CLKPHA_MAX
} USARTCLKPHA;

/// enumerate the USART events
typedef enum _USARTIRQEVENTS
{
  USART_IRQ_EVENT_NONE = 0,  ///< no event
  USART_IRQ_EVENT_RXCHAR,    ///< receive character event
  USART_IRQ_EVENT_RXERR,     ///< receive error event
  USART_IRQ_EVENT_TXEMP,     ///< transmit empty event
  USART_IRQ_EVENT_TXCMP,     ///< transmit complete event
  USART_IRQ_EVENT_TXENB,     ///< transmit enable event
} USARTIRQEVENTS;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUSARTINTCALLBACK )( USARTIRQEVENTS, U8 );

/// define the structure to define a USART channel
typedef struct _USARTDEF
{
  USARTCHAN         eChan;        ///< USART channel
  U32               uBaudRate;    ///< baud rate
  USARTMODE         eMode;        ///< mode
  USARTWRDLEN       eWrdLen;      ///< wordlenght
  USARTSTOP         eStopBits;    ///< stop bits
  USARTPARITY       eParity;      ///< parity
  USARTCLKSRC       eClkSource;   ///< clock source
  USARTCLKPOL       eClkPolarity; ///< clock polarity for SPI
  USARTCLKPHA       eClkPhase;    ///< clock phase for SPI
  U16               wRxBufSize;   ///< RX buffer size
  U16               wTxBufSize;   ///< RX buffer size
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  PU8               pnRxBuffer;   ///< RX buffer pointer
  PU8               pnTxBuffer;   ///< TX buffer pointer
  #endif // SYSTEMDEFINE_OS_SELECTION
  PVUSARTINTCALLBACK pvCallback;   ///< callback
  struct
  {
    BOOL            bRxChar : 1;    ///< event on receive character
    BOOL            bRxErr  : 1;    ///< event on receive error
    BOOL            bTxEmp  : 1;    ///< event on transmit empty
    BOOL            bTxCmp  : 1;    ///< event on transmit shift register empty
    BOOL            bTxEnb  : 1;    ///< TX enable
  } tEventFlags;
} USARTDEF, *PUSARTDEF;
#define USARTDEF_SIZE        sizeof( USARTDEF )

/**@} EOF Uart_def.h */

#endif  // _USART_DEF_H