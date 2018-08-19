/******************************************************************************
 * @file TPL0401XDigitalPot.c
 *
 * @brief TPL0401X Digital pot implementation
 *
 * This file provides the implementation for TPL0401X digital pot device driver
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
 * \addtogroup TPL0401XDigitalPot
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TPL0401XDigitalPot/TPL0401XDigitalPot.h"
#include "TPL0401XDigitalPot/TPL0401XDigitalPot_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  I2CXFRCTL tI2cXfr;

// constant parameter initializations -----------------------------------------
static  const CODE  U8  anDeviceAddress[ TPL0401X_DIGPOT_DEV_MAX ] =
{
  0x5E,         ///< Type A device
  0x7E,         ///< Type B device
};

/******************************************************************************
 * @function TPL0401XDigitalPot_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization 
 *
 *****************************************************************************/
void TPL0401XDigitalPot_Initialize( void )
{
}

/******************************************************************************
 * @function TPL0401XDigitalPot_Write
 *
 * @brief write a value
 *
 * This function will validate the device, and write the value to the device
 *
 * @param[in]   eDevice       device enumeration
 * @param[in]   nValue        value to write
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TPL0401DIGPOTERR TPL0401XDigitalPot_Write( TPL0401XDIGPOTDEV eDevice, U8 nValue )
{
  TPL0401DIGPOTERR  eError = TPL0401X_DIGPOT_ERR_NONE;
  
  // test for valid device
  if ( eDevice < TPL0401X_DIGPOT_DEV_MAX )
  {
    // fill the transfer control
    tI2cXfr.nDevAddr = anDeviceAddress[ eDevice ];
    tI2cXfr.nAddrLen = 1;
    tI2cXfr.tAddress.anValue[ LE_U32_LSB_IDX ] = 0;
    tI2cXfr.wDataLen = 1;
    tI2cXfr.pnData = &nValue; 
    tI2cXfr.uTimeout = 100;
    
    // now write it
    I2c_Write( TPL0401XDIGITALPOT_I2C_ENUM, &tI2cXfr );      
  }
  else
  {
    // set the error
    eError = TPL0401X_DIGPOT_ERR_ILLDEV;
  }    

  // return the error
  return( eError );  
}

/******************************************************************************
 * @function TPL0401XDigitalPot_Read
 *
 * @brief read a value
 *
 * This function will validate the devie and read the value from that device
 *
 * @param[in]   eDevice       device enumeration
 * @param[in]   pnValue       pointer to store the read value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
TPL0401DIGPOTERR TPL0401XDigitalPot_Read( TPL0401XDIGPOTDEV eDevice, PU8 pnValue )
{
  TPL0401DIGPOTERR  eError = TPL0401X_DIGPOT_ERR_NONE;
  
  // test for valid device
  if ( eDevice < TPL0401X_DIGPOT_DEV_MAX )
  {
    // fill the transfer control
    tI2cXfr.nDevAddr = anDeviceAddress[ eDevice ];
    tI2cXfr.nAddrLen = 1;
    tI2cXfr.tAddress.anValue[ LE_U32_LSB_IDX ] = 0;
    tI2cXfr.wDataLen = 1;
    tI2cXfr.pnData = pnValue;
    tI2cXfr.uTimeout = 100;
    
    // now write it
    I2c_Read( TPL0401XDIGITALPOT_I2C_ENUM, &tI2cXfr );
  }
  else
  {
    // set the error
    eError = TPL0401X_DIGPOT_ERR_ILLDEV;
  }

  // return the error
  return( eError );
}
 
/**@} EOF TPL0401XDigitalPot.c */
