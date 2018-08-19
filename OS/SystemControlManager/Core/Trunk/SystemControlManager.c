/******************************************************************************
 * @file SystemControlManager.c
 *
 * @brief system control manager implementation
 *
 * This file provides the system control manager implementation
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemControlManager/SystemControlManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  STATEEXECENGCONTROL tStateControl;

// local function prototypes --------------------------------------------------
static  BOOL  CheckForModeExecution( SYSCTRLMGRMODE eNewMode,  SYSCTRLMGRMODE eCurMode );
static  void  SetTaskStates( void );
static  U8    Mode00_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_00_INITIALIZE
static  U8    Mode01_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_01_CONFIGURATION
static  U8    Mode02_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_02_POWERRECOVERY
static  U8    Mode03_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_03_STANDBY
static  U8    Mode04_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_04_IDLE
static  U8    Mode05_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_05_RUN
static  U8    Mode06_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_06_POWERLOSS
static  U8    Mode07_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_07_ERROR
static  U8    Mode08_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_08_UNDEF
static  U8    Mode09_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_09_UNDEF
static  U8    Mode10_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_10_UNDEF
static  U8    Mode11_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_11_UNDEF
static  U8    Mode12_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_12_UNDEF
static  U8    Mode13_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_13_UNDEF
static  U8    Mode14_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_14_UNDEF
static  U8    Mode15_Execute( STATEEXECENGARG xArg );  // SYSCTRLMNGR_MODE_15_UNDEF

/// command handlers
#if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
static  ASCCMDSTS CmdQryMod( U8 nCmdEnum );
static  ASCCMDSTS CmdSetMod( U8 nCmdEnum );
static  ASCCMDSTS CmdExtSpc( U8 nCmdEnum );
static  ASCCMDSTS CmdEntDag( U8 nCmdEnum );
static  ASCCMDSTS CmdEntMan( U8 nCmdEnum );
#endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS

// constant parameter initializations -----------------------------------------
/// declare the events for all modes except error
static  const CODE STATEEXECENGEVENT  atAnyEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( SYSCTRLMNGR_EVENT_POWERLOSS, SYSCTRLMNGR_MODE_06_POWERLOSS,  FALSE ),
  STATEEXECENGEVENT_ENTRY( SYSCTRLMNGR_EVENT_ERRORDET,  SYSCTRLMNGR_MODE_07_ERROR,      FALSE ),
  STATEEXECENGEVENT_ENTRY( 0,                           0,                              FALSE ),
};

/// delare the state table
static  const CODE STATEEXECENGTABLE  atStates[ SYSCTRLMNGR_MODE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_00_INITIALIZE,    SetTaskStates, Mode00_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_01_CONFIGURATION, SetTaskStates, Mode01_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_02_POWERRECOVERY, SetTaskStates, Mode02_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_03_STANDBY,       SetTaskStates, Mode03_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_04_IDLE,          SetTaskStates, Mode04_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_05_RUN,           SetTaskStates, Mode05_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_06_POWERLOSS,     SetTaskStates, Mode06_Execute, NULL, NULL         ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_07_ERROR,         SetTaskStates, Mode07_Execute, NULL, NULL         ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_08_UNDEF,         SetTaskStates, Mode08_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_09_UNDEF,         SetTaskStates, Mode09_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_10_UNDEF,         SetTaskStates, Mode10_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_11_UNDEF,         SetTaskStates, Mode11_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_12_UNDEF,         SetTaskStates, Mode12_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_13_UNDEF,         SetTaskStates, Mode13_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_14_UNDEF,         SetTaskStates, Mode14_Execute, NULL, &atAnyEvents ),
  STATEXECENGETABLE_ENTRY( SYSCTRLMNGR_MODE_15_UNDEF,         SetTaskStates, Mode15_Execute, NULL, &atAnyEvents )
};

/// command strings
#if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
static  const CODE C8 szQryMod[ ]   = { "QMOD" };
static  const CODE C8 szSetMod[ ]   = { "SMOD" };
static  const CODE C8 szEntDag[ ]   = { "DIAG" };
static  const CODE C8 szEntMan[ ]   = { "MANL" };
static  const CODE C8 szExtSpc[ ]   = { "EXIT" };

/// initialize the command table
const CODE ASCCMDENTRY atSysCtrlMngrCmdHandlerTable[ ] =
{
  ASCCMD_ENTRY( szQryMod, 4, 0, ASCFLAG_COMPARE_NONE, 0,                               CmdQryMod ),
  ASCCMD_ENTRY( szSetMod, 4, 1, ASCFLAG_COMPARE_NONE, 0,                               CmdSetMod ),
  ASCCMD_ENTRY( szEntDag, 4, 0, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_IDLE,        CmdEntDag ),
  ASCCMD_ENTRY( szEntMan, 4, 0, ASCFLAG_COMPARE_EQ,   SYSCTRLMNGR_LCLMODE_IDLE,        CmdEntMan ),
  ASCCMD_ENTRY( szExtSpc, 4, 0, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdExtSpc ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/// define the response strings
static  const CODE C8 szRspMod[ ]   = { "RMOD, %3d:0x%02X\n\r" };
#endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS

/******************************************************************************
 * @function SystemControlManager_Initialize
 *
 * @brief initilize the system control manager
 *
 * This function will perform the initialization of the system control manager
 * and enter the initialization state
 *
 *****************************************************************************/
