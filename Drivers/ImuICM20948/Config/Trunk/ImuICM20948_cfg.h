/******************************************************************************
 * @file ImuICM20948_cfg.h
 *
 * @brief IMU Icm20948 configuration declarations
 *
 * This file provides the IMU ICM20948 configuration declarations
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
 * \addtogroup ImuICM20948
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IMUICM20948_CFG_H
#define _IMUICM20948_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// define the number of events
#define IMUICM20948_NUM_EVENTS                          ( 2 )
#define IMUICM20948_HANDLEIRQ_TASKENUM                  ( TASKMANAGER_SCHD_ILLEGAL )
#define IMUICM20948_HANDLEIRQ_EVENT                     ( 0xDEED )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  ImuICM20948_ProcessCallback ( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  void  ImuICM20948_ProcessIrqCallback( U8 Irq, BOOL bState );
extern  BOOL  ImuICM20948_ReadRegisters( U8 nBaseReg, PU8 pnData, U8 nLength );
extern  BOOL  ImuICM20948_WriteRegisters( U8 nBaseReg, PU8 pnData, U8 nLength );

/**@} EOF ImuICM20948_cfg.h */

#endif  // _IMUICM20948_CFG_H