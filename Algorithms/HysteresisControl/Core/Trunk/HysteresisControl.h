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
#include "HysteresisControl/HysteresisControl_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// set the argument size
#if ( HYSTCONTROL_ARG_TYPE == HYSTCONTROL_ARGTYPE_FLOAT )
	typedef FLOAT HYTSARG;
#elif ( HYSTCONTROL_ARG_TYPE == HYSTCONTROL_ARGTYPE_INTEGER )
  #if ( HYSTCONTROL_INTARG_SIZE_BYTES == 1 )
    typedef S8    HYSTARG;
  #elif ( HYSTCONTROL_INTARG_SIZE_BYTES == 2 )
    typedef S16   HYSTARG;
  #elif ( HYSTCONTROL_INTARG_SIZE_BYTES == 4 )
    typedef S32   HYSTARG;
  #else
    #error illegal Hysteresis integer argument length
  #endif
#else
  #error Illegal Hysteresis arugment type
#endif // HYSTCONTROL_ARG_TYPE


// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the definition structure
typedef struct _HYSTCONTROLDEF
{
  HYSTARG     xPosHysteresis;
  HYSTARG     xNegHysteresis;
} HYSTCONTROLDEF, *PHYSTCONTROLDEF;
#define HYSTCONTROLDEF_SIZE           sizeof( HYSTCONTROLDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  HysteresisControl_Initialize( void );
extern  BOOL  HysteresisControl_Process( HYSTCONTROLDEF ptHysDef, HYSTARG xSetPoint, HYSTARG xProcVar );

/**@} EOF HysteresisControl.h */

#endif  // _HYSTERESISCONTROL_H