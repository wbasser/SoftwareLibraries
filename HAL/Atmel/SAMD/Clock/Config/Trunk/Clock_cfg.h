/******************************************************************************
 * @file Clock_cfg.h
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup Clock
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CLOCK_CFG_H
#define _CLOCK_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Clock/Clock_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE CLOCKOSCDEF  g_atClockOscDefs[ ];
extern  const CODE CLOCKMAINDEF g_tClockMainDef;
extern  const CODE CLOCKGENDEF  g_atClockGenDefs[ ];
extern  const CODE CLOCKMUXDEF  g_atClockMuxDefs[ ];

/**@} EOF Clock_cfg.h */

#endif  // _CLOCK_CFG_H