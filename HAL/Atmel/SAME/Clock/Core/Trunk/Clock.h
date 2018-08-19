/******************************************************************************
 * @file Clock.h
 *
 * @brief clock module definitions
 *
 * This file provides the clock initializatin and up/down scaling
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_H
#define _CLOCK_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  U32 g_ClockMasterFrequency;

// global function prototypes --------------------------------------------------
extern  void  Clock_Initialize( void );
extern  void  Clock_Close( void );
extern  U32   Clock_GetCpuFreq( void );
extern  U32   Clock_GetMastClockFreq( void );
extern  U32   Clock_GetProgClockFreq( CLOCKPRGCLKENUM eClkEnum );

/**@} EOF Clock.h */

#endif  // _CLOCK_H
