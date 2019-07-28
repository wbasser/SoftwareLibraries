/******************************************************************************
 * @file VSK11XXHandler.c
 *
 * @brief 
 *
 * This file 
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
 * \addtogroup VSK11XXHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "VSK10XXHandler/VSK10XXHandler.h"
#include "VSK10XXHandler/VSK10XXHandler_prm.h"

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
// define the mode control masks
#define MODE_DIFF       ( 0x0001 )
#define MODE_ZERO0      ( 0x0002 )
#define MODE_RESET 	    ( 0x0004 )
#define MODE_EXWAV 	    ( 0x0008 )
#define MODE_PDOWN 	    ( 0x0010 )
#define MODE_TESTS 	    ( 0x0020 )
#define MODE_STREM 	    ( 0x0040 )
#define MODE_ZERO1 	    ( 0x0080 )
#define MODE_CLKSL 	    ( 0x0100 )
#define MODE_ORDER	    ( 0x0200 )
#define MODE_SHARE	    ( 0x0400 )
#define	MODE_INEW       ( 0x0800 )
#define	MODE_ADPCM	    ( 0x1000 )
#define	MODE_HIFIL	    ( 0x2000 )
#define	MODE_LINEI	    ( 0x4000 )

// define the read/write commands
#define	VSK_CMD_RD      ( 0x03 )
#define	VSK_CMD_WR      ( 0x02 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  WriteCtl( U8 nAddress, U16UN tData );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function VSK10XXHandler_Initializaton
 *
 * @brief initialize the VSK10XX controller
 *
 * This function will reset the chip and set up native mode
 *
 *****************************************************************************/
void VSK10XXHandler_Initialize( void ) 
{
  U16UN tData;

  #if ( VSK10XX_RST_ENABLED == 1 )
  // reset the device
  Gpio_Set( VSK10XX_RST_ENUM, ON );
  SystemTick_DelayMsec( 1 );
  Gpio_Set( VSK10XX_RST_ENUM, OFF );
  #endif // ( VSK10XX_RST_ENABLED )

  // generate a reset
  SystemTick_DelayMsec( 1 );
  VSK10XXHandler_Reset( );
  SystemTick_DelayMsec( 1 );

  // now set the clock frequency
  tData.wValue = 0x6000;
  WriteCtl( VSK10XX_REG_CLKF, tData );
  SystemTick_DelayMsec( 1 );
}

/******************************************************************************
 * @function VSK10XXHandler_Reset 
 *  
 * @brief reset the device
 *
 * This function will reset the VSK device
 *
 *****************************************************************************/
void VSK10XXHandler_Reset( void )
{
  U16UN tCommand;

  // se the command
  tCommand.wValue = ( MODE_RESET | MODE_INEW );
  WriteCtl( VSK10XX_REG_MODE, tCommand );
  SystemTick_DelayMsec( 1 );

  // wait for data ready
  while( !VSK10XXHandler_DataReady( ));
}

/******************************************************************************
 * @function VSK10XXHandler_StartPlay
 *  
 * @brief start playing
 *
 * This function will start a play sequence
 *
 *****************************************************************************/
void VSK10XXHandler_StartPlay( void )
{
  U16UN tCommand;

  // reset the playboack
  tCommand.wValue = ( MODE_LINEI | MODE_INEW );
  WriteCtl( VSK10XX_REG_MODE, tCommand );

  // re-sync
  tCommand.wValue = 0x1E29;
  WriteCtl( VSK10XX_REG_ADDR, tCommand );
  tCommand.wValue = 0;
  WriteCtl( VSK10XX_REG_WRAM, tCommand );

  // reset the decode time
  tCommand.wValue = 0;
  WriteCtl( VSK10XX_REG_DTIM, tCommand );
  WriteCtl( VSK10XX_REG_DTIM, tCommand );
}

/******************************************************************************
 * @function VSK10XXHandler_WriteData
 *
 * @brief write a block of data
 *
 * This function will write a block of data
 *
 * @param[in]   pnData          pointer to the data block
 * @param[in]   nCount          data length
 *
 *****************************************************************************/
void VSK10XXHandler_WriteData( PU8 pnData, U8 nLength )
{
  // set the data chip select
  Gpio_Set( VSK10XX_CDS_ENUM, ON );

  // output the block
  Spi_WriteBlock( SPI_DEV_ENUM_LCLBUS, pnData, nLength, FALSE );

  // deselect the chip
  Gpio_Set( VSK10XX_CDS_ENUM, OFF );
}

/******************************************************************************
 * @function VSK10XXHandler_SetVolume
 *
 * @brief set the volume
 *
 * This function will write the new valume levels
 *
 * @param[in]   nLeft           left volume
 * @param[in]   nRight          right volume
 *
 *****************************************************************************/
