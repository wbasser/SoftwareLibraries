/******************************************************************************
 * @file 
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GraphBasic/GraphBasic.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

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
void GraphBasic_Initialize( void )
{
  // call the local initialization to initialize the actual display
  GraphBasic_LocalInitialize( );
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
void GraphBasic_DrawPixel( U16 wX, U16 wY, GRAPHBASICPIXACT eAction, GRAPHBASICCLR eColor )
{
}

/******************************************************************************
 * @function GraphBasic_DrawLine
 *
 * @brief draw a line
 *
 * This function will draw a line
 *
 * @param[in]   wXStart   start X coordiante
 * @param[in]   wYStart   start Y coordinant
 * @param[in]   wXEnd     end X coordiante
 * @param[in]   wYEnd     end Y coordinant
 * @param[in]   eColor    desired color
 *
 *****************************************************************************/
void GraphBasic_DrawLine( U16 wStartX, U16 wStartY, U16 wEndX, U16 wEndY, GRAPHBASICCLR eColor )
{
  S16 sX, sY, sDx, sDy, sSx, sSy, sErr, sErr2;
  
  // set up the initial values
  sX = wEndX - wStartX;
  sY = wEndY - wStartY;
  sDx = abs( sX );
  sSx = ( wStartX < wEndX ) ? 1 : -1;
  sDy = -abs( sY );
  sSy = ( wStartY < wEndY ) ? 1 : -1;
  sErr = sDx + sDy;
  
  FOREVER
  {
    // write a pixel
    GraphBasic_DrawPixel( wStartX, wStartY, GRAPHBASIC_PIXACT_SET, eColor );
    
    // adjust it
    sErr2 = 2 * sErr;
    if ( sErr2 >= sDy )
    {
      // check for done
      if ( wStartX == wEndX )
      {
        // break out of loop
        break;
      }
      
      // adjust 
      sErr += sDy;
      wStartX += sSx;
    }
    
    if ( sErr2 <= sDx )
    {
      // check for done
      if ( wStartY == wEndY )
      {
        // break out of loop
        break;
      }
      
      // adjust
      sErr += sDx;
      wStartY += sSy;
    }
  }
  
  // refresh the screen
  GraphBasic_RefreshScreen( );
}


/******************************************************************************
 * @function GraphBasic_DrawRectangle
 *
 * @brief draw a rectangle
 *
 * This function draw a rectangle and fill it if it requires it
 *
 * @param[in]   wTopLeft    x left coordinate
 * @param[in]   wTopRight   x right coordinate
 * @param[in]   wLength   length of rectangle
 * @param[in]   wWidth    width of rectangle
 * @param[in]   eColor    desired color
 *
 *****************************************************************************/
void GraphBasic_DrawRectangle( U16 wTopLeft, U16 wTopRight, U16 wLength, U16 wWidth, GRAPHBASICCLR eColor )
{
  // draw the rectangle
  GraphBasic_DrawHorizontalLine( wTopLeft, wTopRight, wLength, eColor );
  GraphBasic_DrawHorizontalLine( wTopLeft, wTopRight + wWidth, wLength, eColor );
  GraphBasic_DrawVerticalLine( wTopLeft, wTopRight, wWidth, eColor );
  GraphBasic_DrawVerticalLine( wTopLeft + wLength, wTopRight, wWidth, eColor );
  
  // refresh the screen
  GraphBasic_RefreshScreen( );
}

/******************************************************************************
 * @function DisplayILI9314_DrawCircle
 *
 * @brief draw a circle
 *
 * This function will draw a circle
 *
 * @param[in]   wXPoint   x left coordinate
 * @param[in]   wYPoint   y right coordinate
 * @param[in]   wRadius   radius
 * @param[in]   eColor    desired color
 *
 *****************************************************************************/
