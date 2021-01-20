/******************************************************************************
 * @file SystemMonitor.h
 *
 * @brief System Monitor declarations
 *
 * This file provides the declarations for the system monitor
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup SystemMonitor
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SYSTEMMONITOR_H
#define _SYSTEMMONITOR_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemMonitor/SystemMonitor_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error
typedef enum _SYSMONERRS
{
  SYSMON_ERRS_NONE = 0,         ///< no errors
  SYSMON_ERRS_SENDRESP,         ///< send response
  SYSMON_ERRS_ILLCMD = 0xF0,    ///< illegal command
  SYSMON_ERRS_ILLENUM,          ///< illegal enuemration
  SYSMON_ERRS_ILLLENGTH,        ///< illegal length
  SYSMON_ERRS_ILLSETUP,         ///< illegal setup
  SYSMON_ERRS_FRAMEOVF,         ///< frame overflow
  SYSMON_ERRS_ILLVALUE,         ///< illegal value on put
  SYSMON_ERRS_ILLTIME,          ///< illegal time
} SYSMONERRS;

/// enumerate the commands
typedef enum _SYSMONCMDS
{
  SYSMON_CMDS_SETUP = 1,        ///< setup the system monitor
  SYSMON_CMDS_START,            ///< start the transmission
  SYSMON_CMDS_STOP,             ///< stop the transmission
  SYSMON_CMDS_GET,              ///< get a value
  SYSMON_CMDS_PUT,              ///< put a value
  SYSMON_CMDS_RESPONSE = 0x20,  ///< auto response
} SYSMONCMDS;

// global function prototypes --------------------------------------------------
extern  void        SystemMonitor_Initialize( void );
extern  SYSMONERRS  SystemMonitor_ProcessCommand( U8 nCommand, PU8 pnRcvBuffer, U16 wRcvLength, PU8 pnXmtBuffer, PU16 pwXmtLength, PVSYSMONTRANSMIT pvTransmitFunc, U8 nCmdEnum );
extern  void        SystemMonitor_ProcessTransmit( void );

/**@} EOF SystemMonitor.h */

#endif  // _SYSTEMMONITOR_H