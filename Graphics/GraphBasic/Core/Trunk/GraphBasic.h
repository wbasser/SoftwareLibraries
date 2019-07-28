/******************************************************************************
 * @file GraphBasic.h
 *
 * @brief graphics basic library declaration 
 *
 * This file provides the declarations for the graphics library
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
 * \addtogroup GraphBasic
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GRAPHBASIC_H
#define _GRAPHBASIC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GraphBasic/GraphBasic_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the colors
typedef enum _GRAPHBASICCLR
{
  GRAPHBASIC_CLR_ENUM_BLK = 0,
  GRAPHBASIC_CLR_ENUM_RED,
  GRAPHBASIC_CLR_ENUM_GRN,
  GRAPHBASIC_CLR_ENUM_BLU,
  GRAPHBASIC_CLR_ENUM_YEL,
  GRAPHBASIC_CLR_ENUM_CYN,
  GRAPHBASIC_CLR_ENUM_PUR,
  GRAPHBASIC_CLR_ENUM_WHT,
  GRAPHBASIC_CLR_MAX
} GRAPHBASICCLR;

/// enumerate the pixel acton
typedef enum _GRAPHBASICPIXACT
{
  GRAPHBASIC_PIXACT_CLR = 0,
  GRAPHBASIC_PIXACT_SET,
  GRAPHBASIC_PIXACT_XOR,
  GRAPHBASIC_PIXACT_MAX
} GRAPHBASICPIXACT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  GraphBasic_Initialize( void );
extern  void  GraphBasic_DrawPixel( U16 wX, U16 wY, GRAPHBASICPIXACT eAction, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawLine( U16 wStartX, U16 wStartY, U16 wEndX, U16 wEndY, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawRectangle( U16 wTopLeft, U16 wTopRight, U16 wLength, U16 wWidth, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawCircle( U16 wXPoint, U16 wYPoint, U16 wRadius, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawTriangle( U16 wXPoint1, U16 wYPoint1, U16 wXPoint2, U16 wYPoint2, U16 wXPoint3, U16 wYPoint3, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawString( U16 wStartX, U16 wStartY, PC8 pszString, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawChar( U16 wStartX, U16 wStartY, C8 cChar, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawVerticalLine( U16 wXStart, U16 wYStart, U16 wLength, GRAPHBASICCLR eColor );
extern  void  GraphBasic_DrawHorizontalLine( U16 wXStart, U16 wYstart, U16 wWidth, GRAPHBASICCLR eColor );

/**@} EOF GraphBasic.h */

#endif  // _GRAPHBASIC_H