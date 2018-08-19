/******************************************************************************
 * @file FilterLeadLag.h
 *
 * @brief Filter LeadLagdeclarations
 *
 * This file provides the declaration for the average filter implementation
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
 * \addtogroup FilterLeadLag
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FILTERLEADLAG_H
#define _FILTERLEADLAG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// Define the Lead/Lag Definition Structure
typedef struct _FILTLDLGDEF
{
	float	fLagTc;			// lag time constant
	float	fLeadTc;		// lead time constant
} FILTLDLGDEF, *PFILTLDLGDEF;
#define FILTLDLGDEFSIZE sizeof( FILTLDLGDEF )

/// Define the lead/lag control structure
typedef struct _FILTLDLGCTL
{
	FILTLDLGDEF tDef;			// Definition structure
	float		fPrvValue;		// previous value
	float		fPrvOutput;		// previous output value
	FLOAT		fOutput;		// Output Value
} FILTLDLGCTL, *PFILTLDLGCTL;
#define FILTLDLGCTLSIZE	sizeof( FILTLDLGCTL )

// global parameter declarations -------------------------------------------------

// global function prototypes ----------------------------------------------------
extern  BOOL FilterLeadLag_Execute( PFILTLDLGCTL ptCtl, U16 fNewValue, U32 uNewTime );
extern  U16  FilterLeadLag_GetValue( PFILTLDLGCTL ptCtl );

/**@} EOF FilterLeadLag.h */

#endif  // _FILTERLEADLAG_H