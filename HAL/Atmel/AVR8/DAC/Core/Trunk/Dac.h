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
#include "DAC/Dac_cfg.h" 

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Dac_Initialize( void );
extern  BOOL  Dac_SetOutput( U16 wValue );
extern  void  Dac_SetPercentOutput( U16 wPercent );

/**@} EOF Dac.h */

#endif  // _DAC_H