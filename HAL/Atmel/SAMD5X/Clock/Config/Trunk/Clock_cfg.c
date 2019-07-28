/******************************************************************************
 * @file Clock_cfg.c
 *
 * @brief 
 *
 * This file 
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
/// main clock defainition
const CODE CLOCKMAINDEF g_tClockMainDef =
{
  // fill in this structure using the below macro
  // CLOCKDEFMAINM( cpu, hs, rtcsrc ) 
};

// oscillators definitions
const CODE CLOCKOSCDEF  g_atClockOscDefs[ ] =
{
  // create entries using the below macros
  // CLOKXOSC32DEFM( ondemand, runstby, wrtlock, enb1kout, enb32kout, xtalenb, cfdpre, cfdswb, cfdenb, cfdevn, startup, gain )
  // CLOCKXOSCDEFM( oscsel, ondemand, runstby, xtalenb, cfdswb, cfdenb, loopenb, lbgenb, cfdpre, startdly, imult, iref )
  // CLOCKDPLLDEFM( oscsel, ondemand, runstby, filtenb, fastwkenb, lckbypassenb, loopfrac, loopratio, divider, filtsel, lcktim, refclk )
  // CLOCKDFLLDEFM( on demand, runstby, waitlock, crslckbyp, quicklkdsb, chillcycdsb, usbrecv, loselock, calaftlock, clsenb, multratio, coarse, fine, crsstep, finestep, multi )
  
  // do not remove this entry
  CLOCKOSCDEF_EOT( )
};

/// clock generators definitions
const CODE CLOCKGENDEF  g_atClockGenDefs[ ] =
{
  // fill in this structure using the below macro
  // CLOCKGENDEFM( clkgenid, clksrc, divselect, divisor, standby, outenb, outoffenb, dutycyle )
  
  // do not remove his entry
  CLOCKGENDEF_EOT( )
};

/// clock multiplexers definitions
const CODE CLOCKMUXDEF  g_atClockMuxDefs[ ] =
{
  // fill in this structure using the below macro
  // CLOCKDEFMUXM( id, clkgenid, writelock )

  // do not remove this entry
  CLOCKDEFMUX_EOT( )
};

/// peripheral clock
const CODE CLOCKPERIPHDEF g_atClockPeriphs[ ] =
{
  // fill in this structure using the below macro
g  // CLOCKPERIPHDEFM( enum, state )
  
  // do not remove this entry
  CLOCKPERIPHDEF_EOT( ) 
};


/**@} EOF Clock_cfg.c */
