/******************************************************************************
 * @file Sim_cfg.h
 *
 * @brief SIM configuration definitions 
 *
 * This file provides the configuration definitions for the System Integration
 * MOdule (SIM)
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
 * \addtogroup Sim
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SIM_CFG_H
#define _SIM_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SIM/Sim_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const SIMDEFCR4 tSimDefCR4;
extern  const SIMDEFCR5 tSimDefCR5;
extern  const SIMDEFCR6 tSimDefCR6;
extern  const SIMDEFCR7 tSimDefCR7;

/**@} EOF Sim_cfg.h */

#endif  // _SIM_CFG_H