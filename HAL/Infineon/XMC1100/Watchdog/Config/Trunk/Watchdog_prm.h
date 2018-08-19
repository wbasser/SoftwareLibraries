/******************************************************************************
 * @file Watchdog_prm.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Watchdog
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _WATCHDOG_PRM_H
#define _WATCHDOG_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the low and high boundry values in milliseconds
#define WATCHDOG_LO_BOUNDARY      ( 100 )
#define WATCHDOG_HI_BOUNDARY      ( 1000 )

/**@} EOF Watchdog_prm.h */

#endif  // _WATCHDOG_PRM_H