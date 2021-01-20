/******************************************************************************
 * @file HysteresisControl.c
 *
 * @brief Hysteresis control implementation 
 *
 * This file provides the implementation for the hysteresis control
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
 * \addtogroup HysteresisControl
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HysteresisControl/HysteresisControl.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function HysteresisControl_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the hysteresis control
 *
 *****************************************************************************/
void HysteresisControl_Initialize( void )
{
}

/******************************************************************************
 * @function HysteresisControl_Process
 *
 * @brief process the control task
 *
 * This function will perform the process
 *
 * @param[in]   ptHystCtl     pointer to the definition structure
 * @param[in]   xSetPoint     setpoint value
 * @param[in]   xProcVar      process variable value
 *
 * @return      state of the output variable
 *
 *****************************************************************************/
BOOL HysteresisControl_Process( HYSTCONTROLDEF ptHystDef, HYSTARG xSetPoint, HYSTARG xProcVar );
{
  BOOL  bOutputState = OFF;
  
  // check for on/off
  if (( xProcVar <= ( xSetPoint - PGM_RDWORD( ptHystDef->xPosHysteresis ))) ||
      ( xProcVar >= ( xSetPoint - PGM_RDWORD( ptHystDef-xiNegHysteresis ))))
  {
    // turn on output
    bOutputState = ON;
  }

  // return the output state
  return( bOutputState );
}

/**@} EOF HysteresisControl.c */
