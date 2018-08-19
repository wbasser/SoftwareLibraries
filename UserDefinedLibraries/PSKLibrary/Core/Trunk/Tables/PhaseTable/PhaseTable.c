/******************************************************************************
 * @file PhaseTable.c
 *
 * @brief phase table implementation
 *
 * This file provides the phase table implementation
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
 * \addtogroup PhaseTable
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PhaseTable/PhaseTable.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const PHASETABLE	atPhaseTable[ PHASE_SYMBOL_MAX ][ PHASE_STEP_MAX ] =
{
	// symbol - 0 - SYM_NOCHANGE
	{
		{ SHAPE_PP, SHAPE_PP, PHASE_STEP_0   },
		{ SHAPE_MM, SHAPE_PP, PHASE_STEP_90  },
		{ SHAPE_MM, SHAPE_MM, PHASE_STEP_180 },
		{ SHAPE_PP, SHAPE_MM, PHASE_STEP_270 },
		{ SHAPE_ZZ, SHAPE_ZZ, PHASE_STEP_OFF }	
	},
	
	// symbol - 1 - SYM_P90 - advance 90 degrees
	{
		{ SHAPE_PM, SHAPE_PP, PHASE_STEP_90  },
		{ SHAPE_MM, SHAPE_PM, PHASE_STEP_180 },
		{ SHAPE_MP, SHAPE_MM, PHASE_STEP_270 },
		{ SHAPE_PP, SHAPE_MP, PHASE_STEP_0   },
		{ SHAPE_ZP, SHAPE_ZP, PHASE_STEP_0   }	
	},
	
	// symbol - 2 - SYM_P180 - advance 180 degrees
	{
		{ SHAPE_PM, SHAPE_PM, PHASE_STEP_180 },
		{ SHAPE_MP, SHAPE_PM, PHASE_STEP_270 },
		{ SHAPE_MP, SHAPE_MP, PHASE_STEP_0   },
		{ SHAPE_PM, SHAPE_MP, PHASE_STEP_90  },
		{ SHAPE_ZP, SHAPE_ZP, PHASE_STEP_0   }
	},
	
	// symbol - 3 - SYM_M90 - retard 90 degrees
	{
		{ SHAPE_PP, SHAPE_PM, PHASE_STEP_270 },
		{ SHAPE_MP, SHAPE_PP, PHASE_STEP_0   },
		{ SHAPE_MM, SHAPE_MP, PHASE_STEP_90  },
		{ SHAPE_PM, SHAPE_MM, PHASE_STEP_180 },
		{ SHAPE_ZP, SHAPE_ZP, PHASE_STEP_0   }
	},
	
	// symbol - 4 - SYM_OFF
	{
		{ SHAPE_PZ, SHAPE_PZ, PHASE_STEP_OFF },
		{ SHAPE_MZ, SHAPE_PZ, PHASE_STEP_OFF },
		{ SHAPE_MZ, SHAPE_MZ, PHASE_STEP_OFF },
		{ SHAPE_PZ, SHAPE_MZ, PHASE_STEP_OFF },
		{ SHAPE_ZZ, SHAPE_ZZ, PHASE_STEP_OFF }
	},
	
	// symbol - 5 - SYM_ON
	{
		{ SHAPE_PP, SHAPE_PP, PHASE_STEP_0   },
		{ SHAPE_MP, SHAPE_PP, PHASE_STEP_0   },
		{ SHAPE_MP, SHAPE_MP, PHASE_STEP_0   },
		{ SHAPE_PP, SHAPE_MP, PHASE_STEP_0   },
		{ SHAPE_ZP, SHAPE_ZP, PHASE_STEP_0   }
	}
};	

/******************************************************************************
 * @function PhaseTable_GetEntry
 *
 * @brief get an entry from the phase table
 *
 * This function will return the entry based on the symbol and step
 *
 * @param[in]   eSymbol   current symbol
 * @param[in]   eStep     current step
 *
 * @return      pointer to the correct phase table entry
 *
 *****************************************************************************/
PPHASETABLE PhaseTable_GetEntry( PHASESYMBOL eSymbol, PHASESTEP eStep )
{
  PPHASETABLE ptEntry = NULL;

  // validate entry
  if(( eSymbol < PHASE_SYMBOL_MAX ) && ( eStep < PHASE_STEP_MAX ))
  {
    // set the pointer
    ptEntry = ( PPHASETABLE )&atPhaseTable[ eSymbol ][ eStep ];
  }

  // return the entry
  return( ptEntry );
}
 
/**@} EOF .c */
