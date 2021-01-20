#include "../stdafx.h"
#include "Crc32Tabl.h"


// CRC 32 lookup table
static	const DWORD		m_adwCrcTable[ ] =
{
	0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC,
	0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
	0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
	0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
};

///////////////////////////////////////////////////////////////
// construction/destruction
///////////////////////////////////////////////////////////////
CCrc32Tabl::CCrc32Tabl( void )
{
}

CCrc32Tabl::~CCrc32Tabl(void)
{
}

///////////////////////////////////////////////////////////////
// operations
///////////////////////////////////////////////////////////////
DWORD CCrc32Tabl::GetInitialValue( void )
{
	// return the initial value
	return( 0xFFFFFFFF );
}

DWORD CCrc32Tabl::CrcCalcBlock( BYTE* pnBlock, int iLength )
{
	// calcuate the crc for each byte in block
	DWORD dwCrc = 0xFFFFFFFF;
	for ( int i = 0; i < iLength; i++ )
		dwCrc = CrcCalcByte( dwCrc, *( pnBlock + i ));

	// return the value
	return( ~dwCrc );
}

DWORD CCrc32Tabl::CrcCalcByte( DWORD dwCrc, BYTE nData )
{
	// calculate a crc for one byte
	dwCrc = m_adwCrcTable[(( dwCrc ^ nData ) & 0x0000000F )] ^ (dwCrc >> 4);
  dwCrc = m_adwCrcTable[((dwCrc ^ ( nData >> 4)) & 0x0000000F)] ^ (dwCrc >> 4);

  // return the CRC
  return(dwCrc);
}

