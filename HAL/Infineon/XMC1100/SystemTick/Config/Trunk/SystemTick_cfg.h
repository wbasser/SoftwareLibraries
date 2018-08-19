/******************************************************************************
 * @file SystemTick_cfg.h
 *
 * @brief configuration file declarations
 *
 * This file declares the initialization structures for system tick
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
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _SYSTEMTICK_CFG_H
#define _SYSTEMTICK_CFG_H

// library includes -----------------------------------------------------------
#include "Micro/Types.h"

// local includes -------------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the system tick functions
typedef void ( *PVSYSTEMTICKFUNC )( void );

// global parameter declarations -----------------------------------------------
/// declare the system tick structure
extern  const CODE PVSYSTEMTICKFUNC apvSystemTickFunctions[ ];

/**@} EOF Micro_cfg.c */

#endif // _MICRO_CFG_H
