/******************************************************************************
 * @file DisplayILI9341.h
 *
 * @brief Display ILI9341 declarations 
 *
 * This file provides the declarations for ILI9341 display driver
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
 
// ensure only one instantiation
#ifndef _DISPLAYILI9341_H
#define _DISPLAYILI9341_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DisplayILI9341/DisplayILI9341_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the RGB macro
#define RGB( red, green, blue )       ((( red & 0x1F ) << 12 ) | (( grn & 0x3F ) << 6 ) | ( blu & 0x1F ))

/// define the basic colors
#define DISPLAY_ILI9314_COLOR_RED     RGB( 0x1F, 0x00, 0x00 )
#define DISPLAY_ILI9314_COLOR_GRN     RGB( 0x00, 0x3F, 0x00 )
#define DISPLAY_ILI9314_COLOR_BLU     RGB( 0x00, 0x00, 0x1F )
#define DISPLAY_ILI9314_COLOR_BLK     RGB( 0x00, 0x00, 0x00 )
#define DISPLAY_ILI9314_COLOR_YEL     RGB( 0x1F, 0x3F, 0x00 )
#define DISPLAY_ILI9314_COLOR_WHT     RGB( 0x1F, 0x3F, 0x1F )
#define DISPLAY_ILI9314_COLOR_CYN     RGB( 0x00, 0x3F, 0x1F )
#define DISPLAY_ILI9314_COLOR_LGY     RGB( 0x10, 0x20, 0x10 )
#define DISPLAY_ILI9314_COLOR_DGY     RGB( 0x80, 0x10, 0x80 )

/// define the maimum X/Y
#define DISPLAY_ILI9314_MIN_X         (   0 )
#define DISPLAY_ILI9314_MAX_X         ( 239 )
#define DISPLAY_ILI9314_MIN_Y         (   0 )
#define DISPLAY_ILI9314_MAX_Y         ( 319 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DisplayILI9314_Initialize( void );
extern  void  DisplayILI9314_SetColumn( U16 wStartCol, U16 wEndCol );
extern  void  DisplayILI9314_SetPage( U16 wStartPage, U16 wEndPage );
extern  void  DisplayILI9314_SetXY( U16 wXPoint, U16 wYPoint );
extern  void  DisplayILI9314_SetPixel( U16 wXPoint, U16 wYPoint, U16 wColor );
extern  void  DisplayILI9314_FillScreen( U16 wXLeft, U16 wXRight, U16 wYUp, U16 wYDown, U16 wColor );
extern  void  DisplayILI9314_ClearScreen( void );
extern  void  DisplayILI9314_DrawString( PC8 pcString, U8 nCharSize, U16 wXPoint, U16 wYPoint, U16 wColor );
extern  void  DisplayILI9314_DrawChar( C8 cChar, U8 nCharSize, U16 wXPoint, U16 wYPoint, U16 wColor );
extern  void  DisplayILI9314_DrawRectangle( BOOL bFill, U16 wXLeft, U16 wXRight, U16 wLength, U16 wWidth, U16 wColor ); 
extern  void  DisplayILI9314_DrawCircle( BOOL bFill, U16 wXPoint, U16 wYPoint, U16 wRadius, U16 wColor );
extern  void  DisplayILI9314_DrawTriangle( BOOL bFill, U16 wXPoint1, U16 wYPoint1, U16 wXPoint2, U16 wYPoint2, U16 wXPoint3, U16 wYPoint3, U16 wColor );
extern  void  DisplayILI9314_DrawVerticalLine( U16 wXStart, U16 wYStart, U16 wLength, U16 wColor );
extern  void  DisplayILI9314_DrawHorizontalLine( U16 wXStart, U16 wYstart, U16 wWidth, U16 wColor );
extern  void  DisplayILI9314_DrawLine( U16 wXStart, U16 wYStart, U16 wXEnd, U16 wYEnd, U16 wColor );

/**@} EOF DisplayILI9341.h */

#endif  // _DISPLAYILI9341_H