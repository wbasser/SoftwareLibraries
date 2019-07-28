/******************************************************************************
 * @file SenBME280.c
 *
 * @brief Bosch Sensortech BME280 sensor implementation 
 *
 * This file provides the implementation for the Bosch Sensortec BME280
 * temperature/humitdity/pressure sensor.
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenBME280/SenBME280.h"

// library includes -----------------------------------------------------------
#include "I2c/I2c.h"
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
/// define the base address
#define SENBME280_DEV_ADDR            ( 0x76 )

/// define the chip ID value
#define SENBME280_CHIP_ID             ( 0x60 )

/// define the register addresss for the sensor
#define REG_TEMPPRES_CALIB            ( 0x88 )
#define REG_CHIPID                    ( 0xD0 )
#define REG_TEMPHUMID_CALIB           ( 0xE1 )
#define REG_RESET                     ( 0xE0 )
#define REG_CTLHUMID                  ( 0xF2 )
#define REG_STATUS                    ( 0xF3 )
#define REG_CTLMEAS                   ( 0xF4 )
#define REG_CONFIG                    ( 0xF5 )
#define REG_MEAS_BASE                 ( 0xF7 )

/// define the temperature min/max values
#define TEMP_MIN_VALUE                ( -4000 )
#define TEMP_MAX_VALUE                ( 8500 )

/// define the pressure min/max values
#define PRESS_MIN_VALUE               ( 30000 )
#define PRESS_MAX_VALUE               ( 110000 )

/// define the humidity max values
#define HUMID_MAX_VALUE               ( 100000 )

/// define the soft reset command
#define SFW_RESET_COMMAND             ( 0xB6 )

// enumerations ---------------------------------------------------------------
// enumerate the control measurement register operating mode
typedef enum _CTLMEASMODE
{
  CTLMEAS_MODE_SLEEP = 0,         ///< sleep mode
  CTLMEAS_MODE_FORCE1,            ///< force mode #1
  CTLMEAS_MODE_FORCE2,            ///< force mode #2
  CTLMEAS_MODE_NORMAL             ///< normal mode
} CTLMEASMODE;

/// enumerate the control measurement register oversampling rate
typedef enum _CTLMEASOSR
{
  CTLMEAS_OSR_DISABLED = 0,       ///< oversampling disabled
  CTLMEAS_OSR_1X,                 ///< oversampling 1X
  CTLMEAS_OSR_2X,                 ///< oversampling 2X
  CTLMEAS_OSR_4X,                 ///< oversampling 4X
  CTLMEAS_OSR_8X,                 ///< oversampling 8X
  CTLMEAS_OSR_16X,                ///< oversampling 16X
} CTLMEASOSR;

/// enumerate the configuration filter length
typedef enum _CTLCONFIGFILTLEN
{
  CTLCONFIG_FILTLEN_OFF = 0,      ///< filter off
  CTLCONFIG_FILTLEN_2,            ///< filter length of 2
  CTLCONFIG_FILTLEN_4,            ///< filter length of 4
  CTLCONFIG_FILTLEN_8,            ///< filter length of 8
  CTLCONFIG_FILTLEN_16,           ///< filter length of 16
} CTLCONFIGFILTLEN;

/// enumerate the configuration standy times(MS)
typedef enum _CTLCONFIGSTBY
{
  CTLCONFIG_STBY_500US = 0,       ///< 500 USEC
  CTLCONFIG_STBY_62MS5,           ///<  62.5 MSEC
  CTLCONFIG_STBY_125MS,           ///<  125 MSEC
  CTLCONFIG_STBY_250MS,           ///<  250 MSEC
  CTLCONFIG_STBY_500MS,           ///<  500 MSEC
  CTLCONFIG_STBY_1000MS,          ///<  1000 MSEC
  CTLCONFIG_STBY_10MS,            ///<  10 MSEC
  CTLCONFIG_STBY_20MS,            ///<  20 MSEC
} CTLCONFIGSTBY;

// structures -----------------------------------------------------------------
/// define the temperature/pressure calibration structure
typedef struct _TEMPPRESCALIB
{
  U16   wT1;                    ///< T1 calibration 
  S16   wT2;                    ///< T2 calibration
  S16   wT3;                    ///< T3 calibration
  U16   wP1;                    ///< P1 calibration
  S16   wP2;                    ///< P2 calibration
  S16   wP3;                    ///< P3 calibration
  S16   wP4;                    ///< P4 calibration
  S16   wP5;                    ///< P5 calibration
  S16   wP6;                    ///< P6 calibration
  S16   wP7;                    ///< P7 calibration
  S16   wP8;                    ///< P8 calibration
  S16   wP9;                    ///< P9 calibration
  U8    nReserved;              ///< reserved
  U8    nH1;                    ///< H1 calibration
} TEMPPRESCALIB, *PTEMPPRESCALIB;
#define TEMPPRESCALIB_SIZE          sizeof( TEMPPRESCALIB )

/// define the humidity calibration structure
typedef struct _HUMIDCALIB
{
  S16   wH2;                    ///< H2 calibration
  U8    nH3;                    ///< H3 calibration
  S16   wH4;                    ///< H4 calibration
  S16   wH5;                    ///< H5 calibration
  S8    nH6;                    ///< H6 calibration
} HUMIDCALIB, *PHUMIDCALIB;
#define HUMIDCALIB_SIZE            sizeof( HUMIDCALIB )

/// define the combined calibration structure
typedef struct _CALIBRATION
{
  TEMPPRESCALIB   tTempPres;    ///< temperature pressure calibration
  HUMIDCALIB      tHumidity;    ///< humidity calibration
  S32             uCalcFine;    ///< temperature fine
} CALIBRATION, *PCALIBRATION;
#define CALIBRATION_SIZE          sizeof( CALIBRATION )

/// define the data structure
typedef struct _MEASUREMENTS
{
  U8    nPressMsb;              ///< pressure MSB
  U8    nPressLsb;              ///< pressure LSB
  U8    nPressXlsb;             ///< pressure XLSB
  U8    nTempMsb;               ///< temperature MSB
  U8    nTempLsb;               ///< temperature LSB
  U8    nTempXlsb;              ///< temperature XLSB
  U8    nHumidMsb;              ///< humidity MSB
  U8    nHumidLsb;              ///< humidity LSB
} MEASUREMENTS, *PMEASUREMENTS;
#define MEASUREMENTS_SIZE        sizeof( MEASUREMENTS )

/// define the control measurement register
typedef struct _REGCTLMEAS
{
  U8  nMode;            ///< mode (bits 1-0 on U8)
  U8  nPresOS;          ///< pressure oversampling (bits 4-2 on U8)
  U8  nTempOS;          ///< temperature oversampling (bits 7-5 on U8)
} REGCTLMEAS, *PREGCTLMEAS;
#define REGCTLMEAS_SIZE             sizeof( REGCTLMEAS )

/// define the control humidity register
typedef struct _REGCTLHUMID
{
  U8  nHumidOS;         ///< humidity oversampling (bits 2-0 on U8)
} REGCTLHUMID, *PREGCTLHUMID;
#define REGCTLHUMID_SIZE            sizeof( REGCTLHUMID )

/// define configuration register
typedef struct _REGCONFIG
{
  BOOL  bSpi3WEnable;   ///< SPI enabled (bit 0 on U8)
  BOOL  bReserved;      ///< reserved (bit 1 on U8)
  U8    nFiltLen;       ///< filter length (bits 4-2 on U8)
  U8    nStandby;       ///< standby (bits 7-5 on U8)
} REGCONFIG, *PREGCONFIG;
#define REGCONFIG_SIZE              sizeof( REGCONFIG )

/// define the register structure
typedef struct _REGISTERS
{
  REGCTLMEAS    tCtlMeas;
  REGCONFIG     tConfig;
  REGCTLHUMID   tCtlHumid;
  U8            nByte;
} REGISTERS, *PREGISTERS;
#define REGISTERS_SIZE            sizeof( REGISTERS )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  CALIBRATION tCalibrationData;
static  FLOAT       afActualValues[ SENBME280_MEASTYPE_MAX ];
static  S32         alRawValues[ SENBME280_MEASTYPE_MAX ];
static  BOOL        bSensorOk;

// local function prototypes --------------------------------------------------
static  FLOAT   CompensateTemperature( S32 lRawTemp );
static  FLOAT   CompensatePressure( S32 uRawPress );
static  FLOAT   CompensateHumidity( S32 uRawHumid );
static  void    ParseHumidityCalibration( U8 *tHumidCalib );
static  BOOL    ReadCalibrationData( void );
static  BOOL    SetSensorControl( CTLMEASMODE eMode );
static  BOOL    WriteRegister( U8 nRegister, U8 nData );
static  BOOL    ReadRegisters( U8 nRegister, PU8 pnData, U8 nLength );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SenBME280_Initialize
 *
 * @brief initialization
 *
 * This function will read the chip and verify correct chip, read the calibration
 * values and store them
 *
 *****************************************************************************/
