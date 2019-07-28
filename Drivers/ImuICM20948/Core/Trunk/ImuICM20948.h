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
/// typedef enum _IMUICM20948ERR
{
  IMUICM20948_ERR_NONE = 0,
  IMUICM20948_ERR_NODATA,
} IMUICMU20948ERR;

// structures -----------------------------------------------------------------
/// define the basic data structure
typedef struct _IMUICM20948BASE
{
  FLOAT   fValueX;          ///< X value
  FLOAT   fValueY;          ///< Y value
  FLOAT   fValueZ;          ///< Z value
} IMUICM20948BASE, *PIMUICM20948BASE
#define  IMUICM20948BASE_SIZE                 sizeof( IMUICM20948BASE )

/// define the data structure
typedef struct _IMUICM20948DATA
{
  IMUICM20948BASE tAccel;   ///< acceleration
  IMUICM20948BASE tGyro;    ///< gyro
  IMUICM20948BASE tMagn;    ///< magnetometer
} IMUICM20948DATA, *PIMUICM20948DATA;
#define IMUICM20948DATA_SIZE                  sizeof( IMUICM20948DATA )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            ImuICM20948_Initialize( void );
extern  IMUICM20948ERR  ImuICM20948_GetAccel( PIMUICM20948BASE ptAccel );
extern  IMUICM20948ERR  ImuICM20948_GetGyro( PIMUICM20948BASE ptGyro );
extern  IMUICM20948ERR  ImuICM20948_GetMagn( PIMUICM20948BASE ptMagn );
extern  IMUICM20948ERR  ImuICM20948_GetAllData( PIMUICM20948DATA ptData );
extern  void            ImuICM20948_ProcessIrq( void );

/**@} EOF ImuICM20948.h */

#endif  // _IMUICM20948_H