void SystemControlManager_Initialize( void )
{
  // set the current mode
  tStateControl.nCurState = SYSCTRLMNGR_MODE_00_INITIALIZE;
  
  // set the state table in the state control structure
  tStateControl.ptStates = ( STATEEXECENGTABLE const* )&atStates;
  
  // initialize the state controller
  StateExecutionEngine_Initialize( &tStateControl, SYSCTRLMNGR_MODE_00_INITIALIZE );
  
  // call the local initialization
  SystemControlManager_LocalInitialize( );
}

/******************************************************************************
 * @function SystemControlManager_ProcessEvent
 *
 * @brief event handler
 *
 * This function will process the incoming event and change states if permitted
 *
 * @param[in]   xArg    event argument to proces
 *
 *
 *****************************************************************************/
void SystemControlManager_ProcessEvent( STATEEXECENGARG xArg )
{
  // process the event
  StateExecutionEngine_Process( &tStateControl, xArg );
}

/******************************************************************************
 * @function SystemControlManager_SetMode
 *
 * @brief request a new mode of operation
 *
 * This function will request a new mode of operation and if it can be acheived
 * it will return an indication of success/failure
 *
 * @param[in]   eMode     new mode
 *
 * @return      error indicating success/failure
 *
 *****************************************************************************/
