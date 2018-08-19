/******************************************************************************
 * @file HysteresisControl.h
 *
 * @brief hystermesis control declarations 
 *
 * This file provides the declarations for the hysteresis control
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
 
// ensure only one instantiation
#ifndef _HYSTERESISCONTROL_H
#define _HYSTERESISCONTROL_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HysteresisControl/HysteresisControl_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events/execution rate
#define HYSTERESISCONTROL_NUM_EVENTS            ( 4 )
#define HYSTERESISCONTROL_EXEC_RATE             ( TASK_TIME_SECS( 1 ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  HysteresisControl_Initialize( void );
extern  BOOL  HysteresisControl_ProcessControl( TASKARG xArg );
extern  void  HysteresisControl_PutSetpoint( HYSTCONTROLENUM eHystCtlEnum, S16 iSetpoint );

/**@} EOF HysteresisControl.h */

#endif  // _HYSTERESISCONTROL_H