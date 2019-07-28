/******************************************************************************
 * @file DisplaySSD1306.c
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
 * \addtogroup DisplaySSD1306
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DisplaySSD1306/DisplaySSD1306_prm.h"
#include "DisplaySSD1306/DisplaySSD1306.h"

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
/// determine the display type/set various options
#if ( DISPLAYSSD1306_TYPE_SELECT == DISPLAYSSD1306_TYPE_128_64 )
  #define DISPLAY_WIDTH                             ( 128 )
  #define DISPLAY_HEIGHT                            ( 64 )
  #define DISPLAY_END_PAGE                          ( 7 )
  #define SSD1306_MULTIPLEX_VALUE                   ( 0x1F )
  #define SSD1306_COMPINS_VALUE                     ( 0x12 )
  #define SSD1306_DEVICE_ADDRESS                    ( 0x3D )
  #if ( DISPLAYSSD1306_VCC_SELECT == DISPLAYSSD1306_VCC_EXTRNAL )
    #define SSD1306_CONTRAST_VALUE                  ( 0x9F )
    #define SSD1306_VCC_VALUE                       ( 0x10 )
    #define SDD1306_PRECHARGE_VALUE                 ( 0x22 )
  #else
    #define SSD1306_CONTRAST_VALUE                  ( 0xCF )
    #define SSD1306_VCC_VALUE                       ( 0x14 )
    #define SDD1306_PRECHARE_VALUE                  ( 0xF1 )
  #endif // DISPLAYSSD1306_VCC_SELECT
#elif ( DISPLAYSSD1306_TYPE_SELECT == DISPLAYSSD1306_TYPE_128_32 )
  #define DISPLAY_WIDTH                             ( 128 )
  #define DISPLAY_HEIGHT                            ( 32 )
  #define DISPLAY_END_PAGE                          ( 3 )
  #define SSD1306_MULTIPLEX_VALUE                   ( 0x3F )
  #define SSD1306_COMPINS_VALUE                     ( 0x02 )
  #define SSD1306_DEVICE_ADDRESS                    ( 0x3C )
  #if ( DISPLAYSSD1306_VCC_SELECT == DISPLAYSSD1306_VCC_EXTRNAL )
    #define SSD1306_CONTRAST_VALUE                  ( 0x8F )
    #define SSD1306_VCC_VALUE                       ( 0x10 )
    #define SSD1306_PRECHARGE_VALUE                 ( 0x22 )
  #else
    #define SSD1306_CONTRAST_VALUE                  ( 0xCF )
    #define SSD1306_VCC_VALUE                       ( 0x14 )
    #define SSD1306_PRECHARE_VALUE                  ( 0xF1 )
  #endif // DISPLAYSSD1306_VCC_SELECT
#elif ( DISPLAYSSD1306_TYPE_SELECT == DISPLAYSSD1306_TYPE_96_16 )
  #define DISPLAY_WIDTH                             ( 96 )
  #define DISPLAY_HEIGHT                            ( 16 )
  #define DISPLAY_END_PAGE                          ( 1 )
  #define SSD1306_MULTIPLEX_VALUE                   ( 0x0F )
  #define SSD1306_COMPINS_VALUE                     ( 0x02 )
  #define SSD1306_DEVICE_ADDRESS                    ( 0x3C )
  #if ( DISPLAYSSD1306_VCC_SELECT == DISPLAYSSD1306_VCC_EXTRNAL )
    #define SSD1306_CONTRAST_VALUE                  ( 0x10 )
    #define SSD1306_VCC_VALUE                       ( 0x10 )
    #define SSD1306_PRECHARGE_VALUE                 ( 0x22 )
  #else
    #define SSD1306_CONTRAST_VALUE                  ( 0xAF )
    #define SSD1306_VCC_VALUE                       ( 0x14 )
    #define SSD1306_PRECHARE_VALUE                  ( 0xF1 )
  #endif // DISPLAYSSD1306_VCC_SELECT
#else
` #error "DISPLAYSSD1306_TYPE_SELECT incorrectly set" 
#endif // DISPLAYSSD1306_TYPE_SELECT

/// define the command
#define SSD1306_SETCONTRAST                       ( 0x81 )
#define SSD1306_DISPLAYALLON_RESUME               ( 0xA4 )
#define SSD1306_DISPLAYALLON                      ( 0xA5 )
#define SSD1306_NORMALDISPLAY                     ( 0xA6 )
#define SSD1306_INVERTDISPLAY                     ( 0xA7 )
#define SSD1306_DISPLAYOFF                        ( 0xAE )
#define SSD1306_DISPLAYON                         ( 0xAF )
#define SSD1306_SETDISPLAYOFFSET                  ( 0xD3 )
#define SSD1306_SETCOMPINS                        ( 0xDA )
#define SSD1306_SETVCOMDETECT                     ( 0xDB )
#define SSD1306_SETDISPLAYCLOCKDIV                ( 0xD5 )
#define SSD1306_SETPRECHARGE                      ( 0xD9 )
#define SSD1306_SETMULTIPLEX                      ( 0xA8 )
#define SSD1306_SETLOWCOLUMN                      ( 0x00 )
#define SSD1306_SETHIGHCOLUMN                     ( 0x10 )
#define SSD1306_SETSTARTLINE                      ( 0x40 )
#define SSD1306_MEMORYMODE                        ( 0x20 )
#define SSD1306_COLUMNADDR                        ( 0x21 )
#define SSD1306_PAGEADDR                          ( 0x22 )
#define SSD1306_COMSCANINC                        ( 0xC0 )
#define SSD1306_COMSCANDEC                        ( 0xC8 )
#define SSD1306_SEGREMAP                          ( 0xA0 )
#define SSD1306_CHARGEPUMP                        ( 0x8D )
#define SSD1306_EXTERNALVCC                       ( 0x01 )
#define SSD1306_SWITCHCAPVCC                      ( 0x02 )
#define SSD1306_ACTIVATE_SCROLL                   ( 0x2F )
#define SSD1306_DEACTIVATE_SCROLL                 ( 0x2E )
#define SSD1306_SET_VERTICAL_SCROLL_AREA          ( 0xA3 )
#define SSD1306_RIGHT_HORIZONTAL_SCROLL           ( 0x26 )
#define SSD1306_LEFT_HORIZONTAL_SCROLL            ( 0x27 )
#define SSD1306_VERTICAL_RIGHT_HORIZONTAL_SCROLL  ( 0x29 )
#define SSD1306_VERTICAL_LEFT_HORIZONTAL_SCROLL   ( 0x2A )

/// define the clock divide ratio
#define SSD1306_CLKDIV_RATIO                      ( 0x80 )

/// define the buffer size
#define BUFFER_SIZE                               (( DISPLAY_HEIGHT * DISPLAY_WIDTH ) / 8 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8  anBuffer[ BUFFER_SIZE ];
#if ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_I2C )
  static  U8  nI2cCommand;
#endif //DISPLAYSSD1306_INTERFACE_SELECT

// local function prototypes --------------------------------------------------
static  void  WriteCommand( U8 nValue );
static  void  WriteData( U8 nValue );
static  void  WriteValue( U8 nValue );

// constant parameter initializations -----------------------------------------
static  const U8    anInitializationSequence[ ] =
{
  SSD1306_DISPLAYOFF,
  SSD1306_SETDISPLAYCLOCKDIV,
  SSD1306_CLKDIV_RATIO,
  SSD1306_SETMULTIPLEX,
  SSD1306_MULTIPLEX_VALUE,
  SSD1306_SETDISPLAYOFFSET,
  0,
  SSD1306_SETSTARTLINE,
  SSD1306_CHARGEPUMP,
  SSD1306_VCC_VALUE,
  SSD1306_MEMORYMODE,
  0x00,
  SSD1306_SEGREMAP | 0x1,
  SSD1306_COMSCANDEC,
  SSD1306_SETCOMPINS,
  SSD1306_COMPINS_VALUE,
  SSD1306_SETCONTRAST,
  SSD1306_CONTRAST_VALUE,
  SSD1306_SETPRECHARGE,
  SDD1306_PRECHARGE_VALUE,
  SSD1306_SETVCOMDETECT,
  0x40,
  SSD1306_DISPLAYALLON_RESUME,
  SSD1306_NORMALDISPLAY,
  SSD1306_DISPLAYON
};

/******************************************************************************
 * @function DisplaySSD1306_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the display
 *
 *****************************************************************************/
