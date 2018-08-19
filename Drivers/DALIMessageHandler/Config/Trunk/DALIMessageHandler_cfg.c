/******************************************************************************
 * @file  DALIMessageHandler_cfg.c
 *
 * @brief
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
 * $Rev: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  DALIMessageHandler_OutputXmtMsg
 *
 * @brief outputs a transmit message
 *
 * This function will output a transmit response
 *
 * @param[in]     nResponse
 *
 *****************************************************************************/
void DALIMessageHandler_OutputXmtMsg( U8 nResponse )
{
  #if ( DALI_ENABLE_DEBUG == 1 )
  #endif
}

/******************************************************************************
 * @function  DALIMessageHandler_OutputRcvMsg
 *
 * @brief sets a receive message
 *
 * This function call the appropriate handler to output a receive message
 *
 * @param[in]     kptMsg    pointer to the message
 *
 *****************************************************************************/
void DALIMessageHandler_OutputRcvMsg( PDALIMSG ptMsg )
{
  #if ( DALI_ENABLE_DEBUG == 1 )
  #endif
}

/******************************************************************************
 * @function  
 *
 * @brief sets the led and table select 
 *
 * This function 
 *
 * @param[in]     
 * @param[in]     
 *
 *****************************************************************************/
void DALIMessageHandler_StartReceive( PU8 pnData, U8 nLength )
{
}

/******************************************************************************
 * @function  
 *
 * @brief sets the led and table select 
 *
 * This function 
 *
 * @param[in]     
 * @param[in]     
 *
 *****************************************************************************/
void DALIMessageHandler_StopReceive( void )
{
}

/******************************************************************************
 * @function  
 *
 * @brief sets the led and table select 
 *
 * This function 
 *
 * @param[in]     
 * @param[in]     
 *
 *****************************************************************************/
void DALIMessageHandler_StartTransmit( PU8 pnData, U8 nLength )
{
}

/******************************************************************************
 * @function  DALImessageHandler_SetLedLevel
 *
 * @brief sets the led and table select 
 *
 * This function will set the LED level to the desired table and level
 *
 * @param[in]     wLevel        level to set to
 *
 *****************************************************************************/
void DALIMessageHandler_SetLedLevel( U16 wLevel )
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
 * @function  DALImessageHandler_SeedRandom
 *
 * @brief seeds the random generator
 *
 * This function will seed the rnadom number generator
 *
 *****************************************************************************/
void DALIMessageHandler_SeedRandom( void )
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

/******************************************************************************
 * @function  DALImessageHandler_
 *
 * @brief 
 *
 * This function will 
 *
 * @param[in]     
 * @param[in]     
 *
 * @return    
 *
 *****************************************************************************/
BOOL DALIMessageHandler_WriteMemoryBlock(U8 nMemoryBlock, U8 nBlockAddress, U8 nValue, BOOL bBypassLockByte )
{
}

/******************************************************************************
 * @function  DALImessageHandler_
 *
 * @brief 
 *
 * This function will 
 *
 * @param[in]     
 * @param[in]     
 *
 * @return    
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ReadMemoryBlock( U8 nMemoryBlock, U8 nBlockAddress, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALImessageHandler_
 *
 * @brief 
 *
 * This function will 
 *
 * @param[in]     
 * @param[in]     
 *
 * @return    
 *
 *****************************************************************************/
BOOL DALIMessageHandler_WriteMBParameter( U8 nParam, U8 nValue, BOOL bBypassLockByte )
{
}

/******************************************************************************
 * @function  DALImessageHandler_GetDefaultValue
 *
 * @brief get a parameter value
 *
 * This function will get a parameter value
 *
 * @param[in]     nParam    parameter number
 * @param[in]     pnValue   pointer to the parameter storage
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetDefaultValue( U8 nParam, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALImessageHandler_PutValue
 *
 * @brief put a parameter value
 *
 * This function will write the parameter value
 *
 * @param[in]     nParam    parameter number
 * @param[in]     nValue    parameter value
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_PutValue( U8 nParam, U8 nValue )
{
}

/******************************************************************************
 * @function  DALImessageHandler_GetValue
 *
 * @brief get a parameter value
 *
 * This function will get a parameter value
 *
 * @param[in]     nParam    parameter number
 * @param[in]     pnValue   pointer to the parameter storage
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetValue( U8 nParam, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALImessageHandler_
 *
 * @brief 
 *
 * This function will 
 *
 * @param[in]     
 * @param[in]     
 *
 * @return    
 *
 *****************************************************************************/
void DALIMessageHandler_ResetUserDefaults( void )
{
}

/**@} EOF DALIMessageHandler_cfg.c */
