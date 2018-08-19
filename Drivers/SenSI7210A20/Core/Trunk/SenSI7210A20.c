/******************************************************************************
 * @file SenSI7210A20.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup SenSI7210A20
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include SenSI7210A20/SenSI7210A20.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the commands
#define MEASURE_RELHUM_BLOCK                ( 0xE5 )
#define MEASURE_RELHUM_NOBLK                ( 0xF5 )
#define MEASURE_TEMP_BLOCK                  ( 0xE3 )
#define MEASURE_TEMP_NOBLK                  ( 0xF3 )
#define READ_LAST_TEMP                      ( 0xE0 )
#define RESET                               ( 0xFE )
#define WRITE_REG_ONE                       ( 0xE6 )
#define READ_REG_ONE                        ( 0xE7 )
#define WRITE_HEATER_REG                    ( 0x51 )
#define READ_HEATER_REG                     ( 0x11 )

/// define the base address for the device
#define DEVICE_ADDR                         ( 0x40 )

/// define the max tempature in C
#define MAX_TEMP_C                          ( 175.72 )
#define TEMP_OFFSET                         ( 46.85 )

/// define the maximum humidity in %
#define MAX_HUMID_PERCENT                   ( 125.0 )
#define HUMIDITY_OFFSET                     ( 6.0 )

/// define the maximum counts of the A2D
#define A2D_MAX_COUNTS                      ( 65536.0 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  FLOAT afCurValues[ SENSI7210A20_TYPE_MAX ];

// local function prototypes --------------------------------------------------
static  FLOAT SenSI7210A20_Linearize( U16 wValue, U8 nOption );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SenSI7210A20_Initialize
 *
 * @brief initialization
 *
 * This function will perofrm any required  initiaization
 *
 *****************************************************************************/
void SenSI7210A20_Initialize( void )
{
  SENSI7210A20TYPE eSensor;
  
  // clear both values
  for ( eSensor = SENSI7210A20_TYPE_HUMID; eSensor <= SENSI7210A20_TYPE_MAX; eSensor++ )
  {
    afCurValues[ eSensor ] = 0.0;
  }
}


/******************************************************************************
 * @function SenSI7210A20_StartConverstion
 *
 * @brief start a conversion
 *
 * This function will start a converstion
 *
 *****************************************************************************/
void  SenSI7210A20_StartConversion( void )
{
  I2CXFRCTL tData;
  U8        nCommand = MEASURE_RELHUM_NOBLK;
  
  // fill in the structure
  tData.nDevAddr    = DEVICE_ADDR;
  tData.nAddrLen    = 0;
  tData.pnData      = &nCommand;
  tData.wDataLen    = 1;
  tData.uTimeout    = 1;

  // now write it
  I2c_Write( SENSI7210A20_I2C_ENUM, &tData );

/******************************************************************************
 * @function SenSI7210A20_GetValue
 *
 * @brief get a value
 *
 * This function will return the correct value ( humidity/temperature )
 *
 * @param[in]   eType     type of value
 *
 * @return      the current value for the appropriate channel
 *
 *****************************************************************************/
U16 SenSI7210A20_GetValue( SENSI7210A20TYPE eType )
{
  U16       wCounts = 0;
  I2CXFRCTL tData;
  U8        nCommand = MEASURE_RELHUM_NOBLK;
  
  // fill in the structure
  tData.nDevAddr    = DEVICE_ADDR;
  tData.nAddrLen    = 0;
  tData.uTimeout    = 1;
  
  // determine what we are trying to read
  switch( eType )
  {
    case SENSI7210A20_TYPE_HUMID :
      // clear data
      tData.pnData    = &wCounts;
      tData.wDataLen  = 2;
      
      // now read it
      I2c_Read( SENSI7210A20_I2C_ENUM, &tData );
      break;
      
    case SENSI7210A20_TYPE_TEMP :
      // set the command
      nCommand = READ_LAST_TEMP;
      tData.wDataLen  = 0;
      
      // now write it
      I2c_Write( SENSI7210A20_I2C_ENUM, &tData );
      
      // now read the temp
      tData.pnData    = &wCounts;
      tData.wDataLen  = 2;
      
      // now read it
      I2c_Read( SENSI7210A20_I2C_ENUM, &tData );
      
      // store it
      wPrevTemp = wCounts;
      break;
      
    default :
      break;
  }

  // return the counts
  return( wCounts );
}

/******************************************************************************
 * @function Lineraize
 *
 * @brief value lineratization
 *
 * This function will taked the passed value ( A2D counts ) and convert them
 * to a valid sensor value
 *
 * @param[in]   wValue      current value in A2D counts
 * @param[in]   nOption     option
 *
 * @return      current real word value
 *
 *****************************************************************************/
static FLOAT Linearize( U16 wValue, U8 nOption )
{
  FLOAT fValue = 0.0;
  
  // determine the variable
  switch( nOption )
  {
    case SENSI7210A20_TYPE_HUMID :
      // convert to percent
      fValue = ((( FLOAT )wValue * MAX_HUMID_PERCENT ) / A2D_MAX_COUNTS ) - HUMIDITY_OFFSET;
      break;
      
    case SENSI7210A20_TYPE_TEMP :
    ``// convert to centigrade
    ` fValue = ( FLOAT )((( FLOAT )wValue * MAX_TEMP_C ) / A2D_MAX_COUNTS ) - TEMP_OFFSET;
      break;
      
    default :
      break;
  }
  
  // return it 
  return( fValue );
}


/**@} EOF SenSI7210A20.c */
