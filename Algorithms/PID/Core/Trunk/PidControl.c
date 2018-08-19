/******************************************************************************
 * @file PidControl.c
 *
 * @brief PID control
 *
 * This file provides the PID control implementation
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup PidControl
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PID/PidControl.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function PidControl_Initialize
 *
 * @brief initilialization
 *
 * This function will initialize the PID 
 *
 *****************************************************************************/
void PidControl_Initialize( void )
{
}

/******************************************************************************
 * @function PidControl_Process
 *
 * @brief process a PID control loop
 *
 * This function will process a new setpoint and process variable and return
 * the new output
 *
 * @param[in]   ptPidCtl      pointer to the control structure
 * @param[in]   xSetPoint     setpoint
 * @param[in]   xProcVar      process variable
 *
 * @return      new output value
 *
 *****************************************************************************/
PIDARG PidControl_Process( PPIDCTRL ptPidCtl, PIDARG xSetPoint, PIDARG xProcVar )
{
	PIDARG	xError;
	PIDARG	xOutput;

	// compute the error
	xError = xSetPoint - xProcVar;

	// compute the integral/check for limits
	ptPidCtl->xTotalError += xError;
	ptPidCtl->xTotalError = ( ptPidCtl->xTotalError > ptPidCtl->xLd ) ? ptPidCtl->xLd : ptPidCtl->xTotalError;
	ptPidCtl->xTotalError = ( -ptPidCtl->xTotalError < -ptPidCtl->xLd ) ? -ptPidCtl->xLd : ptPidCtl->xTotalError;

 // compute derivative
  ptPidCtl->xLastError = ptPidCtl->xCurError;
  ptPidCtl->xCurError = xError;
  
	// compute output
	xOutput = ptPidCtl->xKp * xError;
	xOutput += ptPidCtl->xKi * ptPidCtl->xTotalError;
	xOutput += ptPidCtl->xKd * ( xError - ptPidCtl->xLastError );
  
  // clamp the output to min, max
  xOutput = MAX( ptPidCtl->xMinOutput, xOutput );
  xOutput = MIN( xOutput, ptPidCtl->xMaxOutput );

	// return the new output value
	return( xOutput );
}

/**@} EOF PidControl.c */
