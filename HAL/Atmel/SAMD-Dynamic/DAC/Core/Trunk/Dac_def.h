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

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// enumerations ---------------------------------------------------------------
/// enumerate the reference select
typedef enum _DACREFSEL
{
  DAC_REFSEL_INT1V = 0,     ///< internal 1V reference
  DAC_REFSEL_AVCC,          ///< VCC analog
  DAC_REFSEL_VREFP,         ///< external reference
  DAC_REFSEL_MAX      
} DACREFSEL;

// structures -----------------------------------------------------------------
/// define the overall structure
typedef struct _DACDEF
{
  BOOL      bExtOutEnable;    ///< external output enable
  BOOL      bIntOutEnable;    ///< internal output enable
  BOOL      bVPumpDisable;    ///< voltage pump disable
  DACREFSEL eRefSelect;       ///< refernce select
} DACDEF, *PDACMDEF;
#define DACDEF_SIZE      sizeof( DACDEF )

/**@} EOF Dac_def.h */

#endif  // _DAC_DEF_H