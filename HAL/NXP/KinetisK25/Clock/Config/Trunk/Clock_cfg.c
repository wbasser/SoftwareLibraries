/******************************************************************************
 * @file Clock_def.c
 *
 * @brief CLock configuration implementation 
 *
 * This file provides the implementation for the clock configuration module
 *
 * @copyright Copyright (c) 2012 Guardhat
 * This document contains proprietary data of Guardhat, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Guardhat, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Guardhat, LLC.
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
const CLOCKDEF  tClockDef =
{
  // create the entry with the following macro
  // CLOCKFEIDEFM( filldiv, outdiv1, outdiv4, intrunonstop ) 
  // CLOCKPEEDEFM( oscsel, osccap, filldiv, outdiv1, outdiv4 )
  // CLOCKBPLEDEFM( oscsel, osccap, outdiv1, outdiv4 )
  // CLOCKIHSDEFM( outdiv1, outdiv4, intrunonstop )

};

/**@} EOF Clock_def.c */