SYSCTRLMGRERR SystemControlManager_SetMode( SYSCTRLMGRMODE eMode )
{
  SYSCTRLMGRERR eError;
  
  // is this a valid mode
  if ( eMode < SYSCTRLMNGR_MODE_MAX )
  {
    // ensure we are not in an error condition
    StateExecutionEngine_Process( &tStateControl, eMode );

    // determine if we switched states
    if ( tStateControl.nCurState == eMode )
    {
      // no error
      eError = SYSCTRLMNGR_ERROR_NONE;
    }
    else
    {
      // report the error
      eError = SYSCTRLMNGR_ERROR_ILL_MODE;
    }
  }
  else
  {
    eError = SYSCTRLMNGR_ERROR_ILL_MODE_OUT_OF_RANGE;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function SystemControlManager_GetMode
 *
 * @brief get the current mode
 *
 * This function will return the current mode of operation
 *
 * @return      current mode of operation
 *
 *****************************************************************************/
SYSCTRLMGRMODE SystemControlManager_GetMode( void )
{
  // return the current mode
  return(( SYSCTRLMGRMODE )tStateControl.nCurState );
}

/******************************************************************************
 * @function CheckForModeExecution
 *
 * @brief calls an external function to test for new mode allowed
 *
 * This function will return TRUE if mode is allowed
 *
 * @return      TRUE - switch to mode, FALSE - do not switch
 *
 *****************************************************************************/
static BOOL CheckForModeExecution( SYSCTRLMGRMODE eNewMode, SYSCTRLMGRMODE eCurMode )
{
  BOOL              bStatus = TRUE;
  PVSYSCTRLCHKFUNC  pvCheck;

  // is there a check handler for this mode
  if (( pvCheck = ( PVSYSCTRLCHKFUNC )PGM_RDWORD( atSysCtrlMngrEntChkFuncs[ eCurMode ].pvCheck )) != NULL )
  {
    // call the check function
    bStatus = pvCheck( eNewMode );
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function SetTaskStates
 *
 * @brief set task states
 *
 * This function will set each tasks's run condition based on the entry
 * in the table
 *
 *****************************************************************************/
static void SetTaskStates( void )
{
  U8                      nTaskIndex;
  U16                     wData;
  SYSCTRLMNGRDEF const*   ptDef;
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  TaskHandle_t pxTaskHandle;
  #else
  TASKSCHDENUMS	eTaskEnum;      ///< task enumeration
  #endif // SYSTEMDEFINE_OS_SELECTION
  BOOL                    bState;
  PVSYSCTRLCTRLFUNC       pvCtrlFunc;
  PVSYSCTRLENTRYFUNC      pvEntryFunc;

  // for each item in the table
  for( nTaskIndex = 0; nTaskIndex < SystemControlManager_GetNumberDefs( ); nTaskIndex++ )
  {
    // get the entry
    ptDef = &atSysCtrlMngrDefs[ nTaskIndex ];

    // get the task/data
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    pxTaskHandle = PGM_RDWORD( ptDef->pxTaskHandle );
    #else
    eTaskEnum = PGM_RDBYTE( ptDef->eTaskEnum );
    #endif // SYSTEMDEFINE_OS_SELECTION
    wData = PGM_RDWORD( ptDef->tModes.wData );

    // calculate the state
    bState = (( wData & BIT( tStateControl.nCurState )) != 0 ) ? TRUE : FALSE;

    // call the task disable/enable with the appropriate state
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    if ( pxTaskHandle != NULL )
    {
      // suspend or resume the task
      if ( bState )
      {
        // resume task
        vTaskResume( pxTaskHandle );
      }
      else
      {
        // suspend task
        vTaskSuspend( pxTaskHandle );
      }
    }
    #else
    if( eTaskEnum != TASK_SCHD_ILLEGAL )
    {
      // enable/disable based on state
      TaskManager_EnableDisable( eTaskEnum, bState );
    }
    #endif // SYSTEMDEFINE_OS_SELECTION

    // get the function pointer/if not null/execute with state
    if (( pvCtrlFunc = ( PVSYSCTRLCTRLFUNC )PGM_RDWORD( ptDef->pvCtrlFunc )) != NULL )
    {
      // execute it
      pvCtrlFunc( bState );
    }
  }

  // execute the additional entry functions if not null
  if (( pvEntryFunc = ( PVSYSCTRLENTRYFUNC )PGM_RDWORD( atSysCtrlMngrEntChkFuncs[ tStateControl.nCurState ].pvEntry )) != NULL )
  {
    // execute it
    pvEntryFunc( );
  }
}

/******************************************************************************
 * @function Mode00_Execute
 *
 * @brief mode 01 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode00_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState = STATEEXECENG_STATE_NONE;

  // check for initialization done
  if ( xArg == SYSCTRLMNGR_EVENT_INITDONE )
  {
    // for now return the configuration state
    nNewState = SYSCTRLMNGR_MODE_01_CONFIGURATION;
  }

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode01_Execute
 *
 * @brief mode 01 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode01_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;

  // process the event
  switch( xArg )
  {
    case SYSCTRLMNGR_EVENT_CONFIGDONE :
      nNewState = SYSCTRLMNGR_MODE_02_POWERRECOVERY;
      break;

    default :
      nNewState = STATEEXECENG_STATE_NONE;
      break;
  }

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode02_Execute
 *
 * @brief mode 02 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode02_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState = STATEEXECENG_STATE_NONE;

  // process the event
  switch( xArg )
  {
    case SYSCTRLMNGR_EVENT_GOIDLE :
      nNewState = SYSCTRLMNGR_MODE_04_IDLE;
      break;
        
    case SYSCTRLMNGR_EVENT_GORUN :
      nNewState = SYSCTRLMNGR_MODE_05_RUN;
      break;
    
    default :
      nNewState = STATEEXECENG_STATE_NONE;
      break;
  }

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode03_Execute
 *
 * @brief mode 03 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode03_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState = STATEEXECENG_STATE_NONE;

  // process the event
  switch( xArg )
  {
    case SYSCTRLMNGR_EVENT_GOIDLE :
      nNewState = SYSCTRLMNGR_MODE_04_IDLE;
      break;
  
    default :
      nNewState = STATEEXECENG_STATE_NONE;
      break;
  }

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode04_Execute
 *
 * @brief mode 04 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode04_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;

  // process the event
  switch( xArg )
  {
    case SYSCTRLMNGR_EVENT_GORUN :
    case SYSCTRLMNGR_MODE_05_RUN :
      nNewState = SYSCTRLMNGR_MODE_05_RUN;
      break;
      
    case SYSCTRLMNGR_MODE_08_UNDEF :
    case SYSCTRLMNGR_MODE_09_UNDEF :
    case SYSCTRLMNGR_MODE_10_UNDEF :
    case SYSCTRLMNGR_MODE_11_UNDEF :
    case SYSCTRLMNGR_MODE_12_UNDEF :
    case SYSCTRLMNGR_MODE_13_UNDEF :
    case SYSCTRLMNGR_MODE_14_UNDEF :
    case SYSCTRLMNGR_MODE_15_UNDEF :
      nNewState = xArg;
      break;

    default :
      nNewState = STATEEXECENG_STATE_NONE;
      break;
  }

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode05_Execute
 *
 * @brief mode 05 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode05_Execute( STATEEXECENGARG xArg )
{
  U8                nNewState;

  // process the event
  switch( xArg )
  {
    case SYSCTRLMNGR_EVENT_GOIDLE :
    case SYSCTRLMNGR_MODE_04_IDLE :
      nNewState = SYSCTRLMNGR_MODE_04_IDLE;
      break;
      
    default :
      nNewState = STATEEXECENG_STATE_NONE;
      break;
  }

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode06_Execute
 *
 * @brief mode 07 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode06_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState = STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode07_Execute
 *
 * @brief mode 07 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode07_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState = STATEEXECENG_STATE_NONE;

  // this is our error state - loop here forever
  FOREVER
  {
    // we do not nothing for now
  }

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode08_Execute
 *
 * @brief mode 08 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode08_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode09_Execute
 *
 * @brief mode 09 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode09_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode10_Execute
 *
 * @brief mode 10 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode10_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode11_Execute
 *
 * @brief mode 11 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode11_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode12_Execute
 *
 * @brief mode 12 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode12_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode13_Execute
 *
 * @brief mode 13 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode13_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode14_Execute
 *
 * @brief mode 14 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode14_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

/******************************************************************************
 * @function Mode15_Execute
 *
 * @brief mode 15 execution
 *
 * This function will process the event for this mode
 *
 * @param[in]   xArg      event argument
 *
 *****************************************************************************/
static U8 Mode15_Execute( STATEEXECENGARG xArg )
{
  U8 nNewState;
  
  // set the new state to the argument
  nNewState = ( xArg < SYSCTRLMNGR_MODE_MAX ) ? xArg : STATEEXECENG_STATE_NONE;

  // check for allowed
  nNewState = ( CheckForModeExecution( nNewState, tStateControl.nCurState )) ? nNewState : STATEEXECENG_STATE_NONE;

  // return the new state
  return( nNewState );
}

#if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == 1 )
/******************************************************************************
 * @function CmdWryMod
 *
 * @brief querys the current mode
 *
 * This function will query the mode
 *
 * @return  appropriate status
  *****************************************************************************/
static ASCCMDSTS CmdQryMod( U8 nCmdEnum )
{
  SYSCTRLMGRMODE  eMode;
  PC8             pcBuffer;

  // fetch it 
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the current system control mode
  eMode = SystemControlManager_GetMode( );

  // output the result
  SPRINTF_P( pcBuffer, ( char const * )szRspMod, eMode, eMode );

  // return status
  return( ASCCMD_STS_OUTPUTBUFFER );
}

/******************************************************************************
 * @function CmdSetMod
 *
 * @brief sets the current mode
 *
 * This function will attempt to set the mode
 *
 * @return  appropriate status
  *****************************************************************************/
static ASCCMDSTS CmdSetMod( U8 nCmdEnum )
{
  ASCCMDSTS       eStatus = ASCCMD_STS_NONE;
  U32UN           tTemp;
  SYSCTRLMGRERR   eError;
  PC8             pcBuffer;

  // fetch it 
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // get the address/data
  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tTemp.uValue );

  // set the mode
  if (( eError = SystemControlManager_SetMode(( SYSCTRLMGRMODE )tTemp.anValue[ LE_U32_LSB_IDX ] )) != SYSCTRLMNGR_ERROR_NONE )
  {
    // output the error
    SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
    eStatus = ASCCMD_STS_OUTPUTBUFFER;
  }

  // return status
  return( eStatus );
}

/******************************************************************************
 * @function CmdExtSpc
 *
 * @brief exits the diagnostic mode
 *
 * This function will exit the diagnostic mode
 *
 * @return  nothing
 *****************************************************************************/
static ASCCMDSTS CmdExtSpc( U8 nCmdEnum )
{
  SYSCTRLMGRMODE  eMode;
  PC8             pcBuffer;

  // fetch it 
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // go back ot idle
  SystemControlManager_SetMode( SYSCTRLMNGR_LCLMODE_IDLE );

  // get the current system control mode
  eMode = SystemControlManager_GetMode( );

  // output the result
  SPRINTF_P( pcBuffer, ( char const * )szRspMod, eMode, eMode );

  // change the prompt character
  AsciiCommandHandler_SetPromptCharacter( nCmdEnum, 0 );

  // return status
  return( ASCCMD_STS_OUTPUTBUFFER );
};

/******************************************************************************
 * @function CmdEntDag
 *
 * @brief exits the diagnostic mode
 *
 * This function will exit the diagnostic mode
 *
 * @return  nothing
 *****************************************************************************/
static  ASCCMDSTS CmdEntDag( U8 nCmdEnum )
{
  ASCCMDSTS       eStatus = ASCCMD_STS_NONE;
  SYSCTRLMGRERR   eError;
  PC8             pcBuffer;

  // fetch it 
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // set the mode
  if (( eError = SystemControlManager_SetMode( SYSCTRLMNGR_LCLMODE_DIAGNOSTICS )) != SYSCTRLMNGR_ERROR_NONE )
  {
    // output the error
    SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
    eStatus = ASCCMD_STS_OUTPUTBUFFER;
  }

  // change the prompt character
  AsciiCommandHandler_SetPromptCharacter( nCmdEnum, '+' );

  // return status
  return( eStatus );
}

/******************************************************************************
 * @function CmdEntMan
 *
 * @brief exits the manual mode
 *
 * This function will exit the diagnostic mode
 *
 * @return  nothing
 *****************************************************************************/
static ASCCMDSTS CmdEntMan( U8 nCmdEnum )
{
  ASCCMDSTS       eStatus = ASCCMD_STS_NONE;
  SYSCTRLMGRERR   eError;
  PC8             pcBuffer;

  // fetch it 
  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  // set the mode
  if (( eError = SystemControlManager_SetMode( SYSCTRLMNGR_LCLMODE_DIAGNOSTICS )) != SYSCTRLMNGR_ERROR_NONE )
  {
    // output the error
    SPRINTF_P( pcBuffer, ( char const * )g_szAsciiErrStrn, eError, eError );
    eStatus = ASCCMD_STS_OUTPUTBUFFER;
  }

  // change the prompt character
  AsciiCommandHandler_SetPromptCharacter( nCmdEnum, '@' );

  // return status
  return( eStatus );
}
#endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS

/**@} EOF SystemControlManager.c */
