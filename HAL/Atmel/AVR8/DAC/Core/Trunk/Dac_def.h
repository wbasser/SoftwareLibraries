/******************************************************************************
 * @file Dac_def
 *
 * @brief DAC Definition file
 *
 * This file provides the definitions for the DAC
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
 
// ensure only one instantiation
#ifndef _DAC_DEF_H
#define _DAC_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DAC/Dac_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// enumerations ---------------------------------------------------------------
#if ( DAC_CONTROL_REF == 1 )
/// enumerate the reference select
typedef enum _DACREFSEL
{
  DAC_REFSEL_INT1V = 0,     ///< internal 1V reference
  DAC_REFSEL_AVCC,          ///< VCC analog
  DAC_REFSEL_VREFP,         ///< external reference
  DAC_REFSEL_MAX      
} DACREFSEL;
#endif // DAC_CONTROL_REF

/// enumerate the trigger select
typedef enum _DACTRIGSEL
{
  DAC_TRIGSEL_NONE= 0,      ///< none
  DAC_TRIGSEL_ANACMP0 = 8,  ///< analog comparator 0
  DAC_TRIGSEL_ANACMP1,      ///< analog comparator 1
  DAC_TRIGSEL_EXTIRQ0,      ///< external IRQ 0
  DAC_TRIGSEL_TC0CMP,       ///< timer/counter 0 compare match
  DAC_TRIGSEL_TC0OVF,       ///< timer/counter 0 overflow
  DAC_TRIGSEL_TC1CMPB,      ///< timer/counter 1B compare match
  DAC_TRIGSEL_TC1OVF,       ///< timer/counter 1 overflow
  DAC_TRIGSEL_TC1CAP,       ///< timer/counter 1 capture
} DACTRIGSEL;

// structures -----------------------------------------------------------------
/// define the overall structure
typedef struct _DACDEF
{
  BOOL        bExtOutEnable;    ///< external output enable
  BOOL        bLeftAdjust;      ///< left adjust DAC value
  DACTRIGSEL  eTrgSelect;       ///< trigger select
  #if ( DAC_CONTROL_REF == 1 )
  DACREFSEL   eRefSelect;       ///< refernce select
  #endif // DAC_CONTROL_REF
} DACDEF, *PDACMDEF;
#define DACDEF_SIZE      sizeof( DACDEF )

/**@} EOF Dac_def.h */

#endif  // _DAC_DEF_H