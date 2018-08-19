/******************************************************************************
 * @file CRC16.c
 *
 * @brief CRC16 implementation
 *
 * This file provides the implementation of the CRC calucation module
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
 * $Log: $
 * 
 *
 * \addtogroup CRC16
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "CRC16/Crc16.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define	CRC_INITIAL		0xFFFF

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
// define the lookup values
static	const CODE U16	awCrcTbl[ ] =
{
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF
};

/******************************************************************************
 * @function CRC16_GetInitialValue
 *
 * @brief returns the initial value
 *
 * This function returns the initial value for the CRC calc
 *
 * @return      the initial value
 *
 *****************************************************************************/
U16 CRC16_GetInitialValue( void )
{
  // return the initial value
  return( CRC_INITIAL );
}

/******************************************************************************
 * @function CRC16_CalculateByte
 *
 * @brief add a byte value to a CRC
 *
 * This function adds the byte to the current CRC and returns the value
 *
 * @param[in]   wOldCrc   old CRC value
 * @param[in]   nData     data to add to the CRC
 *
 * @return      new calculated CRC
 *
 *****************************************************************************/
U16	CRC16_CalculateByte( U16 wOldCrc, U8 nData )
{
  // add the data
	wOldCrc = ( wOldCrc << 4 ) ^ PGM_RDWORD( awCrcTbl[ ( wOldCrc >> 12 ) ^ ( nData >> 4 ) ] );
	wOldCrc = ( wOldCrc << 4 ) ^ PGM_RDWORD( awCrcTbl[ ( wOldCrc >> 12 ) ^ ( nData & 0x0F ) ] );
	
	// return the crc
	return ( wOldCrc );
}

/******************************************************************************
 * @function CRC16_CalculateBlock
 *
 * @brief calculate a CRC for a block of data
 *
 * This function will calculate a CRC for a block of data
 *
 * @param[in]   pnData    pointer to the data
 * @param[in]   wLength   number of bytes to add
 *
 * @return      new calculated CRC
 *
 *****************************************************************************/
U16	CRC16_CalculateBlock( PU8 pnData, U16 wLength )
{
  U16 wCrc, wIdx;
  U8  nData;
  
  // set the CRC to it's initial value
  wCrc = CRC_INITIAL;
  
  // for each byte calculate the CRC
  for( wIdx = 0; wIdx < wLength; wIdx++ )
  {
    nData = *( pnData + wIdx );
    wCrc = ( wCrc << 4 ) ^ PGM_RDWORD( awCrcTbl[ ( wCrc >> 12 ) ^ ( nData >> 4 ) ] );
    wCrc = ( wCrc << 4 ) ^ PGM_RDWORD( awCrcTbl[ ( wCrc >> 12 ) ^ ( nData & 0x0F ) ] );
  }
	
	// return the crc
	return ( wCrc );
}
 
/**@} EOF CRC16.c */
