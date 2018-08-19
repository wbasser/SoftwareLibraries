/**************************************************************************//**
 * @file ScheduleManager.c
 *
 * @brief light control manager implementation
 *
 * This file provides the implementation for the light control manager
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
 * @defgroup schedulerManager Schedule Manager
 * @ingroup applicationGroup
 * \addtogroup schedulerManager 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ScheduleManager/ScheduleManager.h"
#include "ScheduleManager/ScheduleManager_prm.h"

// library includes -----------------------------------------------------------
#include "ConfigManager/ConfigManager.h"
#if ( SCHDMNGR_ENABLE_DEBUG == 1 )
#include "DebugManager/DebugManager.h"
#endif // SCHDMMNGR_ENABLE_DEBUG

// Macros and Defines ---------------------------------------------------------
/// define the base event for the light control manager
#if ( SCHDMNGR_ENABLE_DEBUG == 1 )
#define SCHDMMNGR_DEBUG_BASE        ( 0x5000 )
#endif // SCHDMMNGR_ENABLE_DEBUG

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  SCHDMNGRDEFTBL      tSchedule;        ///< the acutal schedule
static  PSCHDMNGRDEFENTRY   ptSchedEntry;     ///< schedule entry
static  SCHDMNGRDEFENUM     eCurIndex;        ///< cyurrent index                  

// local function prototypes --------------------------------------------------
static  SCHDMNGRDEFENUM     FindScheduleEntry( void );
static  void                ProcessScheduleChange( SCHDMNGRDEFENUM eIndex );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ScheduleManager_Initialize
 *
 * @brief initialization
 *
 * This function will perform any initialization actions
 *
 *****************************************************************************/
void ScheduleManager_Initialize( void )
{
  // set the index to default
  eCurIndex = SCHDMNGRDEF_ENUM_DFLT;
}

/******************************************************************************
 * @function ScheduleManager_Control
 *
 * @brief control
 *
 * This function will enable/disable the schedule manager
 *
 * @param[in]   bState      current desired state
 *
 *****************************************************************************/
void ScheduleManager_Control( BOOL bState )
{
  SCHDMNGRDEFENUM eSchdEnum;

  // determine the rule
  eSchdEnum = ( bState == ON ) ? eCurIndex : SCHDMNGRDEF_ENUM_DFLT;

  // process a change
  ProcessScheduleChange( eSchdEnum );
}

/******************************************************************************
 * @function ScheduleManager_GetActlTable
 *
 * @brief get the actual rule table
 *
 * This function will return the actual rule table
 *
 * @return      pointer to the actual rule table
 *
 *****************************************************************************/
PSCHDMNGRDEFTBL ScheduleManager_GetActlTable( void )
{
 // return the pointer to the schedule table
  return( &tSchedule );
}

/******************************************************************************
 * @function ScheduleManager_GetDftlTable
 *
 * @brief get the default rule table
 *
 * This function will return the default rule table
 *
 * @return      pointer to the default rule table
 *
 *****************************************************************************/
PSCHDMNGRDEFTBL ScheduleManager_GetDfltTable( void )
{
  // return the pointer to the default table
  return(( PSCHDMNGRDEFTBL )&tSchdDefTable );
}


