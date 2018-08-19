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
#include "LcdHD44780/LcdHD44780.h"

// library includes -----------------------------------------------------------
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------
// define the command masks
#define	CMD_CLR_MASK	      ( 0x01 )
#define	CMD_HOM_MASK	      ( 0x02 )
#define	CMD_MOD_MASK	      ( 0x04 )
#define	CMD_CTL_MASK	      ( 0x08 )
#define	CMD_CUR_MASK	      ( 0x10 )
#define	CMD_FNC_MASK	      ( 0x20 )
#define	CMD_CGA_MASK	      ( 0x40 )
#define	CMD_DDA_MASK	      (	0x80 )

// define the mode masks
#define	MOD_SHF_MASK	      ( 0x01 )
#define	MOD_INC_MASK	      ( 0x02 )

// define the control masks
#define	CTL_BLN_MASK	      ( 0x01 )
#define	CTL_CON_MASK	      ( 0x02 )
#define	CTL_DSP_MASK	      ( 0x04 )

// define the cursor masks
#define	CUR_RSH_MASK	      ( 0x04 )
#define	CUR_DSH_MASK	      ( 0x08 )

// define the function masks
#define	FNC_F10_MASK	      ( 0x04 )
#define	FNC_LN2_MASK	      ( 0x08 )
#define	FNC_DL8_MASK	      ( 0x10 )

// define the busy mask
#define	LCD_BSY_MASK	      ( 0x80 )

// define the maximum user defined character location
#define	MAX_USR_LOC		      ( 16 )

// define the number of bar graph characters and their sizes
#define	NUM_BAR_CHRS	      ( 6 )

// define the number of bars graphs
#define	MAX_NUM_BARS	      ( 4 )

// define the bar graph characters
#define BAR_EMPTY_INDEX     ( 0 )
#define BAR_FULL_INDEX      ( 5 )

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _REGSEL
{
  REGSEL_CMD = 0,
  REGSEL_DAT
} REGSEL;

// structures -----------------------------------------------------------------
/// define the bar structure
typedef struct _BARS
{
  U8	nRow;		    // row 
  U8	nCol;		    // coloumn
  U8	nWidth;		  // width
} BARS, *PARS;
#defome BARS_SIZE       sizeof( BARS )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	BARS	atBars[ MAX_NUM_BARS ];

// local function prototypes --------------------------------------------------
static	void	SetCursor( U8 nRow, U8 nCol );
static	void	OutValue( U8 nRegister, U8 nValue, BOOL bWait );
static	U8		InValue( U8 nRegister );
static	void	WaitNotBusy( void );
static	U8		ReadValue( void );
static	void	Delay( void );

// constant parameter initializations -----------------------------------------
// bar graph characters
static	const CODE U8   anBarChars[ NUM_BAR_CHRS ][ LCD_CHR_SIZ ] =
{
  { 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F },
  { 0x1F, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x1F },
  { 0x1F, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x1F },
  { 0x1F, 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x1F },
  { 0x1F, 0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x00, 0x1F },
  { 0x1F, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x1F }
};

// initialize the column offsets
#if (( LCDHD44780_NUM_ROWS == 4 ) && ( LCDHD44780_NUM_COLS == 20 ))
static  const CODE C8   anRowOffset[ LCDHD44780_NUM_ROWS ] =
{
  0x00, 0x40, 0x20, 0x54 
};
#elif (( LCDHD44780_NUM_ROWS == 2 ) && ( LCDHD44780_NUM_COLS == 16 ))
static  const CODE C8   anRowOffset[ LCDHD44780_NUM_ROWS ] =
{
  0x00, 0x40
};
#endif

