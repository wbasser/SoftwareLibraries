/******************************************************************************
 * @file MorseCodeTable.c
 *
 * @brief morse code table implementations
 *
 * This file provides the implementation of the morse code table
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
 * \addtogroup MorseCodeTable
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MorseCodeTable/MorseCodeTable.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the code table offset
#define CODE_TABLE_OFFSET           ( ' ' )

/// define the code table size
#define CODE_TABLE_SIZE             ( 'Z' - CODE_TABLE_OFFSET + 1 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static  const CODE U16  awCodeTable[ CODE_TABLE_SIZE ] =
{
	0xF000,		// 1111 1100 0000 0000b  ( 32)       WORD SPACE
	0x0000,		// 0000 0000 0000 0000b  ( 33)  !
	0x0000,		// 0000 0000 0000 0000b  ( 34)  "
	0x0000,		// 0000 0000 0000 0000b  ( 35)  #
	0x0000,		// 0000 0000 0000 0000b  ( 36)  $
	0x0000,		// 0000 0000 0000 0000b  ( 37)  %
	0x0000,		// 0000 0000 0000 0000b  ( 38)  &
	0x0000,		// 0000 0000 0000 0000b  ( 39)  '
	0x0000,		// 0000 0000 0000 0000b  ( 40)  (
	0x0000,		// 0000 0000 0000 0000b  ( 41)  )
	0x566C,		// 0101 0110 0110 1100b  ( 42)  *    ...-.-  SK
	0x6670,		// 0110 0110 0111 0000b  ( 43)  +    .-.-.   AR
	0xA5AC,		// 1010 0101 1010 1100b  ( 44)  ,    --..--
	0x0000,		// 0000 0000 0000 0000b  ( 45)  -
	0x666C,		// 0110 0110 0110 1100b  ( 46)  .    .-.-.-
	0x9670,		// 1001 0110 0111 0000b  ( 47)  /    -..-.
	0xAAB0,		// 1010 1010 1011 0000b  ( 48)  0    -----
	0x6AB0,		// 0110 1010 1011 0000b  ( 49)  1    .----
	0x5AB0,		// 0101 1010 1011 0000b  ( 50)  2    ..---
	0x56B0,		// 0101 0110 1011 0000b  ( 51)  3    ...--
	0x55B0,		// 0101 0101 1011 0000b  ( 52)  4    ....-
	0x5570,		// 0101 0101 0111 0000b  ( 53)  5    .....
	0x9570,		// 1001 0101 0111 0000b  ( 54)  6    -....
	0xA570,		// 1010 0101 0111 0000b  ( 55)  7    --...
	0xA970,		// 1010 1001 0111 0000b  ( 56)  8    ---..
	0xAA70,		// 1010 1010 0111 0000b  ( 57)  9    ----.
	0x0000,		// 0000 0000 0000 0000b  ( 58)  :
	0x0000,		// 0000 0000 0000 0000b  ( 59)  ;
	0x0000,		// 0000 0000 0000 0000b  ( 60)  <    
	0x95B0,		// 1001 0101 1011 0000b  ( 61)  =    -...-   BT
	0x0000,		// 0000 0000 0000 0000b  ( 62)  >
	0x5A5C,		// 0101 1010 0101 1100b  ( 63)  ?    ..--..
	0x0000,		// 0000 0000 0000 0000b  ( 64)  @
	0x6C00,		// 0110 1100 0000 0000b  ( 65)  A    .-
	0x95C0,		// 1001 0101 1100 0000b  ( 66)  B    -...
	0x99C0,		// 1001 1001 1100 0000b  ( 67)  C    -.-.
	0x9700,		// 1001 0111 0000 0000b  ( 68)  D    -..
	0x7000,		// 0111 0000 0000 0000b  ( 69)  E    .
	0x59C0,		// 0101 1001 1100 0000b  ( 70)  F    ..-.
	0xA700,		// 1010 0111 0000 0000b  ( 71)  G    --.
	0x55C0,		// 0101 0101 1100 0000b  ( 72)  H    ....
	0x5C00,		// 0101 1100 0000 0000b  ( 73)  I    ..
	0x6AC0,		// 0110 1010 1100 0000b  ( 74)  J    .---
	0x9B00,		// 1001 1011 0000 0000b  ( 75)  K    -.-
	0x65C0,		// 0110 0101 1100 0000b  ( 76)  L    .-..
	0xAC00,		// 1010 1100 0000 0000b  ( 77)  M    --
	0x9C00,		// 1001 1100 0000 0000b  ( 78)  N    -.
	0xAB00,		// 1010 1011 0000 0000b  ( 79)  O    ---
	0x69C0,		// 0110 1001 1100 0000b  ( 80)  P    .--.
	0xA6C0,		// 1010 0110 1100 0000b  ( 81)  Q    --.-
	0x6700,		// 0110 0111 0000 0000b  ( 82)  R    .-.
	0x5700,		// 0101 0111 0000 0000b  ( 83)  S    ...
	0xB000,		// 1011 0000 0000 0000b  ( 84)  T    -
	0x5B00,		// 0101 1011 0000 0000b  ( 85)  U    ..-
	0x56C0,		// 0101 0110 1100 0000b  ( 86)  V    ...-
	0x6B00,		// 0110 1011 0000 0000b  ( 87)  W    .--
	0x96C0,		// 1001 0110 1100 0000b  ( 88)  X    -..-
	0x9AC0,		// 1001 1010 1100 0000b  ( 89)  Y    -.--
	0xA5C0		// 1010 0101 1100 0000b  ( 90)  Z    --..
};

/******************************************************************************
 * @function MorseCodeTable_CodeToAscii
 *
 * @brief morse code to ascii lookup
 *
 * This function will translate a morse code value into an ascii character
 * if valid
 *
 * @param[in]   wCode   morse code
 *
 * @return      ascii character
 *
 *****************************************************************************/
U8 MorseCodeTable_CodeToAscii( U16 wCode )
{
  U8  nIndex, nCode;

  // set the return to 0
  nCode = 0;

  // search
  for( nIndex = 0; nIndex < CODE_TABLE_SIZE; nIndex++ )
  {
    // do we have a match
    if ( awCodeTable[ nIndex ] == wCode )
    {
      // found it - add offset/break out of loop
      nCode = nIndex + CODE_TABLE_OFFSET;
    }
  }

  // return the code
  return( nCode );
}

/******************************************************************************
 * @function MorseCodeTable_AsciiToCode
 *
 * @brief translate ascii to code
 *
 * This function will lookup the ascii code and return the morse code 
 *
 * @param[in]   nCode     ascii code
 *
 * @return      morse code entry
 *
 *****************************************************************************/
U16 MorseCodeTable_AsciiToCode( U8 nCode )
{
  U16 wCode = 0;

  // ensure valid character
  if (( nCode >= CODE_TABLE_OFFSET ) && ( nCode <= 'Z' ))
  {
    // get the code from the table
    wCode = awCodeTable[ nCode ];
  }

  // return the code
  return( wCode );
}

/**@} EOF MorseCodeTable.c */
