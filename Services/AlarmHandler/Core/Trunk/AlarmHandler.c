/******************************************************************************
 * @file AlarmHandler.c
 *
 * @brief Alarm handler implementation
 *
 * This file provides the implementation for the alarm handler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AlarmHandler/AlarmHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AlarmHandler_CheckAlarm
 *
 * @brief check a valud for an alarm condition
 *
 * This function will check a given value against the defined limits
 * and return the current status
 *
 * @param[in]   xArg        value to check
 * @param[in]   ptAlarmDef  pointer to an alarm definition
 *
 * @return      ALARMSTS    valid alarm status
 *
 *****************************************************************************/
ALARMSTS AlarmHandler_CheckAlarm( ALARMARG xArg, PALARMDEF ptAlarmDef )
{
  ALARMSTS  eStatus = ALARM_STS_NONE;
  
  // determine if we are in a LO-LO alarm if enabled
  if (( ptAlarmDef->tEnableFlags.bLoLoEnable ) && ( xArg <= ptAlarmDef->xLoLoThreshold ))
  {
    // set the alarm
    eStatus = ALARM_STS_LOLO;
  }
  else if (( ptAlarmDef->tEnableFlags.bLoEnable ) && ( xArg <= ptAlarmDef->xLoThreshold ))
  {
    // set the alarm
    eStatus = ALARM_STS_LO;
  }
  else if (( ptAlarmDef->tEnableFlags.bHiHiEnable ) && ( xArg >= ptAlarmDef->xHiHiThreshold ))
  {
    // set the alarm
    eStatus = ALARM_STS_HIHI;
  }
  else if (( ptAlarmDef->tEnableFlags.bHiEnable ) && ( xArg >= ptAlarmDef->xHiThreshold ))
  {
    // set the alarm
    eStatus = ALARM_STS_HI;
  }
  
  // return the status
  return( eStatus );
}
 
/**@} EOF AlarmHandler.c */
