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
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  IMUICM20948DATA     tRawData;
static  IMUICM20948ALL      tCurData;
static  BOOL                bValidData;

// local function prototypes --------------------------------------------------
static  BOOL  WriteMagRegisters( MAGNREGS eMagReg, U8 nData );
static  BOOL  ReadMagRegisters( MAGNREGS eMagReg, PU8 pnData, U8 nLength );

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
  BANKXREGS   tRegs;
  U8          nData;
  U16         wSampleRate;

  // call the local initialization routine
  ImuICM20948_LocalInitialize( );

  // clear the valid data flag
  bValidData = FALSE;
  
  // check id
  if ( ImuICM20948_ReadRegisters( BANK0_REGS_WHOAMI, &nData, 1 ))
  {
    // valid read - check id
    if ( nData == CHIP_ID_VALUE )
    {
      // select the clock select to auto
      tRegs.nByte = 0;
      tRegs.tBank0PwrMgnt1.nClkSel = 1;
      ImuICM20948_WriteRegisters( BANK0_REGS_PWRMGMT1, &tRegs.nByte, 1 );

      tRegs.nByte = 0;
      tRegs.tBank0PwrMgnt2.nDisableAccel = 0x07;
      tRegs.tBank0PwrMgnt2.nDisableGyro = 0x07;
      ImuICM20948_WriteRegisters( BANK0_REGS_PWRMGMT2, &tRegs.nByte, 1 );

      // delay
      SystemTick_DelayMsec( 1 );
    
      tRegs.nByte = 0;
      ImuICM20948_WriteRegisters( BANK0_REGS_PWRMGMT2, &tRegs.nByte, 1 );

      // turn on the sensors
      tRegs.nByte = 0;
//      tRegs.tBank0LpConfig.bAccelCycle = ON;
//      tRegs.tBank0LpConfig.bGyroCycle = ON;
      tRegs.tBank0LpConfig.bI2cMstCycle = ON;
      ImuICM20948_WriteRegisters( BANK0_REGS_LPCONFIG, &tRegs.nByte, 1 );

      // set up the accelerometer low pass and full scale
      tRegs.nByte = 0;
      tRegs.tBank2AccelConfig.bAccelFChoice = ON;
      tRegs.tBank2AccelConfig.nAccelFSSel = IMUICM20948_ACCEL_FS_SELECT;
      tRegs.tBank2AccelConfig.nAccelDPLFCfg = IMUICM20948_ACCEL_LOWPASS_SELECT;
      ImuICM20948_WriteRegisters( BANK2_REGS_ACCELCONFIG, &tRegs.nByte, 1 );

      // set up the accelerometer decimation
      tRegs.nByte = 0;
      tRegs.tBank2AccelConfig2.nDec3Cfg = IMUICM20948_ACCEL_DECIMATE_SELECT;
      ImuICM20948_WriteRegisters( BANK2_REGS_ACCELCONFIG2, &tRegs.nByte, 1 );

      // set up the sample rate
      wSampleRate = 1125 / IMUICM20948_ACCEL_SAMPLE_RATE_HZ;
      tRegs.nByte = HI16 (wSampleRate );
      ImuICM20948_WriteRegisters( BANK2_REGS_ACCELSMPLRTDIV1, &tRegs.nByte, 1 );
      tRegs.nByte = LO16( wSampleRate );
      ImuICM20948_WriteRegisters( BANK2_REGS_ACCELSMPLRTDIV2, &tRegs.nByte, 1 );

      #if ( IMUICM20948_WAKE_ON_MOTION_ENABLE == 1 )
        // set up wake on motion
        tRegs.nByte = 6;
        ImuICM20948_WriteRegisters( BANK2_REGS_ACCELWOMTHR, &tRegs.nByte, 1 );
        tRegs.nByte = 0;
        tRegs.tBank2AccelIntelCtrl.bAccelIntelEn = ON;
        tRegs.tBank2AccelIntelCtrl.bAccelIntelModeInt = ON;
        ImuICM20948_WriteRegisters( BANK2_REGS_ACCELINTELCTRL, &tRegs.nByte, 1 );
      #endif

      // enable odr align
      tRegs.nByte = 0;
      tRegs.tBank2OdrAlignEn.bOdrALignEnab = ON;
      ImuICM20948_WriteRegisters( BANK2_REGS_ODRALIGNEN, &tRegs.nByte, 1 );

      // now set up the gyro
      tRegs.nByte = 0;
      tRegs.tBank2GyroCfg2.nGyroAvgCfg = IMUICM20948_GYRO_AVGLENGTH_SELECT;
      ImuICM20948_WriteRegisters( BANK2_REGS_GYROCONFIG2, &tRegs.nByte, 1 );
      tRegs.nByte = 0;
      tRegs.tBank2GyroCfg1.bGyroFChoice = ON;
      tRegs.tBank2GyroCfg1.nGyroDlpfCfg = IMUICM20948_GYRO_LOWPASS_SELECT;
      tRegs.tBank2GyroCfg1.nGyroFsSelec = IMUICM20948_GYRO_FS_SELECT;
      ImuICM20948_WriteRegisters( BANK2_REGS_GYROCONFIG1, &tRegs.nByte, 1 );
      wSampleRate = 1125 / IMUICM20948_GYRO_SAMPLE_RATE_HZ;
      tRegs.nByte = LO16( wSampleRate );
      ImuICM20948_WriteRegisters( BANK2_REGS_GYROSMPLRT, &tRegs.nByte, 1 );
//
//      // set up the interrupt pin
//      tRegs.nByte = 0;
//      tRegs.tBank0IntPinCfg.bInt1Actl = ON;
//      tRegs.tBank0IntPinCfg.bInt1OpenDrn = ON;
//      tRegs.tBank0IntPinCfg.bInt1LatchEn = ON;
//      ImuICM20948_WriteRegisters( BANK0_REGS_INTPINCFG, &tRegs.nByte, 1 );
//
//      // enable the interrupts
//      tRegs.nByte = 0;
//      tRegs.tBank0IntEnable.bWomIntEn = ON;
//      tRegs.tBank0IntEnable.bDmpInt1En = ON;
//      ImuICM20948_WriteRegisters( BANK0_REGS_INTENABLE, &tRegs.nByte, 1 );
//      tRegs.nByte = ON;
//      ImuICM20948_WriteRegisters( BANK0_REGS_INTENABLE1, &tRegs.nByte, 1 );

      // set up the magnetometer
      tRegs.nByte = 0;
      tRegs.tBank0UserCtrl.bI2cMstEn = ON;
      ImuICM20948_WriteRegisters( BANK0_REGS_USERCTRL, &tRegs.nByte, 1 );
      tRegs.nByte = 0;
      tRegs.tBank3I2cMstCtrl.nI2cMstClk = 7;
      ImuICM20948_WriteRegisters( BANK3_REGS_IC2MSTCTRL, &tRegs.nByte, 1 );
      nData = MAGN_SAMPLERATE_20HZ;
      WriteMagRegisters( MAGN_REGS_CNTL2, MAGN_SAMPLERATE_20HZ );
    }
  }
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
IMUICM20948ERR ImuICM20948_GetAccel( PIMUICM20948AXISVAL ptAccel )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NODATA;

  // check for data valid
  if ( bValidData )
  {
    // copy the data
    memcpy( ptAccel, &tCurData.tAccel, IMUICM20948AXISVAL_SIZE );
    eError = IMUICM20948_ERR_NONE;
  }
  
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
IMUICM20948ERR ImuICM20948_GetGyro( PIMUICM20948AXISVAL ptGyro )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NODATA;

  // check for data valid
  if ( bValidData )
  {
    // copy the data
    memcpy( ptGyro, &tCurData.tGyro, IMUICM20948AXISVAL_SIZE );
    eError = IMUICM20948_ERR_NONE;
  }
  
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
IMUICM20948ERR ImuICM20948_GetMagn( PIMUICM20948AXISVAL ptMagn )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NODATA;

  // check for data valid
  if ( bValidData )
  {
    // copy the data
    memcpy( ptMagn, &tCurData.tMagn, IMUICM20948AXISVAL_SIZE );
    eError = IMUICM20948_ERR_NONE;
  }
  
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
IMUICM20948ERR ImuICM20948_GetAllData( PIMUICM20948ALL ptData )
{
  IMUICM20948ERR  eError = IMUICM20948_ERR_NODATA;

  // check for data valid
  if ( bValidData )
  {
    // copy the data
    memcpy( ptData, &tCurData, IMUICM20948ALL_SIZE );
    eError = IMUICM20948_ERR_NONE;
  }
  
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
void ImuICM20948_ProcessDataReady( void )
{
  // temporary data registers
  U8  anTempData[( BANK0_REGS_EXTSLVSENSD05 - BANK0_REGS_ACCELXOUTH ) + 1 ];

  // mark data as invalid
  bValidData = FALSE;

  // read all the data from the device
  if ( ImuICM20948_ReadRegisters( BANK0_REGS_ACCELXOUTH, anTempData, (( BANK0_REGS_EXTSLVSENSD05 - BANK0_REGS_ACCELXOUTH ) + 1 )))
  {
    // mark is as good
    bValidData = TRUE;

    // read the data with attention to enddianess
    tRawData.tAccel.tValueX.anValue[ LE_U16_MSB_IDX ] = anTempData[ 0 ];
    tRawData.tAccel.tValueX.anValue[ LE_U16_LSB_IDX ] = anTempData[ 1 ];
    tRawData.tAccel.tValueY.anValue[ LE_U16_MSB_IDX ] = anTempData[ 2 ];
    tRawData.tAccel.tValueY.anValue[ LE_U16_LSB_IDX ] = anTempData[ 3 ];
    tRawData.tAccel.tValueZ.anValue[ LE_U16_MSB_IDX ] = anTempData[ 4 ];
    tRawData.tAccel.tValueZ.anValue[ LE_U16_LSB_IDX ] = anTempData[ 5 ];
    tRawData.tGyro.tValueX.anValue[ LE_U16_MSB_IDX ] = anTempData[ 6 ];
    tRawData.tGyro.tValueX.anValue[ LE_U16_LSB_IDX ] = anTempData[ 7 ];
    tRawData.tGyro.tValueY.anValue[ LE_U16_MSB_IDX ] = anTempData[ 8 ];
    tRawData.tGyro.tValueY.anValue[ LE_U16_LSB_IDX ] = anTempData[ 9 ];
    tRawData.tGyro.tValueZ.anValue[ LE_U16_MSB_IDX ] = anTempData[ 10 ];
    tRawData.tGyro.tValueZ.anValue[ LE_U16_LSB_IDX ] = anTempData[ 11 ];
    tRawData.tTemp.anValue[ LE_U16_MSB_IDX ]         = anTempData[ 12 ];
    tRawData.tTemp.anValue[ LE_U16_LSB_IDX ]         = anTempData[ 13 ];
    tRawData.tMagn.tValueX.anValue[ LE_U16_LSB_IDX ] = anTempData[ 14 ];
    tRawData.tMagn.tValueX.anValue[ LE_U16_MSB_IDX ] = anTempData[ 15 ];
    tRawData.tMagn.tValueY.anValue[ LE_U16_LSB_IDX ] = anTempData[ 16 ];
    tRawData.tMagn.tValueY.anValue[ LE_U16_MSB_IDX ] = anTempData[ 17 ];
    tRawData.tMagn.tValueZ.anValue[ LE_U16_LSB_IDX ] = anTempData[ 18 ];
    tRawData.tMagn.tValueZ.anValue[ LE_U16_MSB_IDX ] = anTempData[ 19 ];

    // convert to real data
    tCurData.tAccel.fAxisX = ( FLOAT )(( S16 )tRawData.tAccel.tValueX.wValue ) * IMUICM20948_ACCEL_CONV_K;
    tCurData.tAccel.fAxisY = ( FLOAT )(( S16 )tRawData.tAccel.tValueY.wValue ) * IMUICM20948_ACCEL_CONV_K;
    tCurData.tAccel.fAxisZ = ( FLOAT )(( S16 )tRawData.tAccel.tValueZ.wValue ) * IMUICM20948_ACCEL_CONV_K;
    tCurData.tGyro.fAxisX = ( FLOAT )(( S16 )tRawData.tGyro.tValueX.wValue ) * IMUICM20948_GYRO_CONV_K;
    tCurData.tGyro.fAxisY = ( FLOAT )(( S16 )tRawData.tGyro.tValueY.wValue ) * IMUICM20948_GYRO_CONV_K;
    tCurData.tGyro.fAxisZ = ( FLOAT )(( S16 )tRawData.tGyro.tValueZ.wValue ) * IMUICM20948_GYRO_CONV_K;
    tCurData.tMagn.fAxisX = ( FLOAT )(( S16 )tRawData.tMagn.tValueX.wValue ) * IMUICM20948_MAGN_CONV_K;
    tCurData.tMagn.fAxisY = ( FLOAT )(( S16 )tRawData.tMagn.tValueY.wValue ) * IMUICM20948_MAGN_CONV_K;
    tCurData.tMagn.fAxisZ = ( FLOAT )(( S16 )tRawData.tMagn.tValueZ.wValue ) * IMUICM20948_MAGN_CONV_K;

    // call the indirection on good data
    ImuICM20948_PostDataEvent( );
  }
}

/******************************************************************************
 *
 * @function ImuICM20948_GetImuFieldValue
 *
 * @brief  get the field value
 *
 * This function will return the field's value
 * 
 * @param[in]   eField    field enumeration
 *
 * @return      current field value or 0 if not valid
 *
 *****************************************************************************/
FLOAT ImuICM20948_GetImuFieldValue( IMUICM20948DATAFLDS eField )
{
  FLOAT   fValue = 0;

  // determine the field
  switch( eField )
  {
    case IMUICM20948_DATAFLDS_ACCELX :
      fValue = tCurData.tAccel.fAxisX;
      break;

    case IMUICM20948_DATAFLDS_ACCELY :
      fValue = tCurData.tAccel.fAxisY;
      break;

    case IMUICM20948_DATAFLDS_ACCELZ :
      fValue = tCurData.tAccel.fAxisZ;
      break;

    case IMUICM20948_DATAFLDS_GYROX :
      fValue = tCurData.tGyro.fAxisX;
      break;

    case IMUICM20948_DATAFLDS_GYROY :
      fValue = tCurData.tGyro.fAxisY;
      break;

    case IMUICM20948_DATAFLDS_GYROZ :
      fValue = tCurData.tGyro.fAxisZ;
      break;

    case IMUICM20948_DATAFLDS_MAGNX :
      fValue = tCurData.tMagn.fAxisX;
      break;

    case IMUICM20948_DATAFLDS_MAGNY :
      fValue = tCurData.tMagn.fAxisY;
      break;

    case IMUICM20948_DATAFLDS_MAGNZ :
      fValue = tCurData.tMagn.fAxisZ;
      break;

    default :
      fValue = 0.0;
      break;
  }

  // return the value
  return( fValue );
}

/******************************************************************************
 *
 * @function ImuICM20948_GetRawData
 *
 * @brief  get raw data
 *
 * This function will return the current raw data
 * 
 * @param[io]   ptRawData   pointer to copy the raw data to
 *
 *****************************************************************************/
void ImuICM20948_GetRawData( PIMUICM20948DATA ptRawData )
{
  // return the last known read data
  memcpy( ptRawData, &tRawData, IMUICM20948DATA_SIZE );
}

/******************************************************************************
 *
 * @function WriteMagRegisters
 *
 * @brief  write the registers
 *
 * This function will write the registers
 * 
 * @param[in]   eMagReg     magnetometer register
 * @param[in]   nData       data to write
 *
 * @return      TRUE
 *
 *****************************************************************************/
static BOOL WriteMagRegisters( MAGNREGS eMagReg, U8 nData )
{
  BANKXREGS   tRegs;

  // set the device address/MAG address/data/execute it
  tRegs.nByte = 0;
  tRegs.tBank3I2cSlvxAddr.nI2cSlvAddr = MAGN_AK09916_I2C_ADDR;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0ADDR, &tRegs.nByte, 1 );
  tRegs.nByte = eMagReg;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0REG, &tRegs.nByte, 1 );
  tRegs.nByte = nData;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0DO, &tRegs.nByte, 1 );
  tRegs.nByte = 0;
  tRegs.tBank3I2cSlvxCtrl.bI2cSlvxEn = ON;
  tRegs.tBank3I2cSlvxCtrl.nI2cSlvxLength = 1;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0CTRL, &tRegs.nByte, 1 );

  // return true
  return ( TRUE );
}

