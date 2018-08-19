/******************************************************************************
 * @file DisplayILI9341.c
 *
 * @brief ILI9341 display driver implementation
 *
 * This file implements the ILI9341 driver
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
 * \addtogroup DisplayILI9341
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include  "DisplayILI9341/DisplayILI9341.h"
#include  "DisplayILI9341/DisplayILI9341_prv.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macros for initializing the initialization table
#define CMDDEF( value ) \
  { \
    .eType = CMDDATA_TYPE_CMD, \
    .nValue = value, \
  }
#define DATDEF( value ) \
  { \
    .eType = CMDDATA_TYPE_DAT, \
    .nValue = value, \
  }
#define DLYDEF( value ) \
  { \
    .eType = CMDDATA_TYPE_DLY, \
    .nValue = value, \
  }
#define ENDDEF( ) \
  { \
    .eType = CMDDATA_TYPE_END, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the command/data
typedef enum _CMDDATA
{
  CMDDATA_TYPE_CMD = 0,
  CMDDATA_TYPE_DAT,
  CMDDATA_TYPE_DLY,
  CMDDATA_TYPE_END,
} CMDDATA;

// structures -----------------------------------------------------------------
/// define the initialization structure
typedef struct _INITDATA
{
  CMDDATA eType;
  U8      nValue;
} INITDATA, *PINITDATA;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  WriteDataBlock( PU8 pnData, U16 wLegnth );
static  void  WriteDataWord( U16 wValue );
static  void  WriteDataByte( U8 nValue );
static  void  WriteCommand( U8 nValue );
static  void  Write( U8 nValue );

// constant parameter initializations -----------------------------------------
static  const CODE atInitData[ ] =
{
  CMDDEF( 0x01 ),
  DLYDEF( 200  ),
  
  CMDDEF( 0xCF ),
  DATDEF( 0x00 ),
  DATDEF( 0x8B ),
  DATDEF( 0x30 ),
  
  CMDDEF( 0xED ),
  DATDEF( 0x67 ),
  DATDEF( 0x03 ),
  DATDEF( 0x12 ),
  DATDEF( 0x81 ),
  
  CMDDEF( 0xE8 ),
  DATDEF( 0x85 ),
  DATDEF( 0x10 ),
  DATDEF( 0x7A ),
  
  CMDDEF( 0xcB ),
  DATDEF( 0x39 ),
  DATDEF( 0x2C ),
  DATDEF( 0x00 ),
  DATDEF( 0x34 ),
  DATDEF( 0x02 ),
  
  CMDDEF( 0xF7 ),
  DATDEF( 0x20 ),
  
  CMDDEF( 0xEA ),
  DATDEF( 0x00 ),
  DATDEF( 0x00 ),
  
  CMDDEF( 0xC0 ),   // power control
  DATDEF( 0x1B ),   // VRH[5:0]
  
  CMDDEF( 0xC1 ),   // power control
  DATDEF( 0x10 ),   // SAP[2:0], BT[3:0]
  
  CMDDEF( 0xC5 ),   // VCM control
  DATDEF( 0x3F ),
  DATDEF( 0x3C ),
  
  CMDDEF( 0xC7 ),   // VCM control 2
  DATDEF( 0xB7 ),
  
  CMDDEF( 0x36 ),   // memory access control
  DATDEF( 0x08 ),
  
  CMDDEF( 0x3A ),
  DATDEF( 0x55 ),
  
  CMDDEF( 0xB1 ),
  DATDEF( 0x00 ),
  DATDEF( 0x1B ),
  
  CMDDEF( 0xB6 ),   // display function control
  DATDEF( 0x0A ),
  DATDEF( 0xA2 ),
  
  CMDDEF( 0xF2 ),   // 3gamma function disable
  DATDEF( 0x00 ),
  
  CMDDEF( 0x26 ),   // gamma curve selected
  DATDEF( 0x01 ),
  
  CMDDEF( 0xE0 ),   // set gamma
  DATDEF( 0x0F ),
  DATDEF( 0x2A ),
  DATDEF( 0x28 ),
  DATDEF( 0x08 ),
  DATDEF( 0x0E ),
  DATDEF( 0x08 ),
  DATDEF( 0x54 ),
  DATDEF( 0xA9 ),
  DATDEF( 0x43 ),
  DATDEF( 0x0A ),
  DATDEF( 0x0F ),
  DATDEF( 0x00 ),
  DATDEF( 0x00 ),
  DATDEF( 0x00 ),
  DATDEF( 0x00 ),
  
  CMDDEF( 0xE1 ),   // set gamma
  DATDEF( 0x00 ),
  DATDEF( 0x15 ),
  DATDEF( 0x17 ),
  DATDEF( 0x07 ),
  DATDEF( 0x11 ),
  DATDEF( 0x06 ),
  DATDEF( 0x2B ),
  DATDEF( 0x56 ),
  DATDEF( 0x3C ),
  DATDEF( 0x05 ),
  DATDEF( 0x10 ),
  DATDEF( 0x0F ),
  DATDEF( 0x3F ),
  DATDEF( 0x3F ),
  DATDEF( 0x0F ),
  
  CMDDEF( 0x11 ),   // exit sleep
  DLYDEF( 120  ),
  
  CMDDEF( 0x29 ),   // display on
  
  ENDDEF
};

/******************************************************************************
 * @function DisplayILI9314_Initialize
 *
 * @brief initilaization
 *
 * This function will step throught he initialization structure and execute
 * the appropriate commands
 *
 *****************************************************************************/
