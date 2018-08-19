/******************************************************************************
 * @file AfeLMP9100X.c
 *
 * @brief Analog Front End LMP9100X implementation
 *
 * This file provides the implementation for the Analog Front End LMP9100X
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
 * \addtogroup AfeLMP9100X
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AfeLMP9100X/AfeLMP9100X.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the device address
#define LMP9100X_DEV_ADDR         ( 0x48 )

/// define the register addresses
#define ADR_REG_STATUS            ( 0X00 )
#define ADR_REG_LOCK              ( 0x01 )
#define ADR_REG_TIACN             ( 0x10 )
#define ADR_REG_REFCN             ( 0x11 )
#define ADR_REG_MODECN            ( 0x12 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the status register
typedef union _REGSTATUS
{
  struct
  {
    BOOL  bStatus   : 1;          ///< status bit (0-not ready, 1-ready )
  } tFields;
  U8  nByte;
} REGSTATUS, *PREGSTATUS;
#define REGSTATUS_SIZE                   sizeof( REGSTATUS )

/// define the lock register
typedef union _REGLOCK
{
  struct
  {
    BOOL  bLock   : 1;          ///< lock bit (0-not locked, 1-locked )
  } tFields;
  U8  nByte;
} REGLOCK, *PREGLOCK;
#define REGLOCK_SIZE                   sizeof( REGLOCK )

/// define the TIA control register
typedef union _REGTIACN
{
  struct
  {
    U8    nRLoad  : 2;          ///< R Load selection
    U8    nGain   : 3;          ///< gain selection
  } tFields;
  U8  nByte;
} REGTIACN, *PREGTIACN;
#define REGTIACN_SIZE                 sizeof( REGTIACN )

/// define the REF control register
typedef union _REGREFCN
{
  struct
  {
    U8    nBias   : 4;          ///< bias selection
    BOOL  bSign   : 1;          ///< bias sigh
    U8    nIntZ   : 2;          ///< internal zero selection
    BOOL  bRefSrc : 1;          ///< reference source
  } tFields;
  U8 nByte;
} REGREFCN, *PREGREFCN;
#define REGREFCN_SIZE                 sizeof( REGREFCN )

/// define the mode control register
typedef union _REGMODECN
{
  struct
  {
    U8    nMode     : 3;        ///< operational mode
    U8    nReserv   : 4;        ///< reserved
    BOOL  bFetShort : 1;        ///< FET short enabled
  } tFields;
  U8  nByte;
} REGMODECN, *PREGMODECN;
#define REGMODECN_SIZE                sizeof( REGMODECN )

/// define the command structure for writing to the LMP9100X
typedef union _CMDBUFFER
{
  struct
  {
    U8  nRegAddr;                ///< register address
    union
    {
      REGLOCK   tLock;            ///< lock register
      REGTIACN  tTiaCn;           ///< TIA Control register
      REGREFCN  tRefCn;           ///< REF control register
      REGMODECN tModeCn;          ///< mode control register
    } tRegs;
  } tCommands;
  U8  anBuffer[ 1 ];              ///< buffer
} CMDBUFFER, *PCMDBUFFER;
#define CMDBUFFER_SIZE                sizeof( CMDBUFFER )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  BOOL  WriteCommand( PCMDBUFFER ptBuffer, PAFELMLP9100XCTLDEF ptDef );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AfeLMP9100X_Initialize
 *
 * @brief initialization
 *
 * This function will perform any initilaization required
 *
 *****************************************************************************/
void AfeLMP9100X_Initialize( void )
{
}

/******************************************************************************
 * @function AfeLMP9100X_SetConfig
 *
 * @brief set the configuration for a channel
 *
 * This function will set the configuration for a channel
 *
 * @param[in]   eDevice     device enumeration
 * @param[in]   ptCfgDef    pointer to a configuration structure
 * 
 * @return      appropriate error
 *
 *****************************************************************************/
