/******************************************************************************
 * @file SystemControlManager_def.h
 *
 * @brief System Control Manager Definitions declarations
 *
 * This file provides the declaratons for the System Control Manager
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
 * $Rev: $
 * 
 *
 * \addtogroup SystemControlManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SYSTEMCONTROLMANAGE_DEF_H
#define _SYSTEMCONTROLMANAGE_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemControlManager/SystemControlManager_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#include "FreeRTOS/FreeRTOS.h"
#else
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
/// helper macro to create the entry in the system control table
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define  SYSCNTRLMNGRSCHD_DEF( task, mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15, func ) \
  { \
    .pxTaskHandle = task, \
    .tModes = \
    { \
      .tBits = \
      { \
        .bEnb00 = mode0, \
        .bEnb01 = mode1, \
        .bEnb02 = mode2, \
        .bEnb03 = mode3, \
        .bEnb04 = mode4, \
        .bEnb05 = mode5, \
        .bEnb06 = mode6, \
        .bEnb07 = mode7, \
        .bEnb08 = mode8, \
        .bEnb09 = mode9, \
        .bEnb10 = mode10, \
        .bEnb11 = mode11, \
        .bEnb12 = mode12, \
        .bEnb13 = mode13, \
        .bEnb14 = mode14, \
        .bEnb15 = mode15 \
      }, \
    }, \
    .pvCtrlFunc = func \
  }
#else
#define  SYSCNTRLMNGRSCHD_DEF( task, mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15, func ) \
  { \
    .eTaskEnum = task, \
    .tModes = \
    { \
      .tBits = \
      { \
        .bEnb00 = mode0, \
        .bEnb01 = mode1, \
        .bEnb02 = mode2, \
        .bEnb03 = mode3, \
        .bEnb04 = mode4, \
        .bEnb05 = mode5, \
        .bEnb06 = mode6, \
        .bEnb07 = mode7, \
        .bEnb08 = mode8, \
        .bEnb09 = mode9, \
        .bEnb10 = mode10, \
        .bEnb11 = mode11, \
        .bEnb12 = mode12, \
        .bEnb13 = mode13, \
        .bEnb14 = mode14, \
        .bEnb15 = mode15 \
      }, \
    }, \
    .pvCtrlFunc = func \
  }
#endif // SYSTEMDEFINE_OS_SELECTION

/// helper macro to create the entry in the system control table
#if ( TASK_TICK_ENABLE == 1 )
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
#define  SYSCNTRLMNGRTICK_DEF( task, mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15 ) \
{ \
  .pxTaskHandle = task, \
  .tModes = \
  { \
    .tBits = \
    { \
      .bEnb00 = mode0, \
      .bEnb01 = mode1, \
      .bEnb02 = mode2, \
      .bEnb03 = mode3, \
      .bEnb04 = mode4, \
      .bEnb05 = mode5, \
      .bEnb06 = mode6, \
      .bEnb07 = mode7, \
      .bEnb08 = mode8, \
      .bEnb09 = mode9, \
      .bEnb10 = mode10, \
      .bEnb11 = mode11, \
      .bEnb12 = mode12, \
      .bEnb13 = mode13, \
      .bEnb14 = mode14, \
      .bEnb15 = mode15 \
    }, \
  }, \
}
#else
#define  SYSCNTRLMNGRTICK_DEF( task, mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15 ) \
{ \
  .eTaskEnum = task, \
  .tModes = \
  { \
    .tBits = \
    { \
      .bEnb00 = mode0, \
      .bEnb01 = mode1, \
      .bEnb02 = mode2, \
      .bEnb03 = mode3, \
      .bEnb04 = mode4, \
      .bEnb05 = mode5, \
      .bEnb06 = mode6, \
      .bEnb07 = mode7, \
      .bEnb08 = mode8, \
      .bEnb09 = mode9, \
      .bEnb10 = mode10, \
      .bEnb11 = mode11, \
      .bEnb12 = mode12, \
      .bEnb13 = mode13, \
      .bEnb14 = mode14, \
      .bEnb15 = mode15 \
    }, \
  }, \
}
#endif // SYSTEMDEFINE_OS_SELECTION
#endif // TASK_TICK_ENABLE

/// define the helper macro for defining an table entry for entry/check
#define SYSCTRLMNGGR_ENTCHK( entry, check ) \
  { \
    .pvEntry = entry, \
    .pvCheck = check, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _SYSCTRLMGRERR
{
  SYSCTRLMNGR_ERROR_NONE = 0,               ///< no error
  SYSCTRLMNGR_ERROR_ILL_MODE,               ///< illegal mode
  SYSCTRLMNGR_ERROR_ILL_MODE_OUT_OF_RANGE,  ///< mode out of range
} SYSCTRLMGRERR;

/// enumerate the modes
typedef enum _SYSCTRLMGRMODE
{
  SYSCTRLMNGR_MODE_00_INITIALIZE = 0,     ///< initialization
  SYSCTRLMNGR_MODE_01_CONFIGURATION,      ///< configuration
  SYSCTRLMNGR_MODE_02_POWERRECOVERY,      ///< power recovery
  SYSCTRLMNGR_MODE_03_STANDBY,            ///< standby
  SYSCTRLMNGR_MODE_04_IDLE,               ///< idle
  SYSCTRLMNGR_MODE_05_RUN,                ///< run
  SYSCTRLMNGR_MODE_06_POWERLOSS,          ///< power loss
  SYSCTRLMNGR_MODE_07_ERROR,              ///< error
  SYSCTRLMNGR_MODE_08_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_09_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_10_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_11_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_12_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_13_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_14_UNDEF,              ///< undefined
  SYSCTRLMNGR_MODE_15_MANUFACTURING,      ///< manufacturing
  SYSCTRLMNGR_MODE_MAX,
  SYSCTRLMNGR_MODE_ILLEGAL
} SYSCTRLMGRMODE;

/// enumerate the events
typedef enum _SYSCTLMNGREVENT
{
  SYSCTRLMNGR_EVENT_CONFIGDONE = 0x20,  ///< config done
  SYSCTRLMNGR_EVENT_POWERLOSS,          ///< power loss
  SYSCTRLMNGR_EVENT_ERRORDET,           ///< error detected
  SYSCTRLMNGR_EVENT_GOIDLE,             ///< go to the idle state
  SYSCTRLMNGR_EVENT_GORUN,              ///< go to the run state
  SYSCTRLMNGR_EVENT_INITDONE,           ///< initialization done
} SYSCTLMNGREVENT;


// structures -----------------------------------------------------------------
/// define the control function declarations
typedef void  ( *PVSYSCTRLCTRLFUNC )( BOOL );

/// define the entry task function declarations
typedef void  ( *PVSYSCTRLENTRYFUNC )( void );

/// define the check for go to other state function declaration
typedef BOOL  ( *PVSYSCTRLCHKFUNC )( SYSCTRLMGRMODE );

/// define the control structure
typedef struct PACKED _SYSCTRLMNGRSCHDDEF
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  TaskHandle_t pxTaskHandle;
  #else
  TASKSCHDENUMS	eTaskEnum;      ///< task enumeration
  #endif // SYSTEMDEFINE_OS_SELECTION
  union
  {
    struct
    {
      U16   bEnb00 : 1;         ///< mode  0 enable flag
      U16   bEnb01 : 1;         ///< mode  1 enable flag
      U16   bEnb02 : 1;         ///< mode  2 enable flag
      U16   bEnb03 : 1;         ///< mode  3 enable flag
      U16   bEnb04 : 1;         ///< mode  4 enable flag
      U16   bEnb05 : 1;         ///< mode  5 enable flag
      U16   bEnb06 : 1;         ///< mode  6 enable flag
      U16   bEnb07 : 1;         ///< mode  7 enable flag
      U16   bEnb08 : 1;         ///< mode  8 enable flag
      U16   bEnb09 : 1;         ///< mode  9 enable flag
      U16   bEnb10 : 1;         ///< mode 10 enable flag
      U16   bEnb11 : 1;         ///< mode 11 enable flag
      U16   bEnb12 : 1;         ///< mode 12 enable flag
      U16   bEnb13 : 1;         ///< mode 13 enable flag
      U16   bEnb14 : 1;         ///< mode 14 enable flag
      U16   bEnb15 : 1;         ///< mode 15 enable flag
    } tBits;
    U16 wData;
  } tModes;
  PVSYSCTRLCTRLFUNC pvCtrlFunc; ///< optional control function
} SYSCTRLMNGRSCHDDEF, *PSYSCTRLMNGRSCHDDEF;
#define SYSCTRLMNGRSCHDDEF_SIZE   sizeof( SYSCTRLMNGRSCHDDEF )

/// define the control structure
#if ( TASK_TICK_ENABLE == 1 )
typedef struct PACKED _SYSCTRLMNGRTICKDEF
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  TaskHandle_t pxTaskHandle;
  #else
  TASKTICKENUMS	eTaskEnum;      ///< task enumeration
  #endif // SYSTEMDEFINE_OS_SELECTION
  union
  {
    struct
    {
      U16   bEnb00 : 1;         ///< mode  0 enable flag
      U16   bEnb01 : 1;         ///< mode  1 enable flag
      U16   bEnb02 : 1;         ///< mode  2 enable flag
      U16   bEnb03 : 1;         ///< mode  3 enable flag
      U16   bEnb04 : 1;         ///< mode  4 enable flag
      U16   bEnb05 : 1;         ///< mode  5 enable flag
      U16   bEnb06 : 1;         ///< mode  6 enable flag
      U16   bEnb07 : 1;         ///< mode  7 enable flag
      U16   bEnb08 : 1;         ///< mode  8 enable flag
      U16   bEnb09 : 1;         ///< mode  9 enable flag
      U16   bEnb10 : 1;         ///< mode 10 enable flag
      U16   bEnb11 : 1;         ///< mode 11 enable flag
      U16   bEnb12 : 1;         ///< mode 12 enable flag
      U16   bEnb13 : 1;         ///< mode 13 enable flag
      U16   bEnb14 : 1;         ///< mode 14 enable flag
      U16   bEnb15 : 1;         ///< mode 15 enable flag
    } tBits;
    U16 wData;
  } tModes;
} SYSCTRLMNGRTICKDEF, *PSYSCTRLMNGRTICKDEF;
#define SYSCTRLMNGRTICKDEF_SIZE   sizeof( SYSCTRLMNGRTICKDEF )
#endif // TASK_TICK_ENABLE

/// define the structure for the local entry/check functions
typedef struct _SYSCTRLMGRENTCHK 
{
  PVSYSCTRLENTRYFUNC  pvEntry;
  PVSYSCTRLCHKFUNC    pvCheck;
} SYSCTRLMGRENTCHK, *PSYSCTRLMGRENTCHK;
#define SYSCTRLMGRENTCHK_SIZE   sizeof( SYSCTRLMGRENTCHK )

/**@} EOF SystemControlManager_def.h */

#endif  // _SYSTEMCONTROLMANAGE_DEF_H