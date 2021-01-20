/******************************************************************************
 * @file I2c.c
 *
 * @brief I2C software implementation
 *
 * This file provides the implementation for the I2C interface
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
 * $Log: $
 * 
 *
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
/// define the control register macro
#define SET_TWCR( cur, mask, val ) ( TWCR = (( cur & mask ) | val ))

/// define the nStatus register macro
#define GET_TWSR  TWSR & 0xF8

/// define the mask values
#define TWCR_NRM_MASK ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWEA ) | BIT( TWIE ))
#define TWCR_XDN_MASK ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWEA ))
#define TWCR_RDN_MASK ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWIE ))
#define TWCR_BER_MASK ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWEA ) | BIT( TWIE ) | BIT( TWSTA ) | BIT( TWSTO ))
#define TWCR_ERR_MASK ( BIT( TWEA )  | BIT( TWEN ) | BIT( TWSTA ) | BIT( TWSTO ))

/// define the command values
#define TWCR_CMD_ENB  ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWIE ))
#define TWCR_CMD_STR  ( BIT( TWINT ) | BIT( TWSTA ) | BIT( TWIE ))
#define TWCR_CMD_RST  ( BIT( TWSTA ) | BIT( TWEA ))
#define TWCR_CMD_STP  ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWSTO ))
#define TWCR_CMD_SSTP ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWSTO ) | BIT( TWEA ) | BIT( TWIE ))
#define TWCR_CMD_SSTR ( BIT( TWINT ) | BIT( TWEN ) | BIT( TWEA ) | BIT( TWIE ))

/// Master nStatus
#define I2C_START                   0x08
#define I2C_REP_START               0x10

/// master transmitter nStatus
#define I2C_MT_SLA_ACK              0x18
#define I2C_MT_SLA_NACK             0x20
#define I2C_MT_DATA_ACK             0x28
#define I2C_MT_DATA_NACK            0x30
#define I2C_MX_ARB_LOST             0x38

/// master receiver nStatus
#define I2C_MR_SLA_ACK              0x40
#define I2C_MR_SLA_NACK             0x48
#define I2C_MR_DATA_ACK             0x50
#define I2C_MR_DATA_NACK            0x58

/// slave transmitter nStatus
#define I2C_ST_SLA_ACK              0xA8
#define I2C_ST_ARB_LOST_SLA_ACK     0xB0
#define I2C_ST_DATA_ACK             0xB8
#define I2C_ST_DATA_NACK            0xC0
#define I2C_ST_LAST_DATA            0xC8

/// slave receiver nStatus
#define I2C_SR_SLA_ACK              0x60
#define I2C_SR_ARB_LOST_SLA_ACK     0x68
#define I2C_SR_GCALL_ACK            0x70
#define I2C_SR_ARB_LOST_GCALL_ACK   0x78
#define I2C_SR_DATA_ACK             0x80
#define I2C_SR_DATA_NACK            0x88
#define I2C_SR_GCALL_DATA_ACK       0x90
#define I2C_SR_GCALL_DATA_NACK      0x98
#define I2C_SR_STOP                 0xA0

/// miscellaneous nStatus
#define I2C_NO_INFO                 0xF8
#define I2C_BUS_ERROR               0x00

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PVI2CCALLBACK     pvLclCallback;
static  I2CMODE           eCurMode;
static  VBOOL             bBusBusy;
static  U16               wMstIndex;
static  BOOL              bMstInProgress;
static  BOOL              bWriteMode;
static  I2CERROR          eLclError;
static  PI2CXFRCTL        ptLclXfrCtl;

// local function prototypes --------------------------------------------------
static  void  ResetDevice( void );

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief intialize the I2C
 *
 * This function will perform any necessary initialization for I2C
 *
 *****************************************************************************/
