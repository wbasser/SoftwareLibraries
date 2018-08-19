/******************************************************************************
 * @file ExtendedCommands
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

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler.h"
#include "DALIMessageHandler/ExtendedCommands/ExtendedCommands.h"

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
static void CmdEnableDeviceType( void)
{
  // set the application extended requested flag
  g_ptDaliCtl->tFlags.bAppExtRequested = ( g_ptDaliCtl->tCurMessage.tFields.nData == g_ptDaliCtl->nDeviceType ) ? TRUE : FALSE;
}

/******************************************************************************
 * @function CmdDataTransReg1
 *
 * @brief set the DTR1 value
 *
 * This function will copy the data to the DTR1
 *
 *****************************************************************************/
static void CmdDataTransReg1( void)
{
  // copy the value
  g_ptDaliCtl->nDataTransReg1 = g_ptDaliCtl->tCurMessage.tFields.nData;
}

/******************************************************************************
 * @function CmdDataTransReg2
 *
 * @brief set the DTR2 value
 *
 * This function will copy the data to the DTR2
 *
 *****************************************************************************/
static void CmdDataTransReg2( void)
{
  // copy the value
  g_ptDaliCtl->nDataTransReg2 = g_ptDaliCtl->tCurMessage.tFields.nData;
}

/******************************************************************************
 * @function
 *
 * @brief
 *
 * This function will
 *
 *****************************************************************************/
static void CmdWriteMemLoc( void)
{
  if ( g_ptDaliCtl->tFlags.bWriteMemoryEnabled )
  { 
    // write the memory location
    if ( DALIMessageHandler_WriteMemoryBlock( g_ptDaliCtl->nDataTransReg1,g_ptDaliCtl->nDataTransReg++, g_ptDaliCtl->tCurMessage.tFields.nData, FALSE ))
    {
      // set the dynamic flag/response field
      g_ptDaliCtl->nResponseValue = g_ptDaliCtl->tCurMessage.tFields.nData;
      g_ptDaliCtl->tFlags.bResponseRequested = TRUE;
    }
  } 
}

/**@} EOF ExtendedCommands.c */
