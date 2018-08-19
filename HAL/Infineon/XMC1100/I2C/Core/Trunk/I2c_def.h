/******************************************************************************
 * @file I2C_def.h
 *
 * @brief I2C definitions
 *
 * This file contains the I2C configuration definitions
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
 * \addtogroup I2C
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _I2C_DEF_H
#define _I2C_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a non blocking device
#define I2C_DEVICE_NONBLK( chan, sdamux, sclmux, fastbaud, callback ) \
	{ \
    .eChan = chan, \
    .eSdaMux = sdamux, \
    .eSclMux = sclmux, \
    .bFastBaud = fastbaud, \
		.pvCallBack = ( PVI2CINTCALLBACK )callback \
	}

/// define the helper macro for creating a blocking device
#define I2C_DEVICE_BLKING( chan, sdamux, sclmux, fastbaud, callback ) \
	{ \
    .eChan = chan, \
    .eSdaMux = sdamux, \
    .eSclMux = sclmux, \
    .bFastBaud = fastbaud, \
		.pvCallBack = NULL \
	}

// enumerations ---------------------------------------------------------------
typedef enum _I2CIRQEVENTS
{
  I2C_IRQ_EVENT_RXCHAR,              ///< receive character event
} I2CIRQEVENTS;

/// enumerate the I2C channels
typedef enum _I2CCHAN
{
  I2C_CHAN_SC00 = 0,
  I2C_CHAN_SC01,
  I2C_CHAN_MAX
} I2CCHAN;

/// enumerate the SDA mux
typedef enum _I2CSDAMUX
{
  I2C_SDAMUX_DX0A = 0,    ///< DX0A
  I2C_SDAMUX_DX0B,        ///< DX0B
  I2C_SDAMUX_DX0C,        ///< DX0C
  I2C_SDAMUX_DX0D,        ///< DX0D
  I2C_SDAMUX_DX0E,        ///< DX0E
  I2C_SDAMUX_DX0F,        ///< DX0F
  I2C_SDAMUX_DX0G,        ///< DX0G
} I2CSDAMUX;

/// enumerate the SCL mux
typedef enum _I2CSCLMUX
{
  I2C_SCLMUX_DX1A = 0,    ///< DX1A
  I2C_SCLMUX_DX1B,        ///< DX1B
  I2C_SCLMUX_DX1C,        ///< DX1C
  I2C_SCLMUX_DX1D,        ///< DX1D
  I2C_SCLMUX_DX1E,        ///< DX1E
  I2C_SCLMUX_DX1F,        ///< DX1F
  I2C_SCLMUX_DX1G,        ///< DX1G
} I2CSCLMUX;

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVI2CINTCALLBACK )( I2CIRQEVENTS eEvent );

/// define the I2C definition structure
typedef struct _I2CDEF
{
  I2CCHAN           eChan;          ///< USIC channel
  I2CSDAMUX         eSdaMux;        ///< SDA mux select
  I2CSCLMUX         eSclMux;        ///< SCL mus select
  BOOL              bFastBaud;      ///< fast baud rate ( 0-100K, 1-400K )
  PVI2CINTCALLBACK  pvCallBack;     ///< callback function
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE sizeof( I2CDEF )

/**@} EOF I2C_def.h */

#endif  // _I2C_DEF_H
