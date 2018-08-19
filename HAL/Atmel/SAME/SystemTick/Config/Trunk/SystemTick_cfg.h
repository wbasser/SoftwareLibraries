/******************************************************************************
 * @file SystemTick_cfg.h
 *
 * @brief configuration file declarations
 *
 * This file declares the initialization structures for system tick
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
#ifndef _SYSTEMTICK_CFG_H
#define _SYSTEMTICK_CFG_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the system tick functions
typedef void ( *PVSYSTEMTICKFUNC )( void );

// global parameter declarations -----------------------------------------------
/// declare the system tick structure
extern  const CODE PVSYSTEMTICKFUNC apvSystemTickFunctions[ ];

/**@} EOF Micro_cfg.c */

#endif // _MICRO_CFG_H