/******************************************************************************
 * @function LcdHC44780_dInitialize
 *
 * @brief initialization
 *
 * This function will initialize the LCD display
 *
 *****************************************************************************/
 void LcdHD44780_Initialize( void )
{
  U8	nTemp;

  // set up the control port
  LCD_CTL_DDR |= LCD_REG_MSK | LCD_RDW_MSK | LCD_ENB_MSK;

  // delay 15 msec/reset LCD/ delay 3MSEC/reset LCD/repeat three times
  SystemTick_DelayMsec( 15 );	
  OutValue( REGSEL_CMD, 0x33, FALSE );
  SystemTick_DelayMsec( 3 );
  OutValue( REGSEL_CMD, 0x33, FALSE );
  SystemTick_DelayMsec( 3 );
  OutValue( REGSEL_CMD, 0x33, FALSE );
  SystemTick_DelayMsec( 3 );
  OutValue( REGSEL_CMD, 0x33, FALSE );
  SystemTick_DelayMsec( 3 );

  // now set up the interface length and number of lines/delay 6 msec
  nTemp = ( CMD_FNC_MASK | FNC_LN2_MASK );
  if ( LCD_FRB_MODE == 0 )
    nTemp |= FNC_DL8_MASK;

  OutValue( REGSEL_CMD, nTemp, FALSE );
  SystemTick_DelayMsec( 6 );

  // resend length-number of lines/turn on display/clear/cursor home/set mode
  OutValue( REGSEL_CMD, nTemp, TRUE );
  OutValue( REGSEL_CMD, CMD_CTL_MASK | CTL_DSP_MASK, TRUE );
  OutValue( REGSEL_CMD, CMD_CLR_MASK, TRUE );
  OutValue( REGSEL_CMD, CMD_HOM_MASK, TRUE );
  OutValue( REGSEL_CMD, CMD_MOD_MASK | MOD_INC_MASK, TRUE );

  // initialize the bar array
  for ( nTemp = 0; nTemp < MAX_NUM_BARS; nTemp++ )
    atBars[ nTemp ].nWidth = 0xFF;
 
  // create the characters
  for ( nTemp = 0; nTemp < NUM_BAR_CHRS; nTemp++ )
  {
    #ifdef __ATMEL_AVR__
      LcdHD44780_LoadCharP( nTemp, &anBarChars[ nTemp ][ 0 ] );
    #else
      LcdHD44780_LoadChar( nTemp, &anBarChars[ nTemp ][ 0 ] );
    #endif // __ATMEL_AVR__
  }
}

/******************************************************************************
 * @function LcdHD44780_Clear
 *
 * @brief clear the display
 *
 * This function will clear the display
 *
 *****************************************************************************/
void LcdHD44780_Clear( void )
{
  // sent the clear command
  OutValue( REGSEL_CMD, CMD_CLR_MASK, TRUE );
}

/******************************************************************************
 * @function LcdHD44780_PutChar
 *
 * @brief display a character
 *
 * This function will display a character at the current cursor postion
 *
 * @param[in]   nChar       character to display
 *
 *****************************************************************************/
void LcdHD44780_PutChar( U8 nChar )
{
  // write a character
  OutValue( REGSEL_DAT, nChar, TRUE );
}

/******************************************************************************
 * @function LcdHD44780_GetChar
 *
 * @brief get a character
 *
 * This function will return the character at the current cursor position
 *
 * @return      character
 *
 *****************************************************************************/
U8 LcdHD44780_GetChar( void )
{
  // reads a character
  return( InValue( REGSEL_DAT ));
}

/******************************************************************************
 * @function LcdHD44780_CursorControl
 *
 * @brief Set the cursor at the row/col
 *
 * This function will set the cursor to the given row/column and state
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 * @param[in]   eCursor   cursor state
 *
 *****************************************************************************/
void LcdHD44780_CursorControl( U8 nRow, U8 nCol, LCDCURSOR eCursor )
{
  // position the cursor/set the state
  SetCursor( nRow, nCol );
  OutValue( REGSEL_CMD, CMD_CTL_MASK | CTL_DSP_MASK | eCursor, TRUE );
}

/******************************************************************************
 * @function LcdHD44780_DisplayChr
 *
 * @brief display a character
 *
 * This function will display a character at the desired row/column
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 * @param[in]   cChar     character to display
 *
 *****************************************************************************/
void LcdHD44780_DisplayChr( U8 nRow, U8 nCol, C8 cChar )
{
  // set the cursor/output the character
  SetCursor( nRow, nCol );
  OutValue( REGSEL_DAT, cChar, TRUE );
}

/******************************************************************************
 * @function LcdHD44780_DisplayLin
 *
 * @brief display a line
 *
 * This function will display a message and will clear the remaining characters
 * on the given row
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 * @param[in]   pcMsg     pointer to a message
 *
 *****************************************************************************/