void DisplayILI9314_Initialize( void )
{
  U8        nIndex = 0;
  PINITDATA ptInitEntry;
  BOOL      bRunFlag = TRUE;
  
  // for each entry in the table
  while( bRunFlag )
  {
    // set the pointer
    ptInitEntry = ( PINITDATA )&atInitData[ nIndex++ ];
    
    // process the type
    switch( ptInitEntry->eType )
    {
      case CMDDATA_TYPE_CMD :
        // write a command byte
        WriteCommand( ptInitEntry->nValue );
        break;
        
      case CMDDATA_TYPE_DAT :
        // write a data byte
        WriteData( ptInitEntry->nValue );
        break;
        
      case CMDDATA_TYPE_DLY :
        // delay for the time
        SystemTick_DelayMsec( ptInitEntry->nValue );
        break;
        
      case CMDDATA_TYPE_END :
      default :
        // clear the run flag
        bRunFlag = FALSE;
        break;
    }
  };
}

/******************************************************************************
 * @function DisplayILI9314_SetColumn
 *
 * @brief set the column
 *
 * This function will set the start and end columns
 *
 * @param[in]   wStartCol
 * @param[in]   wEndCol
 *
 *****************************************************************************/
void DisplayILI9314_SetColumn( U16 wStartCol, U16 wEndCol )
{
  // set the command/start col/end col
  WriteCommand( 0x2A );     // colum command address
  WriteDataWord( wStartCol );
  WriteDataWord( wEndCol );
}

/******************************************************************************
 * @function DisplayILI9314_SetPage
 *
 * @brief set the page
 *
 * This function will set the start and end page
 *
 * @param[in]   wStartPage
 * @param[in]   wEndPage
 *
 *****************************************************************************/
void DisplayILI9314_SetPage( U16 wStartPage, U16 wEndPage )
{
  // set the page/start col/end col
  WriteCommand( 0x2B );     // page command address
  WriteDataWord( wStartPage );
  WriteDataWord( wEndPage );
}

/******************************************************************************
 * @function DisplayILI9314_SetXY
 *
 * @brief set the X,Y point
 *
 * This function will set the cursor to a desired x,y location
 *
 * @param[in]   wXPoint   x coordinate
 * @param[in]   wYPoint   y coordinate
 *
 *****************************************************************************/
void DisplayILI9314_SetXY( U16 wXPoint, U16 wYPoint )
{
  // set the column with xPoint, page with yPoint
  DisplayILI9314_SetColumn( wxPoint, wxPoint );
  DisplayILI9314_SetPage( wYPoint, wYPoint );
  
  // send the command
  WriteCommand( 0x2C );
}