AFELMP9100XERR AfeLMP9100X_SetConfig( AFELMP9100ENUM eDevice, PAFELMP9100XCFGDEF ptCfgDef )
{
  AFELMP9100XERR      eError = AFELMP9100X_ERR_NONE;
  CMDBUFFER           tCmdBuf;
  PAFELMLP9100XCTLDEF ptDef;
 
  // ensure valid device enumeration
  if ( eDevice < AFELMP9100_ENUM_MAX )
  {
    // get the pointer to the definition
    ptDef = ( PAFELMLP9100XCTLDEF )&atAfeLMP9100XDefs[ eDevice ]; 
    
    // set the error
    eError = AFLEMP9100X_ERR_INITFAIL;
    
    // now build the TIA command
    tCmdBuf.tCommands.nRegAddr = ADR_REG_TIACN;
    tCmdBuf.tCommands.tRegs.tTiaCn.tFields.nRLoad = ptCfgDef->eTiaLoad;
    tCmdBuf.tCommands.tRegs.tTiaCn.tFields.nGain  = ptCfgDef->eTiaGain;
    
    // write it - check for errors
    if ( !WriteCommand( &tCmdBuf, ptDef ))
    {
      // no failure - now set the reference
      tCmdBuf.tCommands.nRegAddr = ADR_REG_REFCN;
      tCmdBuf.tCommands.tRegs.tRefCn.tFields.nBias    = ptCfgDef->eRefBiasPct;
      tCmdBuf.tCommands.tRegs.tRefCn.tFields.bSign    = ptCfgDef->eRefBiasSign;
      tCmdBuf.tCommands.tRegs.tRefCn.tFields.nIntZ    = ptCfgDef->eRefIntZero;
      tCmdBuf.tCommands.tRegs.tRefCn.tFields.bRefSrc  = ptCfgDef->eRefSource;
      
      // write it - check for errors
      if ( !WriteCommand( &tCmdBuf, ptDef ))
      {
        // clear the error flag
        eError = AFELMP9100X_ERR_NONE;
      }
    }
  }
  else
  {
    // set the error
    eError = AFELMP9100X_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function AfeLMP9100X_SetOperationalMode
 *
 * @brief set the operational mode
 *
 * This function will set the channel to desired mode
 *
 * @param[in]   eDevice     device enumeration
 * @param[in]   eMode       operational mode
 * 
 * @return      appropriate error
 *
 *****************************************************************************/
AFELMP9100XERR AfeLMP9100X_SetOperationalMode( AFELMP9100ENUM eDevice, AFELMP9100XOPMODE eMode )
{
  AFELMP9100XERR      eError = AFELMP9100X_ERR_NONE;
  CMDBUFFER           tCmdBuf;
  PAFELMLP9100XCTLDEF ptDef;
 
  // ensure valid device enumeration
  if ( eDevice < AFELMP9100_ENUM_MAX )
  {
    // check for proper mode
    if ( eMode < AFELMP9100X_OPMODE_MAX )
    {
      // get the pointer to the definition
      ptDef = ( PAFELMLP9100XCTLDEF )&atAfeLMP9100XDefs[ eDevice ]; 
      
      // build the mode command
      tCmdBuf.tCommands.nRegAddr = ADR_REG_MODECN;
      tCmdBuf.tCommands.tRegs.tModeCn.tFields.nMode = eMode;
      
      // write it - check for errors
      if ( WriteCommand( &tCmdBuf, ptDef ))
      {
        // clear the error flag
        eError = AFLEMP9100X_ERR_MODEFAIL;
      }
    }
    else
    {
      // set the error
      eError = AFLEMP9100X_ERR_ILLMODE;
    }
  }
  else
  {
    // set the error
    eError = AFELMP9100X_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function WriteCommand
 *
 * @brief write a command value to the device
 *
 * This function will enable the device write the value out on the I2C bus and
 * then disable the device
 *
 * @param[in]   ptBuffer    pointer to the command buffer
 * @param[in]   ptDef       pointer to the definition structure
 * 
 * @return      TRUE if error detected
 *
 *****************************************************************************/
static BOOL WriteCommand( PCMDBUFFER ptBuffer, PAFELMLP9100XCTLDEF ptDef )
{
  BOOL        bStatus = FALSE;
  I2CXFRCTL   tI2cXfrCtl;
  GPIOPINENUM eGpioPin;
  
  // get the gpio pin
  eGpioPin = PGM_RDBYTE( ptDef->eGpioPin );
   
  // first enable the device
  Gpio_Set( eGpioPin, ON );
  
  // fill out the I2C transfer control structure
  tI2cXfrCtl.nDevAddr = LMP9100X_DEV_ADDR;
  tI2cXfrCtl.nAddrLen = 0,
  tI2cXfrCtl.pnData   = ( PU8 )&ptBuffer->anBuffer;
  tI2cXfrCtl.wDataLen = CMDBUFFER_SIZE;
  tI2cXfrCtl.uTimeout = 100;
  
  // now write the device
  if ( I2c_Write( PGM_RDBYTE( ptDef->eI2cDev), &tI2cXfrCtl ) != I2C_ERR_NONE )
  {
    // set the error
    bStatus = TRUE;
  }
  
  // release the device
  Gpio_Set( eGpioPin, OFF );
  
  // return status
  return( bStatus );
}

/**@} EOF AfeLMP9100X.c */
