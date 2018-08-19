/******************************************************************************
 * @file  DALIProtocolHandler_cfg.c
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
 * \addtogroup  DALIProtocolHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIProtocolHandler/DALIProtocolHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  DALIProtocolHandler_OutputXmtMsg
 *
 * @brief outputs a transmit message
 *
 * This function will output a transmit response
 *
 * @param[in]     nResponse
 *
 *****************************************************************************/
void DALIProtocolHandler_OutputXmtMsg( U8 nResponse )
{
  #if ( DALI_ENABLE_DEBUG == 1 )
  #endif
}

/******************************************************************************
 * @function  DALIProtocolHandler_OutputRcvMsg
 *
 * @brief sets a receive message
 *
 * This function call the appropriate handler to output a receive message
 *
 * @param[in]     kptMsg    pointer to the message
 *
 *****************************************************************************/
void DALIProtocolHandler_OutputRcvMsg( PDALIMSG ptMsg )
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
void DALIProtocolHandler_StartReceive( PU8 pnData, U8 nLength )
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
void DALIProtocolHandler_StopReceive( void )
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
void DALIProtocolHandler_StartTransmit( PU8 pnData, U8 nLength )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_SetLedLevel
 *
 * @brief sets the led and table select 
 *
 * This function will set the LED level to the desired table and level
 *
 * @param[in]     wLevel        level to set to
 *
 *****************************************************************************/
void DALIProtocolHandler_SetLedLevel( U16 wLevel )
{
  // insert your function call here
}

/******************************************************************************
 * @function  DALIProtocolHandler_GetLedStatus
 *
 * @brief returns the state of the LED controller
 *
 * This function gets and returns the state of the LED controller
 *
 * @return    state of the LED controller
 *
 *****************************************************************************/
BOOL DALIProtocolHandler_GetLedStatus( void )
{
  BOOL bStatus = FALSE;
  
  // insert the function call to get the current status of the LED controller
  
  // return the state of the led controller status
  return( bStatus );
}

/******************************************************************************
 * @function  DALIProtocolHandler_SeedRandom
 *
 * @brief seeds the random generator
 *
 * This function will seed the rnadom number generator
 *
 *****************************************************************************/
void DALIProtocolHandler_SeedRandom( void )
{
  // insert your function call here
}

/******************************************************************************
 * @function  DALIProtocolHandler_GetEOLTestMode
 *
 * @brief returns the state of the EOL test mode
 *
 * This function gets and returns the state of the EOL test mode
 *
 * @return    state of the EOL test mode
 *
 *****************************************************************************/
U32 DALIProtocolHandler_GenerateRandom( void )
{
  U32 uRandom = 0;
  
  // call the random number generator
  
  // return the random number
  return( uRandom );
}

/******************************************************************************
 * @function  DALIProtocolHandler_WriteMemoryBlock
 *
 * @brief write a value to a memory block
 *
 * This function will write an entry to a memory block
 *
 * @param[in]     nDeviceIndex      device index
 * @param[in]     nMemoryBlock      memory block number
 * @param[in]     nBlockAddress     block address
 * @param[in]     nValue            value to write
 * @param[in]     bBypassLockByte   ignore the lock byte flag
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIProtocolHandler_WriteMemoryBlock( U8 nDeviceIndex, U8 nMemoryBlock, U8 nBlockAddress, U8 nValue, BOOL bBypassLockByte )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_ReadMemoryBlock
 *
 * @brief read a value from a memory block
 *
 * This function will read a value from the defined memory block
 *
 * @param[in]     nDeviceIndex      device index
 * @param[in]     nMemoryBlock      memory block number
 * @param[in]     nBlockAddress     block address
 * @param[io]     pnValue           pointer to store the read value
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIProtocolHandler_ReadMemoryBlock( U8 nDeviceIndex, U8 nMemoryBlock, U8 nBlockAddress, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_WriteMBParameter
 *
 * @brief 
 *
 * This function will 
 *
 * @param[in]     nDeviceIndex      device index
 * @param[in]     nMemoryBlock      memory block number
 * @param[in]     nBlockAddress     block address
 * @param[in]     nValue            value to write
 * @param[in]     bBypassLockByte   ignore the lock byte flag
 *
 * @return   TRUE if OK, FALSE if not 
 *
 *****************************************************************************/
BOOL DALIProtocolHandler_WriteMBParameter( U8 nDeviceIndex, U8 nParam, U8 nValue, BOOL bBypassLockByte )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_GetDefaultValue
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
BOOL DALIProtocolHandler_GetDefaultValue( U8 nDeviceIndex, U8 nParam, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_PutValue
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
BOOL DALIProtocolHandler_PutValue( U8 nDeviceIndex, U8 nParam, U8 nValue )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_GetValue
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
BOOL DALIProtocolHandler_GetValue( U8 nDeviceIndex, U8 nParam, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALIProtocolHandler_
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
void DALIProtocolHandler_ResetUserDefaults( void )
{
}

/**@} EOF DALIProtocolHandler_cfg.c */