void I2c_Initialize( void )
{
  I2CCFGDEF tDef;
  
  // get a copy of the definition block
  MEMCPY_P( &tDef, &tI2cDef, I2CCFGDEF_SIZE );
  
  // save the callback function/mode
  pvLclCallback = tDef.pvCallback;
  eCurMode = tDef.eMode;

  // set the slave address
  TWAR = ( tDef.nSlaveAddr << 1 ) | 1;

  // set the speed
  I2c_Ioctl( I2C_DEV_ENUM_LCLBUS, I2C_ACTION_SET_BAUDRATE, ( PVOID )&tDef.uBaudRate );

  // are we in slave or master/slave mode
  if(( tDef.eMode == I2C_MODE_SLAVE ) || ( tDef.eMode == I2C_MODE_MULTI_MASTER ))
  {
    // enable the TWI
    TWCR = TWCR_NRM_MASK;
  }
}

/******************************************************************************
 * @function I2c_Write    
 *
 * @brief I2C write function
 *
 * This function will start a write function and wait for completion
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXfrCtl    pointero to the transfer control  
 *
 * @return      appropriate error
 *
 *****************************************************************************/
I2CERROR I2c_Write( I2CDEVENUM eDevEnum, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR  eError = I2C_ERROR_NONE;
  U32       uTimeValue;

  // check to see if we are in master mode
  if( eCurMode != I2C_MODE_SLAVE )
  {
    // check for bus busy
    if( bBusBusy == FALSE )
    {
      // copy the parameters
      ptLclXfrCtl = ptXfrCtl;
      wMstIndex = 0;
      bWriteMode = TRUE;

      // start the transfer/set the transmitter busy
      bMstInProgress = TRUE;
      SET_TWCR( TWCR, TWCR_XDN_MASK, TWCR_CMD_STR );

      // is there a timeout
      if(  ptLclXfrCtl->uTimeout != 0 )
      {
        // get the current time/add the timeout
        uTimeValue = SystemTick_GetTimeMsec( ) +  ptLclXfrCtl->uTimeout;

        // wait till done
        while( bMstInProgress == TRUE )
        {
          // if timeout not equal to 0
          if(  ptLclXfrCtl->uTimeout != 0 )
          {
            if( SystemTick_GetTimeMsec( ) >= uTimeValue )
            {
              // reset the device/set the error
              ResetDevice( );
              eError = I2C_ERROR_TIMEOUT;
              break;
            }
          }
        }
      }

      // check for timeout
      if( eError != I2C_ERROR_TIMEOUT )
      {
        // set the return error
        if( eLclError == I2C_ERROR_NONE )
        {
          // set error based on complete transmission
          eError = ( wMstIndex ==  ptLclXfrCtl->wDataLen ) ? I2C_ERROR_NONE : I2C_ERROR_PARTIAL_XFR;
        }
        else
        {
          // set the return error equal to local error
          eError = eLclError;
        }
      }
    }
    else
    {
      // indicate a busy busy error
      eError = I2C_ERROR_BUS_BUSY;
    }
  }
  else
  {
    // indicate an illegal mode
    eError = I2C_ERROR_ILL_MODE;
  }

  // clear the master size
   ptLclXfrCtl->wDataLen = 0;
  
  // return nStatus
  return( eError );
}

/******************************************************************************
 * @function I2c_Read
 *
 * @brief I2C read function
 *
 * This function will start a read of a device and return the values read
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXfrCtl    pointero to the transfer control  
 *
 * @return      appropriate error
 *
 *****************************************************************************/
