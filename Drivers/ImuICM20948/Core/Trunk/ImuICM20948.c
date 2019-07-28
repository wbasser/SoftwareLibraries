/******************************************************************************
 * @file ImuICM20948.c
 *
 * @brief IMU ICm20948 driver implementation
 *
 * This file provides the implementation for the ICM20958 IMU
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ImuICM20948/ImuICM20948.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  IMUICM20948DATA   tCurData;
static  BOOL              bValidData;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 *
 * @function ImuICM20948_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void ImuICM20948_Initialize( void )
{
  // clear the valid data flag
  bValidData = FALSE:
}

/******************************************************************************
 *
 * @function ImuICM20948_GetAccel
 *
 * @brief get acceleration data
 *
 * This function will get the acceleration data
 *
 * @param[in]   ptAccel   pointer to the data storage
 *
 * @return      approriate error
 *
 *****************************************************************************/
IMUICM20948ERR ImuICM20948_GetAccel( PIMUICM20948BASE ptAccel )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 *
 * @function ImuICM20948_GetGyro
 *
 * @brief get gyro data
 *
 * This function will get the gyro data
 *
 * @param[in]   ptAccel   pointer to the data storage
 *
 * @return      approriate error
 *
 *****************************************************************************/
IMUICM20948ERR ImuICM20948_GetGyro( PIMUICM20948BASE ptGyro )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 *
 * @function ImuICM20948_GetMagn
 *
 * @brief get magnetometer data
 *
 * This function will get the magnetometer data
 *
 * @param[in]   ptAccel   pointer to the data storage
 *
 * @return      approriate error
 *
 *****************************************************************************/
IMUICM20948ERR ImuICM20948_GetMagn( PIMUICM20948BASE ptMagn )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 *
 * @function ImuICM20948_GetAllData
 *
 * @brief get all data
 *
 * This function will get all data
 *
 * @param[in]   ptAccel   pointer to the data storage
 *
 * @return      approriate error
 *
 *****************************************************************************/
IMUICM20948ERR ImuICM20948_GetAllData( PIMUICM20948DATA ptData )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NONE;
  
  // return the error
  return( eError );
}

/******************************************************************************
 *
 * @function ImuICM20948_ProcessIrq
 *
 * @brief  process the IRQ
 *
 * This function will determine the casue of the IRQ and process appropriately
 *
 *****************************************************************************/
void ImuICM20948_ProcessIrq( void )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NONE;
  
  // return the error
  return( eError );
}

/**@} EOF ImuICM20948.c */
