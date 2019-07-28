/******************************************************************************
 * @file RtcDs1340.c
 *
 * @brief RTC DS1340 implementation
 *
 * This file provides the implementation of the driver for the DS1340
 * Readl Time CLock
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup RtcDs1340
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RTCDS1340/RtcDs1340_prm.h"
#include "RTCDS1340/RtcDs1340.h"

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the DS1340 RTC slave address
#define	DS1340_SLV_ADDR	      ( 0x68 )

/// define the battery charging default value
#define	TCH_BAT_DFL		0xAB

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _RTCREG
{
  RTC_REG_SEC = 0,                ///< seconds
  RTC_REG_MIN,                    ///< minutes
  RTC_REG_CHR,                    ///< century/hours
  RTC_REG_DAY,                    ///< day
  RTC_REG_DAT,                    ///< date
  RTC_REG_MON,                    ///< month
  RTC_REG_YER,                    ///< year
  RTC_REG_CTL,                    ///< control
  RTC_REG_TCH,                    ///< trickle charger
  RTC_REG_FLG                     ///< flag
} RTCREG;

/// enujmerate the chargeing resistor
typedef enum _CHARGERESEL
{
  CHARGESEL_RES_DISABLED = 0,     ///< disabled
  CHARGESEL_RES_250,              ///< 250 ohm
  CHARGESEL_RES_2000,             ///< 2000 ohm
  CHARGESEL_RES_4000,             ///< 4000 ohm
} CHARGERESSEL;

/// enumerate the diode selection
typedef enum _DIODSEL
{
  DIODESEL_NONE_RESDSB = 0,       ///< disabled
  DIODESEL_NONE_RESENB,           ///< diode disabled no resistor
  DIODESEL_ENAB_RESENB            ///< disode enabled, resistor enabled
} DIODEENB;

// structures -----------------------------------------------------------------
/// define the RTC structure
typedef struct _D1340SREGS
{
  union
  {
    struct
    {
      U8    nSecOnes      : 4;      ///< seconds 
      U8    nSecTens      : 3;      ///< seconds - tens
      U8    bOscDisabled  : 1;      ///< osciallator disabled
    } tFields;
    U8  nByte;
  } tSecs;
  union
  {
    struct
    {
      U8    nMinOnes      : 4;      ///< minutes 
      U8    nMinTens      : 3;      ///< minutes - tens
      U8    nReserved     : 1;      ///< reserved
    } tFields;
    U8  nByte;
  } tMins;
  union
  {
    struct 
    {
      U8    nHorOnes      : 4;      ///< hours
      U8    nHorTens      : 2;      ///< hours - tens
      U8    bCentFlg      : 1;      ///< century flag
      U8    bCentEnb      : 1;      ///< enable century
    } tFields;
    U8  nByte;
  } tHours;
  union
  {
    struct  
    {
      U8  nDay            : 3;      ///< days
      U8  nReserved       : 5;      ///< reserve
    } tFields;
    U8 nByte;
  } tDay;
  union
  {
    struct  
    {
      U8  nDateOnes       : 4;      ///< date
      U8  nDateTens       : 2;      ///< date - tens   
    } tFields;
    U8 nByte;
  } tDate;  
  union
  {
    struct
    {
      U8    nMonOnes      : 4;      ///< months
      U8    nMonTens      : 1;      ////< months - tens
      U8    nReserved     : 3;      ///< reserved
    } tFields;
    U8  nByte;
  } tMonth;
  union
  {
    struct
    {
      U8    nYerOnes      : 4;      ///< years
      U8    nYerTens      : 1;      ///< years - ten
      U8    nReserved     : 3;      ///< reserved
    } tFields;
    U8  nByte;
  } tYear;
  union
  {
    struct
    {
      U8    nCalib        : 5;      ///< calibration
      U8    bCalibNeg     : 1;      ///< negative calibration
      U8    bFreqTestEnb  : 1;      ///< enable the frequency test
      U8    bOutInbDsb    : 1;      ///< disable the inversion on the Output pin
    } tFields;
    U8  nByte;
  } tCtl;
  union
  {
    struct
    {
      U8    nResOut       : 2;      ///< resistor  select
      U8    nDiodeResSel  : 2;      ///< diode enable
      U8    nChargeSel    : 4;      ///<  charge rate
    } tFields;
    U8  nByte;
  } tTrickle;
  union
  {
    struct
    {
      U8    bOscPwrLoss   : 1;      ///< oscillator power loss
      U8    nReserved     : 7;      ///< reserved
    } tFields;
    U8  nByte;
  } tFlag;
} DS1340REGS, *PRTGCREGS;
#define DS1340REGS_SIZE          sizeof( DS1340REGS )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  U8    HexToBcd( U8 nHexValue );
static  void  BcdToHex( U8 nBcdMsb, U8 nBcdLsb, PU8 pnHexValue );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RtcDs1340_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the real time clock 
 *
 *****************************************************************************/
