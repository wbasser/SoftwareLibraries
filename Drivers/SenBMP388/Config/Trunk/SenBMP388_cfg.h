/******************************************************************************
 * @file SenBMP388_cfg.h
 *
 * @brief Bosch Sensortech BMP388 configuration declarations 
 *
 * This file provides the declarations for the BMP388 sensor 
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
 * \addtogroup SenBMP388
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SENBMP388_CFG_H
#define _SENBMP388_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenBMP388/SenBMP388_def.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/// define the number of events
#define SENBMP388_NUM_EVENTS                          ( 2 )
#define SENBMP388_DATAREADY_TASKENUM                  ( TASK_SCHD_ILLEGAL )
#define SENBMP388_DATAREADY_EVENT                     ( 0xDEED )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE SENBMP388CONFIG  g_tSenBMP388Config;

// global function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  BOOL  SenBMP388_ProcessCallback( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
extern  void  SenBMP388_ProcessIrqCallback( U8 Irq, BOOL bState );
extern  BOOL  SenBMP388_Read( U8 nBaseReg, PU8 pnData, U8 nLength );
extern  BOOL  SenBMP388_Write( U8 nBaseReg, PU8 pnData );
extern  void  SenBMP388_DelayMsec( U16 wDelay );

/**@} EOF SenBMP388_cfg.h */

#endif  // _SENBMP388_CFG_H