/******************************************************************************
 * @file ModuleAssert.c
 *
 * @brief module assertion implementation 
 *
 * This file provides the implementation for the module assert module
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
#include "ModuleAssert/ModuleAssert.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ModuleAssert_Execute
 *
 * @brief module assertion execution
 *
 * This function will compare the two values and if they are not equal, the 
 * assertion module will stop, indicate the error and stop
 *
 * @param[in]   eModuleEnum			module enumeration
 * @param[in]		eTestEnum				test enumeration
 * @param[in]		pvCompare1			pointer to the compare value #1
 * @param[in]		pvCompare2			pointer to the compare value #2
 *
 *****************************************************************************/
void ModuleAssert_Execute( MODULEASSERTENUM eModuleEnum, MODULEASSERTEST eTestEnum, PVOID pvCompare1, PVOID pvCompare2 )
{
	BOOL	bAssert = FALSE;
	
	// perform the action
	switch( eTestEnum )
	{
		case MODULEASSERT_TEST_EQ :
			bAssert = ( pvCompare1 ==  pvCompare2 );
			break;
			
		case MODULEASSERT_TEST_NE :
			bAssert = ( pvCompare1 != pvCompare2 );
			break;
			
		default :
			break;
	}
	
	// now test for assert
	if ( bAssert )
	{
		// test output type
		if ( MODULEASSERT_OUT_TYPE == MODULEASSERT_OUT_ONCE )
		{
			// call the abstracted output function
			ModuleAssert_Output( eModuleEnum );
			
			// now loop forever
			FOREVER
			{
			}
		}
		else
		{
			// execute the forever loop
			FOREVER
			{
				// call the abstracted output function
				ModuleAssert_Output( eModuleEnum );
			}
		}
	}
}

/**@} EOF ModuleAssert.c */
