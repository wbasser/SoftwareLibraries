/******************************************************************************
 * @file SystemMonitor_prm.h
 *
 * @brief System monitor parameter declarations
 *
 * This file declares any customization for the system monitor
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
#ifndef _SYSTEMMONITOR_PRM_H
#define _SYSTEMMONITOR_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the task manager enum for the transmit task
#define SYSTEMMONITOR_TRANSMIT_TASK_ENUM      ( TASK_SCHD_ENUM_ILLEGAL )

/// define the baud rate for the monitor
#define SYSTEMMONITOR_TRANSMIT_BYTE_RATE      ( 230000 )

/// define the minimum poll time
#define SYSTEMMONITOR_MINIMUM_POLL_TIME_MSEC  ( 250 )

/**@} EOF SystemMonitor_prm.h */

#endif  // _SYSTEMMONITOR_PRM_H