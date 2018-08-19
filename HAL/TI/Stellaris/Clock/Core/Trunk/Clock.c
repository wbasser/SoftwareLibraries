/******************************************************************************
 * @file Clock.c
 *
 * @brief Clock module implementation
 *
 * This file provides the implementation for the clock module
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "Clock/Clock.h"
#include "Clock/Clock_prm.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Clock_Initialize
 *
 * @brief clock initialization
 *
 * This function will initialize the clock
 *
 *****************************************************************************/
void Clock_Initialize( void )
{
  // initialize the system
  SysCtlClockSet( CLOCK_SYSDIV | CLOCK_PLL_OSC | CLOCK_OSC_SEL | CLOCK_XTAL_FREQ );
}
 
/**@} EOF .c */
