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
#include <math.h>

// local includes -------------------------------------------------------------
#include "SenBMP388/SenBMP388.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the co-efficients for computeing altitude
#define PRESTOALT_K1                        ( 1013.25f )
#define PRESTOALT_K2                        ( 0.190284f )
#define PRESTOALT_K3                        ( 44330.0f )

/// define the chip ID value
#define BMP388_CHIP_ID                      ( 0x50 )

/// define the trim coeeficents constants
#define TRIM_COEEF_T1_K                     ( 0.00390625f )
#define TRIM_COEEF_T2_K                     ( 1073741824.0f )
#define TRIM_COEEF_T3_K                     ( 281474976710656.0f)
#define TRIM_COEEF_P1P2_K1                  ( 16384 )
#define TRIM_COEEF_P1_K2                    ( 1048576.0f )
#define TRIM_COEEF_P2_K2                    ( 536870912.0f )
#define TRIM_COEEF_P3_K                     ( 4294967296.0f )
#define TRIM_COEEF_P4_K                     ( 137438953472.0f )
#define TRIM_COEEF_P5_K                     ( 0.125f )
#define TRIM_COEEF_P6_K                     ( 64.0f)
#define TRIM_COEEF_P7_K                     ( 256.0f )
#define TRIM_COEEF_P8_K                     ( 32768.0f )
#define TRIM_COEEF_P9_K                     ( 281474976710656.0f )
#define TRIM_COEEF_P10_K                    ( 281474976710656.0f )
#define TRIM_COEEF_P11_K                    ( 36893488147419103232.0f )

/// define the commands
#define CMD_NOP                             ( 0x00 )
#define CMD_FIFO_FLUSH                      ( 0xB0 )
#define CMD_SOFT_RESET                      ( 0xB6 )

// enumerations ---------------------------------------------------------------
/// enumerate the register addresses
typedef enum _REGADR
{
  REGADR_CHIP_ID = 0,               /// chip ID
  REGADR_RESV_1,                    /// reserved
  REGADR_ERR_REG,                   /// error register
  REGADR_STATUS,                    /// status registers
  REGADR_DATA0_PRS0007,             /// data - pressure 00-07
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
  REGADR_INT_STATUS,                /// INT status
  REGADR_FIFO_LENGTH0007,           /// FIFO length 00-07
  REGADR_FIFO_LENGTH08,             /// FIFO length 08
  REGADR_FIFO_DATA,                 /// FIFO data
  REGADR_FIFO_WATERMARK0007,        /// FIFO watermark 00-07
  REGADR_FIFO_WATERMARK08,          /// FIFO watermark 08
  REGADR_FIFO_FIFOCONFIG1,          /// FIFO config #1
  REGADR_FIFO_FIFOCONFIG2,          /// FIFO config #2
  REGADR_INT_CONTROL,               /// INT control
  REGADR_INTFC_CONFIG,              /// Inteface control
  REGADR_PWRCTRL,                   /// power control
  REGADR_OSR,                       /// operating sampe rate
  REGADR_ODR,                       /// operating data rate
  REGADR_RESV_4,
  REGADR_CONFIG,                    /// configuration
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
//  CTL_MODE_MAX
} CTLMODE;

// structures -----------------------------------------------------------------
/// define the status register
typedef struct _REGSTATUS
{
  U8  nReserved   : 4;  
  U8  bCmdRdy     : 1;
  U8  bDrdyPress  : 1;
  U8  bDrdyTemp   : 1;
} REGSTATUS, *PREGSTATUS;
#define REGSTATUS_SIZE                  sizeof( REGSTATUS )

/// define the error regiser
typedef struct _REGERR
{
  U8  bFatalErr   : 1;
  U8  bCmdErr     : 1;
  U8  bConfErr    : 1;
} REGERR, *PREGERR;
#define REGERR_SIZE                     sizeof( REGERR )

/// define the power control register
typedef struct _REGPWRCNTL
{
  BOOL    bPressEnable  : 1;          ///< pressure enabled
  BOOL    bTempEnable   : 1;          ///< temperature enabled
  U8      nReserved     : 2;          ///< reserved
  CTLMODE eMode         : 2;          ///< control mode
} REGPWRCNTL, *PREGPWRCNTL;
#define REGPWRCNTL_SIZE                 sizeof( REGPWRCNTL )

