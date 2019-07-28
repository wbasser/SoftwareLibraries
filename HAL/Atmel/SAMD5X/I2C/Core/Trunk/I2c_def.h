/******************************************************************************
 * @file I2C_def.h
 *
 * @brief I2C sercom definitions
 *
 * This file provides the definitions the I2C Sercom implementation
 *
 * @copyright Copyright (c) 2014 CyberIntegration
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
 * \addtogroup 
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _I2C_DEF_H
#define _I2C_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a I2C device
#define I2C_DEVICE_NONBLK( chan, fastspeed, standby, callback ) \
  { \
    .eChan = chan, \
    .bFastSpeed = fastspeed, \
    .bRunStandby = standby, \
    .pvCallBack = ( PVI2CINTCALLBACK )callback \
  }

/// define the helper for a blocking device
#define I2C_DEVICE_BLKING( chan, fastspeed, standby ) \
  { \
    .eChan = chan, \
    .bFastSpeed = fastspeed, \
    .bRunStandby = standby, \
    .pvCallBack = NULL \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the I2C channels
typedef enum _I2CCHAN
{
  I2C_CHAN_0 = 0,          ///< SERCOM channel 0
  I2C_CHAN_1,              ///< SERCOM channel 1
  #ifdef SERCOM2
  I2C_CHAN_2,              ///< SERCOM channel 2
  #endif // SERCOM2
  #ifdef SERCOM3
  I2C_CHAN_3,              ///< SERCOM channel 3
  #endif // SERCOM3
  #ifdef SERCOM4
  I2C_CHAN_4,              ///< SERCOM channel 4
  #endif // SERCOM4
  #ifdef SERCOM5
  I2C_CHAN_5,              ///< SERCOM channel 5
  #endif // SERCOM5
  I2C_CHAN_MAX
} I2CCHAN;

/// enumerate the IRQCB events
typedef enum _I2CIRQCBEVENTS
{
  I2C_IRQCB_EVENT_RXCHAR,   ///< receive character event
} I2CIRQCBEVENTS;

// structures -----------------------------------------------------------------
/// define the interrupt callback type
typedef   void ( *PVI2CINTCALLBACK )( I2CIRQCBEVENTS eEvent );

/// define the I2C definition structure
typedef struct _I2CDEF
{
  I2CCHAN           eChan;        ///< I2C channel
  BOOL              bFastSpeed;   ///< fast speed
  BOOL              bRunStandby;  ///< run in standby
  PVI2CINTCALLBACK  pvCallBack;   ///< callback function
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE     sizeof( I2CDEF )

/**@} EOF I2C_def.h */

#endif  // _I2C_DEF_H