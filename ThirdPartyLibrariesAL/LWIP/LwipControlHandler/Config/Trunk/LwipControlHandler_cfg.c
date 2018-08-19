/******************************************************************************
 * @file LwipControlHandler_cfg.c
 *
 * @brief LWIP handler configuration implementation
 *
 * This file provides the ethernet configuration implementation
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup LwipControlHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// library includes -----------------------------------------------------------
#include "netif/EthernetHandler.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif

// local includes -------------------------------------------------------------
#include "LwipControlHandler/LwipControlHandler_cfg.h"

// global parameter declarations ----------------------------------------------
const LWIPPROTDEF atLwipProtDefs[ LWIP_PROT_MAX ] =
{
  // LWIPPROT( taskinit, taskprocess )
};

/// IP defaults
const LWIPCFGDEF  tLwipCfgDflts =
{
  .eMode = LWIP_CONFIG_MODE_DHCP,
  .tAddr =
  {
    .anValue[ LE_U32_MSB_IDX ] = 10,
    .anValue[ LE_U32_MS2_IDX ] = 0,
    .anValue[ LE_U32_MS1_IDX ] = 0,
    .anValue[ LE_U32_LSB_IDX ] = 100,
  },
  .tMask =
  {
    .anValue[ LE_U32_MSB_IDX ] = 255,
    .anValue[ LE_U32_MS2_IDX ] = 255,
    .anValue[ LE_U32_MS1_IDX ] = 255,
    .anValue[ LE_U32_LSB_IDX ] = 0,
  },
  .tGate =
  {
    .anValue[ LE_U32_MSB_IDX ] = 10,
    .anValue[ LE_U32_MS2_IDX ] = 0,
    .anValue[ LE_U32_MS1_IDX ] = 0,
    .anValue[ LE_U32_LSB_IDX ] = 1,
  },
  .anMac =
  { 
    0x00, 
    0x04, 
    0x25, 
    0x1C, 
    0xBA, 
    0x5A 
  },
};

/******************************************************************************
 * @function LwipControlHandler_Initialize
 *
 * @brief initialization
 *
 * This function will perform all low level initialization
 *
 * @param[in]   ptNetIf   pointer to the network interface structure
 *
 *****************************************************************************/
err_t  LwipControlHandlerPortInitialize( struct netif *ptNetIf )
{
  // call the abstracttion
  return( EthernetHandler_Initialize( ptNetIf ));
}

void LwipControlHandler_ProcessConnectCallback( struct netif *ptNetIf )
{
}

/******************************************************************************
 * @function LwipControlHandler_ProcessRecevie
 *
 * @brief process the timer task
 *
 * This function processes the LWIP timer 
 *
 * @param[in]   xArg   task argument
 *
 * @return      TRUE always
 *
 *****************************************************************************/
void LwipControlHandler_ProcessReceive( void )
{
  // process the input
  EthernetHandler_ProcessInput( );
}


/**@} EOF LwipControlHandler_cfg.c */