/// define the over sampling register
typedef struct _REGOSR
{
  SENBMP388OVERSAMPRATE ePressure     : 3;          ///< oversampling - pressure
  SENBMP388OVERSAMPRATE eTemperature  : 3;          ///< oversampling - temperature
} REGOSR, *PREGOSR;
#define REGOSR_SIZE                     sizeof( REGOSR )

/// define the over data rate register
typedef struct _REGODR
{
  SENBMP388OUTDATARATE eOdrRate      : 5;          ///< sub sampling rate
} REGODR, *PREGODR;
#define REGODR_SIZE                     sizeof( REGODR )

/// defin the interrupt control
typedef struct _REGINTCTL
{
  BOOL  bOutputType   : 1;        ///< open drain
  BOOL  bLevelSel     : 1;        ///< level select
  BOOL  bLatchEnb     : 1;        ///< latch enable
  BOOL  bFifoWmEnb    : 1;        ///< fifo watermark enable
  BOOL  bFifoFullEnb  : 1;        ///< fifo full enable
  BOOL  bReserved     : 1;        ///< reserved
  BOOL  bDataRdyEnb   : 1;        ///< data ready enble
} REGINTCTL, *PREGINTCTL;
#define REGINTCTL_SIZE                  sizeof( REGINTCTL )

/// define the interrupt status
typedef struct _REGINTSTS
{
  BOOL  bFwmInt       : 1;
  BOOL  bFFullInt     : 1;
  BOOL  bReserved     : 1;
  BOOL  bDrdy         : 1;
} REGINTSTS, *PREGINTSTS;
#define REGINTSTS_SIZE                  sizeof( REGINTSTS )

/// define the filter congif
typedef struct _REGCONFIG
{
  BOOL  bRes1         : 1;        ///< reserved 1
  U8    nIirFilter    : 3;        ///< iir filter select
} REGCONFIG, *PREGCONFIG;
#define REGCONFIG_SIZE                  sizeof( REGCONFIG )

/// define the register union
typedef union _REGSALL
{
  REGSTATUS   tRegStatus;
  REGERR      tRegErr;
  REGPWRCNTL  tRegPwrCntl;
  REGOSR      tRegOsr;
  REGODR      tRegOdr;
  REGINTCTL   tRegIntCtl;
  REGCONFIG   tRegConfig;
  REGINTSTS   tRegIntSts;
  U8          nByte;
} REGSALL, *PREGSALL;
#define REGSALL_SIZE                    sizeof( REGSALL )

/// define the data structure
typedef struct PACKED _RAWDATA
{
  U8  nPressure0007;                ///< pressure 0-7
  U8  nPressure0815;                ///< pressure 8-15
  U8  nPressure1623;                ///< pressure 16-23
  U8  nTemperature0007;             ///< pressure 0-7
  U8  nTemperature0815;             ///< pressure 8-15
  U8  nTemperature1623;             ///< pressure 16-23
} RAWDATA, *PRAWDATA;
#define RAWDATA_SIZE                    sizeof( RAWDATA )

/// define the trimming coefficients structure
typedef struct PACKED _TRIMCOEEFRAW
{
  U16 wNvmParT1;
  U16 wNvmParT2;
  S8  cNvmParT3;
  S16 iNvmParP1;
  S16 iNvmParP2;
  S8  cNvmParP3;
  S8  cNvmParP4;
  U16 wNvmParP5;
  U16 wNvmParP6;
  S8  cNvmParP7;
  S8  cNvmParP8;
  S16 iNvmParP9;
  S8  cNvmParP10;
  S8  cNvmParP11;
} TRIMCOEEFRAW, *PTRIMCOEEFRAW;
#define TRIMCOEEFRAW_SIZE                sizeof( TRIMCOEEFRAW )

/// define the trimming coefficients struct float
typedef struct _TRIMCOEEFFLOAT
{
  DOUBLE dParT1;
  DOUBLE dParT2;
  DOUBLE dParT3;
  DOUBLE dParP1;
  DOUBLE dParP2;
  DOUBLE dParP3;
  DOUBLE dParP4;
  DOUBLE dParP5;
  DOUBLE dParP6;
  DOUBLE dParP7;
  DOUBLE dParP8;
  DOUBLE dParP9;
  DOUBLE dParP10;
  DOUBLE dParP11;
} TRIMCOEEFFLOAT, *PTRIMCOEEFFLOAT;
#define TRIMCOEEFFLOAT_SIZE             sizeof( TRIMCOEEFFLOAT )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL            bValidData;
static  FLOAT           fCurPressure;
static  FLOAT           fCurTemperature;
static  FLOAT           fCurAltitude;
static  TRIMCOEEFFLOAT  tTrimCoeef;
static  SENBMP388ERR    eLclError;

