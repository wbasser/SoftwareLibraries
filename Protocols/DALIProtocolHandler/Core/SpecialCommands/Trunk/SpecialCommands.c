/******************************************************************************
 * @file SpecialCommands
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Cyber Integration
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
 * \addtogroup DALIProtocolHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler.h"
#include "DALIProtocolHandler/SpecialCommands/SpecialCommands.h"

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
static  void	CmdTerminate( void );
static  void	CmdDataTransReg( void );
static  void	CmdInitialize( void );
static  void	CmdRandomize( void );
static  void	CmdCompare( void );
static  void	CmdWithdraw( void );
static  void	CmdSearchAddrH( void );
static  void	CmdSearchAddrM( void );
static  void	CmdSearchAddrL( void );
static  void	CmdPgmAddr( void );
static  void	CmdVerAddr( void );
static  void	CmdQryAddr( void );
static  void	CmdPhysSelect( void );

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
static void	CmdTerminate( void )
{
  // stop any timer execution
  TaskManager_StopTimer( DALI_MESSAGE_TASK_ENUM_TIMER );

  // clear flags
  g_ptLclDaliCtl->tFlags.b15MinTimerRunning = FALSE;
  g_ptLclDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  g_ptLclDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;
}

/******************************************************************************
 * @function CmdDataTransReg
 *
 * @brief store the data into the data transfer register
 *
 * This function will move the data into the data transfer register
 *
 *****************************************************************************/
static void	CmdDataTransReg( void )
{
  // store the data value into the DTR
  g_ptLclDaliCtl->nDataTransReg = g_ptLclDaliCtl->tCurMessage.tFields.nData;
}

/******************************************************************************
 * @function CmdInitialize
 *
 * @brief initialization command
 *
 * This function will start the 15 minute timer, set the compare flag to TRUE
 *
 *****************************************************************************/