/******************************************************************************
 *
 * @function ReadMagRegisters
 *
 * @brief  read the registers
 *
 * This function will read the registers
 * 
 * @param[in]   eMagReg     magnetometer register
 * @param[io]   pnData      pointer to store the data in
 * @param[in]   nLength     length of the data to read
 *
 * @return      TRUE
 *
 *****************************************************************************/
static BOOL ReadMagRegisters( MAGNREGS eMagReg, PU8 pnData, U8 nLength )
{
  BANKXREGS   tRegs;

  // set the device address/MAG address/data/execute it
  tRegs.nByte = 0;
  tRegs.tBank3I2cSlvxAddr.nI2cSlvAddr = MAGN_AK09916_I2C_ADDR;
  tRegs.tBank3I2cSlvxAddr.bI2cSlvRdEn = ON;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0ADDR, &tRegs.nByte, 1 );
  tRegs.nByte = eMagReg;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0REG, &tRegs.nByte, 1 );
  tRegs.nByte = 0;
  tRegs.tBank3I2cSlvxCtrl.bI2cSlvxEn = ON;
  tRegs.tBank3I2cSlvxCtrl.nI2cSlvxLength = nLength;
  ImuICM20948_WriteRegisters( BANK3_REGS_I2CSLV0CTRL, &tRegs.nByte, 1 );

  // delay for a millisecond
  SystemTick_DelayMsec( 1 );

  // read the data from bank 0
  ImuICM20948_ReadRegisters( BANK0_REGS_EXTSLVSENSD00, pnData, nLength );

  // return true
  return ( TRUE );
}

/**@} EOF ImuICM20948.c */
