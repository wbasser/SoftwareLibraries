/******************************************************************************
 * @file PhaseTable.h
 *
 * @brief phase table declarations
 *
 * This file provides the declarations for the phase table
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
 
// ensure only one instantiation
#ifndef _PHASETABLE_H
#define _PHASETABLE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ShapeTable/ShapeTable.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
// enumerate the symbols
typedef enum	_PHASESYMBOL
{
	PHASE_SYMBOL_NOC = 0,
	PHASE_SYMBOL_P90,
	PHASE_SYMBOL_P180,
	PHASE_SYMBOL_M90,
	PHASE_SYMBOL_OFF,
	PHASE_SYMBOL_ON,
  PHASE_SYMBOL_MAX
} PHASESYMBOL;

// enumerate the phases
typedef enum	_PHASESTEP
{
	PHASE_STEP_0 = 0,
	PHASE_STEP_90,
	PHASE_STEP_180,
	PHASE_STEP_270,
	PHASE_STEP_OFF,
  PHASE_STEP_MAX
} PHASESTEP;

// structures -----------------------------------------------------------------
// define the phase structure
typedef struct	_PHASETABLE
{
	SHAPES    eIShape;
	SHAPES    eQShape;
	PHASESTEP eNextPhase;
} PHASETABLE, *PPHASETABLE;
#define	PHASETABLESIZE	sizeof( PHASETABLE )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  PPHASETABLE PhaseTable_GetEntry( PHASESYMBOL eSymbol, PHASESTEP eStep );

/**@} EOF PhaseTable.h */

#endif  // _PHASETABLE_H