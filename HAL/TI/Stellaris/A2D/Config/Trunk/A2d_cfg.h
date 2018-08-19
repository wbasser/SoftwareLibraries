/******************************************************************************
 * @file A2d_cfg.h
 *
 * @brief A2D configuration declarations
 *
 * This file contains the A2D configuration declarations
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
 * \addtogroup A2D
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _A2D_CFG_H
#define _A2D_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "A2D/A2d_def.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the A2D enuemrations
typedef enum  _A2DDEVENUM
{
  A2D_DEV_MAX
} A2DDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const A2DDEF atA2dDefs[ ];

/**@} EOF A2d_cfg.h */

#endif  // _A2D_CFG_H