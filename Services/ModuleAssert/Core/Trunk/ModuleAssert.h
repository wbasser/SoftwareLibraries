/******************************************************************************
 * @file ModuleAssert.h	
 *
 * @brief module assertion declaration file	
 *
 * This file provides the declarations for the module assertion
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
 
// ensure only one instantiation
#ifndef _MODULEASSERT_H
#define _MODULEASSERT_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ModuleAssert/ModuleAssert_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the module assert macro
#define	MODULEASSERT( enum, cmp1, cmp2 ) \
	ModuleAssert_Execute( enum, cmp1, cmp2 )
	
// enumerations ---------------------------------------------------------------
/// enumerate the test types
typedef enum _MODULEASSERTEST
{
	MODULEASSERT_TEST_EQ = 0,
	MODULEASSERT_TEST_NE,
	MODULEASSERT_TEST_MAX
} MODULEASSERTEST;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void	ModuleAssert_Execute( MODULEASSERTENUM eModuleEnum, MODULEASSERTEST eTestEnum, PVOID pvCompare1, PVOID pvCompare2 );

/**@} EOF ModuleAssert.h */

#endif  // _MODULEASSERT_H