/******************************************************************************
 * @function DisplayILI9314_SetPixel
 *
 * @brief set a pixel
 *
 * This function will set the pixel at the desired location
 *
 * @param[in]   wXPoint   x coordinate
 * @param[in]   wYPoint   y coordiante
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_SetPixel( U16 wXPoint, U16 wYPoint, U16 wColor )
{
  // set the coumn/page
  DisplayILI9314_SetXY( wXPoint, wYPoint );
  
  // write the data
  WriteDataWord( wColor );
}

/******************************************************************************
 * @function DisplayILI9314_FillScreen
 *
 * @brief fill a screen with a color
 *
 * This function will set the pixel at the desired location
 *
 * @param[in]   wXLeft    x left coordinate
 * @param[in]   wXRight   x right coordinate
 * @param[in]   wYUp      y up coordiante
 * @param[in]   wYDown    y down coordiante
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_FillScreen( U16 wXLeft, U16 wXRight, U16 wYUp, U16 wYDown, U16 wColor )
{
  U32 uCount, uIndex;
  U8  nColorMsb, nColorLsb;

  // adjust for start lower than end
  if ( wXLeft > wXRight )
  {
    // adjus
    wXLeft ^= wXRight;
    wXRight ^= wXLeft;
    wXLeft ^= wXRight;
  }
  
  // adjust for up lower than down
  if ( wYUp > wYDown )
  {
    wYUp ^= wYDown;
    wYDown ^= wYup;
    wYUp ^= wYDown;
  }
  
  // constrain to display size
  wXLeft = CONSTRAIN( wXLeft, DISPLAY_ILI9314_MIN_X, DISPLAY_ILI9314_MAX_X );
  wXRight = CONSTRAIN( wXRight, DISPLAY_ILI9314_MIN_X, DISPLAY_ILI9314_MAX_X );
  wXUp = CONSTRAIN( wXUp, DISPLAY_ILI9314_MIN_Y, DISPLAY_ILI9314_MAX_Y );
  wXDown = CONSTRAIN( wXDown, DISPLAY_ILI9314_MIN_Y, DISPLAY_ILI9314_MAX_Y );

  // compute the total count
  uCount = ( wXRight - wXLeft + 1 ) * ( wYDown - wYUP + 1 );

  // set the column with xPoint, page with yPoint
  DisplayILI9314_SetColumn( wXLeft, wXRight );
  DisplayILI9314_SetPage( wYUp, wYDown );
  
  // send a command
  WriteCommand( 0x2C );
  
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, ON );
  
  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, ON );
  
  // get the color value
  nColorMsb = HI16( wColor );
  nColocLsb = LO16( wColor );
  
  // for the total count
  for ( uIndex = 0; uIndex < uCount; uIndex++ )
  {
    // write the color
    Spi_Write( DISPLAYIL9341_SPI_ENUM, nColorMsb );
    Spi_Write( DISPLAYIL9341_SPI_ENUM, nColorLsb );
  }
  
  // disable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, OFF );
}

/******************************************************************************
 * @function DisplayILI9314_ClearScreen
 *
 * @brief clear the screen
 *
 * This function will clear the screen
 *
 * @param[in]   wXPoint   x coordinate
 * @param[in]   wYPoint   y coordiante
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_ClearScreen( void )
{
  // set the column with xPoint, page with yPoint
  DisplayILI9314_SetColumn( 0, DISPLAYILI9314_MAX_COL );
  DisplayILI9314_SetPage( 0, DISPLAYILI9314_MAX_ROW );
  
  // send the command
  WriteCommand( 0x2C );

  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, ON );
  
  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, ON );
  
  // for each pixel
  for ( wIndex = 0; wIndex < DISPLAYILI9314_MAX_PIXELS; wIndex++ )
  {
    // write 4 bytes
    Spi_Write( DISPLAYIL9341_SPI_ENUM, 0 );
  }
  
  // chip disable
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, OFF );
}

/******************************************************************************
 * @function DisplayILI9314_DrawString
 *
 * @brief draw string
 *
 * This function will a string of characters at the current address and 
 * wrap to the previous line if longer that display
 *
 * @param[in]   pcString  pointer to the string
 * @param[in]   nCharSize character size
 * @param[in]   wXPoint   x left coordinate
 * @param[in]   wYPoint   y right coordinate
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawString( PC8 pcString, U8 nCharSize, U16 wXPoint, U16 wYPoint, U16 wColor )
{
  C8  cChar;
  
  // for each character
  while(( cChar = *( pcString++ )) != 0 )
  {
    // draw the character
    DisplayILI9314_DrawChar( cChar, nCharSize, wXPoint, wYPoint, wColor );
    
    // increment x
    if ( wXPoint < DISPLAY_ILI9314_MAX_X )
    {
      // increment the cursor location
      wXPoint += ( FONT_SPACE * nCharSize );
    }
  }
}

/******************************************************************************
 * @function DisplayILI9314_DrawChar
 *
 * @brief draw a character on the string
 *
 * This function draw a character on the screen
 *
 * @param[in]   cChar     character
 * @param[in]   nCharSize character size
 * @param[in]   wXPoint   x left coordinate
 * @param[in]   wYPoint   y right coordinate
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawChar( C8 cChar, U8 nCharSize, U16 wXPoint, U16 wYPoint, U16 wColor )
{
  U8  nCharRow, nRowIdx, nBitIdx;
  
  // validate character
  if (( cChar < ' ' ) || ( cChar > '~' ))
  {
    // set it to space
    cChar = ' ';
  }
  else
  {
    // adjust it
    cChar -= ' ';
  }
  
  // for each row
  for( nRowIdx = 0; nRowIdx < FONT_X_SIZE; nRowIdx++ )
  {
    // get the row value
    nCharRow = PGM_RDBYTE( anFonts[ cChar ][ nRowIdx ] );
    
    // for each entry in the table
    for ( nBitIdx = 0; nBitIdx < FONT_X_SIZE; nBitIdx++ )
    {
      // if bit is set
      if (( nCharRow & BIT( nBitIdx )) != 0 )
      {
        // draw it
        DisplayILI9314_FillScreen(( wXPoint + ( nRowIdx * nCharSize )), 
                                  (( wYPoint + ( nBitIdx * nCharSize )) + nCharSize ), 
                                  ( wYPoint + ( nBitIdx * nCharSize )), 
                                  (( wYPoint + ( nBitIdx * nCharSize )) + nCharSize, 
                                  wColor );
      }
    }
  }
}

/******************************************************************************
 * @function DisplayILI9314_DrawRectangle
 *
 * @brief draw a rectangle
 *
 * This function draw a rectangle and fill it if it requires it
 *
 * @param[in]   bFill     fill rectangle
 * @param[in]   wXLeft    x left coordinate
 * @param[in]   wXRight   x right coordinate
 * @param[in]   wLength   length of rectangle
 * @param[in]   wWidth    width of rectangle
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawRectangle( BOOL bFill, U16 wXLeft, U16 wXRight, U16 wLength, U16 wWidth, U16 wColor )
{
  // draw the rectangle
  DisplayILI9314_DrawHorizontalLine( wXLeft, wXRight, wLength, wColor );
  DisplayILI9314_DrawHorizontalLine( wXLeft, wXRight + wWidth, wLength, wColor );
  DisplayILI9314_DrawVerticalLine( wXLeft, wXRight, wWidth, wColor );
  DisplayILI9314_DrawVerticalLine( wXLeft + wLength, wXRight, wWidth, wColor );
  
  // check for fill
  if ( bFill )
  {
    // fill it
    DisplayILI9314_FillScreen( wXLeft + 1, wXRight - 1, wYUp + 1, wYDown - 1, wColor );
  }
}

/******************************************************************************
 * @function DisplayILI9314_DrawCircle
 *
 * @brief draw a circle
 *
 * This function will draw a circle
 *
 * @param[in]   bFill     fill rectangle
 * @param[in]   wXPoint   x left coordinate
 * @param[in]   wYPoint   y right coordinate
 * @param[in]   wRadius   radius
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawCircle( BOOL bFill, U16 wXPoint, U16 wYPoint, U16 wRadius, U16 wColor )
{
  S16 sX, sY, sErr, sErr2;
  
  // set the intitial values
  sX = ( S16 )-wRadius;
  sY = 0;
  sErr = 2 - ( 2* wRadius );
  
  // loop
  do
  {
    // determine if fill
    if ( bFill )
    {
      // draw a vertical line
      DisplayILI9314_DrawVerticalLine( wXPoint - sX, wYPoint - sY, 2 * sY, wColor );
      DisplayILI9314_DrawVerticalLine( wXPoint + sX, wYPoint - sY, 2 * sY, wColor );
    }
    else
    {
      // just draw pixel
      DisplayILI9314_SetPixel( wXPoint - sX, wYPoint + sY, wColor );
      DisplayILI9314_SetPixel( wXPoint + sX, wYPoint + sY, wColor );
      DisplayILI9314_SetPixel( wXPoint + sX, wYPoint - sY, wColor );
      DisplayILI9314_SetPixel( wXPoint - sX, wYPoint - sY, wColor );
      
    }
    
    // adjust the error
    sErr2 = sErr;
    if ( sErr2 <= sY )
    {
      // recompute
      sErr += (( sY + 1 ) * 2 ) + 1;
      if (( -sX == sY ) && ( sErr2 <= sX )
      {
        sErr2 = 0;
      }
      if ( sErr2 > sX )
      {
        sErr += (( sX + 1 ) * 2 ) + 1;
      }
    }
  } while( sX <= 0 );
}

/******************************************************************************
 * @function DisplayILI9314_DrawTriangle
 *
 * @brief draw a triangle
 *
 * This function will draw a triangle
 *
 * @param[in]   wXPoint1  x left coordinate
 * @param[in]   wYPoint1  y right coordinate
 * @param[in]   wXPoint2  x left coordinate
 * @param[in]   wYPoint2  y right coordinate
 * @param[in]   wXPoint3  x left coordinate
 * @param[in]   wYPoint3  y right coordinate
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawTriangle( BOOL bFill, U16 wXPoint1, U16 wYPoint1, U16 wXPoint2, U16 wYPoint2, U16 wXPoint3, U16 wYPoint3, U16 wColor )
{
  // draw the three sides
  DisplayILI9314_DrawLine( wXPoint1, wYPoint1, wXPoint2, wYPoint2 );
  DisplayILI9314_DrawLine( wXPoint1, wYPoint1, wXPoint3, wYPoint3 );
  DisplayILI9314_DrawLine( wXPoint2, wYPoint2, wXPoint3, wYPoint3 );
}

/******************************************************************************
 * @function DisplayILI9314_DrawVerticalLine
 *
 * @brief draw a vertical line
 *
 * This function will draw a vertical line
 *
 * @param[in]   wXStart   start X coordiante
 * @param[in]   wYStart   start Y coordinant
 * @param[in]   wLength   length
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawVerticalLine( U16 wXStart, U16 wYStart, U16 wLength, U16 wColor )
{
  U16 wIndex;
  
  // set the column with xPoint, page with yPoint
  DisplayILI9314_SetColumn( wxStart, wxStart );
  DisplayILI9314_SetPage( wYStart, wYStart + wLength );
  
  // send the command
  WriteCommand( 0x2C );
  
  // now write the color
  for ( wIndex = 0; wIndex < wLength; wIndex++ )
  {
    // write the color
    WriteDataWord( wColor );
  }
}

/******************************************************************************
 * @function DisplayILI9314_DrawHorizontalLine
 *
 * @brief draw a horizontal line
 *
 * This function will set the pixel at the desired location
 *
 * @param[in]   wXStart   start X coordiante
 * @param[in]   wYStart   start Y coordinant
 * @param[in]   wWidth    width
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawHorizontalLine( U16 wXStart, U16 wYstart, U16 wWidth, U16 wColor )
{
  U16 wIndex;
  
  // set the column with xPoint, page with yPoint
  DisplayILI9314_SetColumn( wxStart, wxStart + wWidth );
  DisplayILI9314_SetPage( wYStart, wYStart );
  
  // send the command
  WriteCommand( 0x2C );
  
  // now write the color
  for ( wIndex = 0; wIndex < wWidth; wIndex++ )
  {
    // write the color
    WriteDataWord( wColor );
  }
}

/******************************************************************************
 * @function DisplayILI9314_DrawLine
 *
 * @brief draw a line
 *
 * This function will draw a line
 *
 * @param[in]   wXStart   start X coordiante
 * @param[in]   wYStart   start Y coordinant
 * @param[in]   wXEnd     end X coordiante
 * @param[in]   wYEnd     end Y coordinant
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void DisplayILI9314_DrawLine( U16 wXStart, U16 wYStart, U16 wXEnd, U16 wYEnd, U16 wColor )
{
  S16 sX, sY, sDx, sDy, sSx, sSy, sErr, sErr2;
  
  // set up the initial values
  sX = wXEnd - wXStart;
  sY = wYEnd - wYStart;
  sDx = abs( sX );
  sSx = ( wXStart < wXEnd ) ? 1 : -1;
  sDy = -abs( sY );
  sSy = ( wYStart < wYEnd ) ? 1 : -1;
  sErr = sDx + sDy;
  
  FOREVER
  {
    // write a pixel
    DisplayILI9314_SetPixel( wXStart, wYStart, wColor );
    
    // adjust it
    sErr2 = 2 * sErr;
    if ( sErr2 >= sDy )
    {
      // check for done
      if ( wXStart == wXEnd )
      {
        // break out of loop
        break;
      }
      
      // adjust 
      sErr += sDY;
      wXStart += sSx;
    }
    
    if ( sErr2 <= sDx )
    {
      // check for done
      if ( wStartY == wYEnd )
      {
        // break out of loop
        break;
      }
      
      // adjust
      sErr += sDx;
      wYStart += sSy;
    }
  }
}

/******************************************************************************
 * @function ReadData
 *
 * @brief read a data byte
 *
 * This function will send the address, and and then read the data
 *
 * @param[in]   nAddr     address to read
 * @param[in]   nParam    paremeter
 *
 * @return      data read
 *
 *****************************************************************************/
