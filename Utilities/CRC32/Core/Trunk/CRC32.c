/******************************************************************************
 * @file CRC32.c
 *
 * @brief CRC32 implementation
 *
 * This file provides the implementation of the CRC calucation module
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup CRC32
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "CRC32/Crc32.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define	CRC_INITIAL		0xFFFFFFFF

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
// define the lookup values
static	const CODE U32	auCrcTbl[ ] =
{
	0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC,
	0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
	0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
	0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
};

/******************************************************************************
 * @function CRC32_GetInitialValue
 *
 * @brief returns the initial value
 *
 * This function returns the initial value for the CRC calc
 *
 * @return      the initial value
 *
 *****************************************************************************/
U32 CRC32_GetInitialValue( void )
{
  // return the initial value
  return( CRC_INITIAL );
}

/******************************************************************************
 * @function CRC32_CalculateByte
 *
 * @brief add a byte value to a CRC
 *
 * This function adds the byte to the current CRC and returns the value
 *
 * @param[in]   uOldCrc   old CRC value
 * @param[in]   nData     data to add to the CRC
 *
 * @return      new calculated CRC
 *
 *****************************************************************************/
U32	CRC32_CalculateByte( U32 uCrc, U8 nData )
{
  // add the data
  uCrc = PGM_RDDWRD( auCrcTbl[(( uCrc ^ nData ) & 0x0000000F )]) ^ ( uCrc >> 4 );
  uCrc = PGM_RDDWRD( auCrcTbl[(( uCrc ^ ( nData >> 4 )) & 0x0000000F )]) ^ ( uCrc >> 4 );
	
	// return the crc
	return ( uCrc );
}

/******************************************************************************
 * @function CRC32_CalculateBlock
 *
 * @brief calculate a CRC for a block of data
 *
 * This function will calculate a CRC for a block of data
 *
 * @param[in]   pnData    pointer to the data
 * @param[in]   uLength   number of bytes to add
 *
 * @return      new calculated CRC
 *
 *****************************************************************************/
U32	CRC32_CalculateBlock( PU8 pnData, U32 uLength )
{
  U32 uIdx;
  U32 uCrc;
  U8  nData;
  
  // set the CRC to it's initial value
  uCrc = CRC_INITIAL;
  
  // for each byte calculate the CRC
  for( uIdx = 0; uIdx < uLength; uIdx++ )
  {
    nData = *( pnData );

    // add the data
    uCrc = PGM_RDDWRD( auCrcTbl[(( uCrc ^ nData ) & 0x0000000F )]) ^ ( uCrc >> 4 );
    uCrc = PGM_RDDWRD( auCrcTbl[(( uCrc ^ ( nData >> 4 )) & 0x0000000F )]) ^ ( uCrc >> 4 );
	}

	// return the crc
	return ( ~uCrc );
}
 
/**@} EOF CRC32.c */
