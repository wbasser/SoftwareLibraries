/******************************************************************************
 * @file ModuleAssert_cfg.c
 *
 * @brief  module assert configuration implementation 
 *
 * This file provides the implementation for the module assertion configuration
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
 * \addtogroup ModuleAssert
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ModuleAssert/ModuleAssert_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the output type
#define	MODULEASSERT_OUT_TYPE								( MODULEASSERT_OUT_ONCE )

// enumerations ---------------------------------------------------------------
/// define the output type
typedef enum _MODULEASSERTOUT
{
	MODULEASSERT_OUT_ONCE = 0,			///< one output
	MODULEASSERT_OUT_CONT						///< continuous output
} MODULEASSERTOUT;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ModuleAssert_Output
 *
 * @brief module assert output
 *
 * This function provides the abstracted assertion output function
 *
 * @param[in]   eModuleEnum			module enumeration
 *
 *****************************************************************************/
void ModuleAssert_Output( MODULEASSERTENUM eModuleEnum )
{
}

/**@} EOF ModuleAssert_cfg.c */
