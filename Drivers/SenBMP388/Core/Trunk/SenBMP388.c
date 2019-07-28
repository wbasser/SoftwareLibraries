/******************************************************************************
 * @file SenBMP388.c
 *
 * @brief Bosch Sensortec BMP388 pressure sensor implementation
 *
 * This file provides the drive implementation for the BMP388 pressure
 * sensor
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SenBMP388/SenBMP388.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the co-efficients for computeing altitude
#define PRESTOALT_K1                        ( 1013.25 )
#define PRESTOALT_K2                        ( 0.190284 )
#define PRESTOALT_K3                        ( 145366.45 )

/// define the base address
#define BM388_DEV_ADDR                      ( 0x76 )

/// define the chip ID value
#define BMP388_CHIP_ID                      ( 0x50 )

/// define the trim coeeficents constants
#define TRIM_COEEF_T1_K                     ( 0.00390625 )
#define TRIM_COEEF_T2_K                     ( 1073741824 )
#define TRIM_COEEF_T3_K                     ( 281474976711000 )
#define TRIM_COEEF_P1P2_K1                  ( 16384 )
#define TRIM_COEEF_P1_K2                    ( 1048576 )
#define TRIM_COEEF_P2_K2                    ( 536870912 )
#define TRIM_COEEF_P3_K                     ( 4294967296 )
#define TRIM_COEEF_P4_K                     ( 137438953472 )
#define TRIM_COEEF_P5_K                     ( 0.125 )
#define TRIM_COEEF_P6_K                     ( 64 )
#define TRIM_COEEF_P7_K                     ( 256 )
#define TRIM_COEEF_P8_K                     ( 32768 )
#define TRIM_COEEF_P9_K                     ( 281474976711000 )
#define TRIM_COEEF_P10_K                    ( 281474976711000 )
#define TRIM_COEEF_P11_K                    ( 36893488147400000000 )

/// define the commands
#define CMD_NOP                             ( 0x00 )
#define CMD_FIFO_FLUSH                      ( 0xB0 )
#define CMD_SOFT_RESET                      ( 0xB6 )

/// define the BMP388 chip ID
#define BMP388_CHIP_ID                      ( 0x05 )

// enumerations ---------------------------------------------------------------
/// enumerate the register addresses
typedef enum _REGADR
{
  REGADR_CHIP_ID = 0,               /// chip ID
  REGADR_RESV_1,                    /// reserved
  REGADR_ERR_REG,                   /// error register
  REGADR_STATUS,                    /// status registers
  REGADR_DATA0_PRS0007              /// data - pressure 00-07
  REGADR_DATA1_PRS0815,             /// data - pressure 08-15
  REGADR_DATA2_PRS1623,             /// data - pressure 16-23
  REGADR_DATA3_TMP0007,             /// data - temperature 00-07
  REGADR_DATA4_TMP0815,             /// data - temperature 08-15
  REGADR_DATA5_TMP1623,             /// data - temperature 16-23
  REGADR_RESV_2,
  REGADR_RESV_3,
  REGADR_SENSOR_TIME0007,           /// sensor time - 00_07
  REGADR_SENSOR_TIME0815,           /// sensor time - 08-15
  REGADR_SENSOR_TIME1623,           /// sensor time - 16-23
  REGADR_SENSOR_TIME2431,           /// sensor time - 24-31
  REGADR_EVENT,                     /// event
  REGADR_IRQ_STATUS,                /// IRQ status
  REGADR_FIFO_LENGTH0007,           /// FIFO length 00-07
  REGADR_FIFO_LENGTH08,             /// FIFO length 08
  REGADR_FIFO_DATA,                 /// FIFO data
  REGADR_FIFO_WATERMARK0007,        /// FIFO watermark 00-07
  REGADR_FIFO_WATERMARK08,          /// FIFO watermark 08
  REGADR_FIFO_FIFOCONFIG1,          /// FIFO config #1
  REGADR_FIFO_FIFOCONFIG2,          /// FIFO config #2
  REGADR_IRQ_CONTROL,               /// IRQ control
  REGADR_INTFC_CONFIG,              /// Inteface control
  REGADR_PWRCTRL,                   /// power control
  REGADR_OSR,                       /// operating sampe rate
  REGADR_ODR,                       /// operating data rate
  REGADR_RESV_4,
  REGADR_CONFIG                     /// configuration
  REGADR_NVMPAR_T10007 = 0x31,      /// NVM parameter T1 00-07
  REGADR_NVMPAR_T10815,             /// NVM parameter T1 08-15
  REGADR_NVMPAR_T20007,             /// NVM parameter T2 00-07
  REGADR_NVMPAR_T20815,             /// NVM parameter T2 08-15
  REGADR_NVMPAR_T30007,             /// NVM parameter T3 00-07
  REGADR_NVMPAR_P10007,             /// NVM parameter P1 00-07
  REGADR_NVMPAR_P10815,             /// NVM parameter P1 08-15
  REGADR_NVMPAR_P20007,             /// NVM parameter P2 00-07
  REGADR_NVMPAR_P20815,             /// NVM parameter P2 08-15
  REGADR_NVMPAR_P30007,             /// NVM parameter P3 00-07
  REGADR_NVMPAR_P40007,             /// NVM parameter P4 00-07
  REGADR_NVMPAR_P50007,             /// NVM parameter P5 00-07
  REGADR_NVMPAR_P50815,             /// NVM parameter P5 08-15
  REGADR_NVMPAR_P60007,             /// NVM parameter P6 00-07
  REGADR_NVMPAR_P60815,             /// NVM parameter P6 08-15
  REGADR_NVMPAR_P70007,             /// NVM parameter P7 00-07
  REGADR_NVMPAR_P80007,             /// NVM parameter P8 00-07
  REGADR_NVMPAR_P90007,             /// NVM parameter P9 00-07
  REGADR_NVMPAR_P90815,             /// NVM parameter P9 08-15
  REGADR_NVMPAR_P100007,            /// NVM parameter P10 00-07
  REGADR_NVMPAR_P110007,            /// NVM parameter P11 00-07
  REGADR_COMMAND = 0x7E,            /// command
} REGADR;

/// enumerate the control mode
typedef enum _CTLMODE
{
  CTL_MODE_SLEEP = 0,
  CTL_MODE_FORCED,
  CTL_MODE_NORMAL = 3,
  CTL_MODE_MAX
} CTLMODE;

// structures -----------------------------------------------------------------
/// define the trimming coefficients structure
typedef union _TRIMCOEEFRAW
{
  struct
  {
    U16 wNvmParT1;
    U16 wNvmParT2;
    U8  nNvmParT3;
    U16 wNvmParP1;
    U16 wNvmParP2;
    U8  nNvmParP3;
    U8  nNvmParP4;
    U16 wNvmParP5;
    U16 wNvmParP6;
    U8  nNvmParP7;
    U8  nNvmParP8;
    U16 wNvmParP9;
    U8  nNvmParP10;
    U8  nNvmParP11;
  } tFields;
  U8  anBuf[ 1 ];
} TRIMCOEEFRAW, *PTRIMCOEEFRAW;
#define TRIMCOEEFRAW_SIZE                sizeof( TRIMCOEEFRAW )

/// define the trimming coefficients struct float
typedef struct _TRIMCOEEFFLOAT
{
  FLOAT fParT1;
  FLOAT fParT2;
  FLOAT fParT3;
  FLOAT fParP1;
  FLOAT fParP2;
  FLOAT fParP3;
  FLOAT fParP4;
  FLOAT fParP5;
  FLOAT fParP6;
  FLOAT fParP7;
  FLOAT fParP8;
  FLOAT fParP9;
  FLOAT fParP10;
  FLOAT fParP11;
} TRIMCOEEFFLOAT, *PTRIMCOEEFFLOAT;
#define TRIMCOEEFFLOAT_SIZE             sizeof( TRIMCOEEFFLOAT )

/// define the power control register
typedef union _REGPWRCNTL
{
  struct
  {
    BOOL    bPressEnable  : 1;          ///< pressure enabled
    BOOL    bTempEnable   : 1;          ///< temperature enabled
    CTLMODE eMode         : 1;          ///< control mode
  } tFields;
  U8  nReg;
} REGPWRCNTL, *PREGPWRCNTL;
#define REGPWRCNTL_SIZE                 sizeof( REGPWRCNTL )

/// define the over sampling register
typedef union _REGOSR
{
  struct
  {
    SENBMP388OVERSAMPRATE ePressure     : 3;          ///< oversampling - pressure
    SENBMP388OVERSAMPRATE eTemperature  : 3;          ///< oversampling - temperature
  } tFields;
  U8  nReg;
} REGOSR, *PREGOSR;
#define REGOSR_SIZE                     sizeof( REGOSR )

/// define the over data rate register
typedef union _REGODR
{
  struct
  {
    SENBMP388OUTDATARATE eOdrRate      : 5;          ///< sub sampling rate
  } tFields;
  U8  nReg;
} REGODR, *PREGODR;
#define REGODR_SIZE                     sizeof( REGODR )

/// defin the interrupt control
typedef union _REGIRQCTL
{
  struct
  {
    BOOL  bOutputType   : 1;        ///< open drain
    BOOL  bLevelSel     : 1;        ///< level select
    BOOL  bLatchEnb     : 1;        ///< latch enable
    BOOL  bFifoWmEnb    : 1;        ///< fifo watermark enable
    BOOL  bFifoFullEnb  : 1;        ///< fifo full enable
    BOOl  bDataRdyEnb   : 1;        ///< data ready enble
  } tFields;
  U8  nReg;
} REGIRQCTL, *PREGIRQCTL;
#define REGIRQCTL_SIZE                  sizeof( REGIRQCTL )

/// define the filter congif
typedef union _REGCONFIG
{
  struct
  {
    BOOL  bRes1         : 1;        ///< reserved 1
    U8    nIirFilter    : 3;        ///< iir filter select
  } tFields;
  U8 nReg;
} REGCONFIG, *PREGCONFIG;
#define REGCONFIG_SIZE                  sizeof( REGCONFIG )

/// define the data structure
typedef struct _RAWDATA
{
  U8  nPressure0007;                ///< pressure 0-7
  U8  nPressure0815;                ///< pressure 8-15
  U8  nPressure1623;                ///< pressure 16-23
  U8  nTemperature0007;             ///< pressure 0-7
  U8  nTemperature0815;             ///< pressure 8-15
  U8  nTemperature1623;             ///< pressure 16-23
} RAWDATA, *PRAWDATA;
#define RAWDATA_SIZE                    sizeof( RAWDATA )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL            bValidData;
static  BOOL            bSensorOk;
static  FLOAT           fCurPressure;
static  FLOAT           fCurTemperature;
static  TRIMCOEEFFLOAT  tTrimCoeef;

// local function prototypes --------------------------------------------------
static  BOOL  ReadComputeCoeeficients( void );
static  FLOAT CompensateTemperature( U32 uTempRaw );
static  FLOAT CompensatePressure( U32 uPressRaw );
static  BOOL  ReadRegisters( REGADR eRegAddr, PU8 pnData, U8 nNumRegs );
static  BOOL  WriteRegister( REGADR eRegAddr, P8 nData );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SenBMP388_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void SenBMP388_Initialize( void )
{
  U8              nData, nRetryCount;
  REGPWRCNTL      tRegPwrCntl;
  REGOSR          tRegOsr;
  REGODR          tRegOdr;
  REGIRQCTL       tRegIrq;
  REGCONFIG       tRegCfg;
  SENBMP388MODE   eMode;
  CTLMODE         eCtlMode;
  SENBMP388IRQDEF eIrqDef;
         
  // clear the valid data flag/sensor OK flag
  bValidData = bSensorOk = FALSE;
  
  // set the retry loop to 3
  nRetryCount = 3;

  // retry loop
  do
  {
    // delay
    SystemTick_DelayMsec( 5 );
    
    // read the chip ID register
    if ( ReadRegisters( REGADR_CHIP_ID, &nData, 1 ) == TRUE )
    {
      // now compare the chip id
      if ( nData == BMP388_CHIP_ID )
      {
        // set sensor ok/clear the retry count
        bSensorOk = TRUE;
        nRetryCount = 0;
      }
    }
  } while( nRetryCount != 0 );
  
  // if sensor is OK
  if ( bSensorOk == TRUE )
  {
    // read the calibration data
    if ( ReadComputeCoeeficients( ) == TRUE )
    {
      // get the mode
      eMode = PGM_RDBYTE( PGM_RDBYTE( g_tSenBMP388Config.eMode );
      switch( eMode )
      {
        case SENBMP388_MODE_BLKFORCED :
        case SENBMP388_MODE_NONBLKFORCED :
          eCtlMode = CTL_MODE_FORCED;
          break;

        case SENBMP388_MODE_BLKNORMAL :
        case SENBMP388_MODE_NONBLKNORMAL :
          eCtlMode = CTL_MODE_NORMAL;
          break;
          
        default :
          eCtlMode = CTL_MODE_SLEEP;
          break;
      }
      
      // turn on the device
      tRegPwrCntl.nReg = 0;
      tRegPwrCntl.tFields.eMode = eCtlMode;
      tRegPwrCntl.tFields.bPressEnable = ON;
      tRegPwrCntl.tFields.bTempEnable = ON;
      WriteRegister( REGADR_PWRCNTL, tRegPwrCntl.nReg );
      
      // set up OSR
      tRegOsr.nReg = 0;
      tRegOsr.tFields.ePressure = PGM_RDBYTE( g_tSenBMP388Config.ePressSampleRate );
      tRegOsr.tFields.eTemperature = PGM_RDBYTE( g_tSenBMP388Config.eTempSampleRate );
      WriteRegister( REGADR_OSR, tRegOsr.nReg );
      
      // set up ODR
      tRegOdr.nReg = 0;
      tRegOdr.tFields.eOdrRate = PGM_RDBYTE( g_tSenBMP388Config.eDataRate );
      WriteRegister( REGADR_ODR, tRegOdr.nReg );
      
      // write the filter
      tRegCfg.nReg = 0;
      tRegCfg.tFields.nIirFilter = PGM_RDBYTE( g_tSenBMP388Config.eFiltCoeef );
      WriteRegister( REGADR_CONFIG, tRegCfg.nReg );

      // set up the IRQ mode
      tRegIrq.nReg = 0;
      eIrqDef = PGM_RDBYTE( g_tSenBMP388Config.eIrqPin );
      tRegIrq.tFields.bOutputType = (( eIrqDef == SENBMP388_IRQDEF_ODLO ) || ( eIrqDef == SENBMP388_IRQDEF_ODHI )) ? ON : OFF;
      tRegIrq.tFields.bLevelSel = (( eIrqDef == SENBMP388_IRQDEF_ODHI ) || ( eIrqDef == SENBMP388_IRQDEF_PPHI )) ? ON : OFF;
      tRegIrq.tFields.bLatchEnb = OFF;
      tRegIrq.tFields.bFifoWmEnb = OFF;
      tRegIrq.tFields.bFifoFullEnb = OFF;
      tRegIrq.tFields.bDataRdyEnb = ON;
      WriteRegister( REGADR_IRQ_CONTROL, tRegIrq.nReg );
    }
  }
}

/******************************************************************************
 * @function SenBMP388_ProcessDataReady
 *
 * @brief process the data ready
 *
 * This function will process the data ready
 *
 *****************************************************************************/