static U8 ReadDataByte( U8 nAddr, U8 nParam )
{
  U8  nData;
  
  // set the extension command
  WriteCommand( 0xD9 );
  
  // write the data
  WriteData( 0x10 + nParam );
  
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, OFF );
  
  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, ON );

  // send the address
  Spi_Write( DISPLAYIL9341_SPI_ENUM, nAddr );
  
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, ON );
  
  // read the data
  Spi_Read( DISPLAYIL9341_SPI_ENUM, 0x00, &nData );

  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, OFF );
  
  // return the data read
  return( nData );
}

/******************************************************************************
 * @function WriteDataBlock
 *
 * @brief write a data block
 *
 * This function will set the data/command select line to data and call
 * the write function
 *
 * @param[in] pwData        block to be written  
 * @param[in] wLength       numbef of wordw to be written
 *
 *****************************************************************************/
static void WriteDataBlock( PW8 pwData, U16 wLength )
{
  U16UN tValue;
  U8    nIndex;
  
  // wtore it
  tValue.wValue = wValue;
  
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, ON );
  
  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, ON );
  
  // for each entry
  for( nIndex = 0; nIndex < wLength; nIndex++ )
  {
    // store it
    tValue.wValue = *( pwValue + nIndex );
    
    // now write the block
    Spi_Write( DISPLAYIL9341_SPI_ENUM, tValue.anValue[ LE_U16_MSB_IDX ] );
    Spi_Write( DISPLAYIL9341_SPI_ENUM, tValue.anValue[ LE_U16_LSB_IDX ] );
  }
  
  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, OFF );
}

