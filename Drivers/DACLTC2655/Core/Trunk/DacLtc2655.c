/******************************************************************************
 * @file DAcLtc2655.c
 *
 * @brief DAC LTC2655 implementation 
 *
 * This file provides the implementation for the DAC LTC2655
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DAcLtc2655
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DACLTC2655/DacLtc2655.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the all dacs address
#define DAC_ALL_CHANS                     ( 0x0F )

// define the maximum value for 12/16 bit operation
#define   DACLTC2655_12BIT_MAX            ( 4095 )
#define   DACLTC2655_16BIT_MAX            ( 65535 )

// enumerations ---------------------------------------------------------------
/// enumerate the commands
typedef enum  _CMDS
{
  CMD_WRITE_INPUT = 0,
  CMD_UPDATE_REG,
  CMD_WRITE_REG_UPDALL,
  CMD_WRITE_REG_UPDCHN,
  CMD_POWER_DN_CHAN,
  CMD_POWER_DN_ALL,
  CMD_SELECT_INT_REF,
  CMD_SELECT_EXT_REF,
} CMDS;

// structures -----------------------------------------------------------------
/// define the command structure
typedef struct _DACCMD
{
  struct
  {
    U8  nAdr : 4;       /// address
    U8  nCmd : 4;       /// command
  } tCmdAdr;
  U16 wOutput;          /// output value
} DACCMD, *PDACCMD;
#define DACCMD_SIZE                 sizeof( DACCMD )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  DACCMD  tDacCmd;

// local function prototypes --------------------------------------------------
static  void OutputCmdToDac( I2CDEVENUM eI2cDevEnum, DACLTC2655DEVADDR eDevAddr );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function DacLtc2655_Initialize
 *
 * @brief initialize the DAC
 *
 * This function will power up the DAC, set the reference select
 *
 *****************************************************************************/
void DacLtc2655_Initialize( void )
{
  PDACLTC2655DEF    ptDef;
  DACLTC2655DEVENUM eDevice;
  
  // for each configured device
  for( eDevice = 0; eDevice < DACLTC2655_DEV_ENUM_MAX; eDevice++ )
  {
    // get a pointer to the def structure
    ptDef = ( PDACLTC2655DEF )&atDacLtc2655Defs[ eDevice ];
    
    // select reference
    tDacCmd.tCmdAdr.nCmd = PGM_RDBYTE( ptDef->eRefSelect );
    tDacCmd.tCmdAdr.nAdr = 0;
    tDacCmd.wOutput = 0;

    // output the commmand to the DAC
    OutputCmdToDac( PGM_RDBYTE( ptDef->eI2cDevEnum ), PGM_RDBYTE( ptDef->eDevAddr ));
  }
}

