/******************************************************************************
 * @file 
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2014 Cyber Integration
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
#define I2C_DEVICE_NONBLK( chan, slvaddr, fastspeed, callback ) \
  { \
    .eChan = chan, \
    .nSlvAddress = slvaddr, \
    .bFastSpeed = fastspeed, \
    .pvCallBack = ( PVI2CINTCALLBACK )callback \
  }

/// define the helper for a blocking device
#define I2C_DEVICE_BLKING( chan, slvaddr, fastspeed ) \
  { \
    .eChan = chan, \
    .nSlvAddress = slvaddr, \
    .bFastSpeed = fastspeed, \
    .pvCallBack = NULL \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the I2C channels
typedef enum _I2CCHAN
{
  I2C_CHAN_0 = 0,          ///< 
  I2C_CHAN_1,              ///< 
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
  U8                nSlvAddress;  ///< slave address
  BOOL              bFastSpeed;   ///< fast speed
  PVI2CINTCALLBACK  pvCallBack;   ///< callback function
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE     sizeof( I2CDEF )

/**@} EOF I2C_def.h */

#endif  // _I2C_DEF_H