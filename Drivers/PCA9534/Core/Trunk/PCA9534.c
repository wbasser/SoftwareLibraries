/******************************************************************************
 * @file PCA9534.c
 *
 * @brief PCA9534 port expander implementation
 *
 * This file provides the implementation for the PCA9534 port expander
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
 * \addtogroup PCA9534
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PCA9534/PCA9534.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _PCA9534REG
{
  PCA9534_REG_INP = 0,      ///< input 
  PCA9534_REG_OUT,          ///< output
  PCA9534_REG_POL,          ///< polarity inversion
  PCA9534_REG_CFG,          ///< configuration
} PCA9534REG;

/// define the macros for command/data offsets
#define CMD_OFFSET          ( LE_U16_LSB_IDX )
#define DAT_OFFSET          ( LE_U16_MSB_IDX )

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _LCLCTL
{
  U8  nConfig;              ///< config
  U8  nPolarity;            ///< polarity
  U8  nOutput;              ///< output
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE         sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtl[ PCA9534_DEV_ENUM_NAX ];

// local function prototypes --------------------------------------------------
static  BOOL  WriteDataToDevice( I2CDEVENUM eI2cDevEnum, PCA9534DEVADDR eDevAddr, U8 nCommand, U8 nData );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function PCA9534_Initialize
 *
 * @brief initialization
 *
 * This function will initialize all devices
 *
 *****************************************************************************/
void PCA9534_Initialize( void )
{
  PCA9534DEVENUM  eDevEnum;
  PCA9534PINENUM  ePinEnum;
  PPCA9534PINDEF  ptPinDef;
  PPCA9534DEVDEF  ptDevDef;
  PCA9534DIRENUM  ePinDir;
  BOOL            bPolarity, bInitial;
  I2CDEVENUM      eI2cDevEnum;
  PLCLCTL         ptLclCtl;
  PCA9534DEVADDR  eDevAddr;
  
  // for each defined device
  for( eDevEnum = 0; eDevEnum < PCA9534_DEV_ENUM_NAX; eDevEnum++ )
  {
    // get the device definition
    ptDevDef = ( PPCA9534DEVDEF )&atPca9534Devs[ eDevEnum ];
    ptLclCtl = &atLclCtl[ eDevEnum ];
    
    // clear the config/poarity
    ptLclCtl->nConfig = 0;
    ptLclCtl->nOutput = 0;
    ptLclCtl->nPolarity = 0;
    
    // now process the pins
    for ( ePinEnum = 0; ePinEnum < PCA9534_PIN_ENUM_MAX; ePinEnum++ )
    {
      // get a pointer to the pin definition
      ptPinDef = ( PPCA9534PINDEF )&ptDevDef->atPinDefs[ ePinEnum ];
      
      // get direction
      ePinDir = PGM_RDBYTE( ptPinDef->eDirection );
      ptLclCtl->nConfig |= BITS( ePinDir, ePinEnum );
      
      // get polarity
      bPolarity = PGM_RDBYTE( ptPinDef->bInvert );
      ptLclCtl->nPolarity |= BITS( bPolarity, ePinEnum );

      // get the initial value
      bInitial = PGM_RDBYTE( ptPinDef->bInitial );
      
      // if output set the initial
      if ( ePinDir == PCA9534_DIR_OUT )
      {
        // set the output
        ptLclCtl->nOutput |= BITS(( bPolarity ^ bInitial ), ePinEnum );
      }
    }
    
    // get I2C enum/get device address
    eI2cDevEnum = PGM_RDBYTE( ptDevDef->eI2cDevEnum );
    eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );

    // write the config
    if ( WriteDataToDevice( eI2cDevEnum, eDevAddr, PCA9534_REG_CFG, ptLclCtl->nConfig ))
    {
      // write the poliary
      if ( WriteDataToDevice( eI2cDevEnum, eDevAddr, PCA9534_REG_POL, ptLclCtl->nPolarity ))
      {
        // now write the output
        WriteDataToDevice( eI2cDevEnum, eDevAddr, PCA9534_REG_OUT, ptLclCtl->nOutput );
      }
    }
  }
}

