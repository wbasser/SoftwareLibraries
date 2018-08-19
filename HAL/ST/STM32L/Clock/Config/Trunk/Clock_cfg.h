/******************************************************************************
 * @file Clock_cfg.h
 *
 * @brief clock configuration declarations
 *
 * This file provides the declarations for the clock configuration
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_CFG_H
#define _CLOCK_CFG_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock_def.h"

// Macros and Defines ---------------------------------------------------------
/// define the external high speed clock frequency
#define CLK_HSE_FREQ                        ( 0 )

// enumerations ---------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE CLKSYSDEF  tClkSysDef;
extern  const CODE CLKAHBDEF  tClkAhbDef;
extern  const CODE CLKAPB1DEF tClkApb1Def;
extern  const CODE CLKAPB2DEF tClkApb2Def;

/**@} EOF Clock_cfg.h */

#endif  // _CLOCK_CFG_H