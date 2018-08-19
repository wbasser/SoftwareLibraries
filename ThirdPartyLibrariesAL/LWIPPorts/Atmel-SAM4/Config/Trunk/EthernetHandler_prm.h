/******************************************************************************
 * @file EthernetHandler_prm.h
 *
 * @brief ATMEL SAM4 Internet Interface parameters
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies 
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Nan Technologies, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Nan Technologies, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 * \addtogroup EthernetHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ETHERNETHANDLER_PRM_H
#define _ETHERNETHANDLER_PRM_H

// system includes ------------------------------------------------------------
#include "Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
/// define the process receive task
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#define ETHERNETHANDLER_PROCESS_RCV_TASK_ENUM     ( TASK_SCHD_ILLEGAL )
#endif // SYSTEMDEFINE_OS_SELECTION

/**@} EOF EthernetHandler_prm.h */

#endif  // _ETHERNETHANDLERPRM_H/**

