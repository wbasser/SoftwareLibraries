/******************************************************************************
 * @file RelayManager_cfg.c
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RelayManager/RelayManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE RELAYCTLDEF atRelayCtlDefs[ RELAY_ENUM_MAX ] =
{
  // add relays to the table using the helper macro
  #if ( RELAY_MANAGER_ENABLE_EXTDRIVER == 1 )
    #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 0 )
      // RELAYCTLDEFM( gpioctl, gpiodir, keepalive, chargepump, minoff, minon, callback )

    #else
      // RELAYCTLDEFM( gpioctl, gpiodir, keepalive, chargepump, callback )

    #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
  #else
    #if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 0 )
    // RELAYCTLDEFM( gpioctl, keepalive, chargepump, minoff, minon, callback )

    #else
    // RELAYCTLDEFM( gpioctl, keepalive, chargepump, callback )

    #endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
  #endif // RELAY_MANAGER_ENABLE_EXTDRIVER
};

#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
const CODE RELAYCHARGEPUMPDEF atRelayChargePumpDefs[ RELAY_CHARGE_PUMP_ENUM_MAX ] =
{
  // add charge pump definitions here
  // RELAYCHARGEPUMPDEFM( gpio )

};
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP

#if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
const CODE RELAYKEEPALIVEDEF atRelayKeepAliveDefs[ RELAY_KEEP_ALIVE_ENUM_MAX ] =
{
  // add keep alive definitions here
  // RELAYKEEPALIVEDEFM( timer )

};
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE

#if ( RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
const CODE RELAYDYMTIMDEF atRelayTimesDefs[ RELAY_ENUM_MAX ] =
{
  // add on/off times definitions here
  // RELAYDYNTIMESDEFM( minoff, minon )

};
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES

/**@} EOF RelayManager_cfg.c */