void SenBME280_Initialize( void )
{
  U8        nChipId, nRetryCount;
  BOOL      bRetryEnable;
  
  // enable the retry
  bRetryEnable = TRUE;
  nRetryCount = 5;
  bSensorOk = FALSE;
  
  // call the local initialization
  SenBME280_LocalInitialize( );
  
  // retry loop
  do 
  {
    // delay 3 milliseconds
    SystemTick_DelayMsec( 3 );

    // fill out the transfer control to read the chip ID
    if ( !ReadRegisters( REG_CHIPID, &nChipId, 1 ))
    {
      // check to see if we have a valid chip ID
      if ( SENBME280_CHIP_ID == nChipId )
      {
        // clear the retry flag
        bRetryEnable = FALSE;
      }
    }
    else
    {
      // delay for 5 milliseconds
      SystemTick_DelayMsec( 5 );
    }
  } while ( bRetryEnable && ( --nRetryCount != 0 ));
  
  // if retry count is not zero 
  if ( nRetryCount != 0 )
  {
    // read the calibration data
    if ( !ReadCalibrationData( ))
    {
      if ( !SetSensorControl( CTLMEAS_MODE_NORMAL ))
      {
        // set sensor ok
        bSensorOk = TRUE;
      }
    }
  }
}

/******************************************************************************
 * @function SenBME280_ProcessScan
 *
 * @brief process scan task
 *
 * This function will read the values from the device and perform any
 * necessary calibration
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE flush event
 *
 *****************************************************************************/
