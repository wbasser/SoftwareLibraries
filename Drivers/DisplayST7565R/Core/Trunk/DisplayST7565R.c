/******************************************************************************
 * @file DisplayST7565R.c
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
 * \addtogroup DisplayST7565R
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "SPI/Spi.h"

// local includes -------------------------------------------------------------
#include "DisplayST7565R/DisplayST7565R.h"

// Macros and Defines ---------------------------------------------------------
/// define the commands
#define CMD_DISPLAY_OFF           ( 0xAE )
#define CMD_DISPLAY_ON            ( 0xAF )
#define CMD_SET_DISP_START_LINE   ( 0x40 )
#define CMD_SET_PAGE              ( 0xB0 )
#define CMD_SET_COLUMN_UPPER      ( 0x10 )
#define CMD_SET_COLUMN_LOWER      ( 0x00 )
#define CMD_SET_ADC_NORMAL        ( 0xA0 )
#define CMD_SET_ADC_REVERSE       ( 0xA1 )
#define CMD_SET_DISP_NORMAL       ( 0xA6 )
#define CMD_SET_DISP_REVERSE      ( 0xA7 )
#define CMD_SET_ALLPTS_NORMAL     ( 0xA4 )
#define CMD_SET_ALLPTS_ON         ( 0xA5 )
#define CMD_SET_BIAS_9            ( 0xA2 )
#define CMD_SET_BIAS_7            ( 0xA3 )
#define CMD_RMW                   ( 0xE0 )
#define CMD_RMW_CLEAR             ( 0xEE )
#define CMD_INTERNAL_RESET        ( 0xE2 )
#define CMD_SET_COM_NORMAL        ( 0xC0 )
#define CMD_SET_COM_REVERSE       ( 0xC8 )
#define CMD_SET_POWER_CONTROL     ( 0x28 )
#define CMD_SET_RESISTOR_RATIO    ( 0x20 )
#define CMD_SET_VOLUME_FIRST      ( 0x81 )
#define CMD_SET_VOLUME_SECOND     ( 0x00 )
#define CMD_SET_STATIC_OFF        ( 0xAC )
#define CMD_SET_STATIC_ON         ( 0xAD )
#define CMD_SET_STATIC_REG        ( 0x00 )
#define CMD_SET_BOOSTER_FIRST     ( 0xF8 )
#define CMD_SET_BOOSTER_234       ( 0x00 )
#define CMD_SET_BOOSTER_5         ( 0x01 )
#define CMD_SET_BOOSTER_6         ( 0x03 )
#define CMD_NOP                   ( 0xE3 )
#define CMD_TEST                  ( 0xF0 )

/// define the contrasts for the various displays
#define CONTRAST_128M             ( 22 )
#define CONTRAST_128L             ( 16 )
#define CONTRAST_132M             ( 33 )

/// display the max x, y and page sizes
#if ( DISPLAYST7565R_TYPE_SELECT == DISPLAYST7565R_TYPE_128M )
  #define DISPLAY_MAX_X           ( 128 )
  #define DISPLAY_MAX_Y           (  64 )
  #define DISPLAY_X_OFFSET        (   4 )
#elif ( DISPLAYST7565R_TYPE_SELECT == DISPLAYST7565R_TYPE_128L )
  #define DISPLAY_MAX_X           ( 128 )
  #define DISPLAY_MAX_Y           (  64 )
  #define DISPLAY_X_OFFSET        (   4 )
#elif ( DISPLAYST7565R_TYPE_SELECT == DISPLAYST7565R_TYPE_132M )
  #define DISPLAY_MAX_X           ( 132 )
  #define DISPLAY_MAX_Y           (  32 )
  #define DISPLAY_X_OFFSET        (   0 )
#else
  #error  "Illegal Display Type!" 
#endif

// calculate the number 
#define DISPLAY_NUM_PAGES         ( DISPLAY_MAX_Y / 8 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8  nLoModX;          ///< lowest modified X
static  U8  nLoModPage;       ///< lowest modified Y
static  U8  nHiModX;          ///< highest modified X
static  U8  nHiModPage;      ///< highest modified Y
static  U8  anDisplayBuffer[ DISPLAY_NUM_PAGES ][ DISPLAY_MAX_X ];

// local function prototypes --------------------------------------------------
static  void  WriteCommand( U8 nCommand );
static  void  WriteData( U8 nData );
static  void  WriteValue( U8 nValue );

// constant parameter initializations -----------------------------------------
static  const CODE U8   anInitCmds[ ] =
{
  CMD_SET_DISP_START_LINE,
  #if ( DISPLAYST7565R_ORIENTATION_SELECT == DISPLAYST7565R_ORIENTATION_TOP )
    CMD_SET_ADC_NORMAL,
    CMD_SET_COM_REVERSE,
  #elif ( DISPLAYST7565R_ORIENTATION_SELECT == DISPLAYST7565R_ORIENTATION_BOTTOM )
    CMD_SET_ADC_REVERSE,
    CMD_SET_COM_NORMAL,
  #else
    #error "Illegal display orientation!"
  #endif
  CMD_SET_DISP_NORMAL,
  CMD_SET_BIAS_9,
  CMD_SET_POWER_CONTROL | 0x07,
  CMD_SET_BOOSTER_FIRST,
  0x00,
  #if ( DISPLAYST7565R_TYPE_SELECT == DISPLAYST7565R_TYPE_128M )
    CMD_SET_RESISTOR_RATIO | 0x07,
    CMD_SET_VOLUME_FIRST,
    CONTRAST_128M,
  #elif ( DISPLAYST7565R_TYPE_SELECT == DISPLAYST7565R_TYPE_128L )
    CMD_SET_RESISTOR_RATIO | 0x07,
    CMD_SET_VOLUME_FIRST,
    CONTRAST_128L,
  #elif ( DISPLAYST7565R_TYPE_SELECT == DISPLAY_TYPE_132M )
    CMD_SET_RESISTOR_RATIO | 0x03,
    CMD_SET_VOLUME_FIRST,
    CONTRAST_132M,
  #endif
  CMD_SET_STATIC_OFF,
  0x00,
  CMD_DISPLAY_ON,
  0xFF
};

/******************************************************************************
 * @function DisplayST7565R_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the display
 *
 *****************************************************************************/
