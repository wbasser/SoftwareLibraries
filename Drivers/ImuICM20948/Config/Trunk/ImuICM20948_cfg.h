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
#include "ImuICM20948/ImuICM20948_prv.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#define IMUICM20948_POLLTASK_NUM_EVENTS                 ( 2 )
#define IMUICM20948_POLLTASK_EXEC_MSECS                 ( 10 )
#define IMUICM20948_POLLTASK_EXEC_RATE                  ( TASK_TIME_MSECS( IMUICM20948_POLLTASK_EXEC_MSECS ))
#define IMUICM20948_DATAREADY_EVENT                     ( 0XDEED )

/// define the sample rates
#define IMUICM20948_ACCEL_SAMPLE_RATE_HZ                ( 20 )
#define IMUICM20948_GYRO_SAMPLE_RATE_HZ                 ( 20 )

// define the acclerometer full scale select
#define IMUICM20948_ACCEL_FS_SELECT                     ( ACCEL_FULLSCALE_2G )

// macro to enable the wake on motion interrupt
#define IMUICM20948_WAKE_ON_MOTION_ENABLE               ( 0 )

// define the accel conversion constants
#if (IMUICM20948_ACCEL_FS_SELECT == ACCEL_FULLSCALE_2G )
//  #define IMUICM20948_ACCEL_CONV_K                      ( 0.0000625 )
  #define IMUICM20948_ACCEL_CONV_K                      ( 0.03125 )
#elif (IMUICM20948_ACCEL_FS_SELECT == ACCEL_FULLSCALE_4G )
  #define IMUICM20948_ACCEL_CONV_K                      ( 0.0001250 )
#elif (IMUICM20948_ACCEL_FS_SELECT == ACCEL_FULLSCALE_8G )
//  #define IMUICM20948_ACCEL_CONV_K                      ( 0.0002500 )
  #define IMUICM20948_ACCEL_CONV_K                      ( 0.12500 )
#elif (IMUICM20948_ACCEL_FS_SELECT == ACCEL_FULLSCALE_16G )
  #define IMUICM20948_ACCEL_CONV_K                      ( 0.0005000 )
#else
  error "Illegal acceleration full scale select!"
#endif

// define the gyro full scale select
#define IMUICM20948_GYRO_FS_SELECT                      ( GYRO_FULL_SCALE_250DPS )

// define the gyro conversion constants
#if (IMUICM20948_GYRO_FS_SELECT == GYRO_FULL_SCALE_250DPS )
  #define IMUICM20948_GYRO_CONV_K                       ( 0.0076294 )
#elif (IMUICM20948_GYRO_FS_SELECT == GYRO_FULL_SCALE_500DPS )
  #define IMUICM20948_GYRO_CONV_K                       ( 0.0152688 )
#elif (IMUICM20948_GYRO_FS_SELECT == GYRO_FULL_SCALE_1000DPS )
  #define IMUICM20948_GYRO_CONV_K                       ( 0.0351758 )
#elif (IMUICM20948_GYRO_FS_SELECT == GYRO_FULL_SCALE_2000DPS )
  #define IMUICM20948_GYRO_CONV_K                       ( 0.0610351 )
#else
  error "Illegal gyro full scale select!"
#endif

/// define the magnetometer scaling
#define IMUICM20948_MAGN_CONV_K                        ( 0.149902 )

// define the accel decimation select
#define IMUICM20948_ACCEL_DECIMATE_SELECT              ( ACCEL_CONFIG2_DECIMATOR_32 )

// define the gyro average select
#define IMUICM20948_GYRO_AVGLENGTH_SELECT               ( GYRO_AVERAGE_32 )

// define the accel low pass filter settings
#define IMUICM20948_ACCEL_LOWPASS_SELECT                ( ACCEL_LOWPASS_23HZ9 )

/// define the gyro low pass filter settings
#define IMUICM20948_GYRO_LOWPASS_SELECT                 ( GYRO_LOWPASS_23HZ9 )

/// define the device address
#define IMUICM20948_BASE_ADDR                           ( 0x68 )

/// define the IRQ operational mode enable
#define IMUICM20948_IRQ_OPMODE_ENABLE                   ( 0 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  ImuICM20948_LocalInitialize( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  extern  BOOL  ImuICM20948_PollEvent( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
#if ( IMUICM20948_IRQ_OPMODE_ENABLE == 1 )
  extern  void  ImuICM20948_ProcessIrqCallback( U8 Irq, BOOL bState );
#endif
extern  BOOL  ImuICM20948_ReadRegisters( U16 wBaseReg, PU8 pnData, U8 nLength );
extern  BOOL  ImuICM20948_WriteRegisters( U16 wBaseReg, PU8 pnData, U8 nLength );
extern  void  ImuICM20948_PostDataEvent( void );

/**@} EOF ImuICM20948_cfg.h */

#endif  // _IMUICM20948_CFG_H