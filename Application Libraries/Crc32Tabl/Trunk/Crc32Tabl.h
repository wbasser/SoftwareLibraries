#pragma once

class CCrc32Tabl
{

// construction/destruction
public:
	CCrc32Tabl( void );
	~CCrc32Tabl(void);

// attributes

// operation
public:
	DWORD	GetInitialValue( void );
	DWORD	CrcCalcBlock( BYTE* pnBuffer, int iLength );
	DWORD	CrcCalcByte( DWORD wCrc,  BYTE nData );
};
