/******************************************************************************
 * @file DALIBusCommissioner_cfg.c
 *
 * @brief DALI bus commissioner configuration implementation 
 *
 * This file provides the configuration for the DALI bus commissioner
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusCommissioner
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusCommissioner/DALIBusCommissioner_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function DALIBusCommissioner_GetDeviceTableEntry
 *
 * @brief get a device table entry
 *
 * This function will return the device table entry at a given index
 *
 * @param[in]   nIndex        index of the devie table
 *
 * @return      current status of the device status
 *
 *****************************************************************************/
 U8 DALIBusCommissioner_GetDeviceTableEntry( U8 nIndex )
 {
  // call the approiate top level device manager to get a device table index
 }
 
/******************************************************************************
 * @function  DALIBusCommissioner_SetDeviceTableEntry
 *
 * @brief set a device table entry
 *
 * This function will set a device table entry to the given status
 *
 * @param[in]   nIndex      index of the device table
 * @param[in]   nStatus     device status
 *
 *****************************************************************************/
void DALIBusCommissioner_SetDeviceTableEntry( U8 nIndex, U8 nStatus )
{
  // call the appropriate top level device manager to set a device table index
}

/******************************************************************************
 * @function DALIBusCommissioner_GetDeviceCount
 *
 * @brief get a device table count
 *
 * This function will return the device table entry at a given index
 *
 * @return      the number of devices
 *
 *****************************************************************************/
 U8 DALIBusCommissioner_GetDeviceCount( void )
 {
  // call the approiate top level device manager to get the device count
  return( );
 }
 
/******************************************************************************
 * @function  DALIBusCommissioner_PutMessage
 *
 * @brief send a message
 *
 * This function will post a message to the DALI bus manager
 *
 * @param[in]   nAddress        address
 * @param[in]   nData           
 *
 *****************************************************************************/
void DALIBusCommissioner_PutMessage( U8 nAddress, U8 nData )
{
  // translate the message to the top level device manager format and post to queue
}

/******************************************************************************
 * @function  DALIBusCommissioner_GetMessage
 *
 * @brief geta message
 *
 * This function will post a message to the DALI bus manager
 *
 * @param[io]   pnStatus      pointer to store the status
 * @param[io]   pnData        pointer to store the data
 *
 *****************************************************************************/
void DALIBusCommissioner_GetMessage( PU8 pnStatus, PU8 pnData )
{
  // translate the message to the top level device manager format and post to queue
}

/******************************************************************************
 * @function  DALIBusCommissioner_CommissionStartStop
 *
 * @brief calls any additional processing when the commission starts or stops
 *
 * This function will post a message to the DALI bus manager
 *
 * @param[in]   bState      bus state
 * @param[in]   nOPtion     in this case, it this the command handler
 *                          enumeration
 *
 *****************************************************************************/
void DALIBusCommissioner_CommissionStartStop( BOOL bState, U8 nOption )
{
}

/**@} EOF DALIBusCommissioner_cfg.c */
