/******************************************************************************
 * @file SpecialCommands
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ====== 
 * $Rev: $
 * 
 *
 * \addtogroup DALIMessageHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ParameterManager/ParameterManager.h"

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/SpecialCommands/SpecialCommands.h"

// Macros and Defines ---------------------------------------------------------
/// define the EOL test  values
#define EOL_TEST_SEQ1_VAL    ( 0xDE )
#define EOL_TEST_SEQ2_VAL    ( 0xAD )

/// define the search address reset value
#define SEARCH_ADDR_RESET    ( 0xFF )

// enumerations ---------------------------------------------------------------
/// enumerate the EOL Test sequence
typedef enum  _EOLTEST
{
  EOL_TEST_SEQ1 = 0,
  EOL_TEST_SEQ2
} EOLTEST;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  EOLTEST eEolTest;

// local function prototypes --------------------------------------------------
static  void	CmdTerminate( PDALICTL ptDaliCtl );
static  void	CmdDataTransReg( PDALICTL ptDaliCtl );
static  void	CmdInitialize( PDALICTL ptDaliCtl );
static  void	CmdRandomize( PDALICTL ptDaliCtl );
static  void	CmdCompare( PDALICTL ptDaliCtl );
static  void	CmdWithdraw( PDALICTL ptDaliCtl );
static  void	CmdSearchAddrH( PDALICTL ptDaliCtl );
static  void	CmdSearchAddrM( PDALICTL ptDaliCtl );
static  void	CmdSearchAddrL( PDALICTL ptDaliCtl );
static  void	CmdPgmAddr( PDALICTL ptDaliCtl );
static  void	CmdVerAddr( PDALICTL ptDaliCtl );
static  void	CmdQryAddr( PDALICTL ptDaliCtl );
static  void	CmdPhysSelect( PDALICTL ptDaliCtl );

// constant parameter initializations -----------------------------------------
/// define the special commands
const CODE CMDTBL	atScplCmds[ ] =
{
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdTerminate ),					// 0
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdDataTransReg ),       // 1
  CMDTBL_NORMENT( TRUE,  RESPONSE_TYPE_NONE, CmdInitialize ),         // 2
  CMDTBL_SPCLENT( TRUE,  RESPONSE_TYPE_NONE, CmdRandomize ),          // 3
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_FLAG, CmdCompare ),            // 4
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_NONE, CmdWithdraw ), 					// 5
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 6
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 7
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_NONE, CmdSearchAddrH ),				// 8
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_NONE, CmdSearchAddrM ),				// 9
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_NONE, CmdSearchAddrL ),				// 10
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_NONE, CmdPgmAddr ),						// 11
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_FLAG, CmdVerAddr ),						// 12
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_FLAG, CmdQryAddr ),						// 13
  CMDTBL_SPCLENT( FALSE, RESPONSE_TYPE_NONE, CmdPhysSelect ),         // 14
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 15
};

/******************************************************************************
 * @function SpecialCommands_Initialize
 *
 * @brief	initialize the special command handler
 *
 * This function will perform any neccesary intialization for the special
 * command handler
 *
 *****************************************************************************/
void SpecialCommands_Initialize( void )
{
  // reset the EOL test sequence
  eEolTest = EOL_TEST_SEQ1;
}

/******************************************************************************
 * @function CmdTerminate
 *
 * @brief	terminate any of the special commands
 *
 * This function will terminate
 *
 *****************************************************************************/
static void	CmdTerminate( PDALICTL ptDaliCtl )
{
  // stop any timer execution
  TaskManager_StopTimer( TASK_SCHD_DALITIME );

  // clear flags
  ptDaliCtl->tFlags.b15MinTimerRunning = FALSE;
  ptDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;
}

/******************************************************************************
 * @function CmdDataTransReg
 *
 * @brief store the data into the data transfer register
 *
 * This function will move the data into the data transfer register
 *
 *****************************************************************************/
