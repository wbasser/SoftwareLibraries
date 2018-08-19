/******************************************************************************
 * @file DacLtc2655.h
 *
 * @brief DAC LTC 2655 driver implementation declarations
 *
 * This file provides the interface to the DAC LTC2655 
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DACLTC2655
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DACLTC2655_H
#define _DACLTC2655_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DacLtc2655/DacLtc2655_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the channels
typedef enum _DACLTC2655CHAN
{
  DACLTC2655_CHAN_0  = 0,
  DACLTC2655_CHAN_1,
  DACLTC2655_CHAN_2,
  DACLTC2655_CHAN_3,
  DACLTC2655_CHAN_MAX,
  DACLTC2655_CHAN_ALL
} DACLTC2655CHAN;

/// enumerate the DAC errors
typedef enum _DACLTC2655ERR
{
  DACLTC2655_ERR_NONE = 0,
  DACLTC2655_ERR_ILLCHAN = 0xE0,
  DACLTC2655_ERR_ILLDEV
} DACLTC2655ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          DacLtc2655_Initialize( void );
extern  DACLTC2655ERR DacLtc2655_SetOutputDirect( DACLTC2655DEVENUM eDevice, DACLTC2655CHAN eChan, U16 wValue );
extern  DACLTC2655ERR DacLtc2655_SetOutputPercent( DACLTC2655DEVENUM eDevice, DACLTC2655CHAN eChan, U16 wPercent );
extern  DACLTC2655ERR DacLtc2655_PowerControl( DACLTC2655DEVENUM eDevice, BOOL bState );

/**@} EOF DacLtc2655.h */

#endif  // _DACLTC2655_H