void RtcDs1340_Initialize( void )
{
  DS1340REGS  tRegs;
  I2CXFRCTL   tCtl;

  // set up the control block
  tCtl.nDevAddr         = DS1340_SLV_ADDR;
  tCtl.nAddrLen         = 1;
  tCtl.tAddress.uValue  = RTC_REG_TCH;
  tCtl.pnData           = ( PU8 )&tRegs.tTrickle;
  tCtl.wDataLen         = 2;
  tCtl.uTimeout         = 10;
  
  // now read the registers
  if ( I2c_Read( RTCDS1340_I2C_DEV_ENUM, &tCtl ) == I2C_ERR_NONE )
  {
    // now test power loss
    if ( tRegs.tFlag.tFields.bOscPwrLoss )
    {
      // reset all regs
      memset( &tRegs, 0, DS1340REGS_SIZE );
      
      // set the trickle charge to on
      tRegs.tTrickle.tFields.nResOut = CHARGESEL_RES_4000;
      tRegs.tTrickle.tFields.nDiodeResSel = DIODESEL_ENAB_RESENB;
      tRegs.tTrickle.tFields.nChargeSel = 0x05;
      
      // set up control block
      tCtl.nDevAddr         = DS1340_SLV_ADDR;
      tCtl.nAddrLen         = 1;
      tCtl.tAddress.uValue  = RTC_REG_SEC;
      tCtl.pnData           = ( PU8 )&tRegs;
      tCtl.wDataLen         = DS1340REGS_SIZE;
      tCtl.uTimeout         = 100;
      
      // now write it
      I2c_Write( RTCDS1340_I2C_DEV_ENUM, &tCtl );
    }
  }
}

/******************************************************************************
 * @function RtcDs1340_SetDateTime
 *
 * @brief set the date time
 *
 * This function will convert the values passed in the date time structure
 * to BCD, fill out the control block and write it
 *
 * @param[in]   ptDateTime    pointer to the desired date time structure
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RTCDS1340ERR  RtcDs1340_SetDateTime( PDATETIME ptDateTime )
{
  RTCDS1340ERR  eError;
  DS1340REGS    tRegs;
  I2CXFRCTL     tCtl;
  
  // fill the structure
  tRegs.tSecs.nByte = HexToBcd( ptDateTime->nSeconds );
  tRegs.tMins.nByte = HexToBcd( ptDateTime->nMinutes );
  tRegs.tHours.nByte = HexToBcd( ptDateTime->nHours );
  tRegs.tMonth.nByte = HexToBcd( ptDateTime->nMonth );
  tRegs.tDate.nByte = HexToBcd( ptDateTime->nDay );
  tRegs.tDay.nByte = ptDateTime->nDayOfWeek;
  tRegs.tYear.nByte = HexToBcd(( U8 )( ptDateTime->wYear - RTCDS1340_CENTURY_VALUE ));
  
  // set up the control block
  tCtl.nDevAddr         = DS1340_SLV_ADDR;
  tCtl.nAddrLen         = 1;
  tCtl.tAddress.uValue  = RTC_REG_SEC;
  tCtl.pnData           = ( PU8 )&tRegs;
  tCtl.wDataLen         = DS1340REGS_SIZE - 2;
  tCtl.uTimeout         = 100;
  
  // now write it
  eError = ( I2c_Write( RTCDS1340_I2C_DEV_ENUM, &tCtl ) == I2C_ERR_NONE ) ? RTCDS1340_ERR_NONE : RTCDS1340_ERR_XFR;
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function RtcDs1340_GetDateTime
 *
 * @brief get the current date time
 *
 * This function will read the current date time from the RTC and convert it
 * to a time structure
 *
 * @param[in]   ptDateTime    pointer to the date time structure
 *
 *****************************************************************************/
