/******************************************************************************
 * @file PowerManager_cfg.c
 *
 * @brief Power Manager configuration implementation
 *
 * This file provides the implementation of the power manager configuration
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
 * \addtogroup PowerManager.
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PowerManager/PowerManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
const PMDEF g_tPmDef =
{
  // AHB bits
  .tPmAhb.bit =
  {
    .HPB0_      = ON,
    .HPB1_      = ON,
    .HPB2_      = ON,
    .DSU_       = ON,
    .NVMCTRL_   = ON,
    .DMAC_      = ON,
  #ifndef __SAM_D10_SUBFAMILY
    .USB_       = OFF,
  #endif // __SAM_D10_SUBFAMILY
  },

  // APBA bits
  .tPmApbA.bit =
  {
    .PAC0_      = ON,
    .PM_        = ON,
    .SYSCTRL_   = ON,
    .GCLK_      = ON,
    .WDT_       = ON,
    .RTC_       = ON,
    .EIC_       = ON,
  },

  // APBB bits
  .tPmApbB.bit =
  {
    .PAC1_      = ON,
    .DSU_       = ON,
    .NVMCTRL_   = ON,
    .PORT_      = ON,
    .DMAC_      = ON,
  #ifndef __SAM_D10_SUBFAMILY
    .USB_       = OFF,
    .HMATRIX_   = ON,
  #endif // __SAM_D10_SUBFAMILY
  },

  // APBC bits
  .tPmApbC.bit =
  {
  #ifdef __SAM_D10_SUBFAMILY
    .PAC2_      = OFF,
    .EVSYS_     = OFF,
    .SERCOM0_   = OFF,
    .SERCOM1_   = OFF,
    .SERCOM2_   = OFF,
    .TCC0_      = OFF,
    .TC1_       = OFF,
    .TC2_       = OFF,
    .ADC_       = OFF,
    .AC_        = OFF,
    .DAC_       = OFF,
    .PTC_       = OFF,
  #else
    .PAC2_      = OFF,
    .EVSYS_     = OFF,
    .SERCOM0_   = OFF,
    .SERCOM1_   = OFF,
    .SERCOM2_   = OFF,
    .SERCOM3_   = OFF,
    .SERCOM4_   = OFF,
    .SERCOM5_   = OFF,
    .TCC0_      = OFF,
    .TCC1_      = OFF,
    .TCC2_      = OFF,
    .TC3_       = OFF,
    .TC4_       = OFF,
    .TC5_       = OFF,
    .TC6_       = OFF,
    .TC7_       = OFF,
    .ADC_       = OFF,
    .AC_        = OFF,
    .DAC_       = OFF,
    .PTC_       = OFF,
    .I2S_       = OFF,
  #endif // __SAM_D10_SUBFAMILY
  },
};

/**@} EOF PowerManager_cfg..c */
