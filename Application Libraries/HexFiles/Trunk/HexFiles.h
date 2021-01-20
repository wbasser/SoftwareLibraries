/*****************************************************************************
//   $Workfile: HexFiles.h $
//    Function: Hex Files Class Declarations
//      Author: Bill Basser
//   $JustDate:  6/12/07 $
//   $Revision: 1.4 $
//
//	This document contains proprietary data and information of Cyber Integration
//  LLC.  It is the exclusive property of Cyber Integration, LLC and
//  will not be disclosed in any form to any party without prior written 
//  permission of Cyber Integration, LLC.	This document may not be reproduced 
//  or further used without the prior written permission of Cyber Integration
//  LLC.
//  
//  Copyright (C) 2004 Cyber Integration, LLC. All Rights Reserved
//
//   $History: HexFiles.h $
 * 
 * *****************  Version 1  *****************
 * User: Wbasser      Date: 6/12/07    Time: 8:50a
 * Created in $/SfwLibraries/C++
 * INITIAL CHECKIN
 * 
 ******************************************************************************/


#if !defined(AFX_HEXFILES_H__ED8C818F_D785_439B_B300_159BCB07C4DA__INCLUDED_)
#define AFX_HEXFILES_H__ED8C818F_D785_439B_B300_159BCB07C4DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"

// define the default size
#define	HF_DEFAULT_SIZE		1

class CHexFiles : public CByteArray  
{
public:
	CHexFiles( int iSize = HF_DEFAULT_SIZE, bool fBigEndian = false );
	virtual ~CHexFiles();

// attributes
public:
	typedef enum _OUTMODE
	{
		OUTMODE_S19 = 0,
		OUTMODE_HEX,
    OUTMODE_BIN,
	} OUTMODE;
	union
	{
		WORD	wValue;
		BYTE	anValue[ 2 ];
	} tWord;
	union
	{
		DWORD	dwValue;
		BYTE	awValue[ 2 ];
		BYTE	anValue[ 4 ];
	} tDblWord;

protected:
	CString	m_strLocal;
	bool	    m_fBigEndian;	
  int     m_iPage;

	// declare the constants
	static const BYTE	m_OutputByteCount	= 16;

// implementation
public:
	bool	ParseFile( CString strFileName, DWORD dwOffset );
	void	ClearArray( void );
	BYTE	GetByte( short iOffset );
	WORD	GetWord( short iOffset );
	DWORD	GetDword( short iOffset );
	void	GenerateFile( CString strFileName, OUTMODE eMode, WORD wAddress );
	void	ComputeCrc16( DWORD wStartAddress, DWORD wEndAddress, DWORD wOffset );
	void	ComputeCrc32( DWORD wStartAddress, DWORD wEndAddress, DWORD wOffset );

protected:
	bool	ParseLine( CString strLine, DWORD dwOffset );
	bool	ParseHex( DWORD dwOffset );
	bool	ParseS19( DWORD dwOffset );
	WORD	GetLineWord( short iOffset );
	BYTE	AscHex( short iOffset );
	BYTE	GetLineByte( short iOffset );
	DWORD	GetLineAddress( short iOffset, short iLength );
	void	GenHex( CStdioFile* pfilData, DWORD dwStartingAddress );
	void	GenS19( CStdioFile* pfilData, DWORD dwStartingAddress );
  void	GenBin( CFile* pfilData, DWORD dwStartingAddress);
  CString MakeLineAddress( DWORD dwValue, int iAddrLength );
	CString	MakeLineWord( WORD wValue );
	CString	MakeLineByte( BYTE nValue );
	CString	HexAsc( BYTE nValue );
};

#endif // !defined(AFX_HEXFILES_H__ED8C818F_D785_439B_B300_159BCB07C4DA__INCLUDED_)
