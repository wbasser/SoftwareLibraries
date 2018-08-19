/******************************************************************************
 * @file 
 *
 * @brief 
 *
 * This file 
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
#include "I2C/I2c_prm.h"

// Macros and Defines ---------------------------------------------------------

/// define the helper for a blocking device
#define I2C_DEVICE( chan, slvaddr, fastspeed ) \
  { \
    .eChan = chan, \
    .nSlvAddress = slvaddr, \
    .bFastSpeed = fastspeed, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the I2C channels
typedef enum _I2CCHAN
{
  I2C_CHAN_0 = 0,          ///< 
  I2C_CHAN_1,              ///< 
  I2C_CHAN_2,
  I2C_CHAN_MAX
} I2CCHAN;

// structures -----------------------------------------------------------------
/// define the I2C definition structure
typedef struct _I2CDEF
{
  I2CCHAN           eChan;        ///< I2C channel
  U8                nSlvAddress;  ///< slave address
  BOOL              bFastSpeed;   ///< fast speed
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE     sizeof( I2CDEF )

/**@} EOF I2C_def.h */

#endif  // _I2C_DEF_H