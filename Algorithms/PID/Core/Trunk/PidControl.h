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
	typedef FLOAT PIDCONTROLARG;
#elif ( PIDCONTROL_ARG_TYPE == PIDCONTROL_ARGTYPE_INTEGER )
  #if ( PIDCONTROL_INTARG_SIZE_BYTES == 1 )
    typedef S8    PIDCONTROLARG;
  #elif ( PIDCONTROL_INTARG_SIZE_BYTES == 2 )
    typedef S16   PIDCONTROLARG;
  #elif ( PIDCONTROL_INTARG_SIZE_BYTES == 4 )
    typedef S32   PIDCONTROLARG;
  #else
    #error illegal PID integer argument length
  #endif
#else
  #error Illegal PID arugment type
#endif // PIDCONTROL_ARG_TYPE

// structures -----------------------------------------------------------------
/// define the definition structure
typedef struct _PIDCONTROLDEF
{
  PIDCONTROLARG  xKp;              ///< proportional coefficient
  PIDCONTROLARG  xKi;              ///< integral coefficient
  PIDCONTROLARG  xKd;              ///< derivative coefficient
  PIDCONTROLARG  xLd;              ///< limit doeefficient
  PIDCONTROLARG  xMinOutput;       ///< minimum output level
  PIDCONTROLARG  xMaxOutput;       ///< maximum output level
} PIDCONTROLDEF, *PPIDCONTROLDEF;
#define PIDCONTROLDEF_SIZE                      sizeof( PIDCONTROLDEF )


/// define the controls structure
typedef struct _PIDCONTROLVAR
{
  PIDCONTROLARG  xLastError;       ///< last error
  PIDCONTROLARG  xTotalError;      ///< total error
  PIDCONTROLARG  xCurError;        ///< current error
  PIDCONTROLDEF  tDefs;            ///< definitions
} PIDCONTROLVAR, *PPIDCONTROLVAR;
#define PIDCONTROLVAR_SIZE                            sizeof( PIDCONTROLVAR )

// global function prototypes --------------------------------------------------
extern  void            PidControl_Initialize( void );
extern  PIDCONTROLARG   PidControl_Process( PPIDCONTROLVAR ptPidCtl, PIDCONTROLARG xSetPoint, PIDCONTROLARG xProcVar );

/**@} EOF PidControl.h */

#endif  // _PIDCONTROL_H