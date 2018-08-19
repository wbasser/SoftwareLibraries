/******************************************************************************
 * @file InputOutputManager.h
 *
 * @brief input output manager declarations
 *
 * This file provides the declarations for the input/output manager implementation
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
 * \addtogroup InputOutputManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _INPUTOUTPUTMANAGER_H
#define _INPUTOUTPUTMANAGER_H

// local includes -------------------------------------------------------------
#include "InputOutputManager/InputOutputManager_cfg.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the execution rate for the process task
#define INPUTOUTPUTMANAGER_EXEC_RATE          ( TASK_TIME_MSECS( 10 ))
#define INPUTOUTPUTMANAGER_NUM_EVENTS         ( 2 )

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _IOMNGRERR
{
  IOMNGR_ERR_NONE = 0,        ///< no
  IOMNGR_ERR_ILLENUM = 0x80,  ///< illegal enumerator
  IOMNGR_ERR_RLYBSY,          ///< relay is still busy from previous operation
} IOMNGRERR;

// global function prototypes --------------------------------------------------
extern  void      InputOutputManager_Initialize( void );
extern  BOOL      InputOutputManager_ProcessEvent( TASKARG xArg );
extern  IOMNGRERR InputOutputManager_SetRelay( IOMNGRCHNENUM eIOMngrEnum, BOOL bState );
extern  IOMNGRERR InputOutputManager_GetState( IOMNGRCHNENUM eIOMngrEnum, PBOOL pbState );

/**@} EOF InputOutputManager.h */

#endif  // _INPUTOUTPUTMANAGER_H