/******************************************************************************
 * @function DacLtc2655_SetOutputDirect
 *
 * @brief set the desired chan(s) to a value
 *
 * This function will set the desired channel or all channels to the 
 * specified value
 *
 * @param[in]   eDevice   device enumeration
 * @param[in]   eChan     channel index
 * @param[in]   wValue    DAC value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DACLTC2655ERR DacLtc2655_SetOutputDirect( DACLTC2655DEVENUM eDevice, DACLTC2655CHAN eChan, U16 wValue )
{
  DACLTC2655ERR   eError = DACLTC2655_ERR_NONE;
  PDACLTC2655DEF  ptDef;
  
  // check for a valid device
  if ( eDevice < DACLTC2655_DEV_ENUM_MAX )
  {
    // check for valid channel
    if (( eChan < DACLTC2655_CHAN_MAX ) || ( eDevice == DACLTC2655_CHAN_ALL ))
    {
      // get a pointer to the def structure
      ptDef = ( PDACLTC2655DEF )&atDacLtc2655Defs[ eDevice ];

      // adjust channel for all
      if ( eChan == DACLTC2655_CHAN_ALL )
      {
        // set it the all channel value
        eChan = DAC_ALL_CHANS;
      }

      // adjust the value based on number of bits
      if ( PGM_RDBYTE( ptDef->eNumBits ) == DACLTC2655_NUMBITS_12BITS )
      {
        // shift left by four
        wValue <= 4;
      }
      
      // now set the command/value
      tDacCmd.tCmdAdr.nAdr= eChan;
      tDacCmd.tCmdAdr.nCmd = CMD_WRITE_REG_UPDCHN;
      tDacCmd.wOutput = wValue;
      
      // output the commmand to the DAC
      OutputCmdToDac( PGM_RDBYTE( ptDef->eI2cDevEnum ), PGM_RDBYTE( ptDef->eDevAddr ));
    }
    else
    {
      // set the error
      eError = DACLTC2655_ERR_ILLCHAN;
    }
  }
  else
  {
    // set the error
    eError = DACLTC2655_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function DacLtc2655_SetOutputPercent
 *
 * @brief set the desired chan(s) to a percentage
 *
 * This function will set the desired channel or all channels to the 
 * specified percentage
 *
 * @param[in]   eDevice   device enumeration
 * @param[in]   eChan     channel index
 * @param[in]   wPercent  DAC percentage (0:0, 1000:100.0)
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DACLTC2655ERR DacLtc2655_SetOutputPercent( DACLTC2655DEVENUM eDevice, DACLTC2655CHAN eChan, U16 wPercent )
{
  DACLTC2655ERR     eError = DACLTC2655_ERR_NONE;
  U32               uValue;
  PDACLTC2655DEF    ptDef;
  DACLTC2655NUMBITS eNumBits;

  // check for a valid device
  if ( eDevice < DACLTC2655_DEV_ENUM_MAX )
  {
    // check for valid channel
    if ( eChan < DACLTC2655_CHAN_MAX )
    {
      // get a pointer to the def structure
      ptDef = (  PDACLTC2655DEF )&atDacLtc2655Defs[ eDevice ];

      // adjust channel for all
      if ( eChan == DACLTC2655_CHAN_ALL )
      {
        // set it the all channel value
        eChan = DAC_ALL_CHANS;
      }
  
      // get the number of bits
      eNumBits = PGM_RDBYTE( ptDef->eNumBits );

      // calculate the percentage
      uValue = wPercent * ( eNumBits == DACLTC2655_NUMBITS_12BITS ) ? DACLTC2655_12BIT_MAX : DACLTC2655_16BIT_MAX;
      uValue /= 1000;
      
      // adjust the value based on number of bits
      if ( eNumBits  == DACLTC2655_NUMBITS_12BITS )
      {
        // shift left by four
        uValue <= 4;
      }
      
      // now set the command/value
      tDacCmd.tCmdAdr.nAdr= eChan;
      tDacCmd.tCmdAdr.nCmd = CMD_WRITE_REG_UPDCHN;
      tDacCmd.wOutput = ( U16 )uValue;
      
      // output the commmand to the DAC
      OutputCmdToDac( PGM_RDBYTE( ptDef->eI2cDevEnum ), PGM_RDBYTE( ptDef->eDevAddr ));
    }
    else
    {
      // set the error
      eError = DACLTC2655_ERR_ILLCHAN;
    }
  }
  else
  {
    // set the error
    eError = DACLTC2655_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function DacLtc2655_PowerControl
 *
 * @brief power control
 *
 * This function will either turn on or turn off power to the DAC
 *
 * @param[in]   eDevice   device enumeration
 * @param[in]   bState    desired state
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DACLTC2655ERR DacLtc2655_PowerControl( DACLTC2655DEVENUM eDevice, BOOL bState )
{
  DACLTC2655ERR   eError = DACLTC2655_ERR_NONE;
  PDACLTC2655DEF  ptDef;

  // check for a valid device
  if ( eDevice < DACLTC2655_DEV_ENUM_MAX )
  {
    // get a pointer to the def structure
    ptDef =  ( PDACLTC2655DEF )&atDacLtc2655Defs[ eDevice ];

    // select reference
    tDacCmd.tCmdAdr.nCmd = CMD_POWER_DN_ALL;
    tDacCmd.tCmdAdr.nAdr = 0;
    tDacCmd.wOutput = 0;

    // output the commmand to the DAC
    OutputCmdToDac( PGM_RDBYTE( ptDef->eI2cDevEnum ), PGM_RDBYTE( ptDef->eDevAddr ));
  }
  else
  {
    // set the error
    eError = DACLTC2655_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function OutputCmdToDac
 *
 * @brief write the command structure to the dac
 *
 * This function will fill in the I2C control structure and write
 * the data to the DAC
 *
 * @param[in]   eI2cDevEnum     device enuemration
 * @param[in]   eDevAddr        device address
 *
 *****************************************************************************/
static void OutputCmdToDac( I2CDEVENUM eI2cDevEnum, DACLTC2655DEVADDR eDevAddr )
{
  I2CXFRCTL   tCtl;
  
  // set the device address
  tCtl.nDevAddr = eDevAddr;
  
  // set the address length to 0
  tCtl.nAddrLen = 0;
  
  // set the pointer to the data to the current/length
  tCtl.pnData = ( PU8 )&tDacCmd;
  tCtl.wDataLen = DACCMD_SIZE;
  
  // now write the DAC
  I2c_Write( eI2cDevEnum, &tCtl );
}

/**@} EOF DAcLtc2655.c */
