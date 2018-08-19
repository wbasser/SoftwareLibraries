/******************************************************************************
 * @file Sim_cfg.c
 *
 * @brief SIM configuration implementaion  
 *
 * This file provides the implementation for the configuration of the System
 * Integration Module (SIM)
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
 * $Rev: $
 * 
 *
 * \addtogroup Sim
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SIM/Sim_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// clock gating register 4 definition
const SIMDEFCR4 tSimDefCR4 =
{
  // instantiate with the below macro
  // SIMDEFCR4M( i2c0, i2c1, uart0, uart1, uart2, usb, comp, spi0, spi1 )

};

/// clock gating register 5 definition
const SIMDEFCR5 tSimDefCR5 =
{
  // instantiate with the below macro
  // SIMDEFCR5M( lptmr, tsi, porta, portb, portc, portd, porte )

};

/// clock gating register 6 definition
const SIMDEFCR6 tSimDefCR6 =
{
  // instantiate with the below macro
  // SIMDEFCR6M( lftf, dmamux, pit, tpm0, tpm1, tpm2, adc0, rtc, dac0 )

};

/// clock gating register 7 definition
const SIMDEFCR7 tSimDefCR7 =
{
  // instantiate with the below macro
  // SIMDEFCR7M( dma )

};

/**@} EOF Sim_cfg.c */