// local function prototypes --------------------------------------------------
static  BOOL  WriteCommandRegister( U8 nCommand, U8 nDelayMsecs );
static  BOOL  CheckCommandReady( void );
static  BOOL  ReadComputeCoeeficients( void );
static  FLOAT CompensateTemperature( U32 uTempRaw );
static  FLOAT CompensatePressure( U32 uPressRaw );
static  BOOL  ReadRegisters( REGADR eRegAddr, PU8 pnData, U8 nNumRegs );
static  BOOL  WriteRegister( REGADR eRegAddr, U8 nData );

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
  SENBMP388MODE   eMode;
  CTLMODE         eCtlMode;
  SENBMP388IRQDEF eIrqDef;
  REGSALL         tRegs;
         
  // clear the valid data flag/sensor OK flag
  bValidData = FALSE;
  
  // set the retry loop to 3
  nRetryCount = 3;

  // set the error
  eLclError = SENBMP388_ERR_NODEV;

  // retry loop
  do
  {
    // delay
    SenBMP388_DelayMsec( 5 );
    
    // read the chip ID register
    if ( ReadRegisters( REGADR_CHIP_ID, &nData, 1 ) == TRUE )
    {
      // now compare the chip id
      if ( nData == BMP388_CHIP_ID )
      {
        // set sensor ok/clear the retry count
        eLclError = SENBMP388_ERR_NONE;
        nRetryCount = 0;
      }
    }
  } while( nRetryCount != 0 );
  
  // if sensor is OK
  if ( eLclError == SENBMP388_ERR_NONE )
  {
    // perform a software reset
    if ( !WriteCommandRegister( CMD_SOFT_RESET, 2 ))
    {
      // read the calibration data
      if ( ReadComputeCoeeficients( ) == TRUE )
      {
        // set up OSR
        tRegs.nByte = 0;
        tRegs.tRegOsr.ePressure = PGM_RDBYTE( g_tSenBMP388Config.ePressSampRate );
        tRegs.tRegOsr.eTemperature = PGM_RDBYTE( g_tSenBMP388Config.eTempSampRate );
        WriteRegister( REGADR_OSR, tRegs.nByte );
      
        // set up ODR
        tRegs.nByte = 0;
        tRegs.tRegOdr.eOdrRate = PGM_RDBYTE( g_tSenBMP388Config.eDataRate );
        WriteRegister( REGADR_ODR, tRegs.nByte );
      
        // write the filter
        tRegs.nByte = 0;
        tRegs.tRegConfig.nIirFilter = PGM_RDBYTE( g_tSenBMP388Config.eFiltCoeef );
        WriteRegister( REGADR_CONFIG, tRegs.nByte );

        // check for a valid IRQ
        if (( eIrqDef = PGM_RDBYTE( g_tSenBMP388Config.eIrqPin )) != SENBMP388_IRQDEF_NONE)
        {
          // configure the IRQ pin
          tRegs.nByte = 0;
          tRegs.tRegIntCtl.bOutputType = (( eIrqDef == SENBMP388_IRQDEF_ODLO ) || ( eIrqDef == SENBMP388_IRQDEF_ODHI )) ? ON : OFF;
          tRegs.tRegIntCtl.bLevelSel = (( eIrqDef == SENBMP388_IRQDEF_ODHI ) || ( eIrqDef == SENBMP388_IRQDEF_PPHI )) ? ON : OFF;
          tRegs.tRegIntCtl.bLatchEnb = OFF;
          tRegs.tRegIntCtl.bFifoWmEnb = OFF;
          tRegs.tRegIntCtl.bFifoFullEnb = OFF;
          tRegs.tRegIntCtl.bDataRdyEnb = ON;
          WriteRegister( REGADR_INT_CONTROL, tRegs.nByte );
        }

        // check for a config error
        ReadRegisters( REGADR_ERR_REG, &tRegs.nByte, 1 );
        if ( tRegs.tRegErr.bConfErr )
        {
          // set the error
          eLclError = SENBMP388_ERR_CONF;
        }
        else
        {
          // get the mode
          eMode = PGM_RDBYTE( g_tSenBMP388Config.eMode );
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
          tRegs.nByte = 0;
          tRegs.tRegPwrCntl.eMode = eCtlMode;
          tRegs.tRegPwrCntl.bPressEnable = ON;
          tRegs.tRegPwrCntl.bTempEnable = ON;
          WriteRegister( REGADR_PWRCTRL, tRegs.nByte );
        }
      }
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
  REGSALL   tRegs;
  FLOAT     fK1, fK2, fK3;
  
  // clear the data ready bit by reading the status register
  if ( ReadRegisters( REGADR_INT_STATUS, &tRegs.nByte, 1 ) == TRUE )
  {
    // now check for data ready
    if ( tRegs.tRegIntSts.bDrdy )
    {
      // read the data
      if ( ReadRegisters( REGADR_DATA0_PRS0007, ( PU8 )&tRawData, RAWDATA_SIZE ) == TRUE )
      {
        // compensate temperature
        tTemp.anValue[ LE_U32_LSB_IDX ] = tRawData.nTemperature0007;
        tTemp.anValue[ LE_U32_MS1_IDX ] = tRawData.nTemperature0815;
        tTemp.anValue[ LE_U32_MS2_IDX ] = tRawData.nTemperature1623;
        fCurTemperature = CompensateTemperature( tTemp.uValue );
    
        // compensate pressure
        tTemp.anValue[ LE_U32_LSB_IDX ] = tRawData.nPressure0007;
        tTemp.anValue[ LE_U32_MS1_IDX ] = tRawData.nPressure0815;
        tTemp.anValue[ LE_U32_MS2_IDX ] = tRawData.nPressure1623;
        fCurPressure = CompensatePressure( tTemp.uValue );
    
        // calculate the altitude
        fK1 = PRESTOALT_K1;
        fK2 = PRESTOALT_K2;
        fK3 = PRESTOALT_K3;
        fCurAltitude = ( 1 - pow((( fCurPressure / 100.0f ) / fK1 ), fK2 )) * fK3;

        // set valid data
        bValidData = TRUE;
      }
      else
      {
        // clear valid data
        bValidData = FALSE;
      }
    }
  }
}

/******************************************************************************
 * @function SenBMP388_GetPressure
 *
 * @brief get the current pressure value
 *
 * This function will return the current pressure value
 *
 * @param[io]   pfValue   pointer to store the value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SENBMP388ERR SenBMP388_GetPressure( PFLOAT pfValue )
{
  SENBMP388ERR eError = eLclError;

  // check for local error
  if ( eLclError == SENBMP388_ERR_NONE )
  {
    // return the last value
    *( pfValue ) = fCurPressure;
  }
  else
  {
    // set the default
    *( pfValue ) = 0.0;
  }
  
  // return the value
  return( eError );
}

/******************************************************************************
 * @function SenBMP388_GetAltitude
 *
 * @brief get the current altitude
 *
 * This function will return the current altitude
 *
 * @param[io]   pfValue   pointer to store the value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SENBMP388ERR SenBMP388_GetAltitude( PFLOAT pfValue )
{
  SENBMP388ERR  eError = eLclError;
  FLOAT         fValue;
  
  // check for local error
  if ( eLclError == SENBMP388_ERR_NONE )
  {
    *( pfValue ) = fCurAltitude;
  }
  else
  {
    // set the default
    *( pfValue ) = 0.0;
  }
  
  // return the value
  return( eError );
}

/******************************************************************************
 * @function SenBMP388_GetTemperature
 *
 * @brief get the current temperature value
 *
 * This function will return the current temperature value
 *
 * @param[io]   pfValue   pointer to store the value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SENBMP388ERR SenBMP388_GetTemperature( PFLOAT pfValue )
{
  SENBMP388ERR eError = eLclError;
  
  // check for local error
  if ( eLclError == SENBMP388_ERR_NONE )
  {
    // return the last value
    *( pfValue ) = fCurTemperature;
  }
  else
  {
    // set the default
    *( pfValue ) = 0.0;
  }
  
  // return the value
  return( eError );
}

/******************************************************************************
 * @function SenBMP388_GetFieldValue
 *
 * @brief get the current fields value
 *
 * This function will return the value of the requested field or 0 if invalid
 *
 * @param[in]   eField      field enumeration
 *
 * @return      value of the current field
 *
 *****************************************************************************/
FLOAT SenBMP388_GetFieldValue( SENBMP388FIELD eField )
{
  FLOAT fValue = 0;

  // check for local error
  if ( eLclError == SENBMP388_ERR_NONE )
  {
    // process the field
    switch( eField )
    {
      case SENBMP388_FIELD_PRESSURE :
        fValue = fCurPressure;
        break;

      case SENBMP388_FIELD_ALTITUDE :
        fValue = fCurAltitude;
        break;

      case SENBMP388_FIELD_TEMPERATURE :
        fValue = fCurTemperature;
        break;

      default :
        break;
    }
  }
  
  // return the value
  return( fValue );
}

/******************************************************************************
 * @function WriteCommandRegister
 *
 * @brief write a command to the command register
 *
 * This function will determine device ready to accept a command and write
 * that command
 *
 * @param[in]   nCommand      command to send
 * @param[in]   nDelayMsecs   delay time in milliseconds
 * 
 * @return      TRUE if errors, FALSE if no errors
 *
 *****************************************************************************/
static BOOL WriteCommandRegister( U8 nCommand, U8 nDelayMsecs )
{
  BOOL    bStatus = TRUE;
  REGSALL tRegs;
  
  // check for command ready
  if ( CheckCommandReady( ))
  {
    // now send the command
    if ( WriteRegister( REGADR_COMMAND, nCommand ))
    {
      // delay for 2 MSEC
      SenBMP388_DelayMsec( nDelayMsecs );

      // read the error regiser
      if ( ReadRegisters( REGADR_ERR_REG, &tRegs.nByte, REGSALL_SIZE ))
      {
        // check for error
        if (( !tRegs.tRegErr.bFatalErr ) && ( !tRegs.tRegErr.bCmdErr ))
        {
          // set good status
          bStatus = FALSE;
        }
      }
    }
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function CheckCommandReady
 *
 * @brief check for command ready
 *
 * This function will read the status register and determine if the device
 * is ready to accept a command
 *
 * @return      TRUE if ready, FALSE otherwise
 *
 *****************************************************************************/
static BOOL CheckCommandReady( void )
{
  BOOL    bReady = FALSE;
  REGSALL tRegs;
 
  // read the status registers
  if ( ReadRegisters( REGADR_STATUS, &tRegs.nByte, REGSALL_SIZE ))
  {
    // read ok, now test state of status
    bReady = tRegs.tRegStatus.bCmdRdy;
  }

  // return the ready status
  return( bReady );
}

/******************************************************************************
 * @function ReadComputeCoeeficients
 *
 * @brief read and compute coeeficients
 *
 * This function will read and compute the coefficients
 *
 * @return      TRUE if errors, FALSE if no errors
 *
 *****************************************************************************/
static BOOL ReadComputeCoeeficients( void )
{
  TRIMCOEEFRAW  tCoeefRaw;
  BOOL          bStatus;
  DOUBLE        dblTemp;
  
  // read the coeeficients
  if (( bStatus = ReadRegisters( REGADR_NVMPAR_T10007, ( PU8)&tCoeefRaw, TRIMCOEEFRAW_SIZE )) == TRUE )
  {
    // now adjust the values
    dblTemp = TRIM_COEEF_T1_K;
    tTrimCoeef.dParT1 = (( DOUBLE )tCoeefRaw.wNvmParT1 / dblTemp );
    dblTemp = TRIM_COEEF_T2_K;
    tTrimCoeef.dParT2 = (( DOUBLE )tCoeefRaw.wNvmParT2 / dblTemp );
    dblTemp = TRIM_COEEF_T3_K;
    tTrimCoeef.dParT3 = (( DOUBLE )tCoeefRaw.cNvmParT3 / dblTemp );
    dblTemp = TRIM_COEEF_P1_K2;
    tTrimCoeef.dParP1 = ((( DOUBLE )tCoeefRaw.iNvmParP1 - TRIM_COEEF_P1P2_K1 ) / dblTemp );
    dblTemp = TRIM_COEEF_P2_K2;
    tTrimCoeef.dParP2 = ((( DOUBLE )tCoeefRaw.iNvmParP2 - TRIM_COEEF_P1P2_K1 ) / dblTemp );
    dblTemp = TRIM_COEEF_P3_K;
    tTrimCoeef.dParP3 = (( DOUBLE )tCoeefRaw.cNvmParP3 / dblTemp );
    dblTemp = TRIM_COEEF_P4_K;
    tTrimCoeef.dParP4 = (( DOUBLE )tCoeefRaw.cNvmParP4 / dblTemp );
    dblTemp = TRIM_COEEF_P5_K;
    tTrimCoeef.dParP5 = (( DOUBLE )tCoeefRaw.wNvmParP5 / dblTemp );
    dblTemp = TRIM_COEEF_P6_K;
    tTrimCoeef.dParP6 = (( DOUBLE )tCoeefRaw.wNvmParP6 / dblTemp );
    dblTemp = TRIM_COEEF_P7_K;
    tTrimCoeef.dParP7 = (( DOUBLE )tCoeefRaw.cNvmParP7 / dblTemp );
    dblTemp = TRIM_COEEF_P8_K;
    tTrimCoeef.dParP8 = (( DOUBLE )tCoeefRaw.cNvmParP8 / dblTemp );
    dblTemp = TRIM_COEEF_P9_K;
    tTrimCoeef.dParP9 = (( DOUBLE )tCoeefRaw.iNvmParP9 / dblTemp );
    dblTemp = TRIM_COEEF_P10_K;
    tTrimCoeef.dParP10 = (( DOUBLE )tCoeefRaw.cNvmParP10 / dblTemp );
    dblTemp = TRIM_COEEF_P11_K;
    tTrimCoeef.dParP11 = (( DOUBLE )tCoeefRaw.cNvmParP11 / dblTemp );
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
  DOUBLE dCompTemp, dData1, dData2;
  
  // apply the compensation
  dData1 = (( DOUBLE )( uTempRaw ) - tTrimCoeef.dParT1 );
  dData2 = ( dData1 * tTrimCoeef.dParT2 );
  dCompTemp = dData2 + (( dData1 * dData1 ) * tTrimCoeef.dParT3 );
  
  // return the compensated temperature
  return(( FLOAT )dCompTemp );
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
  DOUBLE dCompPress, dData1, dData2, dData3, dData4, dData5, dData6;
  DOUBLE dTempSquared, dTempCubed;
  DOUBLE dPressSquared, dPressCubed;

  // calculate temperature squared/cubed
  dTempSquared = ( DOUBLE )fCurTemperature * ( DOUBLE )fCurTemperature;
  dTempCubed = dTempSquared * ( DOUBLE )fCurTemperature;

  // calculate pressure squared/cubed
  dPressSquared = ( DOUBLE )uPressRaw * ( DOUBLE )uPressRaw;
  dPressCubed = ( DOUBLE )uPressRaw * dPressSquared;
  
  // apply the compensation
  dData1 = tTrimCoeef.dParP6 * ( DOUBLE )fCurTemperature;
  dData2 = tTrimCoeef.dParP7 * dTempSquared;
  dData3 = tTrimCoeef.dParP8 * dTempCubed;
  dData5 = tTrimCoeef.dParP5 + dData1 + dData2 + dData3;
  dData1 = tTrimCoeef.dParP2 * ( DOUBLE )fCurTemperature;
  dData2 = tTrimCoeef.dParP3 * dTempSquared;
  dData3 = tTrimCoeef.dParP4 * dTempCubed;
  dData6 = uPressRaw * ( tTrimCoeef.dParP1 + dData1 + dData2 + dData3 );
  dData2 = tTrimCoeef.dParP9 + tTrimCoeef.dParP10 * ( DOUBLE )fCurTemperature;
  dData3 = dPressSquared * dData2;
  dData4 = ( dData3 + dPressCubed ) * tTrimCoeef.dParP11;
  dCompPress = dData5 + dData6 + dData4;
  
  // return the compensated pressure
  return(( FLOAT )dCompPress );
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

  // call the indirection
  bResult = SenBMP388_Read( eRegAddr, pnData, nNumRegs );
  
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
static BOOL WriteRegister( REGADR eRegAddr, U8 nData )
{
  BOOL bResult;

  // call the indirection
  bResult = SenBMP388_Write( eRegAddr, &nData );
  
  // return the result
  return( bResult );
}

/**@} EOF SenBMP3988.c */