/******************************************************************************
 * @function ScheduleManager_SetSchdRule
 *
 * @brief set a schedule rule
 *
 * This function wiill set a schedule rule with appropriate entries at 
 * a given index
 *
 * @param[in]   eRuleIndex      rule index
 * @param[in]   ptRule          pointer to the rule entry
 * @param[in]   bUpdateNVRom    flag to force an update of the NV rom
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SCHDMNGRERROR ScheduleManager_SetSchdRule( SCHDMNGRDEFENUM eRuleIndex, PSCHDMNGRDEFENTRY ptRule, BOOL bUpdateNVRom )
{
  SCHDMNGRERROR  eError = SCHDMNGR_ERROR_NONE;

  // check for valid rule
  if ( eRuleIndex < SCHDMNGRDEF_ENUM_MAX )
  {
    // copy the data
    memcpy(( PSCHDMNGRDEFENTRY )&tSchdDefTable.atRules[ eRuleIndex ], ptRule, SCHDMNGRDEFENTRY_SIZE );

    // check for an update of the NV
    if ( bUpdateNVRom )
    {
      // post an event
      TaskManager_PostEvent( SCHDMNGR_CONFIGCHANGE_ENUM, CONFIG_TYPE_SCHDMNGR );
    }
  }
  else
  {
    // report the error
    eError = SCHDMNGR_ERROR_ILLRULEINDEX;
  }
}

/******************************************************************************
 * @function ScheduleManager_GetSchdRule
 *
 * @brief get a rule
 *
 * This function will return the rule at a given index
 *
 * @param[in]   eRuleIndex      rule index
 * @param[in]   ptRule          pointer to the rule entry
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SCHDMNGRERROR ScheduleManager_GetSchdRule( SCHDMNGRDEFENUM eRuleIndex, PSCHDMNGRDEFENTRY* pptRule )
{
  SCHDMNGRERROR  eError = SCHDMNGR_ERROR_NONE;

  // check for valid rule
  if ( eRuleIndex < SCHDMNGRDEF_ENUM_MAX )
  {
    // set the pointer to the entry
    *( pptRule ) = ( PSCHDMNGRDEFENTRY )&tSchdDefTable.atRules[ eRuleIndex ];
  }
  else
  {
    // report the error
    eError = SCHDMNGR_ERROR_ILLRULEINDEX;
  }
}

/******************************************************************************
 * @function ScheduleManager_SetScheduleIndex
 *
 * @brief set the current schedule rule index
 *
 * This function will set the current schedule rule index
 *
 * @param[in]   eScheduleIndex    rule index
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SCHDMNGRERROR ScheduleManager_SetScheduleIndex( SCHDMNGRDEFENUM eScheduleIndex )
{
  SCHDMNGRERROR eStatus = SCHDMNGR_ERROR_ILLRULEINDEX;

  // check for invalid RTC
  if ( ScheduleManager_CheckValidDateTime( ) == FALSE )
  {
    // check for valid index
    if (( eScheduleIndex < SCHDMNGRDEF_ENUM_MAX ) && ( eScheduleIndex != eCurIndex ))
    {
      // now check for valid entry
      if ( PGM_RDBYTE( tSchedule.atEntries[ eScheduleIndex ].tStartTime.nDayOfWeek ) != 0xFF )
      {
        // process the schedule change
        ProcessScheduleChange( eScheduleIndex );

        // set good status
        eStatus = SCHDMNGR_ERROR_NONE;
      }
    }
  }

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function ScheduleManager_ForceNVRomUpdate
 *
 * @brief force an NV ROM update
 *
 * This function will force an NV rom update
 *
 *****************************************************************************/
void ScheduleManager_ForceNVRomUpdate( void )
{
  // force an update
  TaskManager_PostEvent( SCHDMNGR_CONFIGCHANGE_ENUM, CONFIG_TYPE_SCHDMNGR );
}

/******************************************************************************
 * @function ScheduleManager_GetScheduleIndex
 *
 * @brief get the current rule index
 *
 * This function will return the current rule index
 *
 *****************************************************************************/
U8 ScheduleManager_GetScheduleIndex( void )
{
  // return the schedule index
  return(( U8 )eCurIndex );
}

/******************************************************************************
 * @function ScheduleManager_ProcessEvent
 *
 * @brief process event handler
 *
 * This function will process the posted event
 *
 * @param[in]   xArg      event arguent
 *
 * @return      TRUE to always fflush event
 *
 *****************************************************************************/