I2CERROR I2c_Read( I2CDEVENUM eDevEnum, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR  eError = I2C_ERROR_NONE;
  U32       uTimeValue;

  // check to see if we are in master mode
  if( eCurMode == I2C_MODE_MASTER )
  {
    // check for bus busy
    if( bBusBusy == FALSE )
    {
      // copy the parameters
      ptXfrCtl = ptXfrCtl;
      wMstIndex = 0;
      bWriteMode = FALSE;

      // start the transfer/set the transmitter busy
      bMstInProgress = TRUE;
      SET_TWCR( TWCR, TWCR_XDN_MASK, TWCR_CMD_STR );

      // is there a timeout
      if(  ptLclXfrCtl->uTimeout != 0 )
      {
        // get the current time/add the timeout
        uTimeValue = SystemTick_GetTimeMsec( ) +  ptLclXfrCtl->uTimeout;

        // wait till done
        while( bMstInProgress == TRUE )
        {
          // if timeout not equal to 0
          if(  ptLclXfrCtl->uTimeout != 0 )
          {
            if( SystemTick_GetTimeMsec( ) >= uTimeValue )
            {
              // reset the device/set the error
              ResetDevice( );
              eError = I2C_ERROR_TIMEOUT;
              break;
            }
          }
        }
      }

      // check for timeout
      if( eError != I2C_ERROR_TIMEOUT )
      {
        // set the return error
        if( eLclError == I2C_ERROR_NONE )
        {
          // set error based on complete transmission
          eError = ( wMstIndex ==  ptLclXfrCtl->wDataLen ) ? I2C_ERROR_NONE : I2C_ERROR_PARTIAL_XFR;
        }
        else
        {
          // set the return error equal to local error
          eError = eLclError;
        }
      }
    }
    else
    {
      // indicate a busy busy error
      eError = I2C_ERROR_BUS_BUSY;
    }
  }
  else
  {
    // indicate an illegal mode
    eError = I2C_ERROR_ILL_MODE;
  }

  // clear the master size
   ptLclXfrCtl->wDataLen = 0;
  
  // return nStatus
  return( eError );
}

/******************************************************************************
 * @function I2c_Ioctl
 *
 * @brief I2C parameter modifications
 *
 * This function will perform the correct parameter  or setup modification 
 * as requested by the action
 *
 * @param[in]   eAction         action to perform
 * @param[in]   pvParam         pointer to a parameters
 *
 * @return      appropriate error
 *
 *****************************************************************************/
I2CERROR I2c_Ioctl( I2CDEVENUM eDevEnum, I2CACTION eAction, PVOID pvParam )
{
  I2CERROR  eError = I2C_ERROR_NONE;
  U32       uTemp;

  switch( eAction )
  {
    case I2C_ACTION_SET_BAUDRATE :
      // calculate the reload value
      uTemp = ((((( 2 * SystemTick_GetTimeMsec( )) / *((U32*)pvParam )) + 1 ) / 2) - 16 ) / 2;

      // check for divide by 16
      if( uTemp > 1020 )
      {
        // divide by 16/set TWPS1
        uTemp /= 16;
        TWSR |= BIT( TWPS1 );
      }
      else
      {
        // clear TWPS1
        TWSR &= ~BIT( TWPS1 );
      }

      // check for devide by     4
      if( uTemp > 255 )
      {
          // divide by 4/set TWPS0
        uTemp /= 4;
        TWSR |= BIT( TWPS0 );
      }
      else
      {
        // clear TWPS0
        TWSR &= ~BIT( TWPS0 );
      }

      // check for valid
      if(( uTemp > 9 ) && ( uTemp < 256 ))
      {
        // set the baudrate
        TWBR = uTemp;
      }
      else
      {
        eError = I2C_ERROR_ILL_PARAMETER;
      }
      break;

    case I2C_ACTION_GET_BAUDRATE :
      uTemp = 2;
      if( TWSR & TWPS0 )
      {
        uTemp *= 4;
      }
      if( TWSR & TWPS1 )
      {
        uTemp *= 16;
      }
      *((U32*)pvParam ) = SystemTick_GetTimeMsec( ) / ( 16 + uTemp * TWBR );
      break;

    case I2C_ACTION_SET_SLVADDR :
      uTemp = (*((U32*)pvParam ));
      TWAR = ( uTemp << 1 ) | 1;
      break;

    case I2C_ACTION_GET_SLVADDR :
      *((U32*)pvParam ) = TWAR >> 1;
      break;
      
    case I2C_ACTION_SET_SLVMASK :
      uTemp = (*((U32*)pvParam ));
      TWAMR = uTemp << 1;
      break;
      
    case I2C_ACTION_GET_SLVMASK :
      *((U32*)pvParam ) = TWAMR >> 1;
      break;

    case I2C_ACTION_GET_BUS_BUSY :
      *((U32*)pvParam ) = bBusBusy;
      break;
      
    case I2C_ACTION_RESET_DEVICE :
      ResetDevice( );
      break;

    default :
      eError = I2C_ERROR_ILL_ACTION;
      break;
  }

  // return nStatus
  return( eError );
}

