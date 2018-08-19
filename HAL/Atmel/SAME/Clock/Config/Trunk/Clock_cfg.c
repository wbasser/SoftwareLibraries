/******************************************************************************
 * @file Clock_cfg.c
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
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
 * \addtogroup Clock
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// define the clock souce definitions
const CODE CLOCKSRCDEF  tClockSrcDef = 
{
  // create an entry with the following macro
  // CLOCKSRCDEFM( slowclksrc, mainclksrc, mainstart, fastsel, xtalfreq )
  
};

/// define the master clock
const CODE CLOCKMASTERCLKDEF tClockMasterDef =
{
  // create an entry with the following macro
  // CCLOCKMASTERCLKDEFM( clksrc, prescale, divide, usbplldiv2 )
};

/// define the PLL definitions
const CODE CLOCKPLLDEF atClockPllDef[ ] =
{
  // create entrys with the following macro
  // CLOCKPLLDEFM( pllsel, divider, multiplier, lockcnt )
  
  // do not remove the entry below
  CLOCKPLLDEFM( CLOCK_PLL_MAX, 0, 0, false )
};

/// define the programmable clocks
const CODE CLOCKPROGCLKDEF   atClockProgDef[ ] =
{
  // create entrys with the following macro
  // CLOCKPROGCLKDEFM( enum, clksrc, prescale )
  
  // do not remove the entry below
  CLOCKPROGCLKDEFM( CLOCK_PRGCLK_ENUM_MAX, 0, 0 )
};

/// define the peripheral clocks
const CODE CLOCKPERIPHCLKDEF atClockPerifDef[ ]
{
  // create entrys with the following macro
  // CLOCKPERIPHCLKDEFM( periphid, clksrc, prescale ) 
  
  // do not remove the entry below
  CLOCKPERIPHCLKDEFM( 0xFF, 0, 0 )
};


/**@} EOF Clock_cfg.c */
