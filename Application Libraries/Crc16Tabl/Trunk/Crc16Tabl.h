#pragma once

#include "../stdafx.h"

// define the length of the lookup table
#define	CRC_TBL_SIZE	16
class CCrc16Tabl
{

// construction/destruction
public:
	CCrc16Tabl( WORD wPolynomial = 0x1021, WORD wInitial = 0xFFFF );
	~CCrc16Tabl(void);

// attributes
protected:
	WORD	m_wInitial;
	WORD	m_awCrcTable[ CRC_TBL_SIZE ];

// operation
public:
	WORD	GetInitialValue( void );
	WORD	CrcCalcBlock( BYTE* pnBuffer, int iLength );
	WORD	CrcCalcByte( WORD wCrc,  BYTE nData );

// implementation
protected:
	void	GenerateTable( WORD wPolynomial );
};
