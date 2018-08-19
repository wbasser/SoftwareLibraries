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

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler.h"
#include "DALIMessageHandler/SpecialCommands/SpecialCommands.h"

// library includes -----------------------------------------------------------

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
static  void	CmdTerminate(void );
static  void	CmdDataTransReg(void );
static  void	CmdInitialize(void );
static  void	CmdRandomize(void );
static  void	CmdCompare(void );
static  void	CmdWithdraw(void );
static  void	CmdSearchAddrH(void );
static  void	CmdSearchAddrM(void );
static  void	CmdSearchAddrL(void );
static  void	CmdPgmAddr(void );
static  void	CmdVerAddr(void );
static  void	CmdQryAddr(void );
static  void	CmdPhysSelect(void );

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
void SpecialCommands_Initialize(void )
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
static void	CmdTerminate(void )
{
  // stop any timer execution
  TaskManager_StopTimer( DALI_MESSAGE_TASK_ENUM_TIMER );

  // clear flags
  g_ptDaliCtl->tFlags.b15MinTimerRunning = FALSE;
  g_ptDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  g_ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;
}

/******************************************************************************
 * @function CmdDataTransReg
 *
 * @brief store the data into the data transfer register
 *
 * This function will move the data into the data transfer register
 *
 *****************************************************************************/
static void	CmdDataTransReg(void )
{
  // store the data value into the DTR
  g_ptDaliCtl->nDataTransReg = g_ptDaliCtl->tCurMessage.tFields.nData;
}

/******************************************************************************
 * @function CmdInitialize
 *
 * @brief initialization command
 *
 * This function will start the 15 minute timer, set the compare flag to TRUE
 *
 *****************************************************************************/
