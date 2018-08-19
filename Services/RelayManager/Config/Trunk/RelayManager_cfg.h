/******************************************************************************
 * @file RelayManager_cfg.h
 *
 * @brief 
 *
 * This file 
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
 * $Log: $
 * 
 *
 * \addtogroup RelayManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RELAYMANAGER_CFG_H
#define _RELAYMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RelayManager/RelayManager_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the relays
typedef enum _RELAYENUM
{
  // add enumerations above this declaration

  // do not remove these entries
  RELAY_ENUM_MAX,
  RELAY_ENUM_ILLEGAL
} RELAYENUM;

#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
/// enumerate the charge pumps
typedef enum _RELAYCHARGEPUMPENUM
{
  // add enumerations above this declaration

  // do not remove these entries
  RELAY_CHARGE_PUMP_ENUM_MAX,
  RELAY_CHARGE_PUMP_ENUM_ILLEGAL
} RELAYCHARGEPUMPENUM;
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP

#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
/// enumerate the Keep alive timers
typedef enum _RELAYKEEPALIVEENUM
{
  // add enumerations above this declaration

  // do not remove these entries
  RELAY_KEEP_ALIVE_ENUM_MAX,
  RELAY_KEEP_ALIVE_ENUM_ILLEGAL
} RELAYKEEPALIVEENUM;
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE  RELAYCTLDEF         atRelayCtlDefs[ ];
#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
extern  const CODE  RELAYCHARGEPUMPDEF  atRelayChargePumpDefs[ ];
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP
#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
extern  const CODE  RELAYKEEPALIVEDEF   atRelayKeepAliveDefs[ ];
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE
#if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
extern  const CODE  RELAYDYMTIMDEF      atRelayTimesDefs[ ];
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES

// global function prototypes --------------------------------------------------

/**@} EOF RelayManager_cfg.h */

#endif  // _RELAYMANAGER_CFG_H