void SenBMP388_ProcessDataReady( void )
{
  RAWDATA   tRawData;
  U32UN     tTemp;
  
  // read the data
  if ( ReadRegisters( REGADR_DATA0_PRS0007, ( PU8 )&tRawData, RAWDATA_SIZE ) == TRUE )
  {
    // compensate temperature
    tTemp.anValue[ LE_U16_LSB_IDX ] = tRawData.nTemperature0007;
    tTemp.anValue[ LE_U16_MS1_IDX ] = tRawData.nTemperature0815;
    tTemp.anValue[ LE_U16_MS2_IDX ] = tRawData.nTemperature1623;
    fCurTemperature = CompensateTemperature( tTemp.uValue );
    
    // compensate pressure
    tTemp.anValue[ LE_U16_LSB_IDX ] = tRawData.nPressure0007;
    tTemp.anValue[ LE_U16_MS1_IDX ] = tRawData.nPressure0007;
    tTemp.anValue[ LE_U16_MS2_IDX ] = tRawData.nPressure0007;
    fCurPressure = CompensatePressure( tTemp.uValue );
    
    // set valid data
    bValidData = TRUE;
  }
  else
  {
    // clear valid data
    bValidData = FALSE;
  }
}

/******************************************************************************
 * @function SenBMP388_GetPressure
 *
 * @brief get the current pressure value
 *
 * This function will return the current pressure value
 *
 * @return      current pressure value
 *
 *****************************************************************************/
