/******************************************************************************
 * @file ManchesterCodec_prm.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup ManchesterCodec
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MANCHESTERCODEC_PRM_H
#define _MANCHESTERCODEC_PRM_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "TaskManager/TaskManager.h"
#include "QueueManager/QueueManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of sync bits
#define MANCHESTERCODEC_NUM_SYNC_BITS         ( 1 )

/// define the number of stop bits
#define MANCHESTERCODEC_NUM_STOP_BITS         ( 2 )

/// define the transmit bit GPIO value
#define MANCHESTERCODEC_XMTBIT_GPIO_ENUM      ( GPIO_PIN_ENUM_ILLEGAL )

/// define the manchester baud rate
#define MANCHESTER_BAUD_RATE                  ( 1200 )

/// define the manchester half bit time
#define MANCHESTER_HALF_BIT_FREQ              ( MANCHESTER_BAUD_RATE * 2 )

/// define the manchester input capture queue
#define MANCHESTER_INPCAP_QUEUE_ENUM          ( QUEUE_ENUM_ILLEGAL )

/// define the manchester process event task
#define MANCHESTER_PROCESS_EVENT_TASK         ( TASK_SCHD_ILLEGAL )

/// define the manchester bit tolerance in percent
#define MANCHESTER_BITTOL_PERCENT             ( 20 )

/// define the invert enable
#define MANCHESTERCODEC_INVERT_ENABLE         ( 0 )

/// define the debug enable
#define MANCHESTERCODEC_RCVDBG_ENABLE         ( 0 )

/// define the transmit debug enable
#define MANCHESTERCODEC_XMTDBG_ENABLE         ( 0 )

/// define the base value for transmit debug
#define MANCHESTERCODEC_DBGXMT_BASE           ( 0x4000 )

/// define the base value for receive debug
#define MANCHESTERCODEC_DBGRCV_BASE           ( 0x5000 )

/**@} EOF ManchesterCodec_prm.h */

#endif  // _MANCHESTERCODEC_PRM_H
