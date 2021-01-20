/******************************************************************************
 * @file SystemTick.h
 *
 * @brief system tick declarations
 *
 * This file declares the function prototypes for the system tick handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
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

// Macros and Defines ---------------------------------------------------------
/// define the system tick functions
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
typedef void ( *PVSYSTEMTICKFUNC )( U16 );
#else
typedef void ( *PVSYSTEMTICKFUNC )( void );
#endif // SYSTEMDEFINE_OS_SELECTION

// global function prototypes --------------------------------------------------
extern  void  SystemTick_Initialize( U32 uTickRateHz, PVSYSTEMTICKFUNC pvCallback );
extern  void  SystemTick_Close( void );
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
extern  U32   SystemTick_GetTickRateUsec( void );
extern  U64   SystemTick_GetTimeUsec( void );
extern  U32   SystemTick_GetTimeMsec( void );
extern  void  SystemTick_DelayMsec( U16 wMilliSeconds );
extern  BOOL  SystemTick_IsDelayExpired( void );
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

/**@} EOF SystemTick.h */

#endif  // _SYSTEMTICK_H