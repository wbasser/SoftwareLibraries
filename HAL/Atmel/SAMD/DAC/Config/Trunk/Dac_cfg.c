/******************************************************************************
 * @file Dac_cfg.c
 *
 * @brief DAC configuration implementation
 *
 * This file provides the implementation of the DAC configuration
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
 * \addtogroup DAC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DAC/Dac_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
const DACDEF g_tDacDef =
{
  .bExtOutEnable = OFF, 
  .bIntOutEnable = OFF,
  .bVPumpDisable = OFF,
  .eRefSelect = DAC_REFSEL_AVCC,
};

/**@} EOF Dac_cfg..c */
