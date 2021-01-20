/******************************************************************************
 * @file Dac.h
 *
 * @brief DAC declarations
 *
 * This file provides the declarations for the DAC module
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
#ifndef _DAC_H
#define _DAC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

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
  GPIOPORT      eDevPort;         ///< device port
  GPIOFUNCMUX   eDevMux;          ///< device mux
  U8            nDacPin;          ///< ain pin
  BOOL          bExtOutEnable;    ///< external output enable
  BOOL          bIntOutEnable;    ///< internal output enable
  BOOL          bVPumpDisable;    ///< voltage pump disable
  DACREFSEL     eRefSelect;       ///< refernce select
} DACDEF, *PDACDEF;
#define DACDEF_SIZE      sizeof( DACDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Dac_Initialize( PDACDEF ptDef );
extern  BOOL  Dac_SetOutput( U16 wValue );
extern  void  Dac_SetPercentOutput( U16 wPercent );

/**@} EOF Dac.h */

#endif  // _DAC_H