void VSK10XXHandler_SetVolume( U8 nLeft, U8 nRight )
{
	U16UN	tCommand;

  // fill the data
	tCommand.anValue[ LE_U16_LSB_IDX  ] = nLeft;
	tCommand.anValue[ LE_U16_MSB_IDX  ] = nRight;

  // write it
	WriteCtl( VSK10XX_REG_VOLM, tCommand );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void VSK10XXHandler_SetEnhance( U8 nTreble, U8 nTrbCut, U8 nBass, U8 nBasCut )
{

}

/******************************************************************************
 * @function VSK10XXHandler_DataReady
 *
 * @brief returns the state of the data ready pin
 *
 * This function will read an return the current state of the data request pin
 *
 * @return      the current state of the data request pin
 *
 *****************************************************************************/
BOOL VSK10XXHandler_DataReady( void )
{
  BOOL bState;

  // get the state of the pin
  Gpio_Get( VSK10XX_DRQ_ENUM, &bState );

  // return the state of the pin
  return( bState );
}

void VSK10XXHandler_GenerateTone( BOOL bOffOn )
{
  U16UN tCommand;
  U8    anData[ 8 ];

  // determine if we are on or off
  if ( bOffOn == ON )
  {
    // turn on test mode
    tCommand.wValue = MODE_INEW | MODE_TESTS;
    WriteCtl( VSK10XX_REG_MODE, tCommand );

    // fill the data
    anData[ 0 ] = 0x53;
    anData[ 1 ] = 0xEF;
    anData[ 2 ] = 0x6E;
    anData[ 3 ] = 0x44;
    anData[ 4 ] = 0x00;
    anData[ 5 ] = 0x00;
    anData[ 6 ] = 0x00;
    anData[ 7 ] = 0x00;

    // wait for data reguest/write block
    while( VSK10XXHandler_DataReady( ) == FALSE );
    VSK10XXHandler_WriteData( anData, 8 );
  }
  else
  {
    // fill the data
    anData[ 0 ] = 0x45;
    anData[ 1 ] = 0x78;
    anData[ 2 ] = 0x69;
    anData[ 3 ] = 0x74;
    anData[ 4 ] = 0x00;
    anData[ 5 ] = 0x00;
    anData[ 6 ] = 0x00;
    anData[ 7 ] = 0x00;

    // wait for data reguest/write block
    while( VSK10XXHandler_DataReady( ) == FALSE );
    VSK10XXHandler_WriteData( anData, 8 );

    // turn off test mode
    tCommand.wValue = MODE_INEW;
    WriteCtl( VSK10XX_REG_MODE, tCommand );
  }
}

U16 VSK10XXHandler_ReadRegister( VSK10XXREG eReg )
{
  U16UN   tData;

  // check for valid register
  if ( eReg < VSK10XX_REG_MAX )
  {
    // chip select the device
    Gpio_Set( VSK10XX_CEN_ENUM, ON );

    // output the write command/address
    Spi_Write( SPI_DEV_ENUM_LCLBUS, VSK_CMD_RD );
    Spi_Write( SPI_DEV_ENUM_LCLBUS, eReg );

    // now write the data
    Spi_Read( SPI_DEV_ENUM_LCLBUS, 0, &tData.anValue[ LE_U16_MSB_IDX ] );
    Spi_Read( SPI_DEV_ENUM_LCLBUS, 0, &tData.anValue[ LE_U16_LSB_IDX ] );

    // delselect the device
    Gpio_Set( VSK10XX_CEN_ENUM, OFF );
  }

  // reutrn the data
  return( tData.wValue );
}

/******************************************************************************
 * @function WriteCtl
 *
 * @brief write a value to a control register
 *
 * This function will enable the control chip select, output the write command,
 * address and the data
 *
 * @param[in]   nAddress      control register address
 * @param[in]   tData           data to be written
 *
 *****************************************************************************/
static void WriteCtl( U8 nAddress, U16UN tData )
{
  // chip select the device
  Gpio_Set( VSK10XX_CEN_ENUM, ON );

  // output the write command/address
  Spi_Write( SPI_DEV_ENUM_LCLBUS, VSK_CMD_WR );
  Spi_Write( SPI_DEV_ENUM_LCLBUS, nAddress );

  // now write the data
  Spi_Write( SPI_DEV_ENUM_LCLBUS, tData.anValue[ LE_U16_MSB_IDX ] );
  Spi_Write( SPI_DEV_ENUM_LCLBUS, tData.anValue[ LE_U16_LSB_IDX ] );

  // delselect the device
  Gpio_Set( VSK10XX_CEN_ENUM, OFF );
}

/**@} EOF VSK11XXHandler.c */

