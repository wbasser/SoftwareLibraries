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
/// define the number of flash wait states
#define FLASH_WAIT_STATES             ( 1 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE CLOCKOSCDEF  atClockOscDefs[ ] =
{
  // create entries using the below macros
  // CLOCKOSCDEFXOSC( ondemand, standby, exttype, extfreq, startup, autogain )
  // CLOCKOSCDEF32KEXT( ondemand, standby, out1kenb, out32kenb, exttype, extfreq, startup, autoamp )
  // CLOCKOSCDEF32KINT( ondemand, standby, out1kenb, out32kenb, id, outenab, outoffenb, impdtycyc, divselect, divisor ) 
  // CLOCKOSCDEF8MHZ( ondemand, standby, outdiv )
  // CLOCKOSCDEFDFLLOPEN( mult, coarse, fine, coarsestep, finestep, ondemand, usbrecover, quicklock, tracklock, lockonwakeup, chillcycle )
  // CLOCKOSCDEFDFLLCLOSED( ondemand, usbrecover, quicklock, tracklock, lockonwakeup, chillcycle, refid, refsrc, refoutenab, refoutoffenb, refimpdtycyc, refdivselect, refdivisor, outid, outoutenab, outoutoffenb, outimpdtycyc, outdivselect, outdivisor ) 

  // do not remove this entry
  CLOCKOSCDEF_EOT( )
};


const CODE CLOCKGENDEF  atClockGenDefs[ ] =
{
  // fill in this structure using the below macro
  // CLOCKGENDEF( clkgenid, clksrc, divselect, divisor, standby, outenb, outoffenb, dutycyle )
  
  // do not remove his entry
  CLOCKGENDEF_EOT( )
};
const CODE CLOCKMUXDEF  atClockMuxDefs[ ] =
{
  // fill in this structure using the below macro
  // CLOCKDEFMUX( id, clkgenid, writelock )
  
  // do not remove this entry
  CLOCKDEFMUX_EOT( )
};

const CODE CLOCKMAINDEF tClockMainDef =
{
  // fill in this structure using the below macro
  // CLOCKMAINDEF( cpu, apba, apba, flash )
  CLOCKDEFMAIN( CLOCK_MAINDIV_1, CLOCK_MAINDIV_1, CLOCK_MAINDIV_1, FLASH_WAIT_STATES )
};

/**@} EOF Clock_cfg.c */