RTCDS1340ERR RtcDs1340_GetDateTime( PDATETIME ptDateTime )
{
  RTCDS1340ERR  eError = RTCDS1340_ERR_NONE;
  DS1340REGS    tRegs;
  I2CXFRCTL     tCtl;
  U8            nTemp;

  // set up the control block
  tCtl.nDevAddr         = DS1340_SLV_ADDR;
  tCtl.nAddrLen         = 1;
  tCtl.tAddress.uValue  = RTC_REG_SEC;
  tCtl.pnData           = ( PU8 )&tRegs;
  tCtl.wDataLen         = DS1340REGS_SIZE - 2;
  tCtl.uTimeout         = 100;
  
  // now read the registers
  if ( I2c_Read( RTCDS1340_I2C_DEV_ENUM, &tCtl ) == I2C_ERR_NONE )
  {
    // fill the time
    BcdToHex( tRegs.tSecs.tFields.nSecTens, tRegs.tSecs.tFields.nSecOnes, &ptDateTime->nSeconds );
    BcdToHex( tRegs.tMins.tFields.nMinTens, tRegs.tMins.tFields.nMinOnes, &ptDateTime->nMinutes );
    BcdToHex( tRegs.tHours.tFields.nHorTens, tRegs.tHours.tFields.nHorOnes, &ptDateTime->nHours );
    BcdToHex( tRegs.tMonth.tFields.nMonTens, tRegs.tMonth.tFields.nMonOnes,  &ptDateTime->nMonth );
    ptDateTime->nDayOfWeek = tRegs.tDay.tFields.nDay;
    BcdToHex( tRegs.tDate.tFields.nDateTens, tRegs.tDate.tFields.nDateOnes, &ptDateTime->nDay );
    BcdToHex( tRegs.tYear.tFields.nYerTens, tRegs.tYear.tFields.nYerOnes,  &nTemp );
    ptDateTime->wYear = nTemp + RTCDS1340_CENTURY_VALUE;
  }
  else
  {
    // set the error
    eError = RTCDS1340_ERR_XFR;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function HexToBcd
 *
 * @brief convert a hex value to a bcd value
 *
 * This function will convert the hex valud to bcd value
 *
 * @param[in]   nHexValue   hex value to convert
 *
 *****************************************************************************/
static U8 HexToBcd( U8 nHexValue )
{
	U8	nBcd = 0;
	U8	nMsb;

	// convert it
	nMsb = nHexValue / 10;
	nBcd = nHexValue - ( nMsb * 10 );
	nBcd |= ( nMsb << 4 );

	// return the value
	return( nBcd );
}


/******************************************************************************
 * @function BcdToHex
 *
 * @brief convert a BCD value to a HEX value
 *
 * This function will convert the bcd value to a hex value
 *
 * @param[in]   nBcdMsb    BCD msb
 * @param[in]   nBcdLsb    BCD lsb
 * @param[in]   pnHexValue  poitner to store hex value
 *
 *****************************************************************************/
static void BcdToHex( U8 nBcdMsb, U8 nBcdLsb, PU8 pnHexValue )
{
  // convert and store
  *( pnHexValue ) = ( nBcdMsb * 10 ) + nBcdLsb;
}

/**@} EOF RtcDs1340.c */
