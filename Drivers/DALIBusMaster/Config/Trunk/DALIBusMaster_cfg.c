/******************************************************************************
 * @file DALIBusMaster_cfg.c
 *
 * @brief DALI Bus Master configuration implementaion 
 *
 * This file provides the implementation for the configurable section of the 
 * DALI Bus Master
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
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
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusMaster/DALIBusMaster_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function DALIBusMaster_Transmit
 *
 * @brief transmit a message out on the bus
 *
 * This function will all the underlying transmit method 
 *
 * @param[in]   pnBuffer    pointer to the buffer
 * @param[in]   nLength     length of the buffer
 *
 *****************************************************************************/
void DALIBusMaster_Transmit( PU8 pnBuffer, U8 nLength )
{
}

/******************************************************************************
 * @function DALIBusMaster_Receive
 *
 * @brief receive a message from the bus
 *
 * This function will call the underlying receive method
 *
 * @param[in]   pnBuffer    pointer to the buffer
 * @param[in]   nLength     length of the buffer
 *
 *****************************************************************************/
void DALIBusMaster_Receive( PU8 pnBuffer, U8 nLength )
{
}
 
/******************************************************************************
 * @function DALIBusMaster_StopReceive
 *
 * @brief stop reception
 *
 * This function will call the underlying stop receive method
 *
 *****************************************************************************/
void DALIBusMaster_StopReceive( void )
{
}

/******************************************************************************
 * @function DALIBusMaster_PutDeviceStatus
 *
 * @brief store the current device status
 *
 * This function will call the parameter manager to correctl store
 * the device status
 *
 * @param[in]  ptStatus   pointer to current status
 *
 *****************************************************************************/
void DALIBusMaster_PutDeviceStatus( PDALIBUSMASTERDEVSTATUS ptStatus )
{
}

/******************************************************************************
 * @function DALIBusMaster_PutDeviceStatus
 *
 * @brief store the current device status
 *
 * This function will call the parameter manager to correctl store
 * the device status
 *
 * @param[in]  ptStatus   pointer to current status
 *
 *****************************************************************************/
void DALIBusMaster_PutDeviceStatus( PDALIBUSMASTERDEVSTATUS ptStatus )
{
}
 
/**@} EOF DALIBusMaster_cfg.c */
