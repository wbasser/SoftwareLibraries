/******************************************************************************
 * @file DisplaySSD1306.h
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
 
// ensure only one instantiation
#ifndef _DISPLAYSSD1306_H
#define _DISPLAYSSD1306_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the actions
typedef enum _DISPLAYPIXACT
{
  DISPLAY_PIXACT_CLR = 0,
  DISPLAY_PIXACT_SET,
  DISPLAY_PIXACT_TGL
} DISPLAYPIXACT;

/// enumerate the scroll operations
typedef enum _DISPLAYSCROLL
{
  DISPLAY_SCROLL_STOP = 0,
  DISPLAY_SCROLL_RIGHT,
  DISPLAY_SCROLL_LEFT,
  DISPLAY_SCROLL_DIAGRIGHT,
  DISPLAY_SCROLL_DIAGLIFT,
  DISPLAY_SCROLL_MAX
} DISPLAYSCROLL;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DisplaySSD1306_Initialize( void );
extern  void  DisplaySSD1306_SetDim( BOOL bState );
extern  void  DisplaySSD1306_ClearScreen( void );
extern  void  DisplaySSD1306_SetPixel( U8 nX, U8 nY, DISPLAYPIXACT eAction );
extern  void  DisplaySSD1306_WriteBuffer( void );
extern  void  DisplaySSD1306_SetScroll( DISPLAYSCROLL eScrollType, U8 nStart, U8 nStop );

/**@} EOF DisplaySSD1306.h */

#endif  // _DISPLAYSSD1306_H