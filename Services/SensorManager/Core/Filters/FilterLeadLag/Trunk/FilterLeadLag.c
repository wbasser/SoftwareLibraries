/******************************************************************************
 * @file FilterLeadLag.c
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
 * \addtogroup FilterLeadLag
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/Filters/FilterLeadLag/FilterLeadLag.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -------------------------------------------


/******************************************************************************
 * @function FilterLeadLag_Process
 *
 * @brief Process the Lead Lag Filter
 *
 * This function will process the lead lag filter
 *
 * @param[in]   ptCtl		pointer to a control structure,
 *                    fNewValue	New Value to be added
 *                    uNewTime    New time value to be added
 *
 * @return      current filtered value
 *
 *****************************************************************************/
BOOL FilterLeadLag_Execute( PFILTLDLGCTL ptCtl, U16 wNewValue, U32 uNewTime )
{
	// compute the delta time - This is just equal to the sample rate in milliseconds
	float fDelta = ( float )( uNewTime );

	// calculate the factors
	float fD1 = ( fDelta + ( 2.0 * ptCtl->tDef.fLeadTc )) / ( fDelta + ( 2.0 * ptCtl->tDef.fLagTc ));
	float fD2 = ( fDelta - ( 2.0 * ptCtl->tDef.fLeadTc )) / ( fDelta + ( 2.0 * ptCtl->tDef.fLagTc ));
	float fD3 = 1.0 - (( 2.0 * fDelta ) / ( fDelta + ( 2.0 * ptCtl->tDef.fLagTc )));

	// calculate the output
	ptCtl->fOutput = ( ( FLOAT )wNewValue * fD1 ) + ( ptCtl->fPrvValue * fD2 ) + ( ptCtl->fPrvOutput * fD3 );

	// copy current to previous
	ptCtl->fPrvValue = ( float )wNewValue;
	ptCtl->fPrvOutput = ptCtl->fOutput;

	// return true
	return( TRUE );
}

/******************************************************************************
 * @function FilterLeadLag_GetValue
 *
 * @brief get the current lead lag filter output
 *
 * This function will get the current lead lag output
 *
 * @param[in]   ptCtl       pointer to a control structure
 *
 * @return      current filtered value
 *
 *****************************************************************************/
U16 FilterLeadLag_GetValue( PFILTLDLGCTL ptCtl )
{
	U16 wValue;
	
	wValue = ptCtl->fOutput;
	
	return( wValue );
}

/**@} EOF FilterLeadLag.c */