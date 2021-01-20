//#include "../stdafx.h"
#include "Crc16Tabl.h"

///////////////////////////////////////////////////////////////
// construction/destruction
///////////////////////////////////////////////////////////////
CCrc16Tabl::CCrc16Tabl( WORD wPolynomial, WORD wInitial )
{
	m_wInitial = wInitial;
	GenerateTable( wPolynomial );
}

CCrc16Tabl::~CCrc16Tabl(void)
{
}

///////////////////////////////////////////////////////////////
// operations
///////////////////////////////////////////////////////////////
WORD CCrc16Tabl::GetInitialValue( void )
{
	// return the initial value
	return( m_wInitial );
}

WORD CCrc16Tabl::CrcCalcBlock( BYTE* pnBlock, int iLength )
{
	// calcuate the crc for each byte in block
	WORD wCrc = m_wInitial;
	for ( int i = 0; i < iLength; i++ )
		wCrc = CrcCalcByte( wCrc, *( pnBlock + i ));

	// return the value
	return( wCrc );
}

WORD CCrc16Tabl::CrcCalcByte( WORD wCrc, BYTE nData )
{
	// calculate a crc for one byte
	wCrc = ( wCrc << 4 ) ^ m_awCrcTable[ ( wCrc >> 12 ) ^ ( nData >> 4 ) ];
	wCrc = ( wCrc << 4 ) ^ m_awCrcTable[ ( wCrc >> 12 ) ^ ( nData & 0x0F ) ];
	return( wCrc );
}

///////////////////////////////////////////////////////////////
// implementation
///////////////////////////////////////////////////////////////
void CCrc16Tabl::GenerateTable( WORD wPolynomial )
{
	WORD wTemp = 0;
	m_awCrcTable[ 0 ] = 0;
    for ( int iIdx = 1; iIdx < CRC_TBL_SIZE; iIdx++ )
		m_awCrcTable[ iIdx ] = m_awCrcTable[ iIdx - 1 ] + wPolynomial;
}

