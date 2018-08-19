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
#define CLOCK_EXTXTAL_FREQ          ( 12000000l )

// enumerations ---------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE  CLOCKSRCDEF       tClockSrcDef;
extern  const CODE  CLOCKMASTERCLKDEF tClockMasterDef;
extern  const CODE  CLOCKPLLDEF       atClockPllDef[ ];
extern  const CODE  CLOCKPROGCLKDEF   atClockProgDef[ ];
extern  const CODE  CLOCKPERIPHCLKDEF atClockPerifDef[ ];

/**@} EOF Clock_cfg.h */

#endif  // _CLOCK_CFG_H