BOOL ScheduleManager_ProcessEvent( TASKARG xArg )
{
   SCHDMNGRDEFENUM eNewIndex;

  // add the event for debug
  #if ( SCHDMNGR_ENABLE_DEBUG == 1 )
  DebugManager_AddElement( SCHDMMNGR_DEBUG_BASEE | tStateCtl.nCurState, xArg  );
  #endif // SCHDMMNGR_DEBUG_BASE

  // process the timeout event
  if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // find a schedule entry
    eNewIndex = FindScheduleEntry( );

    // check to see if different
    if ( eNewIndex != eCurIndex )
    {
      // process the new schedule entry
      ProcessScheduleChange( eNewIndex );
    }
  }

  // return the status
  return( TRUE );
}

/******************************************************************************
 * @function   ProcessScheduleChange
 *
 * @brief lprocess a schedule change
 *
 * This function will iterate through all rules and process the change to
 * to the schedule
 *
 * @param[in]   eIndex    durrent rule index
 *
 *****************************************************************************/
static void ProcessScheduleChange( SCHDMNGRDEFENUM eIndex )
{
  SCHDMNGRRULENTRY  ptRUleEntry;
  PVSETRULEINDEX    pvSetRuleIndex;
  SCHDMNGREXTENUM   eExternalIndex;
  SCHDRULEINDEXTYPE tRuleIndex;
  
  // set the current to the new/set the pointer
  eCurIndex = eIndex;

  // now for all external rules
  for( eExternalIndex = 0; eExternalIndex < SCHDMNGREXT_ENUM_MAX; eExternalIndex++ )
  {
    // now fetch the set rule function
    pvSetRuleIndex = ( PVSETRULEINDEX )PGM_RDWORD( tSchdExtTable.atExtRules[ eIndex ].pvSetRuleIndex );

    // get the rule index
    tRuleIndex = PGM_RDBYTE( tSchedule.atRules[ eIndex ].atRuleIndex[ eExternalIndex ]);

    // now call the function
    pvSetRuleIndex( tRuleIndex );
  }
}

/******************************************************************************
 * @function FindScheduleEntry
 *
 * @brief find a schedule entry which matches our current time
 *
 * This function will attempt to find a schedule entry that matches our 
 * current time
 *
 * @return      rule index
 *
 *****************************************************************************/
static SCHDMNGRDEFENUM FindScheduleEntry( void )
{
  SCHDMNGRDEFENTRY  tEntry;
  SCHDMNGRDEFENUM   eIndex;
  DATETIME          tTime;
  U16               wCurTime;

  // check for valid RTC time
  if ( ScheduleManager_CheckValidDateTime( ) == TRUE )
  {
    // get the current time
    ScheduleManager_GetDateTime( &tTime );
    wCurTime = SCHD_TIME_MINS( tTime.nHours, tTime.nMinutes );

    // for each schedule entry - search in reverse order
    for ( eIndex = SCHDMNGRDEF_ENUM_MAX; eIndex > 0; eIndex-- )
    {
      // copy the data
      MEMCPY_P( &tEntry, &tSchedule.atEntries[ eIndex ], SCHDMNGRDEFENTRY_SIZE );

      // first test for a valid entry
      if ( tEntry.tStartTime.nDayOfWeek != SCHDMNGR_DOW_ILLEGAL )
      {
        // compare for day of week
        if (( tTime.nDayOfWeek >= tEntry.tStartTime.nDayOfWeek ) && ( tTime.nDayOfWeek <= tEntry.tStopTime.nDayOfWeek ))
        {
          // compare for hour
          if (( wCurTime >= tEntry.tStartTime.wHrsMins ) && ( wCurTime <= tEntry.tStopTime.wHrsMins ))
          {
            // break out of the search index
            break;
          }
        }
      }
    }
  }
  else
  {
    // return the default index
    eIndex = SCHDMNGRDEF_ENUM_DFLT;
  }

  // return the indes
  return( eIndex );
}

/**@} EOF .c */