FLOAT SenBMP388_GetPressure( void )
{
  FLOAT fPressure;
  
  // check for valid reading
  if ( bValidData == TRUE )
  {
    // return the last value
    fPressure = fCurPressure;
  }
  else
  {
    // set it to 0
    fPressure = 0;
  }
  
  // return the value
  return( fPressure 0;
}

/******************************************************************************
 * @function SenBMP388_GetAltitude
 *
 * @brief get the current altitude
 *
 * This function will return the current altitude
 *
 * @return      current altitude
 *
 *****************************************************************************/
FLOAT SenBMP388_GetAltitude( void )
{
  FLOAT fAltitude;
  
  // check for valid reading
  if ( bValidData == TRUE )
  {
    // calculate the altitude
    fAltitude = ( 1 - pow(( fCurPressure / PRESTOALT_K1 ), PRESTOALT_K2 )) * PRESTOALT_K3;
  }
  else
  {
    // set it to 0
    fAltitude = 0;
  }
  
  // return the value
  return( fAltitude );
}

/******************************************************************************
 * @function SenBMP388_GetTemperature
 *
 * @brief get the current temperature value
 *
 * This function will return the current temperature value
 *
 * @return      current temperature value
 *
 *****************************************************************************/
FLOAT SenBMP388_GetTemperature( void )
{
  FLOAT fTemperature;
  
  // check for valid reading
  if ( bValidData == TRUE )
  {
    // return the last value
    fTemperature = fCurTemperature;
  }
  else
  {
    // set it to 0
    fTemperature = 0;
  }
  
  // return the value
  return( fTemperature );
}

/******************************************************************************
 * @function ReadComputeCoeeficients
 *
 * @brief read and compute coeeficients
 *
 * This function will read and compute the coefficients
 *
 * @return      TRUE if no errors, FALSE if errors
 *
 *****************************************************************************/
static BOOL ReadComputeCoeeficients( void )
{
  TRIMCOEEFRAW  tCoeefRaw;
  BOOL          bStatus;
  
  // read the coeeficients
  if (( bStatus = ReadRegisters( REGADR_NVMPAR_T10007, ( PU8 )&tCoeefRaw.anBuf[ 0 ], TRIMCOEEFRAW_SIZE )) == TRUE )
  {
    // now adjust the values
    tTrimCoeef.fParT1 = ( FLOAT )( tCoeefRaw.tFields.wNvmParT1 / TRIM_COEEF_T1_K );
    tTrimCoeef.fParT2 = ( FLOAT )( tCoeefRaw.tFields.wNvmParT2 / TRIM_COEEF_T2_K );
    tTrimCoeef.fParT3 = ( FLOAT )( tCoeefRaw.tFields.nNvmParT3 / TRIM_COEEF_T3_K );
    tTrimCoeef.fParP1 = ( FLOAT )(( tCoeefRaw.tFields.wNvmParP1 - TRIM_COEEF_P1P2_K1 ) / TRIM_COEEF_P1_K2 );
    tTrimCoeef.fParP2 = ( FLOAT )(( tCoeefRaw.tFields.wNvmParP2 - TRIM_COEEF_P1P2_K1 ) / TRIM_COEEF_P2_K2 );
    tTrimCoeef.fParP3 = ( FLOAT )( tCoeefRaw.tFields.nNvmParP3 / TRIM_COEEF_P3_K );
    tTrimCoeef.fParP4 = ( FLOAT )( tCoeefRaw.tFields.nNvmParP4 / TRIM_COEEF_P4_K );
    tTrimCoeef.fParP5 = ( FLOAT )( tCoeefRaw.tFields.wNvmParP5 / TRIM_COEEF_P5_K );
    tTrimCoeef.fParP6 = ( FLOAT )( tCoeefRaw.tFields.wNvmParP6 / TRIM_COEEF_P6_K );
    tTrimCoeef.fParP7 = ( FLOAT )( tCoeefRaw.tFields.nNvmParP7 / TRIM_COEEF_P7_K );
    tTrimCoeef.fParP8 = ( FLOAT )( tCoeefRaw.tFields.nNvmParP8 / TRIM_COEEF_P8_K );
    tTrimCoeef.fParP9 = ( FLOAT )( tCoeefRaw.tFields.wNvmParP9 / TRIM_COEEF_P9_K );
    tTrimCoeef.fParP10 = ( FLOAT )( tCoeefRaw.tFields.nNvmParP10 / TRIM_COEEF_P10_K );
    tTrimCoeef.fParP11 = ( FLOAT )( tCoeefRaw.tFields.nNvmParP11 / TRIM_COEEF_P11_K );
  }

  // return status
  return( bStatus );
}

/******************************************************************************
 * @function CompensateTemperature
 *
 * @brief compensate raw temperature
 *
 * This function will apply the compensation parameters to the raw temperature
 *
 * @param[in]   uTempRaw    raw temperature
 *
 * @return      compensated temperature
 *
 *****************************************************************************/
static FLOAT CompensateTemperature( U32 uTempRaw )
{
  FLOAT fCompTemp, fData1, fData2;
  
  // apply the compensation
  fData1 = ( float )( uTempRaw - tTrimCoeef.fParT1 );
  fData2 = ( fData1 * tTrimCoeef.fParT2 );
  fCompTemp = fData2 + ( fData1 * fData1 ) * tTrimCoeef.fParT3 );
  
  // return the compensated temperature
  return( fCompTemp );
}

