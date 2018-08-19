/******************************************************************************
 * @file ButtonManager_def.h
 *
 * @brief key handler declarations
 *
 * This file 
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
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BUTTONMANAGER_DEF_H
#define _BUTTONMANAGER_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager_prm.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for filling a keyboard handler configuation
#define BTNMNGR_CFG_ENTRY( debnc, repdly, reprate, sh_time, lh_time, stuck_time ) \
    .wDebounceTimeMsecs = debnc, \
    .wRepeatDelayMsecs = repdly, \
    .wRepeatRateMsecs = reprate, \
    .wShortHoldTimeMsecs = sh_time, \
    .wLongHoldTimeMsecs = lh_time, \
    .wStuckTimeMsecs = stuck_time \

/// define the helper macro for creating button manager entry events
#define BTNMNGR_DEFCB_ENTRY( keyenum, rel_enb, prs_enb, rep_enb, shh_enb, lng_enb, tgl_enb, callback ) \
  {\
    .eRptMethod = BTNMNGR_RPTMETHOD_CB, \
    .nKeyEnum = keyenum, \
    .tEventFlags = \
    { \
      .bReleaseEnable = rel_enb, \
      .bPressEnable = prs_enb, \
      .bRepeatEnable = rep_enb, \
      .bShortHoldEnable = shh_enb, \
      .bLongHoldEnable = lng_enb, \
      .bToggleEnable = tgl_enb, \
    }, \
    .pvCallback = callback \
  }

#define BTNMNGR_DEFEVENT_ENTRY( keyenum, rel_enb, prs_enb, rep_enb, shh_enb, lng_enb, tgl_enb, task ) \
  {\
    .eRptMethod = BTNMNGR_RPTMETHOD_EVENT, \
    .nKeyEnum = keyenum, \
    .tEventFlags = \
    { \
      .bReleaseEnable = rel_enb, \
      .bPressEnable = prs_enb, \
      .bRepeatEnable = rep_enb, \
      .bShortHoldEnable = shh_enb, \
      .bLongHoldEnable = lng_enb, \
      .bToggleEnable = tgl_enb, \
    }, \
    .eTaskEnum = task \
  }

/// define the shift count/mask
#define EVENT_SHIFT_CNT         ( 5 )
#define EVENT_KEY_MASK          ( 0x1F )

// enumerations ---------------------------------------------------------------
/// enumerate the reporting method
typedef enum _BTNMNGRRPTMETHOD
{
  BTNMNGR_RPTMETHOD_CB = 0,       ///< call back method
  BTNMNGR_RPTMETHOD_EVENT,        ///< event
} BTNMNGRRPTMETHOD;

// structures -----------------------------------------------------------------
/// define the callback function event
typedef void ( *PVBTNMNGRFUNC )( U8, U8 );

/// define the key global settings
typedef struct _BTNMNGRCFG
{
  U16   wDebounceTimeMsecs;       ///< debounce time
  U16   wRepeatDelayMsecs;        ///< repeat delay
  U16   wRepeatRateMsecs;         ///< repeat rate
  U16   wShortHoldTimeMsecs;      ///< short hold time
  U16   wLongHoldTimeMsecs;       ///< long hold time
  U16   wStuckTimeMsecs;          ///< stuck key time
} BTNMNGRCFG, *PBTNMNGRCFG; 
#define BTNMNGRCFG_SIZE  sizeof( BTNMNGRCFG )

/// define the key definition structure
typedef struct _BTNMNGRDEF
{
  U8                nKeyEnum;     ///< key enumerator
  TASKSCHDENUMS     eTaskEnum;    ///< task enumerator
  BTNMNGRRPTMETHOD  eRptMethod;   ///< report method
  struct
  {
    BOOL  bReleaseEnable    : 1;  ///< enable release events
    BOOL  bPressEnable      : 1;  ///< enable press events
    BOOL  bRepeatEnable     : 1;  ///< enable repeat events
    BOOL  bShortHoldEnable  : 1;  ///< enable short hold events
    BOOL  bLongHoldEnable   : 1;  ///< enable long hold events
    BOOL  bToggleEnable     : 1;  ///< enable toggle operations
  } tEventFlags;
  PVBTNMNGRFUNC     pvCallback;   ///< callback function
} BTNMNGRDEF, *PBTNMNGRDEF;
#define BTNMNGRDEF_SIZE   sizeof( BTNMNGRDEF )

/**@} EOF ButtonManager_def.h */

#endif  // _BUTTONMANAGER_DEF_H