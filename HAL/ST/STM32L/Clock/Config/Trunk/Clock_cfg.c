/******************************************************************************
 * @file Clock_cfg.c
 *
 * @brief clock configuration implementation
 *
 * This file provides the implementation for the clock configuration
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

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// global constant parameter initializations -----------------------------------------
/// system clock definition
const CODE CLKSYSDEF  tClkSysDef =
{
  // use one of the below listed helper macros to define the system clock
  // CLKSYSMSIDEFM( msiclkrange, ahbpre, apb1pre, apb2pre, mcosrc, mcopre )
  // CLKSYSMSIPLLDEFM( msiclkrange, ahbpre, apb1pre, apb2pre, mcosrc, mcopre, pllrpre, plldiv, pllmult )

};

/// AHB clock enable definition
const CODE CLKAHBDEF  tClkAhbDef =
{
  // use the below listed helper macro to define the AHB 
  // CLKAHBENBDEFM( dma1, dma2, flash, crc, touch, gpioa, gpiob, gpioc, gpiod, gpioe, gpiof, gpoih, adc, rng, fmc, qpsi )
};

/// APB1 clock enable definition
const CODE CLKAPB1DEF tClkApb1Def =
{
  // use the below listed helper macro to define the APB1
  // CLKAPB1ENBDEFM( tim2, tim3, tim4, tim5, tim6, tim7, wwdgen, spi2, spi3, usart2, usart3, uart4, uart5, i2c1, i2c2, i2c3, can, pwr, dac, opamp, lptim1, lpuart, swp, lptim2 )
};

/// APB2 clock enable definition
const CODE CLKAPB2DEF tClkApb2Def =
{
  // use the below listed helper macro to define the APB2
  // CLKAPB2ENBDEFM( syscfg, fw, sdmmc, tim1, spi1, tim8, usart1, tim5, tim6, tim7, sai1, sai2, dfsdm )
};

/**@} EOF Clock_cfg.c */