/******************************************************************************
 * @function WriteDataWord
 *
 * @brief write a data word
 *
 * This function will set the data/command select line to data and call
 * the write function
 *
 * @param[in] wValue      value to be written  
 *
 *****************************************************************************/
static void WriteDataWord( U16 wValue )
{
  U16UN tValue;
  
  // store it
  tValue.wValue = wValue;
  
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, ON );
  
  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, ON );
  
  // now write the block
  Spi_Write( DISPLAYIL9341_SPI_ENUM, tValue.anValue[ LE_U16_MSB_IDX ] );
  Spi_Write( DISPLAYIL9341_SPI_ENUM, tValue.anValue[ LE_U16_LSB_IDX ] );

  // chip enable it
  Gpio_Set( DISPLAYILI9341_CENB_ENUM, OFF );
}

/******************************************************************************
 * @function WriteData
 *
 * @brief write a data byte
 *
 * This function will set the data/command select line to data and call
 * the write function
 *
 * @param[in] nValue      value to be written  
 *
 *****************************************************************************/
static void WriteData( U8 nValue )
{
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, ON );
  
  // now write it
  Write( nValue );
}

/******************************************************************************
 * @function WriteCommand 
 *
 * @brief write a command byte
 *
 * This function will set the data/command select line to command and call
 * the write function
 *
 * @param[in] nValue      value to be written  
 *
 *****************************************************************************/
static void WriteCommand( U8 nValue )
{
  // set the command low
  Gpio_Set( DISPLAYILI9341_DCSEL_ENUM, OFF );
  
  // now write it
  Write( nValue );
}

/******************************************************************************
 * @function Write
 *
 * @brief write a byte to the device
 *
 * This function will enable the chip select, transfer the value, disable the
 * chip enable
 *
 * @param[in] nValue      value to be written  
 *
 *****************************************************************************/
static void Write( U8 nValue )
{
  // enable the chip select
  Gpio_Set( DISPLAYIL9341_CENB_ENUM, ON );
  
  // write it
  Spi_Write( DISPLAYIL9341_SPI_ENUM, nValue );
  
  // disable the chip select
  Gpio_Set( DISPLAYIL9341_CENB_ENUM, OFF );
}
 
/**@} EOF DisplayILI9341.c */
