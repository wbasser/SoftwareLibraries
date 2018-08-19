/******************************************************************************
 * @file SystemTick.h
 *
 * @brief system tick declarations
 *
 * This file declares the function prototypes for the system tick handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup SystemTick
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _SYSTEMTICK_H
#define _SYSTEMTICK_H

// local includes -------------------------------------------------------------
#include "SystemTick/SystemTick_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// global function prototypes --------------------------------------------------
extern  void  SystemTick_Initialize( void );
extern  void  SystemTick_Close( void );
extern  U32   SystemTick_GetTickRateUsec( void );
extern  U64   SystemTick_GetTimeUsec( void );
extern  U32   SystemTick_GetTimeMsec( void );
extern  void  SystemTick_DelayMsec( U16 wMilliSeconds );
extern  void  SystemTick_DelayMsecNoWait( U16 wMilliSeconds );
extern  BOOL  SystemTick_IsDelayExpired( void );
#if (SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
extern  void  SystemTick_ProcessTick( void );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SystemTick.h */

#endif  // _SYSTEMTICK_H