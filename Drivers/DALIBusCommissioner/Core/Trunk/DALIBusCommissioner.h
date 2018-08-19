/******************************************************************************
 * @file DALIBusCommissioner.h
 *
 * @brief DALI bus commisioner declarations 
 *
 * This file provides the bus commissioning for the DALI bus
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
#ifndef _DALIBUSCOMMISSIONER_H
#define _DALIBUSCOMMISSIONER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusCommissioner/DALIBusCommissioner_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"
#if ( DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS == 1 )
#include "CommandHandlers/ASCII/AsciiCommandHandler.h"
#endif  // DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS

// Macros and Defines ---------------------------------------------------------
///define the number of entries for the receive message queue
#define DALIBUSCOMM_RCVMSG_QUEUE_SIZE           ( 2 )

///define the number of task events
#define DALIBUSCOMM_NUM_EVENTS                  ( 4 )

// enumerations ---------------------------------------------------------------
/// determine the event argument size
#if ( TASK_TSKARG_SIZE_BYTES == 1 )
#define DALIBUSCOM_EXEC_EVENT				        ( 0XC0 )
#define DALIBUSCOM_ABORT_COMMISSION         ( 0xC1 )
#define DALIBUSCOM_START_COMMISSION_PARTIAL ( 0xC2 )
#define DALIBUSCOM_START_COMMISSION_FULL    ( 0xC3 )
#define DALIBUSCOM_COMMISSION_DONE          ( 0xC4 )
#define DALIBUSCOM_ERROR_DETECTED           ( 0xC5 )
#elif ( TASK_TSKARG_SIZE_BYTES == 2 )
#define DALIBUSCOM_EXEC_EVENT               ( 0xC0C0 )
#define DALIBUSCOM_ABORT_COMMISSION         ( 0xC1C1 )
#define DALIBUSCOM_START_COMMISSION_PARTIAL ( 0xC2C2 )
#define DALIBUSCOM_START_COMMISSION_FULL    ( 0xC3C3 )
#define DALIBUSCOM_COMMISSION_DONE          ( 0xC4C4 )
#define DALIBUSCOM_ERROR_DETECTED           ( 0xC5C5 )
#elif ( TASK_TSKARG_SIZE_BYTES == 4 )
#define DALIBUSCOM_EXEC_EVENT               ( 0xC0C0C0C0 )
#define DALIBUSCOM_ABORT_COMMISSION         ( 0xC1C1C1C1 )
#define DALIBUSCOM_START_COMMISSION_PARTIAL ( 0xC2C2C2C2 )
#define DALIBUSCOM_START_COMMISSION_FULL    ( 0xC3C3C3C3 )
#define DALIBUSCOM_COMMISSION_DONE          ( 0xC4C4C4C4 )
#define DALIBUSCOM_ERROR_DETECTED           ( 0xC5C5C5C5 )
#endif

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
#if ( DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS == 1 )
extern  const CODE ASCCMDENTRY atDaliBusCommissionerCmdHandlerTable[ ];
#endif  // DALIBUSCOMMISSIONER_ENABLE_DEBUGCOMMANDS

// global function prototypes --------------------------------------------------
extern  void  DALIBusCommissioner_Initialize( void );
extern  BOOL  DALIBusCommissioner_ProcessEvent( TASKARG xArg );
extern  void  DALIBusCommissioner_Control( TASKARG tState );

/**@} EOF DALIBusCommissioner.h */

#endif  // _DALIBUSCOMMISSIONER_H