void DisplayST7565R_Initialize( void )
{
  U8  nCommand, nIdx;
  
  // set command mode/enable the chip
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, OFF );
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, ON );
  
  // now output the data
  nIdx = 0;
  while(( nCommand = PGM_RDBYTE( anInitCmds[ nIdx++ ] )) != 0xFF )
  {
    // write the command
    Spi_Write( DISPLAYST7565R_SPI_ENUM, nCommand );
  }
  
  // dispable the chip select
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, OFF );

  // reset the min/max mod
  nLoModX = DISPLAY_MAX_X;
  nHiModX = 0;
  nLoModPage = DISPLAY_NUM_PAGES;
  nHiModPage = 0;
};

/******************************************************************************
 * @function DisplayST7565R_SetBrightness
 *
 * @brief set the brightness
 *
 * This function will set the brightness of the display
 *
 * @param[in]   nBrightness   desired brightness
 *
 *****************************************************************************/
void DisplayST7565R_SetBrightness( U8 nBrightness )
{
  WriteCommand( CMD_SET_VOLUME_FIRST );
  WriteCommand( CMD_SET_VOLUME_SECOND | ( nBrightness & 0x3F ));
}

/******************************************************************************
 * @function DisplayST7565R_ClearScreen
 *
 * @brief clear the screen
 *
 * This function will erase the screen
 *
 *****************************************************************************/
void DisplayST7565R_ClearScreen( void )
{
  U8  nPage, nColumn;
  
  // for each page
  for ( nPage = 0; nPage < DISPLAY_NUM_PAGES; nPage++ )
  {
    // set the page address
    WriteCommand( CMD_SET_COLUMN_UPPER );
    WriteCommand( CMD_SET_COLUMN_LOWER | DISPLAY_X_OFFSET );
    
    // for each column in page
    for ( nColumn = 0; nColumn < DISPLAY_MAX_X; nColumn++ )
    {
      // write a 0
      WriteData( 0 );
      anDisplayBuffer[ nPage ][ nColumn ];
    }
  }
  
  // reset the min/max mod
  nLoModX = DISPLAY_MAX_X;
  nHiModX = 0;
  nLoModPage = DISPLAY_NUM_PAGES;
  nHiModPage = 0;
}

