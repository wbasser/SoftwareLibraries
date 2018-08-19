/******************************************************************************
 * @file DALIBusMaster_prm.h
 *
 * @brief DALI Bus master parameter declarations 
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
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSMASTER_PRM_H
#define _DALIBUSMASTER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "QueueManager/QueueManager.h"
#include "GPIO/Gpio.h"
 
// Macros and Defines ---------------------------------------------------------
/// define the event for transmit done
#define DALIBUSMASTER_TRANSMIT_DONE_EVENT                   ( 0 )

/// define the event for a receive done
#define DALIBUSMASTER_RECEIVE_DONE_EVENT                    ( 0 )

/// define the event for a receive error
#define DALIBUSMASTER_RECEIVE_ERROR_EVENT                   ( 0 )

/// define the event for a receive edge
#define DALIBUSMASTER_RECEIVE_EDGE_EVENT                    ( 0 )

/// define the queue for incoming messages
#define DALIBUSMASTER_XMTMSG_QUEUE_ENUM                     ( QUEUE_ENUM_ILLEGAL )

/// define the task enumeration for processing bus events
#define DALIBUSMASTER_BUS_TASK_ENUM                         ( TASK_SCHD_ILLEGAL )

/// define the task enumeration for processing transmit events
#define DALIBUSMASTER_XMT_TASK_ENUM                         ( TASK_SCHD_ILLEGAL )

/// define the GPIO to use as a bus monitor
#define DALIBUSMASTER_BUSMON_GPIO_ENUM                      ( GPIO_PIN_ENUM_ILLEGAL )

/// define the macro to enable the debug 0=OFF, 1=ON
#define DALIBUSMASTER_ENABLE_DEBUG                          ( 1 )

/// define the base values for both bus/transmit debug entries
#define DALIBUSMASTER_BUS_DEBUG_BASE                        ( 0x7000 )
#define DALIBUSMASTER_XMT_DEBUG_BASE                        ( 0x7100 )

/**@} EOF DALIBusMaster_prm.h */

#endif  // _DALIBUSMASTER_PRM_H