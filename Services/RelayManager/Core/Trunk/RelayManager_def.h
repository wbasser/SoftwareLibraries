/******************************************************************************
 * @file RelayManager_def.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
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
 * \addtogroup RelayManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RELAYMANAGER_DEF_H
#define _RELAYMANAGER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RelayManager/RelayManager_prm.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "TaskManager/TaskManager.h"
#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
#include "Timers/Timers.h"
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for relay definition table entries
#if ( RELAY_MANAGER_ENABLE_EXTDRIVER == 1 )
  #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 0 )
    #define RELAYCTLDEFM( gpioctl, gpiodir, keepalive, chargepump, minoff, minon, callback ) \
      { \
        .eGpioCtlEnum = gpioctl, \
        .eGpioDirEnum = gpiodir, \
        .nKeepAliveEnum = keepalive, \
        .nChargePumpEnum = chargepump, \
        .wMinOffTimeSecs = minoff, \
        .wMinOnTimeSecs = minon, \
        .pvCallback = callback, \
      }
  #else
    #define RELAYCTLDEFM( gpioctl, gpiodir, keepalive, chargepump, callback ) \
      { \
        .eGpioCtlEnum = gpioctl, \
        .eGpioDirEnum = gpiodir, \
        .nKeepAliveEnum = keepalive, \
        .nChargePumpEnum = chargepump, \
        .pvCallback = callback, \
      }
  #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
#else
  #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 0 )
    #define RELAYCTLDEFM( gpioctl, keepalive, chargepump, minoff, minon, callback ) \
      { \
        .eGpioCtlEnum = gpioctl, \
        .nKeepAliveEnum = keepalive, \
        .nChargePumpEnum = chargepump, \
        .wMinOffTimeSecs = minoff, \
        .wMinOnTimeSecs = minon, \
        .pvCallback = callback, \
      }
  #else
    #define RELAYCTLDEFM( gpioctl, keepalive, chargepump, callback ) \
      { \
        .eGpioCtlEnum = gpioctl, \
        .nKeepAliveEnum = keepalive, \
        .nChargePumpEnum = chargepump, \
        .pvCallback = callback, \
      }
  #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
#endif // RELAY_MANAGER_ENABLE_EXTDRIVER

#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
/// define the helper macro for charge pump 
#define RELAYCHARGEPUMPDEFM( gpio ) \
  { \
    .eGpioEnum = gpio, \
  }
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP

#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
/// define the helper macro for keep alive
#define RELAYKEEPALIVEDEFM( timer ) \
  { \
    .eTimerEnum = timer, \
  }
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE

#if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
/// define the helper macro for dynamic times
#define RELAYDYNTIMESDEFM( minoff, minon ) \
  { \
    .wMinOffTimeSecs = minoff, \
    .wMinOnTimeSecs = minon, \
  }
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
  
// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the callback function
typedef   void ( *PVRLYMNGRCALLBACK )( BOOL );

#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
/// define the charge pump definitions structure
typedef struct _RELAYCHARGEPUMPDEF
{
  GPIOPINENUM       eGpioEnum;        ///< gpio enum to control charge pump
} RELAYCHARGEPUMPDEF, *PRELAYCHARGEPUMPDEF;
#define RELAYCHARGEPUMPDEF_SIZE       sizeof( RELAYCHARGEPUMPDEF )
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP

#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
/// define the keep alive definitions structure
typedef struct _RELAYKEEPALIVEDEF
{
  TIMERENUM         eTimerEnum;       ///< timer enum
} RELAYKEEPALIVEDEF, *PRELAYKEEPALIVEDEF;
#define RELAYKEEPALIVEDEF_SIZE        sizeof( RELAYKEEPALIVEDEF )
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE

/// define the relay definition structure
typedef struct _RELAYCTLDEF
{
  GPIOPINENUM       eGpioCtlEnum;       ///< GPIO enum to control relay
  #if ( RELAY_MANAGER_ENABLE_EXTDRIVER == 1 )
  GPIOPINENUM       eGpioDirEnum;       ///< GPIO enum to control direction
  #endif // RELAY_MANAGER_ENABLE_EXTDRIVER
//  #if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
  U8                nKeepAliveEnum;     ///< keep alive enumeration
//  #endif // RELAY_MANAGER_ENABLE_KEEPALIVE
//  #if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
  U8                nChargePumpEnum;    ///< charge pump enumeration
//  #endif // RELAY_MANAGER_ENABLE_CHARGEPUMP
#if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 0 )
  U16               wMinOffTimeSecs;    ///< minimum relay on/off time
  U16               wMinOnTimeSecs;     ///< minimum relay on/off time
#endif
  PVRLYMNGRCALLBACK pvCallback;         ///< callback
} RELAYCTLDEF, *PRELAYCTLDEF;
#define RELAYCTLDEF_SIZE          sizeof( RELAYCTLDEF )

#if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
/// define the dynamic on/off times structure
typedef struct _RELAYDYMTIMDEF
{
  U16               wMinOffTimeSecs;    ///< minimum relay on/off time
  U16               wMinOnTimeSecs;     ///< minimum relay on/off time
} RELAYDYMTIMDEF, *PRELAYDYMTIMDEF;
#define RELAYDYMTIMDEF_SIZE       sizeof( RELAYDYMTIMDEF )
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES

/**@} EOF RelayManager_def.h */

#endif  // _RELAYMANAGER_DEF_H