/******************************************************************************
 * @function DisplayST7565R_SetPixel
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
void DisplayST7565R_SetPixel( U8 nX, U8 nY, DISPLAYPIXACT eAction )
{
  U8  nPage, nMask;
  
  // check for valid parameters
  if (( nX < DISPLAY_MAX_X ) && ( nY < DISPLAY_MAX_Y ))
  {
    // compute the page/bit mask
    nPage = nY / 8;
    nMask = nY % 8;
    
    // process the action
    switch( eAction )
    {
      case DISPLAY_PIXACT_CLR :
        // clear the pixel
        anDisplayBuffer[ nPage ][ nX ] &= ~nMask;
        break;
        
      case DISPLAY_PIXACT_SET :
        // set the pixel
        anDisplayBuffer[ nPage ][ nX ] &= ~nMask;
        break;
        
      case DISPLAY_PIXACT_TGL :
        // toggle the pixel
        anDisplayBuffer[ nPage ][ nX ] ^= nMask;
        break;
        
      default :
        break;
    }
    
    // adjust the min/max updates
    if ( nX < nLoModX )
    {
      nLoModX = nX;
    }
    
    if ( nX > nHiModX )
    {
      nHiModX = nX;
    }
    
    if ( nPage < nLoModPage )
    {
      nLoModPage = nPage;
    }
    
    if ( nPage > nHiModPage )
    {
      nHiModPage = nPage;
    }
  }
}

/******************************************************************************
 * @function DisplayST7565R_WriteBuffer
 *
 * @brief outputs the buffer to the dislay
 *
 * This function will update the display with the modified buffer
 *
 *****************************************************************************/
void DisplayST7565R_WriteBuffer( void )
{
  U8  nIndex, nPage;
  
  // enable the chip
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, ON );
  
  // for the modified parts
  for ( nPage = nLoModPage; nPage <= nHiModPage; nPage++ )
  {
    // set command mode/set the page address/beginning column address
    Gpio_Set( DISPLAYST7565R_CDS_GPIO_ENUM, OFF );
    Spi_Write( DISPLAYST7565R_SPI_ENUM, CMD_SET_PAGE | nPage );
    Spi_Write( DISPLAYST7565R_SPI_ENUM, CMD_SET_COLUMN_UPPER | ( nLoModX >> 4 ));
    Spi_Write( DISPLAYST7565R_SPI_ENUM, CMD_SET_COLUMN_LOWER | ( nLoModX & 0x0F ));
    Gpio_Set( DISPLAYST7565R_CDS_GPIO_ENUM, ON );
    
    // now for each x
    for ( nIndex = nLoModX; nIndex < nHiModX; nIndex++ )
    {
      // now write the data
      Spi_Write( DISPLAYST7565R_SPI_ENUM, anDisplayBuffer[ nPage ][ nIndex ] );
    }
  }

  // disable the chip
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, OFF );

  // reset the min/max mod
  nLoModX = DISPLAY_MAX_X;
  nHiModX = 0;
  nLoModPage = DISPLAY_NUM_PAGES;
  nHiModPage = 0;
}


/******************************************************************************
 * @function WriteCommand
 *
 * @brief write a command
 *
 * This function will clear the command/data select and output a command 
 * to the display
 *
 * @param[in]   nCommand      command to write
 *
 *****************************************************************************/
static void WriteCommand( U8 nCommand )
{
  // clear the command data select pin
  Gpio_Set( DISPLAYST7565R_CDS_GPIO_ENUM, OFF );
  
  // write the data
  WriteValue( nCommand );
}

/******************************************************************************
 * @function WriteData
 *
 * @brief write a data
 *
 * This function will set the command/data select and output a data byte
 * to the display
 *
 * @param[in]   nCommand      command to write
 *
 *****************************************************************************/
static void WriteData( U8 nData )
{
  // set the command data select pin
  Gpio_Set( DISPLAYST7565R_CDS_GPIO_ENUM, ON );
  
  // write the data
  WriteValue( nData );
}

/******************************************************************************
 * @function WriteValue
 *
 * @brief write a value to the display
 *
 * This function will enable the chip select, output the value via the
 * SPI, and then de-select
 *
 * @param[in]   nValue    value to output
 *
 *****************************************************************************/
static void WriteValue( U8 nValue )
{
  // set the chip select
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, ON );
  
  // write the data
  Spi_Write( DISPLAYST7565R_SPI_ENUM, nValue );
  
  // deslect the chip
  Gpio_Set( DISPLAYST7565R_CEN_GPIO_ENUM, OFF );
}

/**@} EOF DisplayST7565R.c */
