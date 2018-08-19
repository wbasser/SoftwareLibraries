/******************************************************************************
 * @file DALIBusCommissioner_prm.h
 *
 * @brief DALI bus commissioner parameter declarations 
 *
 * This file provides the bus commissioning parameters
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusCommissioner
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSCOMMISSIONER_PRM_H
#define _DALIBUSCOMMISSIONER_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"
#include "QueueManager/QueueManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the process tack for the DALI bus commissioner
#define DALIBUSCOMMISSIONER_PROCESS_TASK          ( TASK_SCHD_ILLEGAL )

/// define the queue for getting the DALI messages
#define DALIBUSCOMMSSIONER_RCV_QUEUE              ( QUEUE_ENUM_DALIBUSCOMM_RCVMSG )
#define DALIBUSCOMMISIONER_XMT_QUEUE              ( QUEUE_ENUM_DALIBUSMASTER_XMTMSG )

/// define the debug modes
#define DALIBUSCOMMISSIONER_DEBUGMODE_NONE        ( 0 )
#define DALIBUSCOMMISSIONER_DEBUGMODE_MINIMAL     ( 1 )
#define DALIBUSCOMMISSIONER_DEBUGMODE_TXRXDATA    ( 2 )
#define DALIBUSCOMMISSIONER_DEBUGMODE_MAXIMUM     ( 3 )

/// define the macro to allow debugging
#define DALIBUSCOMMISSIONER_DEBUG_MODE            ( DALIBUSCOMMISSIONER_DEBUGMODE_NONE )

/// define the debug base value
#define DALIBUSCOMMISSIONER_DEBUG_BASE            ( 0x2100 )

/// define the program/withdraw timeout in milliseconds
#define DALIBUSCOMMISSIONER_PROGWITH_DELAY        ( 25 )

/// define the device table enumerations
#define DALIBUSCOMMISSIONER_DEVSTS_NOTPRESENT     ( 0 )
#define DALIBUSCOMMISSIONER_DEVSTS_PRESENT        ( 1 )
#define DALIBUSCOMMISSIONER_DEVSTS_COLLISION      ( 2 )
#define DALIBUSCOMMISSIONER_DEVSTS_BAD            ( 3 )

/// define the message statuses
#define DALIBUSCOMMISSIONER_MSGSTS_NOERROR        ( 0 )
#define DALIBUSCOMMISSIONER_MSGSTS_NOERRRCV       ( 1 )
#define DALIBUSCOMMISSIONER_MSGSTS_COLLISION      ( 2 )
#define DALIBUSCOMMISSIONER_MSGSTS_NOERRNORCV     ( 3 )

/// define the macro to enable the ascii debug commands
#define DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS  ( 0 )

/// define the system control handler bus commission mode
#define DALIBUSCOMMISSIONER_SYSCTRL_BUSCOM_MODE   ( 0 )

/// define the system control handler idle mode
#define DALIBUSCOMMISSIONER_SYSCTL_IDLE_MODE      ( SYSCTRLMNGR_LCLMODE_IDLE )


/**@} EOF DALIBusCommissioner_prm.h */

#endif  // _DALIBUSCOMMISSIONER_PRM_H