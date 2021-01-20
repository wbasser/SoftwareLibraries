/******************************************************************************
 * @file ImuICM20948_cfg.c
 *
 * @brief IMU ICM20948 configuration implementations
 *
 * This file provides the configuration implementation for the IMU ICM-20948
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
#include "FallDetectHandler/FallDetectHandler.h"
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------
/// define the transfer timeout
#define IMUICM20948_XFR_TIMEOUT_MSECS                 ( 100 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  BOOL  SelectRegisterBank( U8 nBank );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ImuICM20948_LocalInitialize
 *
 * @brief local initialization
 *
 * This function process the local initialization
 *
 *****************************************************************************/
void ImuICM20948_LocalInitialize( void )
{
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
/******************************************************************************
 * @function ImuICM20948_ProcessCallback
 *
 * @brief process task event
 *
 * This function will process the task event
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE to flush the event
 *
 *****************************************************************************/
BOOL ImuICM20948_PollEvent( TASKARG xArg )
{
  BOOL  bDataReady = FALSE;

  // check what mode we're in
  #if ( IMUICM20948_IRQ_OPMODE_ENABLE == 1 )
    if ( xArg  == IMUICM20948_DATAREADY_EVENT )
    {
      // set data ready
      bDataReady = TRUE;
    }
  #else
    // read the int status1 register
    U8 nData;

    // read the status register
    ImuICM20948_ReadRegisters( BANK0_REGS_INTSTATUS1, &nData, 1 );

    // if data ready
    bDataReady = ( nData != 0 ) ? TRUE : FALSE;

    // clear it if set
    if ( bDataReady )
    {
      nData = 0;
      ImuICM20948_WriteRegisters( BANK0_REGS_INTSTATUS1, &nData, 1 );
    }
  #endif

  // call the base event
  if ( bDataReady )
  {
    // process it
    ImuICM20948_ProcessDataReady( );
  }

  // return tre to flush event
  return( TRUE );
}
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

#if ( IMUICM20948_IRQ_OPMODE_ENABLE == 1 )
/******************************************************************************
 * @function ImuICM20948_ProcessIrqCallback
 *
 * @brief IRQ callback
 *
 * This function will process the IRQ callback
 *
 * @param[in]   nIrq      IRQ enumeration
 * @param[in]   bState    state of the pin
 *
 *****************************************************************************/
void ImuICM20948_ProcessIrqCallback( U8 nIrq, BOOL bState )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    // post the event
    TaskManager_PostEventIrq( TASK_SCHD_ENUM_IMUDATARDY, IMUICM20948_HANDLEIRQ_EVENT );
  #endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
}
#endif

/******************************************************************************
 * @function ImuICM20948_ReadRegisters
 *
 * @brief read registers from the device
 *
 * This function will read the registers from the device per the established
 * interface
 *
 * @param[in]   wBaseReg        base register
 * @param[in]   pnData          pointer to the data
 * @param[in]   nLength         length of the data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL ImuICM20948_ReadRegisters( U16 wBaseReg, PU8 pnData, U8 nLength )
{
  BOOL      bStatus = FALSE;
  I2CXFRCTL tXfrCtl;
  U8        nBank;

  // get the bank
  nBank = HI16( wBaseReg );
  if (( bStatus = SelectRegisterBank( nBank )) == TRUE )
  {
    // now read it
    tXfrCtl.nDevAddr        = IMUICM20948_BASE_ADDR;
    tXfrCtl.nAddrLen        = 1;
    tXfrCtl.tAddress.uValue = LO16( wBaseReg );
    tXfrCtl.pnData          = pnData;
    tXfrCtl.wDataLen        = nLength;
    tXfrCtl.uTimeout        = IMUICM20948_XFR_TIMEOUT_MSECS;

    // now do it
    bStatus = ( I2c_Read( I2C_DEV_ENUM_LCLBUS, &tXfrCtl ) == I2C_ERR_NONE ) ? TRUE : FALSE;
  }
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ImuICM20948_WriteRegisters
 *
 * @brief write registers from the device
 *
 * This function will write the registers from the device per the established
 * interface
 *
 * @param[in]   wBaseReg        base register
 * @param[in]   pnData          pointer to the data
 * @param[in]   nLength         length of the data
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL ImuICM20948_WriteRegisters( U16 wBaseReg, PU8 pnData, U8 nLength )
{
  BOOL bStatus = FALSE;
  I2CXFRCTL tXfrCtl;
  U8        nBank;

  // get the bank
  nBank = HI16( wBaseReg );

  // select the apppropriate bank
  if (( bStatus = SelectRegisterBank( nBank )) == TRUE )
  {
    // now write it
    tXfrCtl.nDevAddr        = IMUICM20948_BASE_ADDR;
    tXfrCtl.nAddrLen        = 1;
    tXfrCtl.tAddress.uValue = LO16( wBaseReg );
    tXfrCtl.pnData          = pnData;
    tXfrCtl.wDataLen        = nLength;
    tXfrCtl.uTimeout        = IMUICM20948_XFR_TIMEOUT_MSECS;

    // now do it
    bStatus = ( I2c_Write( I2C_DEV_ENUM_LCLBUS, &tXfrCtl ) == I2C_ERR_NONE ) ? TRUE : FALSE;
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ImuICM20948_PostDataEvent
 *
 * @brief post data event to external device
 *
 * This function will be called on new data
 *
 *****************************************************************************/
void ImuICM20948_PostDataEvent( void )
{
  // post the new data event
  TaskManager_PostEvent( TASK_SCHD_ENUM_FALLDET, FALLDETECT_NEWDATA_AVAIL_EVENT );
}

/******************************************************************************
 * @function SelectRegisterBank
 *
 * @brief select register bank
 *
 * This function will select the register bank
 *
 * @param[in]   nBank          register bank
 *
 * @return      TRUE if no errors, FALSE otherwise
 *
 *****************************************************************************/
static BOOL SelectRegisterBank( U8 nBank )
{
  I2CXFRCTL   tXfrCtl;
  BANKXBNKSEL tBankSelect;

  // set the bank select
  tBankSelect.nByte = 0;
  tBankSelect.tFields.nBankSel = nBank;

  // select the apppropriate bank
  tXfrCtl.nDevAddr        = IMUICM20948_BASE_ADDR;
  tXfrCtl.nAddrLen        = 1;
  tXfrCtl.tAddress.uValue = BANKX_BANK_SEL;
  tXfrCtl.pnData          = &tBankSelect.nByte;
  tXfrCtl.wDataLen        = 1;
  tXfrCtl.uTimeout        = IMUICM20948_XFR_TIMEOUT_MSECS;
  return(( I2c_Write( I2C_DEV_ENUM_LCLBUS, &tXfrCtl ) == I2C_ERR_NONE ) ? TRUE : FALSE );
}

/**@} EOF ImuICM20948_cfg.c */