void LcdHD44780_DisplayLin( U8 nRow, U8 nCol, PC8 pcMsg )
{
  C8	cChar;
  U8	nPos = nCol;

  // set the cursor
  SetCursor( nRow, nCol );

  // now output the message
  while(( cChar = *( pcMsg++ )) != '\0' )
  {
    // increment position/output char
    nPos++;
    OutValue( REGSEL_DAT, cChar, TRUE );
  }

  // now clear the rest of the line
  while( nPos++ < LCDHD44780_NUM_COLS )
  {
    // output space
    OutValue( REGSEL_DAT, ' ', TRUE );
  }
}

/******************************************************************************
 * @function LcdHD44780_DisplayMsg
 *
 * @brief display a message
 *
 * This function will display a message on the given row/col
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 * @param[in]   pcMsg     pointer to a message
 *
 *****************************************************************************/
void LcdHD44780_DisplayMsg( U8 nRow, U8 nCol, PC8 pcMsg )
{
  S8	cChar;

  // set the cursor
  SetCursor( nRow, nCol );

  // while there are still characters
  while(( cChar = *( pcMsg++ )) != '\0' )
  {
    // output the character
    OutValue( REGSEL_DAT, cChar, TRUE );
  }
}

/******************************************************************************
 * @function LcdHD44780_LoadUserDefinedChar
 *
 * @brief load a user defined character
 *
 * This function will load the user defined character at the given location
 *
 * @param[in]   nLoc        location for the character
 * @param[in]   pnChar      pointer to the character
 *
 *****************************************************************************/
void LcdHD44780_LoadUserDefinedChar( U8 nLoc, PU8 pnChar )
{
  U8	nIdx;

  // check to ensure valid location
  if ( nLoc < MAX_USR_LOC )
  {
    // load the character location
    OutValue( REGSEL_CMD, ( CMD_CGA_MASK | (( nLoc & 0x0F ) << 3 )), TRUE );

    // for each byte
    for ( nIdx = 0; nIdx < LCD_CHR_SIZ; nIdx++ )
    {
      // output it
      OutValue( REGSEL_DAT, *( pnChar + nIdx ), TRUE );
    }
  }
}

/******************************************************************************
 * @function LcdHD44780_CreateBar
 *
 * @brief create and draw a bar 
 *
 * This function will create a bar 
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 * @param[in]   nWidth    width of the bar
 *
 * @return      index of the bar
 *
 *****************************************************************************/
U8 LcdHD44780_CreateBar( U8 nRow, U8 nCol, U8 nWidth )
{
  U8		nIdx;
  U8    nBar = 0xFF;

  // fine an empty location
  for ( nIdx = 0; nIdx < MAX_NUM_BARS; nIdx++ )
  {
    // check for a width of 0XFF
    if ( atBars[ nIdx ].nWidth == 0xFF )
    {
      // check for with
      if (( nCol + nWidth ) <= MAX_USR_LOC )
      {
        // set the handle/copy the values
        nBar = nIdx;
        atBars[ nIdx ].nRow = nRow;
        atBars[ nIdx ].nCol = nCol;
        atBars[ nIdx ].nWidth = nWidth;
        
        // draw the bar
        LcdHD44780_UpdateBar( nBar, 0 );
      }

      // just exit
      break;
    }
  }

  // return the handle
  return( nBar );
}

/******************************************************************************
 * @function LcdHD44780_UpdateBar
 *
 * @brief update a bar
 *
 * This function will update the current value of a bar
 *
 * @param[in]   nBarIdx   bar index
 * @param[in]   cPercent  percent of the bar to fill
 *
 *****************************************************************************/
void LcdHD44780_UpdateBar( U8 nBarIdx, S8 cPercent )
{
  S16	  iValue;
  U8	  nIdx, nFull, nFrac;
  PBARS ptBar;

  // check for a valid bar
  if ( nBar < MAX_NUM_BARS )
  {
    // now get a pointer to the bar
    ptBar = &atBars[ nBarIdx ];
    
    // check for a valid bar
    if ( ptBar->nWidth != 0xFF )
    {
      // position cursor
      SetCursor( ptBar->nRow, ptBar->nCol );

      // now draw the bar
      iValue = ( int )( cPercent * ptBar->nWidth * 5 ) / 100;
      nFull = iValue / 5;
      nFrac = iValue % 5;

      // put the full part
      for ( nIdx = 0; nIdx < nFull; nIdx++ )
      {
        LcdHD447800_PutChar( 5 );
      }

      // process the fraction
      if ( nFrac != 0 )
      {
        LcdHD447800_PutChar( nFrac );
        nFull++;
      }

      // fill the rest
      for ( nIdx = nFull; nIdx < ptBar->nWidth; nIdx++ )
      {
        LcdHD447800_PutChar( 0 );
      }
    }
  }
}

