/******************************************************************************
 * @file Clock_prm.h
 *
 * @brief clock module parameter declarations 
 *
 * This file provides the parameter for the clock module
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
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
#ifndef _CLOCK_PRM_H
#define _CLOCK_PRM_H

// library includes -----------------------------------------------------------
#include "driverlib/sysctl.h"

// Macros and Defines ---------------------------------------------------------
/// define the divisor rate for the system clock
#define CLOCK_SYSDIV        ( SYSCTL_SYSDIV_4 )

/// define the PLL/Main oscillator
#define CLOCK_PLL_OSC       ( SYSCTL_USE_PLL )
#define CLOCK_OSC_SEL       ( SYSCTL_OSC_MAIN )

/// define the crystal frequency
#define CLOCK_XTAL_FREQ     ( SYSCTL_XTAL_16MHZ )

/**@} EOF Clock_prm.h */

#endif  // _CLOCK_PRM_H