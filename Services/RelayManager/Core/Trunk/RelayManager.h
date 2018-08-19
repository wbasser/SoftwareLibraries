/******************************************************************************
 * @file RelayManager.h
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
#ifndef _RELAYMANAGER_H
#define _RELAYMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RelayManager/RelayManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of events and execution time for scan handler
#define RELAYMANAGER_SCAN_NUM_EVENTS      ( 2 )
#define RELAYMANAGER_SCAN_EXEC_RATE       TASK_TIME_MSECS( RELAY_MANAGER_SCAN_TIME )

#if ( RELAY_MANAGER_ENABLE_CHARGE_PUMP == 1 )
#define RELAYMANAGER_CPUMP_NUM_EVENTS     ( 1 )
#define RELAYMANAGER_CPUMP_EXEC_RATE      ( TASK_TIME_MSECS( 1 ))
#endif // RELAY_MANAGER_ENABLE_CHARGE_PUMP

# if ( RELAY_MANAGER_ENABLE_KEEPALIVE == 1 )
/// define the PWM rate for the keep alive
#define RELAYMANAGER_KEEPALIVE_RATEHZ     ( 20000l )
#endif // RELAY_MANAGER_ENABLE_KEEPALIVE

// enumerations ---------------------------------------------------------------
/// enumerate the error
typedef enum RLYMGRERR
{
  RLYMGR_ERR_NONE = 0,      ///< no error
  RLYMGR_ERR_ILLENUM        ///< illegal relay enumerations
} RLYMGRERR;

/// enumerate the states
typedef enum  RELAYSTATE
{
  RELAY_STATE_OFF = 0,      ///< off
  RELAY_STATE_ON,           ///< on
  RELAY_STATE_REQOFF,       /// off requested
  RELAY_STATE_REQON,        /// on requested
  RELAY_STATE_DRVACTIVE,    /// drive is active
  RELAY_STATE_ONDELAY       /// relay is on, but in delay
} RELAYSTATE, *PRELAYSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            RelayManager_Initialize( void );
#if (RELAY_MANAGER_ENABLE_DYNONOFTIMES == 1 )
extern  PRELAYDYMTIMDEF RelayManager_GetConfigDflt( void );
extern  PRELAYDYMTIMDEF RelayManager_GetConfigActl( void );
#endif // RELAY_MANAGER_ENABLE_DYNONOFTIMES
extern  BOOL            RelayManager_ProcessScan( TASKARG xArg );
#if ( RELAY_MANAGER_ENABLE_CHARGEPUMP == 1 )
extern  BOOL            RelayManager_ProcessChargePump( TASKARG xArg );
#endif // RELAY_MANAGER_ENABLE_CHARGEPUMP
extern  RLYMGRERR       RelayManager_SetState( RELAYENUM eRelay, BOOL bState );
extern  RLYMGRERR       RelayManager_GetState( RELAYENUM eRelay, PBOOL pbState );
extern  RLYMGRERR       RelayManager_GetDriveState( RELAYENUM eRelay, PRELAYSTATE peState );
#if ( RELAY_MANAGER_ENABLE_ZEROCROSS == 1 )
extern  void            RelayManager_ProcessZeroCross( void );
#endif // RELAY_MANAGER_ENABLE_ZEROCROSS

/**@} EOF RelayManager.h */

#endif  // _RELAYMANAGER_H