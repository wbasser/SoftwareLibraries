/******************************************************************************
 * @file ExtendedCommands
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

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler.h"
#include "DALIProtocolHandler/ExtendedCommands/ExtendedCommands.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  CmdEnableDeviceType( void);
static  void  CmdDataTransReg1( void);
static  void  CmdDataTransReg2( void);
static  void  CmdWriteMemLoc( void);

// constant parameter initializations -----------------------------------------
/// define the special commands
const CODE CMDTBL	atExtnCmds[ ] =
{
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdEnableDeviceType ),   // 00
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdDataTransReg1 ),      // 01
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, CmdDataTransReg2 ),      // 02
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_FLAG, CmdWriteMemLoc ),        // 03
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 04
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 05
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 06
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 07
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 08
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 09
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 10
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 11
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 12
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 13
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 14
  CMDTBL_NORMENT( FALSE, RESPONSE_TYPE_NONE, NULL ),									// 15
};

/******************************************************************************
 * @function ExtendedCommands_Initialize
 *
 * @brief	initialize the extended command handler
 *
 * This function will perform any neccesary intialization for the extended
 * command handler
 *
 *****************************************************************************/
void ExtendedCommands_Initialize( void)
{
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void CmdEnableDeviceType( void )
{
  // set the application extended requested flag
  g_ptLclDaliCtl->tFlags.bAppExtRequested = ( g_ptLclDaliCtl->tCurMessage.tFields.nData == g_ptLclDaliCtl->nDeviceType ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdDataTransReg1
 *
 * @brief set the DTR1 value
 *
 * This function will copy the data to the DTR1
 *
 *****************************************************************************/
static void CmdDataTransReg1( void )
{
  // copy the value
  g_ptLclDaliCtl->nDataTransReg1 = g_ptLclDaliCtl->tCurMessage.tFields.nData;
}

/******************************************************************************
 * @function CmdDataTransReg2
 *
 * @brief set the DTR2 value
 *
 * This function will copy the data to the DTR2
 *
 *****************************************************************************/
static void CmdDataTransReg2( void )
{
  // copy the value
  g_ptLclDaliCtl->nDataTransReg2 = g_ptLclDaliCtl->tCurMessage.tFields.nData;
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void CmdWriteMemLoc( void )
{
  if ( g_ptLclDaliCtl->tFlags.bWriteMemoryEnabled )
  { 
    // write the memory location
    if ( DALIProtocolHandler_WriteMemoryBlock( g_ptLclDaliCtl->nDataTransReg1,g_ptLclDaliCtl->nDataTransReg++, g_ptLclDaliCtl->tCurMessage.tFields.nData, FALSE ))
    {
      // set the dynamic flag/response field
      g_ptLclDaliCtl->nResponseValue = g_ptLclDaliCtl->tCurMessage.tFields.nData;
      g_ptLclDaliCtl->tFlags.bResponseRequested = TRUE;
    }
  } 
}

/**@} EOF ExtendedCommands.c */