/******************************************************************************
 * @function LcdHD44780_DeleteBar
 *
 * @brief delete a bar
 *
 * This function will remove a bar from the display
 *
 * @param[in]   nBarIdx      bar index
 *
 *****************************************************************************/
void LcdHD44780_DeleteBar( U8 nBarIdx )
{
  PBARS ptBar;

  // check for a valid bar
  if ( nBar < MAX_NUM_BARS )
  {
    // now get a pointer to the bar
    ptBar = &atBars[ nBarIdx ];
    
    // check for a valid bar
    if ( ptBar->nWidth != 0xFF )
    {
      // position cursor
      SetCursor( ptBar->nRow, ptBar->nCol );

      // fill the rest
      for ( nIdx = 0; nIdx < ptBar->nWidth; nIdx++ )
      {
        LcdHD447800_PutChar( 0 );
      }
    }
    
    // now mark this bar as empty
    ptBar->nWidth = 0xFF;
  }
}

 #ifdef __ATMEL_AVR__
/******************************************************************************
 * @function LcdHD44780_DisplayLinP
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
void LcdHD44780_DisplayLinP( U8 nRow, U8 nCol, PFC8 pfcMsg )
{
	PC8	pcBuffer;

	// set up a local buffer
	if (( pcBuffer = ( PC8 )( malloc( strlen_P( pfcMsg )))) != NULL )
	{
		// copy the string
		strcpy_P( pcBuffer, pfcMsg );

		// now display it
		LcdHD44780_DisplayLin( nRow, nCol, pcBuffer );

		// now free the memory
		free( pcBuffer );
	}
}

/******************************************************************************
 * @function LcdHD44780_DisplayMsgP
 *
 * @brief display a message
 *
 * This function will display a message on the given row/col
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 * @param[in]   pcMsg     pointer to a message
 *
 *****************************************************************************/
void LcdHD44780_DisplayMsgP( U8 nRow, U8 nCol, PFC8 pfcMsg )
{
	PC8	pcBuffer;

	// set up a local buffer
	if (( pcBuffer = ( PC8 )( malloc( strlen_P( pfcMsg )))) != NULL )
	{
		// copy the string
		strcpy_P( pcBuffer, pfcMsg );

		// now display it
		LcdHD44780_DisplayMsg( nRow, nCol, pcBuffer );

		// now free the memory
		free( pcBuffer );
	}
}

/******************************************************************************
 * @function LcdHD44780_LoadUserDefinedCharP
 *
 * @brief load a user defined character
 *
 * This function will load the user defined character at the given location
 *
 * @param[in]   nLoc        location for the character
 * @param[in]   pnChar      pointer to the character
 *
 *****************************************************************************/
void LcdHD44780_LoadUserDefinedCharP( U8 nLoc, PFU8 pfnChar )
{
	U8	nBuffer[ LCD_CHR_SIZ ];

	// copy the special character
	memcpy_P( nBuffer, pfnChar, LCD_CHR_SIZ );

	// now display it
	LcdHD44780_LoadUserDefinedChar( nLoc, nBuffer );
}
#endif // __ATMEL_AVR__

/******************************************************************************
 * @function SetCursor
 *
 * @brief set the cursor position
 *
 * This function will se the cursor at the desired row/col
 *
 * @param[in]   nRow      desired row
 * @param[in]   nCol      desired coloumn
 *
 *****************************************************************************/
static void SetCursor( U8 nRow, U8 nCol )
{
	U8	nTemp;

  // check for valid row/column
  if (( nRow < LCDHD44780_NUM_ROWS ) && ( nCol < LCDHD44780_NUM_COLS ))
  {
    // calculate the actual address
    nTemp = anRowOffset[ nRow ];
    nTemp += nCol;
    
    // now output the data address
    OutValue( REGSEL_CMD, CMD_DDA_MASK | nTemp, TRUE );
  }
}

/******************************************************************************
 * @function OutValue
 *
 * @brief output a value
 *
 * This function will wait for busy done, if selected and then write the data
 * to the appropriate register in both 4 bit or 8 bit mode
 *
 * @param[in]   eRegister       command/data register
 * @param[in]   nValue          value to write
 * @param[in]   bWait           wait busy flag
 *
 *****************************************************************************/
