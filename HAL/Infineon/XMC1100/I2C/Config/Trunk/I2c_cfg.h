/******************************************************************************
 * @file I2C_cfg.h
 *
 * @brief I2C configuration declarations
 *
 * This file contains the I2C configuration declarations
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
#ifndef _I2C_CFG_H
#define _I2C_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the I2C enuemrations
typedef enum  _I2CDEVENUM
{
  // add user enumerations below

  // do not remove the entries below
  I2C_DEV_MAX
} I2CDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const I2CDEF atI2cDefs[ ];

/**@} EOF I2c_cfg.h */

#endif  // _I2C_CFG_H
