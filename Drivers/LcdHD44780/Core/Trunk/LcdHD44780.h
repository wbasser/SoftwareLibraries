/******************************************************************************
 * @file LcdHD44780.h
 *
 * @brief LCD display driver declarations
 *
 * This file provides the declarations for the LCD HD44780 driver
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
 * \addtogroup LcdHD44780
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LCDHD44780_H
#define _LCDHD44780_H

// local includes -------------------------------------------------------------
#include "LcdHD44780/LcdHD44780_prm.h"

// enumerations ---------------------------------------------------------------
/// enumerate the LCD cursor enumeration
typedef enum  _LCDCURSOR
{
	LCD_CUR_OFF = 0,      ///< cursor off
	LCD_CUR_ON,           ///< cursor on
	LCD_CUR_BLN           ///< cursor blink
} LCDCURSOR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void 	LcdHD44780_Initialize( void );
extern	void 	LcdHD44780_Clear( void );
extern	void 	LcdHD44780_PutChar( U8 nChar );
extern	U8 		LcdHD44780_GetChar( void );
extern	void 	LcdHD44780_CursorControl( U8 nRow, U8 nCol, LCDCURSOR eCursor );
extern	void	LcdHD44780_DisplayChr( U8 nRow, U8 nCol, C8 cChar );
extern	void	LcdHD44780_DisplayLin( U8 nRow, U8 nCol, PC8 pcMsg );
extern	void 	LcdHD44780_DisplayMsg( U8 nRow, U8 nCol, PC8 pcMsg );
extern	void 	LcdHD44780_LoadUserDefinedChar( U8 nLoc, PU8 pnChar );
extern	U8	  LcdHD44780_CreateBar( U8 nRow, U8 nCol, U8 nWidth );
extern	void	LcdHD44780_UpdateBar( U8 nBarIdx, C8 cPercent );
extern	void  LcdHD44780_DeleteBar( U8 nBarIdx );
 #ifdef __ATMEL_AVR__
extern	void	LcdHD44780_DisplayLinP( U8 nRow, U8 nCol, PFC8 pfcMsg );
extern	void 	LcdHD44780_DisplayMsgP( U8 nRow, U8 nCol, PFC8 pfcMsg );
extern	void 	LcdHD44780_LoadUserDefinedCharP( U8 nLoc, PFU8 pfnChar );
#endif

/**@} EOF LCDHD44780.h */

#endif  // _LCDHD44780_H