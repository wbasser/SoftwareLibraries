/******************************************************************************
 * @file LwipControlHandler_cfg.h
 *
 * @brief LWIP handler configuration declarations
 *
 * This file provides the declarations for the ethernet handler configuration
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
 
// ensure only one instantiation
#ifndef _LWIPCONTROHANDLER_CFG_H
#define _LWIPCONTROHANDLER_CFG_H

// library includes -----------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

#include "netif/EthernetHandler.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// local includes -------------------------------------------------------------
#include "LwipControlHandler/LwipControlHandler_def.h"
#include "LwipControlHandler/LwipControlHandler_prm.h"

// enumerations ---------------------------------------------------------------
/// enumerate the list of lwip protocols
typedef enum _LWIPPROTS
{
  // enumerate user protocols here
  
  // do not remove entries below
  LWIP_PROT_MAX
} LWIPPROTS;

// global parameter declarations -----------------------------------------------
extern  const LWIPPROTDEF atLwipProtDefs[ ];
extern  const LWIPCFGDEF  tLwipCfgDflts;

// global function prototypes --------------------------------------------------
extern  err_t LwipControlHandler_PortInitialize( struct netif *ptNetIf );
extern  void  LwipControlHandler_ProcessConnectCallback( struct netif *ptNetIf );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL LwipControlHandler_ProcessReceive( TASKARG xArg );
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF LwipControlHandler_cfg.h */

#endif  // _LWIPCONTROHANDLER_CFG_H