void GraphBasic_DrawCircle( U16 wXPoint, U16 wYPoint, U16 wRadius, GRAPHBASICCLR eColor )
{
  S16 sX, sY, sErr, sErr2;
  
  // set the intitial values
  sX = ( S16 )-wRadius;
  sY = 0;
  sErr = 2 - ( 2* wRadius );
  
  // loop
  do
  {
    // just draw pixel
    GraphBasic_DrawPixel( wXPoint - sX, wYPoint + sY, GRAPHBASIC_PIXACT_SET, eColor );
    GraphBasic_DrawPixel( wXPoint + sX, wYPoint + sY, GRAPHBASIC_PIXACT_SET, eColor );
    GraphBasic_DrawPixel( wXPoint + sX, wYPoint - sY, GRAPHBASIC_PIXACT_SET, eColor );
    GraphBasic_DrawPixel( wXPoint - sX, wYPoint - sY, GRAPHBASIC_PIXACT_SET, eColor );
    
    // adjust the error
    sErr2 = sErr;
    if ( sErr2 <= sY )
    {
      // recompute
      sErr += (( sY + 1 ) * 2 ) + 1;
      if (( -sX == sY ) && ( sErr2 <= sX ))
      {
        sErr2 = 0;
      }
      if ( sErr2 > sX )
      {
        sErr += (( sX + 1 ) * 2 ) + 1;
      }
    }
  } while( sX <= 0 );
  
  // refresh the screen
  GraphBasic_RefreshScreen( );
}


/******************************************************************************
 * @function GraphBasic_DrawTriangle
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
 * @param[in]   eColor    desired color
 *
 *****************************************************************************/
void GraphBasic_DrawTriangle( U16 wXPoint1, U16 wYPoint1, U16 wXPoint2, U16 wYPoint2, U16 wXPoint3, U16 wYPoint3, GRAPHBASICCLR eColor )
{
  // draw the three sides
  GraphBasic_DrawLine( wXPoint1, wYPoint1, wXPoint2, wYPoint2, eColor );
  GraphBasic_DrawLine( wXPoint1, wYPoint1, wXPoint3, wYPoint3, eColor );
  GraphBasic_DrawLine( wXPoint2, wYPoint2, wXPoint3, wYPoint3, eColor );
  
  // refresh the screen
  GraphBasic_RefreshScreen( );
}

/******************************************************************************
 * @function GraphBasic_DrawString
 *
 * @brief draw string
 *
 * This function will a string of characters at the current address and 
 * wrap to the previous line if longer that display
 *
 * @param[in]   wXPoint   x left coordinate
 * @param[in]   wYPoint   y right coordinate
 * @param[in]   pszString pointer to the string
 * @param[in]   eColor    desired color
 *
 *****************************************************************************/
void GraphBasic_DrawString( U16 wStartX, U16 wStartY, PC8 pszString, GRAPHBASICCLR eColor )
{
  C8  cChar;
  
  // for each character
  while(( cChar = *( pszString++ )) != 0 )
  {
    // draw the character
    GraphBasic_DrawChar( wStartX, wStartY, cChar, eColor );
    
    // increment x
    if ( wStartX < GraphBasic_GetMaxX( ))
    {
      // increment the cursor location
      wStartX += ( GraphBasic_GetFontX( ));
    }
  }
  
  // refresh the screen
  GraphBasic_RefreshScreen( );
}

/******************************************************************************
 * @function GraphBasic__DrawChar
 *
 * @brief draw a character on the string
 *
 * This function draw a character on the screen
 *
 * @param[in]   wXPoint   x left coordinate
 * @param[in]   wYPoint   y right coordinate
 * @param[in]   cChar     character
 * @param[in]   wColor    desired color
 *
 *****************************************************************************/
void GraphBasic_DrawChar( U16 wStartX, U16 wStartY, C8 cChar, GRAPHBASICCLR eColor )
{
}

/******************************************************************************
 * @function GraphBasic_DrawVerticalLine
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
void GraphBasic_DrawVerticalLine( U16 wXStart, U16 wYStart, U16 wLength, GRAPHBASICCLR eColor )
{
}

/******************************************************************************
 * @function GraphBasic_DrawHorizontalLine
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
void GraphBasic_DrawHorizontalLine( U16 wXStart, U16 wYstart, U16 wWidth, GRAPHBASICCLR eColor )
{
}

/**@} EOF GraphBasic.c */