static void	CmdDataTransReg( PDALICTL ptDaliCtl )
{
  // store the data value into the DTR
  ptDaliCtl->nDataTransReg = ptDaliCtl->tCurMessage.tFields.nData;
  
  // check to see if we are in EOL window and if the DTR equal 0xE1
  if ( SystemControlManager_IsEOLTestEnabled( ) == TRUE )
  {
    // determine the state
    switch( eEolTest )
    {
      case EOL_TEST_SEQ1 :
        // check for the first sequence
        if ( ptDaliCtl->nDataTransReg == EOL_TEST_SEQ1_VAL )
        {
          // advance state
          eEolTest = EOL_TEST_SEQ2;
        }
        break;

      case EOL_TEST_SEQ2 :
        // check for second sequence
        if ( ptDaliCtl->nDataTransReg == EOL_TEST_SEQ2_VAL )
        {
          // change mode to EOL test
          SystemControlManager_SetMode( SYSCTRLMNGR_LCLMODE_EOLTEST ); 
        }
        else
        {
          // back to first state
          eEolTest = EOL_TEST_SEQ1;
        }
        break;

      default :
        // reset the sequence
        eEolTest = EOL_TEST_SEQ1;
        break;
    }
  }
}

/******************************************************************************
 * @function CmdInitialize
 *
 * @brief initialization command
 *
 * This function will start the 15 minute timer, set the compare flag to TRUE
 *
 *****************************************************************************/
static void	CmdInitialize( PDALICTL ptDaliCtl )
{
  // check for a proper address
  if (( ptDaliCtl->tCurMessage.tFields.nData == 0 ) || 
      (( ptDaliCtl->tCurMessage.tFields.nData == 0xFF ) && ( ptDaliCtl->tDaliSts.tBits.bMissingShortAddr == TRUE )) ||
      ( ptDaliCtl->tCurMessage.tFields.nData == (( ptDaliCtl->tDaliPrm.nShortAddress << 1 ) | 0x01 )))
  {
    // start the special timer/set the compare flag/set the 15min timer flag
    TaskManager_StartTimer( TASK_SCHD_DALITIME, 0 );
    ptDaliCtl->tFlags.bCompareModeEnabled = TRUE;
    ptDaliCtl->tFlags.b15MinTimerRunning = TRUE;
  }
}

/******************************************************************************
 * @function CmdRandomize
 *
 * @brief randomize command
 *
 * This function will generate three random numbers and store them for further
 * use
 *
 *****************************************************************************/
static void	CmdRandomize( PDALICTL ptDaliCtl )
{
  PARAMARG xValue;
  
  // generate the random number
  ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] = DALIMessageHandler_GenerateRandom( );
  ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] = DALIMessageHandler_GenerateRandom( );
  ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] = DALIMessageHandler_GenerateRandom( );

  // compare to reset value
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_RANDOM_ADDR_H, &xValue );
  ptDaliCtl->tResetState.tBits.bRandAddrH = ( ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] != xValue ) ? TRUE : FALSE;
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_RANDOM_ADDR_M, &xValue );
  ptDaliCtl->tResetState.tBits.bRandAddrM = ( ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] != xValue ) ? TRUE : FALSE;
  ParameterManager_GetDefaultValue( PARAMSEL_DALI_RANDOM_ADDR_L, &xValue );
  ptDaliCtl->tResetState.tBits.bRandAddrL = ( ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] != xValue ) ? TRUE : FALSE;

  // update NVM
  ParameterManager_PutValue( PARAMSEL_DALI_RANDOM_ADDR_H, ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] );
  ParameterManager_PutValue( PARAMSEL_DALI_RANDOM_ADDR_M, ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] );
  ParameterManager_PutValue( PARAMSEL_DALI_RANDOM_ADDR_L, ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] );
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void	CmdCompare( PDALICTL ptDaliCtl )
{
  // set the default
  ptDaliCtl->tFlags.bResponseRequested = FALSE;

  // are we in compare mode
  if ( ptDaliCtl->tFlags.bCompareModeEnabled == TRUE )
  {
    // check for less than or equal
    if ( ptDaliCtl->tDaliPrm.tRandomAddr.uValue <= ptDaliCtl->tSearchAddr.uValue )
    {
      // set the response to true
      ptDaliCtl->nResponseValue = DALI_RESPONSE_YES;
      ptDaliCtl->tFlags.bResponseRequested = TRUE;
    }
  }
}

/******************************************************************************
 * @function CmdWithdraw
 *
 * @brief widthdraw this unit from the search process
 *
 * This function will check to see if the search address and the random address
 * are the same and if they are, then it will clear the compare flag
 *
 *****************************************************************************/
