/******************************************************************************
 * @file Clock.h
 *
 * @brief clock module definitions
 *
 * This file provides the clock initializatin and up/down scaling
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "Clock/Clock_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  Clock_Initialize( void );
extern  void  Clock_Close( void );
extern  U32   Clock_GetFreq( void );
extern  U32   Clock_GetAPB1Frequency( void );
extern  U32   Clock_GetAPB2Frequency( void );

/**@} EOF Clock.h */

#endif  // _CLOCK_H