/******************************************************************************
 * @function PCA9534_SetOutput
 *
 * @brief set a pin as an ouput
 *
 * This function will set a pin's output state if configured as a output
 *
 * @param[in]   eDeviceEnum     device enumeration
 * @param[in]   ePinEnum        pin enumeration
 * @param[in]   bState          state of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
PCA9434ERR PCA9534_SetOutput( PCA9534DEVENUM eDeviceEnum, PCA9534PINENUM ePinEnum, BOOL bState )
{
  PCA9434ERR      eError = PCA9534_ERR_NONE;
  PPCA9534PINDEF  ptPinDef;
  PPCA9534DEVDEF  ptDevDef;
  PLCLCTL         ptLclCtl;
  U8              nMask, nData;
  I2CDEVENUM      eI2cDevEnum;
  PCA9534DEVADDR  eDevAddr;
  
  // valid device
  if ( eDeviceEnum < PCA9534_DEV_ENUM_NAX )
  {
    // check for valid pin
    if ( ePinEnum < PCA9534_PIN_ENUM_MAX )
    {
      // get the device definition
      ptDevDef = ( PPCA9534DEVDEF )&atPca9534Devs[ eDeviceEnum ];
      ptPinDef = ( PPCA9534PINDEF )&ptDevDef->atPinDefs[ ePinEnum ];
      ptLclCtl = &atLclCtl[ eDeviceEnum ];
      
      // check for this pin being an output
      if ( PGM_RDBYTE( ptPinDef->eDirection ) == PCA9534_DIR_OUT )
      {
        // now calculate the mask
        nMask = BIT( ePinEnum );
        
        // check the state
        if ( PGM_RDBYTE( ptPinDef->bInvert ) ^ bState )
        {
          // set the bit
          ptLclCtl->nOutput |= nMask;
        }
        else
        {
          // clear the bit
          ptLclCtl->nOutput &= ~nMask;
        }
        // get the address/device
        eI2cDevEnum = PGM_RDBYTE( ptDevDef->eI2cDevEnum );
        eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );

        // write it
        if ( WriteDataToDevice( eI2cDevEnum, eDevAddr, PCA9534_REG_OUT, ptLclCtl->nOutput ))
        {
          // error detected
          eError = PCA9534_ERR_I2CXFR;
        }
      }
      else
      {
        // set the error
        eError = PCA9534_ERR_ILLACT;
      }
    }
    else
    {
      // set the error
      eError = PCA9534_ERR_ILLPIN;
    }
  }
  else
  {
    // set the error
    eError = PCA9534_ERR_ILLDEV;
  }
  
  // reteurn the error
  return( eError );
}

/******************************************************************************
 * @function PCA9534_GetInput
 *
 * @brief get a pin's state
 *
 * This function will get a pin's input state
 *
 * @param[in]   eDeviceEnum     device enumeration
 * @param[in]   ePinEnum        pin enumeration
 * @param[in]   pbState         pointer to store the state of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
PCA9434ERR PCA9534_GetInput( PCA9534DEVENUM eDeviceEnum, PCA9534PINENUM ePinEnum, PBOOL pbState )
{
  PCA9434ERR  eError = PCA9534_ERR_NONE;
  PPCA9534PINDEF  ptPinDef;
  PPCA9534DEVDEF  ptDevDef;
  PLCLCTL         ptLclCtl;
  U8              nMask, nData;
  I2CDEVENUM      eI2cDevEnum;
  I2CXFRCTL       tI2cXfrCtl;
  
  // valid device
  if ( eDeviceEnum < PCA9534_DEV_ENUM_NAX )
  {
    // check for valid pin
    if ( ePinEnum < PCA9534_PIN_ENUM_MAX )
    {
      // get the device definition
      ptDevDef = ( PPCA9534DEVDEF )&atPca9534Devs[ eDeviceEnum ];
      ptPinDef = ( PPCA9534PINDEF )&ptDevDef->atPinDefs[ ePinEnum ];
      ptLclCtl = &atLclCtl[ eDeviceEnum ];
        
      // now calculate the mask
      nMask = BIT( ePinEnum );
      
      // get the address/device
      eI2cDevEnum = PGM_RDBYTE( ptDevDef->eI2cDevEnum );
      tI2cXfrCtl.nDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );
        
      // now read it
      tI2cXfrCtl.tAddress.uValue = PCA9534_REG_INP;
      tI2cXfrCtl.nAddrLen = 1;
      tI2cXfrCtl.wDataLen = 1;
      tI2cXfrCtl.uTimeout = 1;
      tI2cXfrCtl.pnData = &nData;
      
      // perform read/write
      if ( I2c_Read( eI2cDevEnum, &tI2cXfrCtl ) != I2C_ERR_NONE )
      {
        // set the I2C error
        eError = PCA9534_ERR_I2CXFR;
      }
      else
      {
        // set the error
        eError = PCA9534_ERR_I2CXFR;
      }
      
      // now determine the data
      nData &= nMask;
      if ( PGM_RDBYTE( ptPinDef->bInvert ))
      {
        // invert the data
        nData ^= nMask;
      }
      
      // now stuff the result
      *( pbState ) = ( nData != 0 ) ? TRUE : FALSE;
    }
    else
    {
      // set the error
      eError = PCA9534_ERR_ILLPIN;
    }
  }
  else
  {
    // set the error
    eError = PCA9534_ERR_ILLDEV;
  }
  // reteurn the error
  return( eError );
}

/******************************************************************************
 * @function WriteDataToDevice
 *
 * @brief write data to the device
 *
 * This function will set up the transfer structure, and write the data to the
 * device
 *
 * @param[in]   nI2cDevEnum   I2C device
 * @param[in]   eDeviddr      PCA9534 device address
 * @param[in]   nCommand      command( register )
 * @param[in]   nData         data to write
 *
 * @return      FALSE if OK, TRUE if error
 *
 *****************************************************************************/
static BOOL WriteDataToDevice( I2CDEVENUM eI2cDevEnum, PCA9534DEVADDR eDevAddr, U8 nCommand, U8 nData )
{
  BOOL      bStatus = FALSE;
  I2CXFRCTL tI2cXfrCtl;
  I2CERR    eError;
  U16UN     tCmdData;

  // now fill the command/data/structure
  tCmdData.anValue[ CMD_OFFSET ] = nCommand;
  tCmdData.anValue[ DAT_OFFSET ] = nData;

  // now fill the I2C transfer structure
  tI2cXfrCtl.nDevAddr = eDevAddr;
  tI2cXfrCtl.tAddress.uValue = 0;
  tI2cXfrCtl.nAddrLen = 0;
  tI2cXfrCtl.uTimeout = 5;
  tI2cXfrCtl.wDataLen = sizeof( U16UN );
  tI2cXfrCtl.pnData = ( PU8 )&tCmdData;

  // now write it
  if (( eError = I2c_Write( eI2cDevEnum, &tI2cXfrCtl )) != I2C_ERR_NONE )
  {
    // set the fault flag
    bStatus = TRUE;
  }

  // return good status
  return( bStatus );
}
 
/**@} EOF PCA9534.c */
