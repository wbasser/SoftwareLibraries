/******************************************************************************
 * @file  DALIMessageHandler_cfg.c
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
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec.h"

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_cfg.h"
#include "SystemControlManager/SystemControlManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  DALImessageHandler_SetLedLevel
 *
 * @brief sets the led and table select 
 *
 * This function will set the LED level to the desired table and level
 *
 * @param[in]     nTblSel       table to use
 * @param[in]     nLevel        level to set to
 *
 *****************************************************************************/
void DALIMessageHandler_SetLedLevel( U8 nTblSel, U8 nLevel )
{
  // insert your function call here
}

/******************************************************************************
 * @function  DALImessageHandler_GetLedStatus
 *
 * @brief returns the state of the LED controller
 *
 * This function gets and returns the state of the LED controller
 *
 * @return    state of the LED controller
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetLedStatus( void )
{
  BOOL bStatus = FALSE;
  
  // insert the function call to get the current status of the LED controller
  
  // return the state of the led controller status
  return( bStatus );
}

/******************************************************************************
 * @function  DALImessageHandler_GetEOLTestMode
 *
 * @brief returns the state of the EOL test mode
 *
 * This function gets and returns the state of the EOL test mode
 *
 * @return    state of the EOL test mode
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetEOLTestMode( void )
{
  BOOL bEOLEnabled = FALSE;

  // call the function to return the end of line mode status here
  
  // return the end of line mode
  return( bEOLEnabled );
}

/******************************************************************************
 * @function  DALImessageHandler_SeedRandom
 *
 * @brief seeds the random generator
 *
 * This function will seed the rnadom number generator
 *
 * @param[in]     uSeed         seed value
 *
 *****************************************************************************/
void DALIMessageHandler_SeedRandom( U32 uValue )
{
  // insert your function call here
}

/******************************************************************************
 * @function  DALImessageHandler_GetEOLTestMode
 *
 * @brief returns the state of the EOL test mode
 *
 * This function gets and returns the state of the EOL test mode
 *
 * @return    state of the EOL test mode
 *
 *****************************************************************************/
U32 DALIMessageHandler_GenerateRandom( void )
{
  U32 uRandom = 0;
  
  // call the random number generator
  
  // return the random number
  return( uRandom );
}

/**@} EOF DALIMessageHandler_cfg.c */