static void	CmdInitialize(void )
{
  // check for a proper address
  if (( g_ptDaliCtl->tCurMessage.tFields.nData == 0 ) || 
      (( g_ptDaliCtl->tCurMessage.tFields.nData == 0xFF ) && ( g_ptDaliCtl->tDaliSts.tBits.bMissingShortAddr == TRUE )) ||
      ( g_ptDaliCtl->tCurMessage.tFields.nData == (( g_ptDaliCtl->tDaliPrm.nShortAddress << 1 ) | 0x01 )))
  {
    // start the special timer/set the compare flag/set the 15min timer flag
    TaskManager_StartTimer( DALI_MESSAGE_TASK_ENUM_TIMER, 0 );
    g_ptDaliCtl->tFlags.bCompareModeEnabled = TRUE;
    g_ptDaliCtl->tFlags.b15MinTimerRunning = TRUE;
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
static void	CmdRandomize(void )
{
  U8 nValue;
  
  // generate the random number
  g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] = DALIMessageHandler_GenerateRandom( );
  g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] = DALIMessageHandler_GenerateRandom( );
  g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] = DALIMessageHandler_GenerateRandom( );

  // compare to reset value
  DALIMessageHandler_GetDefaultValue( DALI_PARAM_RANDOM_ADDR_H, &nValue );
  g_ptDaliCtl->tResetState.tBits.bRandAddrH = ( g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] != nValue ) ? TRUE : FALSE;
  DALIMessageHandler_GetDefaultValue( DALI_PARAM_RANDOM_ADDR_M, &nValue );
  g_ptDaliCtl->tResetState.tBits.bRandAddrM = ( g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] != nValue ) ? TRUE : FALSE;
  DALIMessageHandler_GetDefaultValue( DALI_PARAM_RANDOM_ADDR_L, &nValue );
  g_ptDaliCtl->tResetState.tBits.bRandAddrL = ( g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] != nValue ) ? TRUE : FALSE;

  // update NVM
  DALIMessageHandler_PutValue( DALI_PARAM_RANDOM_ADDR_H, g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] );
  DALIMessageHandler_PutValue( DALI_PARAM_RANDOM_ADDR_M, g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] );
  DALIMessageHandler_PutValue( DALI_PARAM_RANDOM_ADDR_L, g_ptDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] );
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void	CmdCompare(void )
{
  // set the default
  g_ptDaliCtl->tFlags.bResponseRequested = FALSE;

  // are we in compare mode
  if ( g_ptDaliCtl->tFlags.bCompareModeEnabled == TRUE )
  {
    // check for less than or equal
    if ( g_ptDaliCtl->tDaliPrm.tRandomAddr.uValue <= g_ptDaliCtl->tSearchAddr.uValue )
    {
      // set the response to true
      g_ptDaliCtl->nResponseValue = DALI_RESPONSE_YES;
      g_ptDaliCtl->tFlags.bResponseRequested = TRUE;
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
static void	CmdWithdraw(void )
{
  // check for address match
  if ( g_ptDaliCtl->tDaliPrm.tRandomAddr.uValue == g_ptDaliCtl->tSearchAddr.uValue )
  {
    // address match, clear compare flag
    g_ptDaliCtl->tFlags.bCompareModeEnabled = FALSE;
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
static void	CmdSearchAddrH(void )
{
  // store the value into the search address
  g_ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_H ] = g_ptDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  g_ptDaliCtl->tResetState.tBits.bSearchAddrH = ( g_ptDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_H ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdSearchAddrM
 *
 * @brief search address mid byte
 *
 * This function will store the data value to the search address mid byte
 *
 *****************************************************************************/
static void	CmdSearchAddrM(void )
{
  // store the value into the search address
  g_ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_M ] = g_ptDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  g_ptDaliCtl->tResetState.tBits.bSearchAddrM = ( g_ptDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_M ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdSearchAddrL
 *
 * @brief search address low byte
 *
 * This function will store the data value to the search address low byte
 *
 *****************************************************************************/
static void	CmdSearchAddrL(void )
{
  // store the value into the search address
  g_ptDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_L ] = g_ptDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  g_ptDaliCtl->tResetState.tBits.bSearchAddrL = ( g_ptDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_L ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
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
static void	CmdPgmAddr(void )
{
  // check for address match
  if (( g_ptDaliCtl->tDaliPrm.tRandomAddr.uValue == g_ptDaliCtl->tSearchAddr.uValue ) ||
			( g_ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED ))
  {
    // set the address/update the NVM
    g_ptDaliCtl->tDaliPrm.nShortAddress = g_ptDaliCtl->tCurMessage.tFields.nData >> 1;
    DALIMessageHandler_PutValue( DALI_PARAM_SHORT_ADDR, g_ptDaliCtl->tDaliPrm.nShortAddress );

    // set the appropriate status
    g_ptDaliCtl->tDaliSts.tBits.bMissingShortAddr = ( g_ptDaliCtl->tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;
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
static void	CmdVerAddr(void )
{
  // determine if address's match
  if ( g_ptDaliCtl->tDaliPrm.nShortAddress == ( g_ptDaliCtl->tCurMessage.tFields.nData >> 1 ))
  {
    // set responce to YES
    g_ptDaliCtl->nResponseValue = DALI_RESPONSE_YES;
    g_ptDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  else
  {
    // force a negative response
    g_ptDaliCtl->tFlags.bResponseRequested = FALSE;
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
static void	CmdQryAddr(void )
{
  // check for address match
  if (( g_ptDaliCtl->tDaliPrm.tRandomAddr.uValue == g_ptDaliCtl->tSearchAddr.uValue ) ||
			( g_ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED ))
  {
    // set the response value
    g_ptDaliCtl->nResponseValue = (( g_ptDaliCtl->tDaliPrm.nShortAddress ) << 1 | 0x01 );
    g_ptDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  else
  {
    // force a negative response
    g_ptDaliCtl->tFlags.bResponseRequested = FALSE;
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
static void	CmdPhysSelect(void )
{
  // check to see if we are disabled
  if ( g_ptDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_DISABLED )
  {
    // set the request to select/clear compare mode
    g_ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_REQUESTED;
    g_ptDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  }
  else
  {
    // clear the enabled state/enable compare mode
    g_ptDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;
    g_ptDaliCtl->tFlags.bCompareModeEnabled = TRUE;
  }
}

/**@} EOF SpecialCommands.c */
