/******************************************************************************
 * @file ImuICM20948.h
 *
 * @brief IMU ICM20948 declarations 
 *
 * This file provides the declarations for the ICM29048 IMU driver
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
#ifndef _IMUICM20948_H
#define _IMUICM20948_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ImuICM20948/ImuICM20948_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _IMUICM20948ERR
{
  IMUICM20948_ERR_NONE = 0,
  IMUICM20948_ERR_NODATA,
} IMUICM20948ERR;

/// enumerate the individual data fields 
typedef enum _IMUICM20948DATAFLDS
{
  IMUICM20948_DATAFLDS_ACCELX = 0,
  IMUICM20948_DATAFLDS_ACCELY,
  IMUICM20948_DATAFLDS_ACCELZ,
  IMUICM20948_DATAFLDS_GYROX,
  IMUICM20948_DATAFLDS_GYROY,
  IMUICM20948_DATAFLDS_GYROZ,
  IMUICM20948_DATAFLDS_MAGNX,
  IMUICM20948_DATAFLDS_MAGNY,
  IMUICM20948_DATAFLDS_MAGNZ,
} IMUICM20948DATAFLDS;

// structures -----------------------------------------------------------------
/// define the base axis struct
typedef struct _IMUICM20948AXISVAL
{
  FLOAT   fAxisX;
  FLOAT   fAxisY;
  FLOAT   fAxisZ;
} IMUICM20948AXISVAL, *PIMUICM20948AXISVAL;
#define IMUICM20948AXISVAL_SIZE                 sizeof( IMUICM20948AXISVAL )

/// define the imu all data structure
typedef struct _IMUICM20948ALL
{
  IMUICM20948AXISVAL  tAccel;
  IMUICM20948AXISVAL  tGyro;
  IMUICM20948AXISVAL  tMagn;
  FLOAT               fTemp;
} IMUICM20948ALL, *PIMUICM20948ALL;
#define IMUICM20948ALL_SIZE                     sizeof( IMUICM20948ALL )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            ImuICM20948_Initialize( void );
extern  IMUICM20948ERR  ImuICM20948_GetAccel( PIMUICM20948AXISVAL ptAccel );
extern  IMUICM20948ERR  ImuICM20948_GetGyro( PIMUICM20948AXISVAL ptGyro );
extern  IMUICM20948ERR  ImuICM20948_GetMagn( PIMUICM20948AXISVAL ptMagn );
extern  IMUICM20948ERR  ImuICM20948_GetAllData( PIMUICM20948ALL ptData );
extern  FLOAT           ImuICM20948_GetImuFieldValue( IMUICM20948DATAFLDS eField );
extern  void            ImuICM20948_ProcessDataReady( void );
extern  void            ImuICM20948_GetRawData( PIMUICM20948DATA ptRawData );

/**@} EOF ImuICM20948.h */

#endif  // _IMUICM20948_H