static void	CmdInitialize( void )
{
  // check for a proper address
  if (( g_ptLclDaliCtl->tCurMessage.tFields.nData == 0 ) || 
      (( g_ptLclDaliCtl->tCurMessage.tFields.nData == 0xFF ) && ( g_ptLclDaliCtl->tDaliSts.tBits.bMissingShortAddr == TRUE )) ||
      ( g_ptLclDaliCtl->tCurMessage.tFields.nData == (( g_ptLclDaliCtl->tDaliPrm.nShortAddress << 1 ) | 0x01 )))
  {
    // start the special timer/set the compare flag/set the 15min timer flag
    TaskManager_StartTimer( DALI_MESSAGE_TASK_ENUM_TIMER, 0 );
    g_ptLclDaliCtl->tFlags.bCompareModeEnabled = TRUE;
    g_ptLclDaliCtl->tFlags.b15MinTimerRunning = TRUE;
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
static void	CmdRandomize( void )
{
  U8 nValue;
  
  // generate the random number
  g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] = DALIProtocolHandler_GenerateRandom( );
  g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] = DALIProtocolHandler_GenerateRandom( );
  g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] = DALIProtocolHandler_GenerateRandom( );

  // compare to reset value
  DALIProtocolHandler_GetDefaultValue( DALI_PARAM_RANDOM_ADDR_H, &nValue );
  g_ptLclDaliCtl->tResetState.tBits.bRandAddrH = ( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] != nValue ) ? TRUE : FALSE;
  DALIProtocolHandler_GetDefaultValue( DALI_PARAM_RANDOM_ADDR_M, &nValue );
  g_ptLclDaliCtl->tResetState.tBits.bRandAddrM = ( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] != nValue ) ? TRUE : FALSE;
  DALIProtocolHandler_GetDefaultValue( DALI_PARAM_RANDOM_ADDR_L, &nValue );
  g_ptLclDaliCtl->tResetState.tBits.bRandAddrL = ( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] != nValue ) ? TRUE : FALSE;

  // update NVM
  DALIProtocolHandler_PutValue( DALI_PARAM_RANDOM_ADDR_H, g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_H ] );
  DALIProtocolHandler_PutValue( DALI_PARAM_RANDOM_ADDR_M, g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_M ] );
  DALIProtocolHandler_PutValue( DALI_PARAM_RANDOM_ADDR_L, g_ptLclDaliCtl->tDaliPrm.tRandomAddr.anValue[ RANDOM_ADDR_L ] );
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void	CmdCompare( void )
{
  // set the default
  g_ptLclDaliCtl->tFlags.bResponseRequested = FALSE;

  // are we in compare mode
  if ( g_ptLclDaliCtl->tFlags.bCompareModeEnabled == TRUE )
  {
    // check for less than or equal
    if ( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.uValue <= g_ptLclDaliCtl->tSearchAddr.uValue )
    {
      // set the response to true
      g_ptLclDaliCtl->nResponseValue = DALI_RESPONSE_YES;
      g_ptLclDaliCtl->tFlags.bResponseRequested = TRUE;
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
static void	CmdWithdraw( void )
{
  // check for address match
  if ( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.uValue == g_ptLclDaliCtl->tSearchAddr.uValue )
  {
    // address match, clear compare flag
    g_ptLclDaliCtl->tFlags.bCompareModeEnabled = FALSE;
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
static void	CmdSearchAddrH( void )
{
  // store the value into the search address
  g_ptLclDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_H ] = g_ptLclDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  g_ptLclDaliCtl->tResetState.tBits.bSearchAddrH = ( g_ptLclDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_H ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdSearchAddrM
 *
 * @brief search address mid byte
 *
 * This function will store the data value to the search address mid byte
 *
 *****************************************************************************/
static void	CmdSearchAddrM( void )
{
  // store the value into the search address
  g_ptLclDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_M ] = g_ptLclDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  g_ptLclDaliCtl->tResetState.tBits.bSearchAddrM = ( g_ptLclDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_M ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdSearchAddrL
 *
 * @brief search address low byte
 *
 * This function will store the data value to the search address low byte
 *
 *****************************************************************************/
static void	CmdSearchAddrL( void )
{
  // store the value into the search address
  g_ptLclDaliCtl->tSearchAddr.anValue[ RANDOM_ADDR_L ] = g_ptLclDaliCtl->tCurMessage.tFields.nData;

  // Compare to reset state value
  g_ptLclDaliCtl->tResetState.tBits.bSearchAddrL = ( g_ptLclDaliCtl->tSearchAddr.anValue [ RANDOM_ADDR_L ] != SEARCH_ADDR_RESET ) ? TRUE : FALSE;
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
static void	CmdPgmAddr( void )
{
  // check for address match
  if (( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.uValue == g_ptLclDaliCtl->tSearchAddr.uValue ) ||
			( g_ptLclDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED ))
  {
    // set the address/update the NVM
    g_ptLclDaliCtl->tDaliPrm.nShortAddress = g_ptLclDaliCtl->tCurMessage.tFields.nData >> 1;
    DALIProtocolHandler_PutValue( DALI_PARAM_SHORT_ADDR, g_ptLclDaliCtl->tDaliPrm.nShortAddress );

    // set the appropriate status
    g_ptLclDaliCtl->tDaliSts.tBits.bMissingShortAddr = ( g_ptLclDaliCtl->tDaliPrm.nShortAddress == 0xFF ) ? TRUE : FALSE;
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
static void	CmdVerAddr( void )
{
  // determine if address's match
  if ( g_ptLclDaliCtl->tDaliPrm.nShortAddress == ( g_ptLclDaliCtl->tCurMessage.tFields.nData >> 1 ))
  {
    // set responce to YES
    g_ptLclDaliCtl->nResponseValue = DALI_RESPONSE_YES;
    g_ptLclDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  else
  {
    // force a negative response
    g_ptLclDaliCtl->tFlags.bResponseRequested = FALSE;
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
static void	CmdQryAddr( void )
{
  // check for address match
  if (( g_ptLclDaliCtl->tDaliPrm.tRandomAddr.uValue == g_ptLclDaliCtl->tSearchAddr.uValue ) ||
			( g_ptLclDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_ENABLED ))
  {
    // set the response value
    g_ptLclDaliCtl->nResponseValue = (( g_ptLclDaliCtl->tDaliPrm.nShortAddress ) << 1 | 0x01 );
    g_ptLclDaliCtl->tFlags.bResponseRequested = TRUE;
  }
  else
  {
    // force a negative response
    g_ptLclDaliCtl->tFlags.bResponseRequested = FALSE;
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
static void	CmdPhysSelect( void )
{
  // check to see if we are disabled
  if ( g_ptLclDaliCtl->ePhysicalSelEnabled == PHYSEL_STATE_DISABLED )
  {
    // set the request to select/clear compare mode
    g_ptLclDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_REQUESTED;
    g_ptLclDaliCtl->tFlags.bCompareModeEnabled = FALSE;
  }
  else
  {
    // clear the enabled state/enable compare mode
    g_ptLclDaliCtl->ePhysicalSelEnabled = PHYSEL_STATE_DISABLED;
    g_ptLclDaliCtl->tFlags.bCompareModeEnabled = TRUE;
  }
}

/**@} EOF SpecialCommands.c */