/******************************************************************************
 * @function I2c_Close
 *
 * @brief close the I2C 
 *
 * This function will stop all action on the I2C bus
 *
 *****************************************************************************/
void I2c_Close( void )
{
  // clear the control register
  TWCR = 0;
}
 
/******************************************************************************
 * @function ResetDevice
 *
 * @brief reset device
 *
 * This function will reset the I2C controller
 *
 *****************************************************************************/
static void ResetDevice( void )
{
  // turn off the device/delay/turn on the device
  TWCR = 0;
  asm volatile( "nop\n\t" );
  asm volatile( "nop\n\t" );
  asm volatile( "nop\n\t" );
  asm volatile( "nop\n\t" );
  asm volatile( "nop\n\t" );
  TWCR = TWCR_NRM_MASK;

  // clear any flags
  bMstInProgress = bBusBusy = FALSE;
}

/***************************************************************//**
 * @function I2C0_Irq_Handler
 *
 * @brief interrupt handler
 *
 * This function is the main loop for this module
 *
 *//****************************************************************/
ISR( TWI_vect )
{
  U8                nStatus, nTemp, nTwcr;
  I2CCALLBACKSTATUS eCbStatus;

  // get the nStatus.current control
  nStatus = TWSR & 0xF8;
  nTwcr = TWCR;

  // process it
  switch( nStatus )
  {
    // 0x08: start condition has been transmitted
    // 0x10: repeated start condiciton has been transmitted
    case I2C_START :
    case I2C_REP_START :
      // do we have transmit data to send
      if(( ptLclXfrCtl->nAddrLen != 0 ) || ((  ptLclXfrCtl->wDataLen != 0 ) && ( bWriteMode == TRUE )))
      {
        // output the slave address in write mode
        TWDR = ptLclXfrCtl->nDevAddr << 1;
      }
      else
      {
        // output the slave address in read mode
        TWDR = ( ptLclXfrCtl->nDevAddr << 1 ) | 1;
      }

      // continue
      SET_TWCR( nTwcr, TWCR_NRM_MASK, 0 );
      break;

    // 0x18: SLA+W has been transmitted and ACK has been received
    // 0x28: data byte has been transmitted and ACK has been received
    case I2C_MT_SLA_ACK :
    case I2C_MT_DATA_ACK :
      // check to see if more address
      if (  ptLclXfrCtl->nAddrLen != 0 )
      {
        // send the next address byte
        TWDR =  ptLclXfrCtl->tAddress.anValue[ ptLclXfrCtl->nAddrLen-- ];
        SET_TWCR( nTwcr, TWCR_NRM_MASK, 0 );
      }
      // check for more data
      else if( wMstIndex < ptLclXfrCtl->wDataLen )
      {
        // output the next byte of data/increment the index
        TWDR = *(  ptLclXfrCtl->pnData + wMstIndex++ );
        SET_TWCR( nTwcr, TWCR_NRM_MASK, 0 );
      }
      else
      {
        // send stop/clear error/clear busy
        if( eCurMode == I2C_MODE_MULTI_MASTER )
        {
          SET_TWCR( nTwcr, TWCR_XDN_MASK, TWCR_CMD_STP );
          asm volatile( "nop\n\t" );
          asm volatile( "nop\n\t" );
          asm volatile( "nop\n\t" );
          asm volatile( "nop\n\t" );
          asm volatile( "nop\n\t" );
          SET_TWCR( nTwcr, TWCR_XDN_MASK, TWCR_CMD_SSTR );
        }
        else
        {
          SET_TWCR( nTwcr, TWCR_XDN_MASK, TWCR_CMD_STP );
        }
        eLclError = I2C_ERROR_NONE;
        bMstInProgress = FALSE;
      }
      break;

    // 0x20: SLA+W has been transmitted,but not acknowledged
    // 0x48: SLA+R has been transmitted, but not acknowledged
    case I2C_MT_SLA_NACK :
    case I2C_MR_SLA_NACK :
      // set the error
      eLclError = I2C_ERROR_SLA_NACK;
      bMstInProgress = FALSE;

      // send stop/clear error/clear busy
      if( eCurMode == I2C_MODE_MULTI_MASTER )
      {
        SET_TWCR( nTwcr, TWCR_ERR_MASK, TWCR_CMD_STP );
        asm volatile( "nop\n\t" );
        asm volatile( "nop\n\t" );
        asm volatile( "nop\n\t" );
        asm volatile( "nop\n\t" );
        asm volatile( "nop\n\t" );
        SET_TWCR( nTwcr, TWCR_ERR_MASK, TWCR_CMD_SSTR );
      }
      else
      {
        SET_TWCR( nTwcr, TWCR_ERR_MASK, TWCR_CMD_STP );
      }
      break;

    // 0x30: Data byte has been transmittd, but not acknowledged
    case I2C_MT_DATA_NACK :
      // set the error
      eLclError = I2C_ERROR_DATA_NACK;
      bMstInProgress = FALSE;

      // send stop/clear error/clear busy
      if( eCurMode == I2C_MODE_MULTI_MASTER )
      {
        SET_TWCR( nTwcr, TWCR_ERR_MASK, TWCR_CMD_SSTP );
      }
      else
      {
        SET_TWCR( nTwcr, TWCR_ERR_MASK, TWCR_CMD_STP );
      }
      break;

    // 0x38: arbitration lost while in master mode
    case I2C_MX_ARB_LOST :
      // resend a start
      SET_TWCR( nTwcr, TWCR_ERR_MASK, BIT( TWSTA ));
      break;

    // 0x40 : SLA+R has been transmitted and ACK has been received
    case I2C_MR_SLA_ACK :
      // normal command
      SET_TWCR( nTwcr, TWCR_NRM_MASK, 0 );
      break;

    // 0x50: data byte has been received and acknoledged
    case I2C_MR_DATA_ACK :
      // store the data
      *(  ptLclXfrCtl->pnData + wMstIndex++ ) = TWDR;

      // only ack the next data byte except the last one
      if(( wMstIndex ) <  ptLclXfrCtl->wDataLen - 1 )
      {
        // send an ACK
        SET_TWCR( nTwcr, TWCR_RDN_MASK, BIT( TWEA ));
      }
      else
      {
        // send continue without ACK
        SET_TWCR( nTwcr, TWCR_RDN_MASK, 0 );
      }
      break;

    // 0x58: data byte has been received, but not ascknoledged
    case I2C_MR_DATA_NACK :
      // store the last data byte
      *(  ptLclXfrCtl->pnData + wMstIndex++ ) = TWDR;

      // send a stop
      SET_TWCR( nTwcr, TWCR_RDN_MASK, TWCR_CMD_STP );
      eLclError = I2C_ERROR_NONE;
      bMstInProgress = FALSE;
      break;

    // 0x60: own SLA+W has been received and acknowledged.
    // 0x68: arbitration lost as master. own SLA+W has been received
    // 0x70: General call address has been receive and acknowledged
    // 0x78: arbitration lost as master.  general call address has been
    //       received and acknowledged.
    case I2C_SR_SLA_ACK :
    case I2C_SR_ARB_LOST_SLA_ACK :
    case I2C_SR_GCALL_ACK :
    case I2C_SR_ARB_LOST_GCALL_ACK :
      // we are entering slave receive mode, mark the interface busy
      bBusBusy = TRUE;

      // call the callback
      if( pvLclCallback != NULL )
      {
        nTemp = TWDR;
        pvLclCallback( I2C_CALLBACK_SLVADR, &nTemp );
      }

      // allow more data      
      SET_TWCR( nTwcr, TWCR_RDN_MASK, BIT( TWEA ));
      break;

    // 0x80: data byte for own SLA has beeen receive and acknowledged
    // 0x90: data byte for general call address has been received and
    //       acknowledged.
    case I2C_SR_DATA_ACK :
    case I2C_SR_GCALL_DATA_ACK :
      // if call back valid, do it
      if( pvLclCallback != NULL )
      {
        // call the function with the data
        nTemp = TWDR;
        if( pvLclCallback( I2C_CALLBACK_SLVRCV, &nTemp )== I2C_CALLBACK_NO_ERROR )
        {
          // allow more data by generating an acknowledge
          SET_TWCR( nTwcr, TWCR_RDN_MASK, BIT( TWEA ));
        }
      }
      break;

    // 0x88: data byte received but not acknowledged
    // 0x98: data byte for general call receive but not acknowledged
    case I2C_SR_DATA_NACK :
    case I2C_SR_GCALL_DATA_NACK :
      // check for master pending
      if (  ptLclXfrCtl->wDataLen != 0 )
      {
        // send a start
        SET_TWCR( nTwcr, TWCR_NRM_MASK, TWCR_CMD_RST );
      }
      else
      {
        // send a acknolwedge
        SET_TWCR( nTwcr, TWCR_RDN_MASK, BIT( TWEA ));
      }
      break;

    // 0xA0: stop condition
    case I2C_SR_STOP :
      // we are exiting slave receive mode, mark the interface not busy
      bBusBusy = FALSE;

      // call the callback
      if( pvLclCallback != NULL )
      {
        nTemp = 0;
        pvLclCallback( I2C_CALLBACK_SLVSTP, &nTemp );
      }

      // check for master pending
      if ((  ptLclXfrCtl->wDataLen != 0 ) || (  ptLclXfrCtl->nAddrLen != 0 ))
      {
        // send a start
        SET_TWCR( nTwcr, TWCR_NRM_MASK, TWCR_CMD_RST );
      }
      else
      {
        // send a acknolwedge
        SET_TWCR( nTwcr, TWCR_RDN_MASK, BIT( TWEA ) );
      }
      break;

    // 0xA8: own SLA+W has been received and acknowledged
    // 0xB0: arbitration lost in master mode own SLA has been
    //       received and acknowledged
    // 0xB8: data byte has been transmitted and acknowledged
    case I2C_ST_SLA_ACK :
    case I2C_ST_ARB_LOST_SLA_ACK :
    case I2C_ST_DATA_ACK :
      if( pvLclCallback != NULL )
      {
        // check for more data to transmit
        eCbStatus = pvLclCallback( I2C_CALLBACK_SLVXMT, &nTemp );
        if( eCbStatus == I2C_CALLBACK_NO_ERROR )
        {
          // output the data/set acknowledge
          TWDR = nTemp;
          SET_TWCR( nTwcr, TWCR_NRM_MASK, BIT( TWEA ));
        }
        else if( eCbStatus == I2C_CALLBACK_LAST_XMT_BYTE )
        {
          TWDR = nTemp;
          SET_TWCR( nTwcr, TWCR_NRM_MASK, 0 );
        }
        else
        {
          // output a dummy byte
          TWDR = 0;
          SET_TWCR( nTwcr, TWCR_RDN_MASK, 0 );
        }
      }
      else
      {
        // output a dummy byte
        TWDR = 0;
        SET_TWCR( nTwcr, TWCR_RDN_MASK, 0 );
      }
      break;

    // 0xC0: data has been transmitted butnot acknowledged
    // 0xC8: last data byte has been transmitted and acknowledged
    case I2C_ST_DATA_NACK :
    case I2C_ST_LAST_DATA :
      // clear bus busy
      bBusBusy = FALSE;

      // check for master pending
      if (  ptLclXfrCtl->wDataLen != 0 )
      {
        // send a start
        SET_TWCR( nTwcr, TWCR_NRM_MASK, TWCR_CMD_RST );
      }
      else
      {
        // send a acknolwedge
        SET_TWCR( nTwcr, TWCR_RDN_MASK, 0 );
      }
      break;

    // 0x00: Bus Error
    case I2C_BUS_ERROR :
      // we have detected an error, mark the interface not busy
      bBusBusy = FALSE;
      eLclError = I2C_ERROR_BUS_ERROR;
      bMstInProgress = FALSE;
      break;

    default :
      break;
  }
}
 
/**@} EOF I2c.c */
