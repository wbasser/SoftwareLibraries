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
PIDCONTROLARG PidControl_Process( PPIDCTRL ptPidCtl, PIDCONTROLARG xSetPoint, PIDCONTROLARG xProcVar )
{
	PIDCONTROLARG	xError;
	PIDCONTROLARG	xOutput;

	// compute the error
	xError = xSetPoint - xProcVar;

	// compute the integral/check for limits
	ptPidCtl->xTotalError += xError;
	ptPidCtl->xTotalError = ( ptPidCtl->xTotalError > ptPidCtl->tDefs.xLd ) ? ptPidCtl->tDefs.xLd : ptPidCtl->xTotalError;
	ptPidCtl->xTotalError = ( -ptPidCtl->xTotalError < -ptPidCtl->tDefs.xLd ) ? -ptPidCtl->tDefs.xLd : ptPidCtl->xTotalError;

 // compute derivative
  ptPidCtl->xLastError = ptPidCtl->xCurError;
  ptPidCtl->xCurError = xError;
  
	// compute output
	xOutput = ptPidCtl->tDefs.xKp * xError;
	xOutput += ptPidCtl->tDefs.xKi * ptPidCtl->xTotalError;
	xOutput += ptPidCtl->tDefs.xKd * ( xError - ptPidCtl->xLastError );
  
  // clamp the output to min, max
  xOutput = MAX( ptPidCtl->tDefs.xMinOutput, xOutput );
  xOutput = MIN( xOutput, ptPidCtl->tDefs.xMaxOutput );

	// return the new output value
	return( xOutput );
}

/**@} EOF PidControl.c */
