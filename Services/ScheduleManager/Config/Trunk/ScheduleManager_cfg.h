/******************************************************************************
 * @file ScheduleManager_cfg.h
 *
 * @brief Schedule Manager Configuration declarations 
 *
 * This file provides the configuration declarations for the Schedule Manager
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
 * \addtogroup ScheduleManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SCHEDULEMANAGER_CFG_H
#define _SCHEDULEMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ScheduleManager/ScheduleManager_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enuemrate the definition
typedef enum _SCHDMNGRDEFENUM
{
  // add user define scheduler enumerations here
  SCHDMNGRDEF_ENUM_DFLT = 0,      ///< default entry
  SCHDMNGDEF_ENUM_USER01,         ///< user schedule entry #01
  SCHDMNGDEF_ENUM_USER02,         ///< user schedule entry #02
  SCHDMNGDEF_ENUM_USER03,         ///< user schedule entry #03
  SCHDMNGDEF_ENUM_USER04,         ///< user schedule entry #04
  SCHDMNGDEF_ENUM_USER05,         ///< user schedule entry #05
  SCHDMNGDEF_ENUM_USER06,         ///< user schedule entry #06
  SCHDMNGDEF_ENUM_USER07,         ///< user schedule entry #07
  SCHDMNGDEF_ENUM_USER08,         ///< user schedule entry #08
  SCHDMNGDEF_ENUM_USER09,         ///< user schedule entry #09
  SCHDMNGDEF_ENUM_USER10,         ///< user schedule entry #10
  SCHDMNGDEF_ENUM_USER11,         ///< user schedule entry #11
  SCHDMNGDEF_ENUM_USER12,         ///< user schedule entry #12
  SCHDMNGDEF_ENUM_USER13,         ///< user schedule entry #13
  SCHDMNGDEF_ENUM_USER14,         ///< user schedule entry #14
  SCHDMNGDEF_ENUM_USER15,         ///< user schedule entry #15
  SCHDMNGDEF_ENUM_USER16,         ///< user schedule entry #16
  SCHDMNGDEF_ENUM_USER17,         ///< user schedule entry #17
  SCHDMNGDEF_ENUM_USER18,         ///< user schedule entry #18
  SCHDMNGDEF_ENUM_USER19,         ///< user schedule entry #19
  SCHDMNGDEF_ENUM_USER20,         ///< user schedule entry #20
  SCHDMNGDEF_ENUM_USER21,         ///< user schedule entry #21
  SCHDMNGDEF_ENUM_USER22,         ///< user schedule entry #22
  SCHDMNGDEF_ENUM_USER23,         ///< user schedule entry #23
  SCHDMNGDEF_ENUM_USER24,         ///< user schedule entry #24
  SCHDMNGDEF_ENUM_USER25,         ///< user schedule entry #25
  SCHDMNGDEF_ENUM_USER26,         ///< user schedule entry #26
  SCHDMNGDEF_ENUM_USER27,         ///< user schedule entry #27
  SCHDMNGDEF_ENUM_USER28,         ///< user schedule entry #28
  SCHDMNGDEF_ENUM_USER29,         ///< user schedule entry #29
  SCHDMNGDEF_ENUM_USER30,         ///< user schedule entry #30
  SCHDMNGDEF_ENUM_USER31,         ///< user schedule entry #31
  
  // do not remove the below entries
  SCHDMNGRDEF_ENUM_MAX            ///< maximum enum
} SCHDMNGRDEFENUM;

/// enuemrate the external rules
typedef enum _SCHDMNGREXTENUM
{
  // add user define scheduler enumerations here
  
  // do not remove the below entries
  SCHDMNGREXT_ENUM_MAX
} SCHDMNGREXTENUM;

// structures -----------------------------------------------------------------
/// define the actual rule table
typedef struct _SCHDRMNGRULETBL
{
  SCHDMNGRRULENTRY  atExtRules[ SCHDMNGREXT_ENUM_MAX ];
} SCHDMNGREXTTBL, *PSCHDMNGREXTTBL;
#define SCHDMNGRRULETBL_SIZE              sizeof( SCHMNGRDEXTTBL )

typedef struct _SCHDRULEINDICES
{
  SCHDRULEINDEXTYPE  atRuleIndex[ SCHDMNGREXT_ENUM_MAX ];
} SCHDRULEINDICES, *PSCHDRULEINDICES;
#define SCHDRULEINDICES_SIZE          sizeof( SCHDRULEINDICES )

/// define the actual schedule
typedef struct _SCHDMNGRDEFTBL
{
  SCHDMNGRDEFENTRY  atEntries[  SCHDMNGRDEF_ENUM_MAX ];
  SCHDRULEINDICES   atRules[ SCHDMNGRDEF_ENUM_MAX ];
} SCHDMNGRDEFTBL, *PSCHDMNGRDEFTBL;
#define SCHDMNGRDEFTBL_SIZE           sizeof( SCHDMNGRDEFTBL )

// global parameter declarations -----------------------------------------------
extern  const CODE  SCHDMNGRDEFTBL    tSchdDefTable;
extern  const CODE  SCHDMNGREXTTBL    tSchdExtTable;

// global function prototypes --------------------------------------------------
extern  BOOL              ScheduleManager_CheckValidDateTime( void );
extern  void              ScheduleManager_GetDateTime( PDATETIME ptDateTime );

/**@} EOF ScheduleManager_cfg.h */

#endif  // _SCHEDULEMANAGER_CFG_H