static void	CmdWithdraw( PDALICTL ptDaliCtl )
{
  // check for address match
  if ( ptDaliCtl->tDaliPrm.tRandomAddr.uValue == ptDaliCtl->tSearchAddr.uValue )
  {
    // address match, clear compare flag
    ptDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  }
}

/******************************************************************************
 * @function CmdSearchAddrH
 *
 * @brief search address high byte
 *
 * This function will store the data value to the search address high byte
 *
 *****************************************************************************/
static void	CmdSearchAddrH( PDALICTL ptDaliCtl )
{
  // store the value into the search address
  ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_H ] = ptDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  ptDaliCtl->tResetState.tBits.bSearchAddrH = ( ptDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_H ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdSearchAddrM
 *
 * @brief search address mid byte
 *
 * This function will store the data value to the search address mid byte
 *
 *****************************************************************************/
static void	CmdSearchAddrM( PDALICTL ptDaliCtl )
{
  // store the value into the search address
  ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_M ] = ptDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  ptDaliCtl->tResetState.tBits.bSearchAddrM = ( ptDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_M ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdSearchAddrL
 *
 * @brief search address low byte
 *
 * This function will store the data value to the search address low byte
 *
 *****************************************************************************/
static void	CmdSearchAddrL( PDALICTL ptDaliCtl )
{
  // store the value into the search address
  ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_L ] = ptDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  ptDaliCtl->tResetState.tBits.bSearchAddrL = ( ptDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_L ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdPgmAddr
 *
 * @brief program address
 *s
 * This function will program the passed address and set the appropriate
 * status bits as well as updating the NVM storage
 *
 *****************************************************************************/
static void	CmdPgmAddr( PDALICTL ptDaliCtl )
{
  // check for address match
  if (( ptDaliCtl->tDaliPrm.tRandomAddr.uValue == ptDaliCtl->tSearchAddr.uValue ) ||
			( ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED ))
  {
    // set the address/update the NVM
    ptDaliCtl->tDaliPrm.nShortAddress = ptDaliCtl->tCurMessage.tFields.nData >> 1;
    ParameterManager_PutValue( PARAMSEL_DALI_SHORT_ADDR, ptDaliCtl->tDaliPrm.nShortAddress );

    // set the appropriate status
    ptDaliCtl->tDaliSts.tBits.bMissingShortAddr = ( ptDaliCtl->tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;
  }
}

/******************************************************************************
 * @function CmdVerAddr
 *
 * @brief verify the address
 *
 * This function will compare the stored address with the passed and respond
 * with a YES if equal
 *
 *****************************************************************************/
static void	CmdVerAddr( PDALICTL ptDaliCtl )
{
  // determine if address's match
  if ( ptDaliCtl->tDaliPrm.nShortAddress == ( ptDaliCtl->tCurMessage.tFields.nData >> 1 ))
  {
    // set responce to YES
    ptDaliCtl->nResponseValue = DALI_RESPONSE_YES;
    ptDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  else
  {
    // force a negative response
    ptDaliCtl->tFlags.bResponseRequested = FALSE;
  }
}

/******************************************************************************
 * @function CmdQryAddr
 *
 * @brief query the stored address
 *
 * This function will return the current stored address
 *
 *****************************************************************************/
static void	CmdQryAddr( PDALICTL ptDaliCtl )
{
  // check for address match
  if (( ptDaliCtl->tDaliPrm.tRandomAddr.uValue == ptDaliCtl->tSearchAddr.uValue ) ||
			( ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED ))
  {
    // set the response value
    ptDaliCtl->nResponseValue = (( ptDaliCtl->tDaliPrm.nShortAddress ) << 1 | 0x01 );
    ptDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  else
  {
    // force a negative response
    ptDaliCtl->tFlags.bResponseRequested = FALSE;
  }
}

/******************************************************************************
 * @function CmdPhysSelect
 *
 * @brief physical selection
 *
 * This function will enabled/disable physical selection based on whether it
 * was already selected.  It will also enable/disable the compare mode
 *
 *****************************************************************************/
static void	CmdPhysSelect( PDALICTL ptDaliCtl )
{
  // check to see if we are disabled
  if ( ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_DISABLED )
  {
    // set the request to select/clear compare mode
    ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_REQUESTED;
    ptDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  }
  else
  {
    // clear the enabled state/enable compare mode
    ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;
    ptDaliCtl->tFlags.bCompareModeEnabled = TRUE;
  }
}

/**@} EOF SpecialCommands.c */
