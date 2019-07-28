/******************************************************************************
 * @file SleepManager.h
 *
 * @brief sleep manager declarations
 *
 * This file provides the declarations for the sleep manager
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
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
 * \addtogroup SleepManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SLEEPMANAGER_H
#define _SLEEPMANAGER_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// define the sleep states
typedef enum _SLEEPMNGRSLEEPMODE
{
  SLEEPMNGR_SLEEP_MODE_IDLE_0 = 0,
  SLEEPMNGR_SLEEP_MODE_IDLE_1,
  SLEEPMNGR_SLEEP_MODE_IDLE_2,
  SLEEPMNGR_SLEEP_MODE_STDBY,
  SLEEPMNGR_SLEEP_MODE_MAX,
} SLEEPMNGRSLEEPMODE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  SleepManager_Initialize( void );
extern  void  SleepManager_EnterSleepState( SLEEPMNGRSLEEPMODE eSleepMode );

/**@} EOF SleepManager.h */

#endif  // _x_H