/******************************************************************************
 * @function CompensatePressure
 *
 * @brief compensate raw pressure
 *
 * This function will apply the compensation parameters to the raw pressure
 *
 * @param[in]   uTempPress    raw pressure
 *
 * @return      compensated pressure
 *
 *****************************************************************************/
static FLOAT CompensatePressure( U32 uPressRaw )
{
  FLOAT fCompPress, fData1, fData2, fData3, fData4, fData5, fData6;
  FLOAT fTempSquared, fTempCubed;
  FLOAT fPressSquared, fPressCubed;

  // calculate temperature squared/cubed
  fTempSquared = fCurTemperature * fCurTemperature;
  fTempCubed = fTempSquared * fCurTemperature;

  // calculate pressure squared/cubed
  fPressSquared = (( FLOAT )uPressRaw * (( FLOAT )uPressRaw;
  fPressCubed = (( FLOAT )uPressRaw * fPressSquared;
  
  // apply the compensation
  fData1 = tTrimCoeef.fParP6 * fCurTemperature;
  fData2 = tTrimCoeef.fParP7 * fTempSquared;
  fData3 = tTrimCoeef.fParP8 * fTempCubed;
  fData5 = tTrimCoeef.fParP5 + fData1 + fData2 + fData3;
  fData1 = tTrimCoeef.fParP2 * fCurTemperature;
  fData2 = tTrimCoeef.fParP3 * fTempSquared;
  fData3 = tTrimCoeef.fParP4 * fTempCubed;
  fData6 = (( FLOAT )uPressRaw * tTrimCoeef.fParP11 ) + fData1 + fData2 + fData3;
  fData1 = (( FLOAT )uPressRaw * (( FLOAT )uPressRaw;
  fData2 = ( tTrimCoeef.fParP9 + tTrimCoeef.fParP10 ) * fCurTemperature;
  fData3 = fData1 * fData2;
  fData4 = ( fData3 + fPressCubed ) * tTrimCoeef.fParP1;
  fCompPress = fData5 + fData6 + fData4;
  
  // return the compensated pressure
  return( fCompPress );
}


/******************************************************************************
 * @function ReadRegisters
 *
 * @brief read registers from the device
 *
 * This function will read the registers from the device 
 *
 * @param[in]   eRegAddr        register address
 * @param[in]   pnData          pointer to the data
 * @param[in]   nLength         length of the data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
static BOOL ReadRegisters( REGADR eRegAddr, PU8 pnData, U8 nNumRegs )
{
  BOOL bResult;
  
  // return the result
  return( bResult );
}

/******************************************************************************
 * @function WriteRegister
 *
 * @brief write register to the device
 *
 * This function will write to the register
 *
 * @param[in]   eRegAddr        register address
 * @param[in]   nData           data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
static BOOL WriteRegisters( REGADR eRegAddr, U8 nData )
{
  BOOL bResult;
  
  // return the result
  return( bResult );
}

/**@} EOF SenBMP3988.c */
