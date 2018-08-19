/******************************************************************************
 * @file LogHandler_cfg.h
 *
 * @brief log handler configuration declarations
 *
 * This file provides the declarations for the log handler configuration
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
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LOGHANDLER_CFG_H
#define _LOGHANDLER_CFG_H

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler_def.h"

// enumerations ---------------------------------------------------------------
/// enumerate the log entry types
typedef enum _LOGTYPE
{
  LOG_TYPE_INIT = 1,
  LOG_TYPE_RESET,
  LOG_TYPE_CFGRST,
  /// user will enter log entries after init and before max

  /// do not remove below
  LOG_TYPE_MAX,
} LOGTYPE, *PLOGTYPE;

// global parameters ------------------------------------------------------
extern  const CODE  LOGENTRY atLogEntries[ ];

// global function prototypes -------------------------------------------
extern  U32   LogHandler_GetSystemTime( void );

/**@} EOF LogHandler_cfg.h */

#endif  // _LOGHANDLER_CFG_H