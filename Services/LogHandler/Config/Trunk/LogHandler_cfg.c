/******************************************************************************
 * @file LogHandler_cfg.c
 *
 * @brief log handler configuration implementation
 *
 * This file provides the implementation of the log handler configuration
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
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler_cfg.h"

// global parameter declarations ----------------------------------------------
const CODE  LOGENTRY atLogEntries[ LOG_TYPE_MAX ] = 
{
  LOGENTRY_CREATE( "Initilization" ),
  LOGENTRY_CREATE( "System Reset" ),
  LOGENTRY_CREATE( "Config Reset" ),
  /// create log entries here
  
};

/******************************************************************************
 * @function LogHandler_GetSystemTime
 *
 * @brief gets the system time
 *
 * This function respond with the current system time
 *
 * @return      system time in USEC
 *
 *****************************************************************************/
U32 LogHandler_GetSystemTime( void )
{
  U32 uTime = 0xBEEFDEAD;
  
  return( uTime ); 
}

/**@} EOF LogHandler_cfg.c */
