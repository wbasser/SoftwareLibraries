/******************************************************************************
 * @file AlarmHandler.h
 *
 * @brief Alarm handler declarations
 *
 * This file provides the declarations for the alarm handler
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
 * $Rev: $
 * 
 *
 * \addtogroup AlarmHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ALARMHANDLER_H
#define _ALARMHANDLER_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "AlarmHandler/AlarmHandler_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the return status
typedef enum _ALARMSTS
{
  ALARM_STS_NONE = 0,
  ALARM_STS_LOLO,
  ALARM_STS_LO,
  ALARM_STS_HI,
  ALARM_STS_HIHI,
} ALARMSTS;

// structures -----------------------------------------------------------------
/// define the argument type
#if ( ALARMHANDLER_ARGUMENT_TYPE == ALARMHANDLER_TYPE_SIGNED16 )
  typedef S16     ALARMARG;
  typedef PS16    PALARMARG;
#elif ( ALARMHANDLER_ARGUMENT_TYPE == ALARMHANDLER_TYPE_SIGNED32 )
  typedef S32     ALARMARG;
  typedef PS32    PALARMARG;
#elif ( ALARMHANDLER_ARGUMENT_TYPE == ALARMHANDLER_TYPE_FLOAT )
  typedef FLOAT   ALARMARG;
  typedef PFLOAT  PALARMARG;
#else
  typedef S16     ALARMARG;
  typedef PS16    PALARMARG;
#endif // ALARMHANDLER_ARGUMENT_TYPE

/// define the alarm control structure
typedef struct _ALARMDEF
{
  ALARMARG  xLoLoThreshold;     ///< lo-lo threshold
  ALARMARG  xLoThreshold;       ///< lo threshold
  ALARMARG  xHiThreshold;       ///< hi threshold
  ALARMARG  xHiHiThreshold;     ///< hi-hi threshold
  struct
  {
    BOOL  bLoLoEnable;          ///< enable lo-lo alarm
    BOOL  bLoEnable;            ///< enable lo alarm
    BOOL  bHiEnable;            ///< enable hi alarm
    BOOL  bHiHiEnable;          ///< enable hi-hi alarm
  } tEnableFlags;
} ALARMDEF, *PALARMDEF;
#define ALARMDEF_SIZE             sizeof( ALARMDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  ALARMSTS  AlarmHandler_CheckAlarm( ALARMARG xArg, PALARMDEF ptAlarmDef );

/**@} EOF AlarmHandler.h */

#endif  // _ALARMHANDLER_H