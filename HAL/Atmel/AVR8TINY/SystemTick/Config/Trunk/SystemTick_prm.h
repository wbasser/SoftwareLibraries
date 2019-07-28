/******************************************************************************
 * @file SystemTick_prm.h
 *
 * @brief system tick parameter declarations
 *
 * This file declares the parameters for the system tick
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
 * $Log: $
 * 
 *
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _SYSTEMTICK_PRM_H
#define _SYSTEMTICK_PRM_H


// Macros and Defines ---------------------------------------------------------
/// define the system tick time in HZ
#define SYSTEMTICK_RATE_1000HZ        ( 0 )
#define`SYSTEMTICK_RATE_500HZ         ( 1 )
#define SYSTEMTICK_RATE_200HZ         ( 2 )
#define SYSTEMTICK_RATE_100HZ         ( 3 )
#define SYSTEMTICK_RATE_40HZ          ( 4 )
#define SYSTEMTICK_RATE_20HZ          ( 5 )
#define SYSTEMTICK_RATE_10HZ          ( 6 )

/// define the selected system tick rate
#define SYSTEMTICK_RATE_SELECTED      ( SYSTEMTICK_RATE_1MS )

/// define the tick timers
#define SYSTEMTICK_TIMER_0            ( 0 )
#define SYSTEMTICK_TIMER_1            ( 1 )

/// define the selected tick timer
#define SYSTEMTICK_TIMER_SELECTED     ( SYSTEMTICK_TIMER_0 )

/**@} EOF SystemTick_prm.h */

#endif  // _SYSTEMTICK_PRM_H