BOOL SenBME280_ProcessScan( TASKARG xArg )
{
  MEASUREMENTS  tMeasurements;
  S32           lMeasValue;
  
  // set up for the read
  if( !ReadRegisters( REG_MEAS_BASE, ( PU8 )&tMeasurements, MEASUREMENTS_SIZE ))
  {
    // perform compensation
    lMeasValue = ( ((U32) tMeasurements.nTempMsb) << 12 );
    lMeasValue |= ( ((U32) tMeasurements.nTempLsb) << 4 );
    lMeasValue |= ( ((U32) tMeasurements.nTempXlsb) >> 4 );
    alRawValues[ SENBME280_MEASTYPE_TEMP ] = lMeasValue;
      
    lMeasValue = ( ((U32) tMeasurements.nPressMsb) << 12 );
    lMeasValue |= ( ((U32) tMeasurements.nPressLsb) << 4 );
    lMeasValue |= ( ((U32) tMeasurements.nPressXlsb) >> 4 );
    alRawValues[ SENBME280_MEASTYPE_PRES ] = lMeasValue;
      
    lMeasValue = ( ((U32) tMeasurements.nHumidMsb) << 8 );
    lMeasValue |= ( tMeasurements.nHumidLsb );
    alRawValues[ SENBME280_MEASTYPE_HUMD ] = lMeasValue;
  }
  else
  {
    // set the error
    bSensorOk = FALSE;
    
    // clear the values
    memset( alRawValues, 0, sizeof( alRawValues ));
    memset( afActualValues, 0, sizeof( afActualValues ));
  }

  // always return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function SenBME280_GetRawData
 *
 * @brief get a measurement raw data
 *
 * This function will return the raw measured data
 *
 * @param[in]     eMeasType   measurement type enumeration
 *
 * @return      measurement value
 *
 *****************************************************************************/
S32 SenBME280_GetRawData( SENBME280MEASTYPE eMeasType )
{
  S32 lValue;
  
  switch( eMeasType )
  {
    case SENBME280_MEASTYPE_TEMP :
      lValue = alRawValues[ SENBME280_MEASTYPE_TEMP ];
      break;
      
    case SENBME280_MEASTYPE_HUMD :
      lValue = alRawValues[ SENBME280_MEASTYPE_HUMD ];
      break;
      
    case SENBME280_MEASTYPE_PRES :
      lValue = alRawValues[ SENBME280_MEASTYPE_PRES ];
      break;
      
    default :
      lValue = 0.0;
      break;
  }
  
  // return the raw value
  return( lValue );
}

/******************************************************************************
 * @function SenBME280_GetMeasurement
 *
 * @brief get a measurement
 *
 * This function will return the measurementvalue
 *
 * @param[in]     eMeasType   measurement type enumeration
 *
 * @return      measurement value
 *
 *****************************************************************************/
FLOAT SenBME280_Compensate( SENBME280MEASTYPE eMeasType, S32 lValue )
{
  FLOAT fValue;
  
  switch( eMeasType )
  {
    case SENBME280_MEASTYPE_TEMP :
      fValue = CompensateTemperature( lValue );
      break;
    
    case SENBME280_MEASTYPE_HUMD :
      fValue = CompensateHumidity( lValue );
      break;
    
    case SENBME280_MEASTYPE_PRES :
      fValue = CompensatePressure( lValue );
      break;
    
    default :
      fValue = 0.0;
      break;
  }
  
  // retrn the compensated value
  return( fValue );
}

/******************************************************************************
 * @function SenBME280_GetMeasurement
 *
 * @brief get a measurement
 *
 * This function will return the measurementvalue
 *
 * @param[in]     eMeasType   measurement type enumeration
 *
 * @return      measurement value
 *
 *****************************************************************************/
BOOL SenBME280_GetMeasurement( SENBME280MEASTYPE eMeasType, PFLOAT pfValue )
{
  BOOL bStatus = TRUE;
  
  // test if ok
  if ( bSensorOk )
  {
    switch( eMeasType )
    {
      case SENBME280_MEASTYPE_TEMP :
        afActualValues[ SENBME280_MEASTYPE_TEMP ] = CompensateTemperature( alRawValues[ SENBME280_MEASTYPE_TEMP ] );
        *( pfValue ) = afActualValues[ SENBME280_MEASTYPE_TEMP ];
        break;
    
      case SENBME280_MEASTYPE_HUMD :
        afActualValues[ SENBME280_MEASTYPE_HUMD ] = CompensateHumidity( alRawValues[ SENBME280_MEASTYPE_HUMD ] );
        *( pfValue ) = afActualValues[ SENBME280_MEASTYPE_HUMD ];
        break;
    
      case SENBME280_MEASTYPE_PRES :
        afActualValues[ SENBME280_MEASTYPE_PRES ] = CompensatePressure( alRawValues[ SENBME280_MEASTYPE_PRES ] );
        *( pfValue ) = afActualValues[ SENBME280_MEASTYPE_PRES ];
        break;
    
      default :
        *( pfValue ) = 0.0;
        break;
    }
    
    // set good status
    bStatus = FALSE;
  }
  
  // return the value
  return( bStatus );
}

/******************************************************************************
 * @function SenBME280_GetTemperature
 *
 * @brief get a temperature value
 *
 * This function will return the temperature value
 *
 * @return      temperature value
 *
 *****************************************************************************/
FLOAT SenBME280_GetTemperature( void )
{
  // return the temperature
  return( afActualValues[ SENBME280_MEASTYPE_TEMP ] );
}

/******************************************************************************
 * @function SenBME280_GetHumidity
 *
 * @brief get a humidity value
 *
 * This function will return the humidity value
 *
 * @return      humidity value
 *
 *****************************************************************************/
FLOAT SenBME280_GetHumidity( void )
{
  // return the humidity
  return( afActualValues[ SENBME280_MEASTYPE_HUMD ] );
}

/******************************************************************************
 * @function SenBME280_GetPressure
 *
 * @brief get a pressure value
 *
 * This function will return the pressure value
 *
 * @return      pressure value
 *
 *****************************************************************************/
FLOAT SenBME280_GetPressure( void )
{
  // return the pressure value
  return( afActualValues[ SENBME280_MEASTYPE_PRES ] );
}

/******************************************************************************
 * @function CompensateTemperature
 *
 * @brief perform temperature compensation
 *
 * This function will apply the compensation parameters to the read values
 *
 * @param[in]   lRawTemp    raw measured temperature
 *
 * @return      compensated value as a float
 *
 *****************************************************************************/
static FLOAT CompensateTemperature( S32 lRawTemp )
{
  FLOAT fValue = 0.0;
  S32   lTemp1, lTemp2, lTemperature;
  
  // perform the compensation
  lTemp1 = ( lRawTemp / 8 ) - ( (( S32 ) tCalibrationData.tTempPres.wT1 ) * 2 );
  lTemp1 = ( lTemp1 * tCalibrationData.tTempPres.wT2 ) / 2048;
  lTemp2 = ( lRawTemp / 16 ) - (( S32 ) tCalibrationData.tTempPres.wT1 );
  lTemp2 = ((( lTemp2 * lTemp2 ) / 4096 ) * (( S32 ) tCalibrationData.tTempPres.wT3 )) / 16384;
  tCalibrationData.uCalcFine = lTemp1 + lTemp2;
  lTemperature = (tCalibrationData.uCalcFine * 5 + 128) / 256;
  
  // ensure within min/max
  if ( lTemperature < TEMP_MIN_VALUE )
  {
    lTemperature = TEMP_MIN_VALUE;
  }
  else if ( lTemperature > TEMP_MAX_VALUE )
  {
    lTemperature = TEMP_MAX_VALUE;
  }
  
  // make it a flot
  fValue = ( FLOAT )lTemperature / ( FLOAT )100.0;
  
  // return the value
  return( fValue );
}

/******************************************************************************
 * @function CompensatePressure
 *
 * @brief compensate the pressure rading
 *
 * This function will perform any needed compensation for the raw pressure
 * value
 *
 * @param[in]   uRawPress   raw pressure value
 *
 * @return      pressure as a floating point
 *
 *****************************************************************************/
static FLOAT CompensatePressure( S32 uRawPress )
{
  FLOAT fValue = 0.0;
  S32   lTemp1, lTemp2, lTemp3, lTemp4;
  U32   uTemp5, uPressure;
  
  // perform the compensation
  lTemp1 = ((( S32 )tCalibrationData.uCalcFine ) >> 1 ) - ( S32 )64000;
  lTemp2 = (( lTemp1 >> 2 ) * ( lTemp1 >> 2 )) >> 11;
  lTemp2 *= (( S32 )tCalibrationData.tTempPres.wP6 );
  lTemp2 = lTemp2 + (( lTemp1 * (( S32 )tCalibrationData.tTempPres.wP5 )) << 1 );
  lTemp2 = ( lTemp2 >> 2 ) + ((( S32 )tCalibrationData.tTempPres.wP4 ) << 16 );
  lTemp3 = ( tCalibrationData.tTempPres.wP3 * ((( lTemp1 >> 2 ) * ( lTemp1 >> 2 )) >> 13 )) >> 3;
  lTemp4 = ((( S32 )tCalibrationData.tTempPres.wP2 ) * lTemp1 ) >> 1;
  lTemp1 = ( lTemp3 + lTemp4 ) >> 18;
  lTemp1 = (( 32768 + lTemp1 ) * (( S32 )tCalibrationData.tTempPres.wP1 )) >> 15;

    // check for division by zero
  if ( lTemp1 != 0 )
  {
    uTemp5 = ( U32 )(( S32 )1048576 ) - uRawPress;
    uPressure = ((( U32 )((( S32 )1048576 ) - uRawPress ) - ( lTemp2 >> 12 ))) * 3125;
    if ( uPressure < 0x80000000 )
    {
      uPressure = ( uPressure << 1 ) / (( U32 )lTemp1 );
    }
    else
    {
      uPressure = ( uPressure / ( U32 )lTemp1 ) * 2;
    }

    // apply the rest of the compensation
    lTemp1 = ((( S32 )tCalibrationData.tTempPres.wP9 ) * (( S32 )((( uPressure >> 3 ) * ( uPressure >> 3 )) >> 13 ))) >> 12;
    lTemp2 = ((( S32 )( uPressure >> 2 )) * (( S32 )tCalibrationData.tTempPres.wP8 )) >> 13;
    uPressure = ( U32 )(( S32 )uPressure + (( lTemp1 + lTemp2 + tCalibrationData.tTempPres.wP7 ) >> 4 ));

    // check for range
    if ( uPressure < PRESS_MIN_VALUE )
    {
      uPressure = PRESS_MIN_VALUE;
    }
    else if ( uPressure > PRESS_MAX_VALUE )
    {
      uPressure = PRESS_MAX_VALUE;
    }
  }
  else
  {
    uPressure = PRESS_MIN_VALUE;
  }
    
  // make it a float
  fValue = ( FLOAT )uPressure / 100.0;

  // return the value
  return( fValue );
}

/******************************************************************************
 * @function CompensateHumidity
 *
 * @brief humidity compensation
 *
 * This function will compensate the raw humidity value
 *
 * @param[in]   uRawHumid   raw humidity value
 *
 * @return      humidity as a compensated float value
 *
 *****************************************************************************/
static FLOAT CompensateHumidity( S32 uRawHumid )
{
  FLOAT fValue = 0.0;
  S32   lVar1, lVar2, lVar3, lVar4, lVar5;
  U32   uHumidity = 0;

  lVar1 = tCalibrationData.uCalcFine - (( S32 )76800);
  lVar2 = ( S32 )( uRawHumid * 16384 );
  lVar3 = ( S32 )((( S32 )tCalibrationData.tHumidity.wH4 ) * 1048576 );
  lVar4 = (( S32 )tCalibrationData.tHumidity.wH5 ) * lVar1;
  lVar5 = ((( lVar2 - lVar3 ) - lVar4 ) + ( S32 )16384) / 32768;
  lVar2 = ( lVar1 * ( S32 )tCalibrationData.tHumidity.nH6 ) / 1024;
  lVar3 = ( lVar1 * ( S32 )tCalibrationData.tHumidity.nH3 ) / 2048;
  lVar4 = (( lVar2 * ( lVar3 + ( S32 )32768 )) / 1024 ) + ( S32 )2097152;
  lVar2 = (( lVar4 * ( S32 )tCalibrationData.tHumidity.wH2 + 8192 )) / 16384;
  lVar3 = lVar5 * lVar2;
  lVar4 = (( lVar3 / 32768 ) * ( lVar3 / 32768 )) /128;
  lVar5 = lVar3 - (( lVar4 * ( S32 )tCalibrationData.tTempPres.nH1 ) / 16 );
  lVar5 = ( lVar5 < 0 ) ? 0 : lVar5;
  lVar5 = ( lVar5 > 419430400 ) ? 41930400 : lVar5;
  uHumidity = ( U32 )( lVar5 / 4096 );

  // check for overflow
  if ( uHumidity > HUMID_MAX_VALUE )
  {
    uHumidity = HUMID_MAX_VALUE;
  }
  
  // convert to a float
  fValue = ( FLOAT )uHumidity / 1000.0;

  // return the value
  return( fValue );
}

/******************************************************************************
 * @function ParseHumidityCalibration
 *
 * @brief parse the raw humidity calibration data
 *
 * This function will parese the raw humidity calibration data received from
 * the device
 *
 * @param[in] *tHumidCalib   Pointer to raw humidity calibration as read from
 *                           the device
 *
 * @return    void
 *****************************************************************************/
static void ParseHumidityCalibration( U8 *tHumidCalib )
{
  S16 wH4msb, wH4lsb, wH5msb, wH5lsb;

  wH4msb = ((S16) tHumidCalib[3]) * 16;
  wH4lsb = (S16) (tHumidCalib[4] & 0x0F);

  wH5msb = ((S16) tHumidCalib[5]) * 16;
  wH5lsb = (S16) (tHumidCalib[4] >> 4) & 0x0F;

  tCalibrationData.tHumidity.wH2 = (((U16) tHumidCalib[1]) << 8) | tHumidCalib[0];
  tCalibrationData.tHumidity.nH3 = tHumidCalib[2];
  tCalibrationData.tHumidity.wH4 = wH4msb | wH4lsb;
  tCalibrationData.tHumidity.wH5 = wH5msb | wH5lsb;
  tCalibrationData.tHumidity.nH6 = tHumidCalib[6];
}

/******************************************************************************
 * @function ReadCalibrationData
 *
 * @brief read calibaration data
 *
 * This function will read the calibration data
 *
 * @return      FALSE if no errors, TRUE otherwise
 *
 *****************************************************************************/
static BOOL ReadCalibrationData( void )
{
  BOOL      bResult = TRUE;
  U8        tTempHumidCalib[7] = {0};
  
  // setup temperature calibration data read
  if ( !ReadRegisters( REG_TEMPPRES_CALIB, ( PU8 )&tCalibrationData.tTempPres, TEMPPRESCALIB_SIZE ))
  {
    // setup humidity calibration data read
    if ( !ReadRegisters( REG_TEMPHUMID_CALIB, tTempHumidCalib, 7))
    {
      // parse humidity calibration
      ParseHumidityCalibration(tTempHumidCalib);
       
      // set the good result flag
      bResult = FALSE;
    }
  }  
  
  // return the status
  return( bResult );
}

/******************************************************************************
 * @function SetSensorControl
 *
 * @brief set up the sensor control registers
 *
 * This function will set up the sensor control registers
 *
 * @param[in]   eMode       run mode
 *
 * @return      FALSE if no errors, TRUE otherwise
 *
 *****************************************************************************/
static BOOL SetSensorControl( CTLMEASMODE eMode )
{
  BOOL      bResult = TRUE;
  REGISTERS tRegisters;

  // Config is only writable in sleep mode, so first ensure that
  tRegisters.nByte = 0x00;
  if (!WriteRegister(REG_CTLMEAS, tRegisters.nByte))
  {
    // set up humidity
    tRegisters.tCtlHumid.nHumidOS = CTLMEAS_OSR_1X;
    tRegisters.nByte = tRegisters.tCtlHumid.nHumidOS;
    if ( !WriteRegister( REG_CTLHUMID, tRegisters.nByte ))
    {
      // set up configuration
      tRegisters.tConfig.bSpi3WEnable = FALSE;
      tRegisters.tConfig.nFiltLen = CTLCONFIG_FILTLEN_16;
      tRegisters.tConfig.nStandby = CTLCONFIG_STBY_62MS5;
      tRegisters.nByte = tRegisters.tConfig.bSpi3WEnable |
                         (tRegisters.tConfig.nFiltLen << 2) |
                         (tRegisters.tConfig.nStandby << 5);
      if ( !WriteRegister( REG_CONFIG, tRegisters.nByte ))
      {
        // set up measurement control
        tRegisters.tCtlMeas.nMode = eMode;
        tRegisters.tCtlMeas.nTempOS = CTLMEAS_OSR_2X;
        tRegisters.tCtlMeas.nPresOS = CTLMEAS_OSR_16X;
        tRegisters.nByte = tRegisters.tCtlMeas.nMode |
                           (tRegisters.tCtlMeas.nPresOS << 2) |
                           (tRegisters.tCtlMeas.nTempOS << 5);
        if ( !WriteRegister( REG_CTLMEAS, tRegisters.nByte ))
        {
          // set okay status
          bResult = FALSE;
        }
      }
    }     
  }
  
  // return the status
  return( bResult );
}

/******************************************************************************
 * @function WriteRegister
 *
 * @brief write a register
 *
 * This function will write the passed value to the selected register
 *
 * @param[in]   nRegister       register to write
 * @param[in]   nData           data to write
 *
 * @return      FALSE if no errors, TRUE otherwise
 *
 *****************************************************************************/
static BOOL WriteRegister( U8 nRegister, U8 nData )
{
  BOOL      bErrDet;
  I2CERR    eI2cErr;
  I2CXFRCTL tXfrCtl;  

  // set up for write
  tXfrCtl.nDevAddr = SENBME280_DEV_ADDR;
  tXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = nRegister;
  tXfrCtl.nAddrLen = 1;
  tXfrCtl.pnData = &nData;
  tXfrCtl.wDataLen = sizeof( U8 );
  tXfrCtl.uTimeout = 100;
  eI2cErr = I2c_Write( SENBME280_I2C_ENUM, &tXfrCtl );
  
  // set the error
  bErrDet = ( eI2cErr == I2C_ERR_NONE ) ? FALSE : TRUE;
  
  // return the status
  return( bErrDet );
}

/******************************************************************************
 * @function ReadRegisters
 *
 * @brief read a registers
 *
 * This function will read the passed value to the selected register
 *
 * @param[in]   nRegisters       register to write
 * @param[in]   pnData           data to write
 *
 * @return      FALSE if no errors, TRUE otherwise
 *
 *****************************************************************************/
static BOOL ReadRegisters( U8 nRegister, PU8 pnData, U8 nLength )
{
  BOOL      bErrDet;
  I2CERR    eI2cErr;
  I2CXFRCTL tXfrCtl;  

  // set up for read
  tXfrCtl.nDevAddr = SENBME280_DEV_ADDR;
  tXfrCtl.tAddress.uValue = 0;
  tXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = nRegister;
  tXfrCtl.nAddrLen = 1;
  tXfrCtl.pnData = pnData;
  tXfrCtl.wDataLen = nLength;
  tXfrCtl.uTimeout = 500;
  eI2cErr = I2c_Read( SENBME280_I2C_ENUM, &tXfrCtl );

  // set the error  
  bErrDet = ( eI2cErr == I2C_ERR_NONE ) ? FALSE : TRUE;
  
  // return the status
  return( bErrDet );
}

/**@} EOF SenBME280.c */
