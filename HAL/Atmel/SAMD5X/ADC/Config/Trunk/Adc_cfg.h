/******************************************************************************
 * @file Adc_cfg.h
 *
 * @brief ADC configuration file declarations 
 *
 * This file provides the declarations for the configuration of the ADC
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
 
// ensure only one instantiation
#ifndef _ADC_CFG_H
#define _ADC_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ADC/Adc_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the macro to enable the bandgap and temperature sensors
#define ADC_ENABLE_1V_BANDGAP                     ( 0 )
#define ADC_ENABLE_TEMP_SENSOR                    ( 0 )

// enumerations ---------------------------------------------------------------
/// enumerate the ADC channels
typedef enum _ADCENUM
{
  // add your enumerations here
 
  // do not remove the below entries
  ADC_ENUM_MAX,       ///< maximum number of ADC defs
  ADC_ENUM_ILLEGAL
} ADCENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const ADCDEF  atAdcDefs[ ];

/**@} EOF Adc_cfg.h */

#endif  // _ADC_CFG_H