static void OutValue( REGSEL eRegister, U8 nValue, BOOL bWait )
{
	// if wait flag, do it
	if ( bWait )
  {
    // wait for not busy
		WaitNotBusy( );
  }

	// set the data port to output
  Gpio_SetGroupDirection( LCDHD44780_DATA_PORT, GPIO_DIR_OUT );

	// select the appropriate register/select write mode
  Gpio_Set( LCDHD44780_REGSEL_PIN, ( eRegister == REGSEL_DAT ) ? ON : OFF );
  Gpio_Set( LCDHD44780_WRTENB_PIN, ON );
		
	// determine mode
	#if ( LCDHD44780_ENABLE_FOUR_BIT_MODE  == 1 )
	{
		// write the MSB
    Gpio_SetGroup( LCDHD44780_DATA_PORT, ( nValue >> 4 ));

		// generate an enable pulse
    Gpio_Set( LCDHD44780_ENABLE_PIN, ON );
		Delay( );
    Gpio_Set( LCDHD44780_ENABLE_PIN, OFF );
	}
	#endif
		
  // write the MSB
  Gpio_SetGroup( LCDHD44780_DATA_PORT, nValue );

  // generate an enable pulse
  Gpio_Set( LCDHD44780_ENABLE_PIN, ON );
  Delay( );
  Gpio_Set( LCDHD44780_ENABLE_PIN, OFF );
}

/******************************************************************************
 * @function InValue
 *
 * @brief read a value from a register
 *
 * This function will read a value from a register
 *
 * @param[in]   eRegister     register to read
 *
 * @return      the read value
 *
 *****************************************************************************/
static U8 InValue( REGSEL eRegister )
{
  U8  nTemp;
  
	// wait for not busy
	WaitNotBusy( );

	// select the appropriate register/select read mode
  Gpio_Set( LCDHD44780_REGSEL_PIN, ( eRegister == REGSEL_DAT ) ? ON : OFF );
  Gpio_Set( LCDHD44780_WRTENB_PIN, OFF );
  
  // read the data
  nTemp = ReadValue( );
		
	// return the read value
	return( nTemp );
}

/******************************************************************************
 * @function WaitNotBusy
 *
 * @brief wait for not busy
 *
 * This function will read the status and will return when not busy
 *
 *****************************************************************************/
static void WaitNotBusy( void )
{
	U8	nValue;

	// set the data port to input
  Gpio_SetGroupDirection( LCDHD44780_DATA_PORT, GPIO_DIR_IN );

	// select the control-status register/set the read mode
  Gpio_Set( LCDHD44780_REGSEL_PIN, OFF );
  Gpio_Set( LCDHD44780_WRTENB_PIN, OFF );

	// loop till not busy or timeout
	do
	{
		nValue = ReadValue( );
	}
	while( nValue & LCD_BSY_MASK );
}

/******************************************************************************
 * @function ReadValue
 *
 * @brief read a value from a register
 *
 * This function will read a value from a register
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
static U8 ReadValue( void )
{
	U8	nValue, nTemp = 0;

	// if 4 bit mode
	#if ( LCDHD44780_ENABLE_FOUR_BIT_MODE  == 1 )
	{
		// generate an enable pulse/read the MSB
    Gpio_Set( LCDHD44780_ENABLE_PIN, ON );
		Delay( );
		Gpio_GetGroup( LCDHD44780_DATA_PORT, &nValue );;
    Gpio_Set( LCDHD44780_ENABLE_PIN, OFF );
    
    // shift to the MSB
    nValue << 4;
	}
	#endif

  // generate an enable pulse/read the MSB
  Gpio_Set( LCDHD44780_ENABLE_PIN, ON );
  Delay( );
  Gpio_GetGroup( LCDHD44780_DATA_PORT, &nTemp );;
  Gpio_Set( LCDHD44780_ENABLE_PIN, OFF );
  
  // or with the original
  nValue |= nTemp;
	
	// return the value
	return( nValue );
}

/******************************************************************************
 * @function Delay
 *
 * @brief delay for some 
 *
 * This function will delay for four cycles to allow I/O settle
 *
 *****************************************************************************/
static void Delay( void )
{
	// delay for four cycles for I/O settle
	asm volatile( "nop\n\t" );
	asm volatile( "nop\n\t" );
	asm volatile( "nop\n\t" );
	asm volatile( "nop\n\t" );
}

/**@} EOF LcdHD44780.c */
