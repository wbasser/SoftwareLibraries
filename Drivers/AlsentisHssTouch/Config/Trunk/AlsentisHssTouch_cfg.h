/******************************************************************************
 * @file AlsentisHssTouch_cfg.h
 *
 * @brief Alsentis HSS driver configuration declarations 
 *
 * This file provides the declarations for the Alsentis HSS driver
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
 * \addtogroup AlsentisHssTouch
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALSENTISHSSTOUCH_CFG_H
#define _ALSENTISHSSTOUCH_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AlsentisHssTouch/AlsentisHssTouch_def.h"

// library includes -----------------------------------------------------------
#include "ADC/Adc.h"
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  AlsentisHssTouch_StartCharging( PALSENTISPULSECONFIG ptPulseConfig );
extern  void  AlsentisHssTouch_StartAdcConvert( ADCENUM eAdcEnum, PU16 pwValue );
extern  void  AlsentisHssTouch_ProcessAdcCallback( U16 wValue );
extern  void  AlsentisHssTouch_PulseEnableCallback( TIMERCBEVENT eEvent, U16 uOption );
extern  void  AlsentisHssTouch_DischargeSensors( void );
extern  void  AlsentisHssTouch_EnableSensors( void );
extern  void  AlsentisHssTouch_SetTouchStatusLed( BOOL bState );

/**@} EOF AlsentisHssTouch.h */

#endif  // _ALSENTISHSSTOUCH_H