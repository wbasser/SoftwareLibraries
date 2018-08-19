/******************************************************************************
 * @file Adc.h
 *
 * @brief ADC declarations
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
 * \addtogroup ADC
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADC_H
#define _ADC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ADC/Adc_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the ADC max counts
#define ADC_MAX_COUNTS            ( 4095 )

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _ADCERRS
{
  ADC_ERR_NONE = 0,       ///< no error
  ADC_ERR_BLOCKING,       ///< blocking operation in progress
  ADC_ERR_ILLENUM = 0x80, ///< illegal enumeration
  ADC_ERR_ILLPRM,         ///< illegal parameter
  ADC_ERR_ILLMODE,        ///< illegal mode
} ADCERRS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    Adc_Initialize( void );
extern  void    Adc_Close( void );
extern  ADCERRS Adc_ConvertChannel( ADCENUM eAdcChan, PU16 pwResult );
extern  ADCERRS Adc_ControlEventChannel( ADCENUM eAdcChan, BOOL bState );

/**@} EOF Adc.h */

#endif  // _ADC_H