void DisplaySSD1306_Initialize( void )
{
  U8  nIdx;
  
  #if ( DISPLAYSD1306_RST_ENABLE == 1 )
    // drive reset low/delay 2 MS/drive reset high
    Gpio_Set( DISPLAYSSD1306_RST_PIN_ENUM, ON );
    SystemTickDelayMsec( 2 );
    Gpio_Set( DISPLAYSSD1306_RST_PIN_ENUM, OFF );
  #endif // DISPLAYSD1306_RST_ENABLE
  
  // for each byte in the initialization sequence
  for ( nIdx = 0; nIdx < sizeof( anInitializationSequence ); nIdx++ )
  {
    // write the command 
    WriteCommand( anInitializationSequence[ nIdx ] );
  }
}

/******************************************************************************
 * @function DisplaySSD1306_SetDim
 *
 * @brief dim or set the brightness
 *
 * This function will set the brightness of the display
 *
 * @param[in]   bBright   
 *
 *****************************************************************************/
void DisplaySSD1306_SetDim( BOOL bState )
{
  U8  nContrastValue;
  
  // check dim/bright
  nContrastValue = ( bState ) ? SSD1306_CONTRAST_VALUE : 0;
  
  // now output the value
  WriteCommand( SSD1306_SETCONTRAST );
  WriteCommand( nContrastValue );
}

