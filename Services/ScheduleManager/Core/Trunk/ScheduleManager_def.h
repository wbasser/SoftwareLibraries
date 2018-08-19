/**************************************************************************//**
 * @file SchedulerManager_def.h
 *
 * @brief light control manager definitions 
 *
 * This file provides the definitions for the light control manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup schedulerManager 
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SCHEDULEMANAGER_DEF_H
#define _SCHEDULEMANAGER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the mins in an hour
#define MINS_PER_HOUR                   ( 60 )
#define HOURS_PER_DAY                   ( 24 )

/// define the macro for converting hours/mins to a value
#define SCHD_TIME_MINS(hrs, mins)       (( hrs * MINS_PER_HOUR) + mins)

/// define the helper macro for creating the below table
#define SCHDMNGRDEFM( sday, shr, smin, eday, ehr, emin  ) \
  { \
    .tStartTime             = \
    { \
      .nDayOfWeek           = sday, \
      .wHrsMins             = SCHD_TIME_MINS( shr, smin ), \
    }, \
    .tStopTime              = \
    { \
      .nDayOfWeek           = eday, \
      .wHrsMins             = SCHD_TIME_MINS( ehr, emin ), \
    }, \
  }

/// define the day of week value indicating an invalid schedule entry
#define SCHDMNGR_DOW_ILLEGAL            ( 0xFF )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the function prototype to set a rule index
typedef U8  ( *PVSETRULEINDEX )( U8 );

/// declare the special type for rule indices
typedef U8    SCHDRULEINDEXTYPE;

// define the local time stucture
typedef struct _TIMENTRY
{
  U8        nDayOfWeek;                  ///< day of week
  U16       wHrsMins;                    ///< hour
} TIMENTRY, *PTIMENTRY;
#define TIMENTRY_SIZE     sizeof( TIMENTRY )

/// define the schedule entry structure
typedef struct _SCHDMNGRDEFENTRY
{
  TIMENTRY              tStartTime;      ///< start time
  TIMENTRY              tStopTime;       ///< stop time
} SCHDMNGRDEFENTRY, *PSCHDMNGRDEFENTRY;
#define SCHDMNGRDEFENTRY_SIZE    sizeof( SCHDMNGRDEFENTRY )

/// define the user define rules handler structure
typedef struct _SCHDMNGRRULENTRY
{
  PVSETRULEINDEX        pvSetRuleIndex;    ///< set rule function
} SCHDMNGRRULENTRY, *PSCHDMNGRRULENTRY;
#define SCHDMNGRRULENTRY_SIZE   sizeof( SCHDMNGRRULENTRY )

/**@} EOF SchedulerManager_def.h */

#endif  // _SCHEDULEMANAGER_DEF_H