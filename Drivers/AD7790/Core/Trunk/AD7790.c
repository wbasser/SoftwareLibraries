/******************************************************************************
 * @file AD7790.c
 *
 * @brief AD7790 A2D implementation 
 *
 * This file provides the implementation for the AD7790 A2D converter
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
 * \addtogroup AD7790
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AD7790/AD7790.h"
#include "AD7790/AD7790_prm.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "SPI/Spi.h"

// Macros and Defines ---------------------------------------------------------
/// define the shift for the mode/range
#define RANGE_SHIFT               ( 4 )
#define MODE_SHIFT                ( 6 )

/// define the maximum counts
#define MAX_RESULT_COUNTS         ( 32767 )

/// define the reference voltage
#define REFERENCE_VOLT            ( 1.2 )

/// define status not ready
#define STATUS_NOT_READY          ( 0x80 )

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _REGISTER
{
  REGISTER_CMST_ENUM = 0,       ///< Ccommand on write/ ststus on read
  REGISTER_MODE_ENUM,           ///< mode
  REGISTER_FILT_ENUM,           ///< filter
  REGISTER_DATA_ENUM,           ///< data
} REGISTER;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  WriteReg( REGISTER eAddress, U8 nData );
static  U8    ReadRegByte( REGISTER eAddress );
static  U16   ReadRegWord( REGISTER eAddress );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AD7790_Initialize
 *
 * @brief initialize
 *
 * This function will perform any initialization
 *
 *****************************************************************************/
void AD7790_Initialize( void )
{
  
}

/******************************************************************************
 * @function AD7790_Setup
 *
 * @brief setup for a conversion
 *
 * This function will set the mode, range and filter for the next conversion
 *
 * @param[in]   eMode     desired mode
 * @param[in]   eRange    input range
 * @param[in]   eFilter   filter selection
 *
 *****************************************************************************/
void AD7790_Setup( AD7790MODE eMode, AD7790RANGE eRange, AD7790FILT eFilter )
{
  // set the mode/filter
  WriteReg( REGISTER_MODE_ENUM, ( eRange << RANGE_SHIFT ) | ( eMode << MODE_SHIFT ));
  WriteReg( REGISTER_MODE_ENUM, eFilter );
}

/******************************************************************************
 * @function AD7790_StartConversion
 *
 * @brief start a conversion 
 *
 * This function will start a conversion if in single conversion mode
 *
 *****************************************************************************/
void AD7790_StartConversion( void )
{
}

/******************************************************************************
 * @function AD7790_GetConversionCounts
 *
 * @brief get the last convertion value in counts
 *
 * This function will return the last conversion value in counts
 *
 * @return  value of the last conversion in counts
 *
 *****************************************************************************/
U16 AD7790_GetConversionCounts( void )
{
  U16   wValue;
  
  // wait for done
  while( ReadRegByte( REGISTER_CMST_ENUM ) & STATUS_NOT_READY );
  
  // now read the data
  wValue = ReadRegWord( REGISTER_DATA_ENUM );
  
  // return the value
  return( wValue );
}

/******************************************************************************
 * @function AD7790_ConvertCountsToVolts
 *
 * @brief get the latest conversion value in volts
 *
 * This function will return the latest conversion value in volts
 *
 * @param[in]   wCounts   curent counts
 *
 * @return      value of the last converstion in volts
 *
 *****************************************************************************/
FLOAT AD7790_ConvertCountsToVolts( U16 wCounts )
{
  FLOAT fValue;
  
  // convert it
  fValue = (( FLOAT )wCounts / MAX_RESULT_COUNTS ) * REFERENCE_VOLT;
  
  // return the value
  return( fValue );
}

/******************************************************************************
 * @function WriteReg
 *
 * @brief write a register
 *
 * This function will write the register address and data
 *
 * @param[in]   eAddress    address
 * @param[in]   nData       data
 *
 *****************************************************************************/
static void WriteReg( REGISTER eAddress, U8 nData )
{
  // enable the device
  Gpio_Set( AD7790_CEN_PIN_ENUM, ON );
  
  // now write the address/data
  Spi_Write( AD7790_SPI_DEV_ENUM, eAddress );
  Spi_Write( AD7790_SPI_DEV_ENUM, nData );
  
  // disable the device
  Gpio_Set( AD7790_CEN_PIN_ENUM, OFF );
}

/******************************************************************************
 * @function ReadRegByte
 *
 * @brief read a byte from a register
 *
 * This function will read a byte from a register
 *
 * @param[in]   eAddress    address
 *
 * @return    value that was read
 * 
 *****************************************************************************/
static U8 ReadRegByte( REGISTER eAddress )
{
  U8 nData;
  
  // enable the device
  Gpio_Set( AD7790_CEN_PIN_ENUM, ON );
  
  // now write the address/data
  Spi_Write( AD7790_SPI_DEV_ENUM, eAddress );
  Spi_Read( AD7790_SPI_DEV_ENUM, 0xAA, &nData );
  
  // disable the device
  Gpio_Set( AD7790_CEN_PIN_ENUM, OFF );
  
  // return the data
}

/******************************************************************************
 * @function ReadRegWord
 *
 * @brief read a word from a register
 *
 * This function will read a byte from a register
 *
 * @param[in]   eAddress    address
 *
 * @return    value that was read
 * 
 *****************************************************************************/
static U16 ReadRegWord( REGISTER eAddress )
{
  U16UN tData;
  
  // enable the device
  Gpio_Set( AD7790_CEN_PIN_ENUM, ON );
  
  // now write the address/data
  Spi_Write( AD7790_SPI_DEV_ENUM, eAddress );
  Spi_Read( AD7790_SPI_DEV_ENUM, 0xAA, &tData.anValue[ LE_U16_MSB_IDX ] );
  Spi_Read( AD7790_SPI_DEV_ENUM, 0xAA, &tData.anValue[ LE_U16_LSB_IDX ] );
  
  // disable the device
  Gpio_Set( AD7790_CEN_PIN_ENUM, OFF );
  
  // return the data
  return( tData.wValue );
}

/**@} EOF AD7790.c */
