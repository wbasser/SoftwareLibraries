/******************************************************************************
 * @file ModuleAssert_cfg.h
 *
 * @brief module assertion configuration declarations 
 *
 * This file provides the declarations for the module assertion configuration
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
 * \addtogroup ModuleAssert_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MODULEASSERT_CFG_H
#define _MODULEASSERT_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the modules here
typedef enum _MODULEASSERTENUM
{
	// add module enumerations here
	
	// do not remove this entry
	MODULEASSERT_ENUM_MAX
} MODULEASSERTENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void	ModuleAssert_Output( MODULEASSERTENUM eModuleEnum );

/**@} EOF ModuleAssert_cfg.h */

#endif  // _MODULEASSERT_CFG_H