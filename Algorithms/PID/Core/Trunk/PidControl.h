/******************************************************************************
 * @file PidControl.h
 *
 * @brief PID control declarations 
 *
 * This file provides the declarations for the PID control
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
#ifndef _PIDCONTROL_H
#define _PIDCONTROL_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PID/PidControl_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// set the argument size
#if ( PIDCONTROL_ARG_TYPE == PIDCONTROL_ARGTYPE_FLOAT )
	typedef FLOAT PIDARG;
#elif ( PIDCONTROL_ARG_TYPE == PIDCONTROL_ARGTYPE_INTEGER )
  #if ( PIDCONTROL_INTARG_SIZE_BYTES == 1 )
    typedef S8    PIDARG;
  #elif ( PIDCONTROL_INTARG_SIZE_BYTES == 2 )
    typedef S16   PIDARG;
  #elif ( PIDCONTROL_INTARG_SIZE_BYTES == 4 )
    typedef S32   PIDARG;
  #else
    #error illegal PID integer argument length
  #endif
#else
  #error Illegal PID arugment type
#endif // PIDCONTROL_ARG_TYPE

// structures -----------------------------------------------------------------
/// enumerate the typedef structure
typedef struct _PIDCTRL
{
  PIDARG  xLastError;       ///< last error
  PIDARG  xTotalError;      ///< total error
  PIDARG  xCurError;        ///< current error
  PIDARG  xKp;              ///< proportional coefficient
  PIDARG  xKi;              ///< integral coefficient
  PIDARG  xKd;              ///< derivative coefficient
  PIDARG  xLd;              ///< limit doeefficient
  PIDARG  xMinOutput;       ///< minimum output level
  PIDARG  xMaxOutput;       ///< maximum output level
} PIDCTRL, *PPIDCTRL;
#define PIDCTRL_SIZE      sizeof( PIDCTRL )

// global function prototypes --------------------------------------------------
extern  void    PidControl_Initialize( void );
extern  PIDARG  PidControl_Process( PPIDCTRL ptPidCtl, PIDARG xSetPoint, PIDARG xProcVar );

/**@} EOF PidControl.h */

#endif  // _PIDCONTROL_H