/******************************************************************************
 * @file SystemTick.h
 *
 * @brief system tick declarations
 *
 * This file declares the function prototypes for the system tick handler
 *
 * @copyright Copyright (c) 2012 Cyber Integration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
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

// library include files
#include "Types/Types.h"
#include "SystemTick/SystemTick_prm.h"

// global function prototypes --------------------------------------------------
extern  void  SystemTick_Initialize( void );
extern  void  SystemTick_Close( void );
extern  U32   SystemTick_GetTickRateUsec( void );
extern  U64   SystemTick_GetTimeUsec( void );
extern  U32   SystemTick_GetTimeMsec( void );
extern  void  SystemTick_DelayMsec( U8 nMsecs );

/**@} EOF SystemTick.h */

#endif  // _SYSTEMTICK_H