/******************************************************************************
 * @function DisplaySSD1306_ClearScreen
 *
 * @brief clear the screen
 *
 * This function will erase the screen
 *
 *****************************************************************************/
void DisplaySSD1306_ClearScreen( void )
{
  // clear the display
  memset( anBuffer, 0, BUFFER_SIZE );
  
  // now write the buffer
  DisplaySSD1306_WriteBuffer( );
}

/******************************************************************************
 * @function DisplaySSD1306_SetPixel
 *
 * @brief set a pixel in the display
 *
 * This function will in the local buffer
 *
 * @param[in]   nX        x location
 * @param[in]   nY        y location
 * @param[in]   eAction   pixel action, clear, set, toggle
 *
 *****************************************************************************/
void DisplaySSD1306_SetPixel( U8 nX, U8 nY, DISPLAYPIXACT eAction )
{
  U16 wOffset;
  U8  nMask;
  
  // check for valid x/y locations
  if (( nX < DISPLAY_WIDTH ) && ( nY < DISPLAY_HEIGHT ))
  {
    // determine if we are rotated
    
    // calculate the offset/mask
    wOffset = nX + ( nY / 8 ) * DISPLAY_WIDTH;
    nMask = BIT( nY & 0x07 );
    
    // determine the action
    switch( eAction )
    {
      case DISPLAY_PIXACT_CLR :
        anBuffer[ wOffset ]  &= ~( nMask );
        break;
        
      case DISPLAY_PIXACT_SET :
        anBuffer[ wOffset ] |= nMask;
        break;
        
      case DISPLAY_PIXACT_TGL :
        anBuffer[ wOffset ] ^= nMask;
        break;
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function DisplaySSD1306_SetColData
 *
 * @brief sets a columns data
 *
 * This function will write the column data at a given page, column
 *
 * @param[in]   nPage     page
 * @param[in]   nCol      column
 * @param[in]   nData     data
 * @param[in]   eAction   pixel action, clear, set, toggle
 *
 *****************************************************************************/
void DisplaySSD1306_SetColData( U8 nPage, U8 nCol, U8 nData, DISPLAYPIXACT eAction )
{
  U16 wOffset;

  // test for valid page/column
  if (( nPage < DISPLAY_END_PAGE ) && ( nCol < DISPLAY_WIDTH )) 
  {
    // compute buffer index
    wOffset = ( nPage * DISPLAY_WIDTH ) + nCol;
    
    // determine the action
    switch( eAction )
    {
      case DISPLAY_PIXACT_CLR :
        anBuffer[ wOffset ] &= ~( nData );
        break;
        
      case DISPLAY_PIXACT_SET :
        anBuffer[ wOffset ] |= nData;
        break;
        
      case DISPLAY_PIXACT_TGL :
        anBuffer[ wOffset ] ^= nData;
        break;
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function DisplaySSD1306_WriteBuffer
 *
 * @brief outputs the buffer to the dislay
 *
 * This function will update the display with the modified buffer
 *
 *****************************************************************************/
void DisplaySSD1306_WriteBuffer( void )
{
  // set the column start address/end addresses/page start/end addresses
  WriteCommand( SSD1306_COLUMNADDR );
  WriteCommand( 0 );
  WriteCommand( DISPLAY_WIDTH - 1 );
  WriteCommand( SSD1306_PAGEADDR );
  WriteCommand( 0 );
  WriteCommand( DISPLAY_END_PAGE );

  #if ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_SPI )
  // drive command/data select HI
  Gpio_Set( DISPLAYSSD1306_SPI_DCS_ENUM, HI );
  
  // enable the chip select
  Gpio_Set( DISPLAYSSD1306_SPI_CEN_ENUM, ON );
  
  // now output the data
  Spi_WriteBlock( DISPLAYSSD1306_SPI_DEV_ENUM, anBuffer, BUFFER_SIZE, FALSE );
  
  // disable the chip select
  Gpio_Set( DISPLAYSSD1306_SPI_CEN_ENUM, OFF );
  #else
  // for each block of data
  for ( U16 wIdx = 0; wIdx < BUFFER_SIZE; wIdx += 16 )
  {
    // fill the transfer control
    I2CXFRCTL tXfrCtl =
    {
      .nDevAddr   = SSD1306_DEVICE_ADDRESS,
      .nAddrLen   = 1,
      .tAddress   = 
      {
        .anValue[ LE_U16_LSB_IDX ] = 0x40ul,
      },
      .pnData     = &anBuffer[ wIdx ],
      .wDataLen   = 16,
      .uTimeout    = 5
    };

    // write it
    I2c_Write( DISPLAYSSD1306_I2C_EVN_ENUM, &tXfrCtl );
  }
  #endif
}

void DisplaySSD1306_SetScroll( DISPLAYSCROLL eScrollType, U8 nStart, U8 nStop )
{
  U8  anTempBuffer[ 10 ];
  U8  nBufferIdx = 0;
  U8  nIdx;

  // determine the scroll
  switch( eScrollType )
  {
    case DISPLAY_SCROLL_STOP :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_DEACTIVATE_SCROLL;
      break;
      
    case DISPLAY_SCROLL_RIGHT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_RIGHT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 0xFF;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    case DISPLAY_SCROLL_LEFT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_LEFT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 0xFF;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    case DISPLAY_SCROLL_DIAGRIGHT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_SET_VERTICAL_SCROLL_AREA;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = DISPLAY_HEIGHT;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_VERTICAL_RIGHT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 1;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    case DISPLAY_SCROLL_DIAGLIFT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_SET_VERTICAL_SCROLL_AREA;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = DISPLAY_HEIGHT;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_VERTICAL_LEFT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 1;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    default :
      break;
  }
  
  // now output the buffer
  for ( nIdx = 0; nIdx < nBufferIdx; nIdx++ )
  {
    // write the buffer
    WriteCommand( anTempBuffer[ nIdx ] );
  }
}

/******************************************************************************
 * @function WriteCommand
 *
 * @brief write a command to the device
 *
 * This function 
 *
 * @param[in]   nValue      value to write
 *
 *****************************************************************************/
static  void  WriteCommand( U8 nValue )
{
  #if ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_SPI )
  // drive command/data select low
  Gpio_Set( DISPLAYSSD1306_SPI_DCS_ENUM, LO );
  #else
  // set the I2C command
  nI2cCommand = 0;
  #endif
  
  // write the value
  WriteValue( nValue );
}

/******************************************************************************
 * @function WriteData
 *
 * @brief write a byte of data to the device
 *
 * This function 
 *
 * @param[in]   nValue      value to write
 *
 *****************************************************************************/
static void WriteData( U8 nValue )
{
  #if ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_SPI )
  // drive command/data select low
  Gpio_Set( DISPLAYSSD1306_SPI_DCS_ENUM, HI );
  #else
  // set the I2C command
  nI2cCommand = 0x40;
  #endif
  
  // write the value
  WriteValue( nValue );
}

/******************************************************************************
 * @function WriteValue
 *
 * @brief write a value
 *
 * This function actually writes the value to the appropriate address
 *
 * @param[in]   nValue      value to write
 *
 *****************************************************************************/
static void WriteValue( U8 nValue )
{
  #if ( DISPLAYSSD1306_INTERFACE_SELECT == DISPLAYSSD1306_INTERFACE_SPI )
  // drive chip select low/output the value/drive the chip select hi
  Gpio_Set( DISPLAYSSD1306_SPI_CEN_ENUM, ON );
  Spi_Write( DISPLAYSSD1306_SPI_DEV_ENUM, nValue );
  Gpio_Set( DISPLAYSSD1306_SPI_CEN_ENUM, OFF );
  #else
  I2CERR  eI2cErr;

  // fill the transfer control
  I2CXFRCTL tXfrCtl =
  {
    .nDevAddr   = SSD1306_DEVICE_ADDRESS,
    .nAddrLen   = 1,
      .tAddress   = 
      {
        .anValue[ LE_U16_LSB_IDX ] = 0x40ul,
      },
    .pnData     = &nValue,
    .wDataLen   = 1,
    .uTimeout   = 5
  };

  // write it
  eI2cErr = I2c_Write( DISPLAYSSD1306_I2C_EVN_ENUM, &tXfrCtl );
  #endif
}

/**@} EOF DisplaySSD1306.c */
