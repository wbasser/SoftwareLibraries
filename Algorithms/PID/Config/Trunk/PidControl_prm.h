/******************************************************************************
 * @file PidControl_prm.h
 *
 * @brief PID control parameter declarations
 *
 * This file provides the declarations of the parameters for the PID control
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
 
// ensure only one instantiation
#ifndef _PIDCONTROL_PRM_H
#define _PIDCONTROL_PRM_H

// Macros and Defines ---------------------------------------------------------
/// define the argument size - valid values are 1, 2 or 4
#define PIDCONTROL_INTARG_SIZE_BYTES        ( 2 )

/// define the argument type
#define PIDCONTROL_ARGTYPE_INTEGER          ( 0 )
#define PIDCONTROL_ARGTYPE_FLOAT            ( 1 )

/// define the agument type
#define PIDCONTROL_ARG_TYPE                 ( PIDCONTROL_ARGTYPE_FLOAT )

/**@} EOF PidControl_prm.h */

#endif  // _PIDCONTROL_PRM_H