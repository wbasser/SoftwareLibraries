/******************************************************************************
 * @file SerialComm_def.h
 *
 * @brief SERIALCOMM definition file
 *
 * This file the enumerations and structures used to define a SERIALCOMM channel
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
#ifndef _SERIALCOMM_DEF_H
#define _SERIALCOMM_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating storage
/// define the helper macro to create a ASYNC SERIALCOMM
#define SERIALCOMM_DEF( chan, baud, parity, len, stop, rxbuf, callback ) \
 { \
  .pszChanName = chan, \
  .eBaudRate = baud, \
  .eParity = parity, \
  .eWordLen = len, \
  .eStopBits = stop, \
  .wRxBufSize = rxbuf, \
  .pvCallback = ( PVSERIALCOMMIRQCALLBACK )callback \
 }
 
// enumerations ---------------------------------------------------------------
/// enumerate the baud rates
typedef enum _SERIALCOMMBAUDRATE
{
  SERIALCOMM_BAUDRATE_300 = 0,
  SERIALCOMM_BAUDRATE_600,
  SERIALCOMM_BAUDRATE_1200,
  SERIALCOMM_BAUDRATE_1800,
  SERIALCOMM_BAUDRATE_2400,
  SERIALCOMM_BAUDRATE_4800,
  SERIALCOMM_BAUDRATE_9600,
  SERIALCOMM_BAUDRATE_19200,
  SERIALCOMM_BAUDRATE_38400,
  SERIALCOMM_BAUDRATE_57600,
  SERIALCOMM_BAUDRATE_115200,
  SERIALCOMM_BAUDRATE_230400,
  SERIALCOMM_BAUDRATE_MAX
} SERIALCOMMBAUDRATE;

/// enumerate the word lengths
typedef enum _SERIALCOMMWLEN
{
  SERIALCOMM_WLEN_5 = 0,
  SERIALCOMM_WLEN_6,
  SERIALCOMM_WLEN_7,
  SERIALCOMM_WLEN_8,
  SERIALCOMM_WLEN_MAX
} SERIALCOMMWLEN;

/// enumerate the stop bits
typedef enum _SERIALCOMMSTOP
{
  SERIALCOMM_STOP_1 = 0,
  SERIALCOMM_STOP_2,
  SERIALCOMM_STOP_MAX
} SERIALCOMMSTOP;

/// enumerate the parity
typedef enum _SERIALCOMMPARITY
{
  SERIALCOMM_PARITY_NONE = 0,
  SERIALCOMM_PARITY_EVEN,
  SERIALCOMM_PARITY_ODD,
  SERIALCOMM_PARITY_MAX
} SERIALCOMMPARITY;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVSERIALCOMMIRQCALLBACK )( U8 );

/// define the SERIALCOMM control structure
typedef struct _SERIALCOMMDEF
{
  PC8                     pszChanName;  ///< channel name
  SERIALCOMMBAUDRATE      eBaudRate;    ///< baud rate
  SERIALCOMMWLEN          eWordLen;     ///< word length
  SERIALCOMMPARITY        eParity;      ///< parity
  SERIALCOMMSTOP          eStopBits;    ///< stop bits
  U16                     wRxBufSize;   ///< receive buffer size
  PVSERIALCOMMIRQCALLBACK pvCallback;   ///< event callback handler
} SERIALCOMMDEF, *PSERIALCOMMDEF;
#define SERIALCOMMDEF_SIZE    sizeof( SERIALCOMMDEF )

/**@} EOF SerialComm_def.h */

#endif  // _SERIALCOMM_DEF_H