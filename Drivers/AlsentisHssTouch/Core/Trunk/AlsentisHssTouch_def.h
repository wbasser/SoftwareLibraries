/******************************************************************************
 * @file AlsentisHssTouch_def.h
 *
 * @brief  Alsentis HSS touch definitions 
 *
 * This file provides the definitiona for the Alsentix HSS touch chip
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
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
#ifndef _ALSENTISHSSTOUCH_DEF_H
#define _ALSENTISHSSTOUCH_DEF_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// local includes
#include "AlsentisHssTouch/AlsentisHssTouch_prm.h"

// Macros and Defines ---------------------------------------------------------
// define the size of the sensor variable
#if ( ALSENTISHSSTOUCH_NUM_SENSORS <= 8 )
typedef  U8         SENSORVAR;
#elif ( ALSENTISHSSTOUCH_NUM_SENSORS <= 16 )
typedef U16         SENSORVAR;
#else
typedef U32         SENSORVAR;
#endif  // ALSENTISHSSTOUCH_NUM_SEBNSORS

// enumerations ---------------------------------------------------------------
/// enumerate the duty cycle
typedef enum _ALSENTIS_DUTY_CYCLE
{
  ALSENTIS_DUTY_CYCLE_25 = 0,           ///< 25%
  ALSENTIS_DUTY_CYCLE_50,               ///< 50%
  ALSENTIS_DUTY_CYCLE_75,               ///< 75%
  ALSENTIS_DUTY_CYCLE_MAX
} ALSENTIS_DUTY_CYCLE;

/// enumerate the power modes
typedef enum _ALSENTIS_POWER_MODE
{
  ALSENTIS_POWER_MODE_HIBERNATE = 0,    ///< hibernate
  ALSENTIS_POWER_MODE_WAKEUP,           ///< wake up
  ALSENTIS_POWER_MODE_NORMAL,           ///< normal
  ALSENTIS_POWER_MODE_MAX
} ALSENTIS_POWER_MODE;

/// enumerate the wake mode
typedef enum _ALSENTIS_WAKE_MODE
{
  ALSENTIS_WAKE_MODE_DONT = 0,          ///< dont wake up
  ALSENTIS_WAKE_MODE_WAKEUP             ///< wake up
} ALSENTIS_WAKE_MODE;

/// enuemrate local events
typedef enum _ALSENTISHSSTOUCHEVENT
{
  ALSENTISHSSTOUCH_EVENT_START_CHARGING = 0x3800,
  ALSENTISHSSTOUCH_EVENT_CHARGING_DONE,
  ALSENTISHSSTOUCH_EVENT_ADCCONV_DONE,
  ALSENTISHSSTOUCH_EVENT_PROCESSING_DONE,
} ALSENTISHSSTOUCHEVENT;


// structures -----------------------------------------------------------------
/// define the zone parameter structure
typedef struct _ALSENTISZONE
{
  U16   wTrzMultiplier;                   ///< multiplier
  U16   wTrzDivider;                      ///< divider
  U16   wTrzTouchZone;                    ///< touch zone
} ALSENTISZONE, *PALSENTISZONE;
#define ALSENTISZONE_SIZE             sizeof( ALSENTISZONE )

/// define the pulse configuration structure
typedef struct _ALSENTISPULSECONFIG
{
  U8  nNumberOfPulses;
  union
  {
    struct
    {
      U16   wPulsePeriod  : 14;               ///< pulse period
      U16   wDutyCycle    : 2;                ///< duty cycle
    } tFields;
    U16 wValue;
  } tPulse;
} ALSENTISPULSECONFIG, *PALSENTISPULSECONFIG;
#define ALSENTISPULSECONFIG_SIZE      sizeof( ALSENTISPULSECONFIG )

/// define the power mode
typedef struct _ALSENTISPOWERMODE
{
  struct  
  {
    U8    nWake         : 4;              ///< wake
    U8    nPower        : 4;              ///< power
  } tFields;
  U8  nByte;
} ALSENTISPOWERMODE, *PALSENTISPOWERMODE;
#define ALSENTISPOWERMODE_SIZE        sizeof( ALSENTISPOWERMODE )

/// define the HSS config #1
typedef struct _ALSENTISHSS1CFG
{
  U8    nApproachContMin;                 ///< approach count minimum
  U8    nSlowLvlOffCntMin;                ///< slow level off count minimum
  U8    nFastLvlOffCntMin;                ///< fast level off count minimum
  U8    nNoTouchDebounceMin;              ///< no touch devounce minimum
} ALSENTISHSS1CFG, *PALSENTISHSS1CFG;
#define ALSENTISHSS1CFG_SIZE          sizeof( ALSENTISHSS1CFG )

/// define the HSS config #2
typedef struct _ALSENTISHSS2CFG
{
  U8    nFastTglDvDtDeviation;            ///< fast toggle DV/DT deviation
  U8    nAbortTouchPercent;               ///< abort touch percentage
  U8    nAbortTouchCountMax;              ///< abort touch count max
  U8    nTooSlowCountMax;                 ///< too slow count max
} ALSENTISHSS2CFG, *PALSENTISHSS2CFG;
#define ALSENTISHSS2CFG_SIZE          sizeof( ALSENTISHSS2CFG )

/// define the Sleep time
typedef struct _ALSENTISSLEEP
{
  U8    nOneTime;                         ///< one time sleep value
  U8    nRepeat;                          ///< repeat value
} ALSENTISSLEEP, *PALSENTISSLEEP;
#define ALSENTISSLEEP_SIZE            sizeof( ALSENTISSLEEP )

/// define the ID structure
typedef struct _ALSENTISID
{
  U16   wType;                            ///< type
  U16   wVersion;                         ///< version
} ALSENTISID, *PALSENTISID;
#define ALSENTISID_SIZE              sizeof( ALSENTISID )

/// define the chip parameter structure
typedef struct _ALSENTISCONFIG
{
  BOOL                bIrqEnabled;            ///< IRQ enabled
  BOOL                bDviMode;               ///< DVI mode
  U8                  nLatchTimeoutSecs;      ///< latch timeout in seconds
  U16                 wProcessIntervalUsecs;  ///< process interval in usecs
  U16                 wNumberSampleSets;      ///< number of sample sets
  U8                  nDiffSensorNumber;      ///< differential sensor number
  SENSORVAR           xDiffSensors;           ///< differential sensor set
  SENSORVAR           xEnabledSensors;        ///< enabled sensors
  ALSENTISID          tId;                    ///< ID
  ALSENTISPULSECONFIG tPulseConfig;           ///< pulse config
  ALSENTISPOWERMODE   tPowerMode;             ///< power mode
  ALSENTISHSS1CFG     tHss1Config;            ///< HSS #1 config
  ALSENTISHSS2CFG     tHss2Config;            ///< HSS #2 config
  ALSENTISSLEEP       tSleep;                 ///< sleep time
  ALSENTISZONE        atZones[ ALSENTISHSSTOUCH_NUM_SENSORS ];
} ALSENTISCONFIG, *PALSENTISCONFIG;
#define ALSENTISCONFIG_SIZE           sizeof( ALSENTISCONFIG )

/**@} EOF AlsentisHssTouch_def.h */

#endif  // _ALSENTISHSSTOUCH_DEF_H
