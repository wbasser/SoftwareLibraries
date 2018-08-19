/******************************************************************************
 * @file Adc_cfg.c
 *
 * @brief ADC configuration implementation
 *
 * This file provides the implementation for the configuraiton of the ADC
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
 * \addtogroup ADC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ADC/Adc_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
const ADCDEF  atAdcDefs[ ADC_ENUM_MAX ] =
{
  // add ADC definitions here using one of the below macros
  // ADCDEF_NORMSING_BLKING( inpchan, refsel, prescale, gain )
  // ADCDEF_NORMSING_NONBLK( inpchan, refsel, prescale, gain, callback )
};

/**@} EOF Adc_cfg.c */
