/******************************************************************************
 * @file Clock_prm.h
 *
 * @brief clock module parameters 
 *
 * This file provides the parameters that control the clock generation
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
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_PRM_H
#define _CLOCK_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the clock integral divider
#define CLOCK_IDIV_VALUE      1

/// define the fractional divider
#define CLOCK_FDIV_VALUE      0

/// define the PLCK == MCLK * 2 enable
#define CLOCK_PCLKX2_ENABLE   0

/// define the clock frequency
#define CLOCK_FREQ            32000000l

/**@} EOF Clock_prm.h */

#endif  // _CLOCK_PRM_H