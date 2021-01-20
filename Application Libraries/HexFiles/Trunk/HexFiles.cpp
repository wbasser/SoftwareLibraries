/*****************************************************************************
//   $Workfile: $
//    Function: Hex Files Class Implementation
//      Author: Bill Basser
//   $JustDate: $
//   $Revision: 1.8 $
//
//	  This document contains proprietary data and information of Cyber Integration
//  LLC.  It is the exclusive property of Cyber Integration, LLC and
//  will not be disclosed in any form to any party without prior written 
//  permission of Cyber Integration, LLC.	This document may not be reproduced 
//  or further used without the prior written permission of Cyber Integration
//  LLC.
//  
//  Copyright (C) 2004 Cyber Integration, LLC. All Rights Reserved
//
//  $History: $
 * 
 ******************************************************************************/

#include "../stdafx.h"
#include "HexFiles.h"
#include "../Crc16Tabl/Crc16Tabl.h"
#include "../Crc32Tabl/Crc32Tabl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// local defines
//////////////////////////////////////////////////////////////////////
#define	HEX_OFFSET_COUNT	  1
#define	HEX_OFFSET_ADDR		3
#define	HEX_OFFSET_TYPE		7
#define	HEX_OFFSET_DATA		9
#define	S19_OFFSET_COUNT	2
#define	S19_OFFSET_ADDR		4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHexFiles::CHexFiles( int iSize,  bool fEndian )
{
	// set the default size/endianess
	SetSize( iSize );
	m_fBigEndian = fEndian;

	// clear the array
	ClearArray( );
}

CHexFiles::~CHexFiles()
{

}

//////////////////////////////////////////////////////////////////////
// implementation
//////////////////////////////////////////////////////////////////////
bool CHexFiles::ParseFile( CString strFileName, DWORD dwOffset )
{
	bool            bStatus = true;
  CStdioFile		    filData;
  CFileException	  excData;

	// clear the array
	ClearArray( );

  // clear t he page
  m_iPage = 0;

	// open the file
	if ( !filData.Open(( LPCTSTR )strFileName, CFile::modeRead | CFile::typeText, &excData ))
	{
    CString strError;
    strError.Format( _T( "Unable to open file - %s!" ), strFileName );
		AfxMessageBox( strError, MB_OK | MB_ICONSTOP );
		bStatus = false;
	}
	else
	{
		// now read the data
		CString strLine;
		while( filData.ReadString( strLine ))
		{
			// parse the line
			if(( bStatus = ParseLine( strLine, dwOffset )) == false ) 
			{
				// report the error
        CString strError;
        strError.Format( _T( "Error on parsing file - %s!" ), strFileName );
				AfxMessageBox( strError, MB_OK | MB_ICONSTOP );
				break;
			}
		}
	}

	// return the status
	return( bStatus );
}

void CHexFiles::GenerateFile(CString strFileName, OUTMODE eMode, WORD wAddress)
{
  CFile		        filBinData;
  CStdioFile      filTxtData;
  CFileException	  excData;
  bool            bBinaryFile = false;
  bool            bFileOpened;

  // build the file name
  int iOffset;
  if ((iOffset = strFileName.Find(_T("."))) != -1)
  {
    // strip off the extension
    strFileName = strFileName.Left(iOffset);
  }
   
  switch (eMode)
  {
  case OUTMODE_S19:
    strFileName += _T(".s19");
    break;

  case OUTMODE_HEX:
    strFileName += _T(".hex");
    break;

  case OUTMODE_BIN:
    strFileName += _T(".bin");
    bBinaryFile = true;
    break;

  default:
    break;
  }

  // open the file
  if (bBinaryFile)
  {
    // open the file
    bFileOpened = filBinData.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite, &excData);
  }
  else
  {
    // open 
    bFileOpened = filTxtData.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &excData);
  }

	if (!bFileOpened )
	{
		AfxMessageBox( _T( "Unable to open file!" ), MB_OK | MB_ICONSTOP );
	}
	else
	{
    switch (eMode)
    {
    case OUTMODE_S19:
      // generate and close
      GenS19(&filTxtData, wAddress);
      filTxtData.Close();
      break;

    case OUTMODE_HEX:
      // generate and close
      GenHex(&filTxtData, wAddress);
      filTxtData.Close();
      break;

    case OUTMODE_BIN:
      // generate and close
      GenBin(&filBinData, wAddress);
      filBinData.Close();
      break;

    default:
      break;
    }
	}
}

bool CHexFiles::ParseLine( CString strLine, DWORD dwOffset )
{
	bool	fReturn = true;

	// copy the string
	m_strLocal = strLine;

	// parse the line
	switch ( m_strLocal[ 0 ])
	{
	case ':' :
		fReturn = ParseHex( dwOffset );
		break;

	case 'S' :
		fReturn = ParseS19( dwOffset );
		break;

	default :
		break;
	}

	// return the status
	return fReturn;
}

void CHexFiles::ClearArray( )
{
	// set all bytes to 0xFF
	for ( int iAddress = 0; iAddress < GetSize( ); iAddress++ )
		SetAt( iAddress, 0xFF );
}

BYTE CHexFiles::GetByte( short iOffset )
{
	// get the value
	return( GetAt( iOffset ));
}

WORD CHexFiles::GetWord( short iOffset )
{
	tWord.anValue[0] = GetAt(( m_fBigEndian ) ? iOffset + 1 : iOffset + 0 );
	tWord.anValue[1] = GetAt(( m_fBigEndian ) ? iOffset + 0 : iOffset + 1 );
	return( tWord.wValue );
}

DWORD CHexFiles::GetDword( short iOffset )
{
	tDblWord.anValue[0] = GetAt(( m_fBigEndian ) ? iOffset + 3 : iOffset + 0 );
	tDblWord.anValue[1] = GetAt(( m_fBigEndian ) ? iOffset + 2 : iOffset + 1 );
	tDblWord.anValue[2] = GetAt(( m_fBigEndian ) ? iOffset + 1 : iOffset + 2 );
	tDblWord.anValue[3] = GetAt(( m_fBigEndian ) ? iOffset + 0 : iOffset + 3 );
	return( tDblWord.dwValue );
}

void CHexFiles::ComputeCrc16( DWORD dwStartAddress, DWORD dwEndAddress, DWORD dwOffset )
{
	CCrc16Tabl* ptCrc = new CCrc16Tabl;
	WORD	wCrc = ptCrc->GetInitialValue( );

	// for each byte
	for ( DWORD dwIndex = dwStartAddress; dwIndex < dwEndAddress; dwIndex++ )
	{
		// compute the crc
		wCrc = ptCrc->CrcCalcByte( wCrc, GetAt( dwIndex));
	}

	// now store it
	SetAtGrow(( dwOffset + 0 ), ( wCrc >> 8 ) && 0xFF );
	SetAtGrow(( dwOffset + 1 ), ( wCrc & 0xFF ));
}

void CHexFiles::ComputeCrc32( DWORD dwStartAddress, DWORD dwEndAddress, DWORD dwOffset )
{
	// create the CRC/get initial value
	CCrc32Tabl* ptCrc = new CCrc32Tabl;
	DWORD	dwCrc = ptCrc->GetInitialValue( );

	// for each byte
	for ( DWORD dwIndex = dwStartAddress; dwIndex < dwEndAddress; dwIndex++ )
	{
		// compute the crc
		dwCrc = ptCrc->CrcCalcByte( dwCrc, GetAt( dwIndex));
	}

  // complement it
  dwCrc = ~dwCrc;

	// now store it
  SetAtGrow((dwOffset + 0), (dwCrc & 0xFF));
  SetAtGrow((dwOffset + 1), (dwCrc >> 8) & 0xFF);
  SetAtGrow((dwOffset + 2), (dwCrc >> 16) & 0xFF);
  SetAtGrow((dwOffset + 3), (dwCrc >> 24) & 0xFF);
}

//////////////////////////////////////////////////////////////////////
// local parsers
//////////////////////////////////////////////////////////////////////
bool CHexFiles::ParseHex( DWORD dwOffset )
{
	int		iArrayIndex;
	bool	fReturn = true;
  DWORD dwAddress;

	// get the byte count
	short iByteCount = GetLineByte( HEX_OFFSET_COUNT );

	// get the type
	short iType = GetLineByte( HEX_OFFSET_TYPE );

	// get the address
	dwAddress = GetLineWord( HEX_OFFSET_ADDR );

  // process the appropriate types
	switch ( iType )
	{
	case 0x00:
    // calculate and check for a valid address
    iArrayIndex = m_iPage + dwAddress - dwOffset;
    if (iArrayIndex < 0)
    {
      // error
      AfxMessageBox(_T("Illegal offset for this file!"), MB_ICONSTOP | MB_OK);
      fReturn = false;
    }
    else
    {
      // now load the data
      for (short iOffset = 0; iOffset < iByteCount; iOffset++)
      {
        // read a byte and stuff it
        BYTE nData = GetLineByte(HEX_OFFSET_DATA + (iOffset * 2));
        SetAtGrow(iArrayIndex + iOffset, nData);
      }
    }
		break;

	case 0x01:
		break;

	case 0x02:
		m_iPage = GetLineWord( HEX_OFFSET_DATA ) * 16;
		break;

	case 0x03:
		break;

	case 0x04:
		break;

	case 0x05:
		break;

	default :
		break;
	}

	// return the status
	return fReturn;
}

bool CHexFiles::ParseS19( DWORD dwOffset )
{
	DWORD	dwAddress;
	int		iAddrLength;
	bool	fReturn = true;
	CString	strType;

	strType = m_strLocal.Mid( 1, 1 );
	if ( strType.Compare( _T( "1" )) == 0 )
		iAddrLength = 2;
	else if ( strType.Compare( _T( "2" )) == 0 )
		iAddrLength = 3;
	else if ( strType.Compare( _T( "3" )) == 0 )
		iAddrLength = 4;
	else
		iAddrLength = 0;

	// test for a valid type
	if ( iAddrLength != 0 )
	{
		// get the byte count
		short iByteCount = GetLineByte( S19_OFFSET_COUNT ) - 1;

		// get a two byte address
		dwAddress = GetLineAddress( S19_OFFSET_ADDR, iAddrLength ) - dwOffset;
		iByteCount -= iAddrLength;

		// now read the data/check length
		for( short iOffset = 0; iOffset < iByteCount; iOffset++ )
		{
			BYTE nData = GetLineByte( S19_OFFSET_ADDR + ( iAddrLength * 2 ) + ( iOffset * 2 ));
			SetAtGrow( dwAddress + iOffset, nData );
		}
	}

	// return the status
	return fReturn;
}

void CHexFiles::GenS19(CStdioFile* pfilData, DWORD dwStartingAddress )
{
	BYTE	    nChecksum;
	BYTE	    nData;
	int		  iAddrLength;
	int		  iByte;
	int		  iActAddr;
	CString	strTemp;
	CString strEndRec;

	// determine address length
	if ( GetCount( ) < 65536 )
	{
		iAddrLength = 2;
		strTemp = _T(  "1" );
		strEndRec = _T( "9" );
	}
	else if ( GetCount( ) < 16777216 )
	{
		iAddrLength = 3;
		strTemp = _T(  "2" );
		strEndRec = _T( "8" );
	}
	else
	{
		iAddrLength = 4;
		strTemp = _T(  "3" );
		strEndRec = _T( "7" );
	}

	// for each byte in the array
	for ( int iAddress = 0; iAddress < GetCount( ); iAddress += m_OutputByteCount )
	{
		// start the checksum process
		nChecksum = m_OutputByteCount + iAddrLength + 1;

		// compute the address
		iActAddr = dwStartingAddress + iAddress;

		// outpput the address
		for ( iByte = 0; iByte < iAddrLength; iByte++ )
		{
			nChecksum += (( iActAddr & ( 0xFF << ( iByte * 8 ))) >> ( iByte * 8 ));
		}

		// output the start character
		m_strLocal = _T( "S" );
		m_strLocal += strTemp;
		m_strLocal += MakeLineByte( m_OutputByteCount + iAddrLength + 1 );
		m_strLocal += MakeLineAddress( iActAddr, iAddrLength );

		// now for each byte
		for ( iByte = 0; iByte < m_OutputByteCount; iByte++ )
		{
			nData = GetAt( iAddress + iByte );
			nChecksum += nData;
			m_strLocal += MakeLineByte( nData );
		}

		// 1's complement the checksum/add it
		nChecksum = ~nChecksum;
		m_strLocal += MakeLineByte( nChecksum );
		m_strLocal += _T( "\n" );

		// write it
		pfilData->Write( m_strLocal, m_strLocal.GetLength());
	}

	// output the start character/byte count/address
	nChecksum = iAddrLength + 1;
	nChecksum = ~nChecksum;
	m_strLocal = _T( "S" );
	m_strLocal += strEndRec;
	m_strLocal += MakeLineByte( iAddrLength + 1 );
	for ( iByte = 0; iByte < iAddrLength; iByte++ )
		m_strLocal += MakeLineByte( 0 );
	m_strLocal += MakeLineByte( nChecksum );
	m_strLocal += _T( "\n" );

	// write it
  pfilData->Write(m_strLocal, m_strLocal.GetLength());
}

void CHexFiles::GenHex(CStdioFile* pfilData, DWORD dwStartingAddress )
{
  BYTE	      nChecksum, nNumOutputBytes;
  DWORD     dwByteIdx;
  DWORD     dwCurPage;
  WORD      wAddress;
  DWORD     dwPageNum;

  // set the page break
  dwCurPage = 0x10000 - dwStartingAddress;
  dwPageNum = 0x10000;

  // now for this block
  for ( dwByteIdx = 0; dwByteIdx < ( DWORD )GetCount( ); dwByteIdx += m_OutputByteCount )
  {
    // check for a page record
    if ( dwByteIdx == dwCurPage )
    {
      // compute the addresws
      wAddress = ( WORD )( dwPageNum >> 4 );

      // generate a block record
      nChecksum = 4;
      nChecksum += ( BYTE )(( wAddress >> 8 ) & 0xFF );
      nChecksum += ( BYTE )( wAddress & 0xFF );

      // output the start character/count/address/type
  		  m_strLocal = _T( ":" );
	  	  m_strLocal += MakeLineByte( 2 );
		  m_strLocal += MakeLineWord( 0 );
		  m_strLocal += MakeLineByte( 2 );

      // output the block address
      m_strLocal += MakeLineWord( wAddress );

      // 2's complement the checksum/add it
		  nChecksum = ~nChecksum;
		  nChecksum++;
		  m_strLocal += MakeLineByte( nChecksum );
      m_strLocal += _T( "\n" );

		  // write it
      pfilData->Write(m_strLocal, m_strLocal.GetLength());

      // now adjust the page
      dwCurPage += 0x10000;
      dwPageNum += 0x10000;
    }

    // compute the number of output bytes
    nNumOutputBytes = ( BYTE )(__min( m_OutputByteCount, ( GetCount( ) - dwByteIdx )));

		// start the checksum process
		nChecksum = nNumOutputBytes;
		nChecksum += ( BYTE )((( dwStartingAddress + dwByteIdx ) >> 8 ) & 0xFF );
		nChecksum += ( BYTE)(( dwStartingAddress + dwByteIdx ) & 0xFF );

		// output the start character/byte count/address
		m_strLocal = _T( ":" );
		m_strLocal += MakeLineByte( nNumOutputBytes );
		m_strLocal += MakeLineWord(( WORD )( dwStartingAddress + dwByteIdx ) & 0xFFFF );
		m_strLocal += MakeLineByte( 0 );

		// now for each byte
		for ( int iByte = 0; iByte < nNumOutputBytes; iByte++ )
		{
			nChecksum += GetAt( dwByteIdx + iByte );
			m_strLocal += MakeLineByte( GetAt( dwByteIdx + iByte ));
		}

		// 2's complement the checksum/add it
		nChecksum = ~nChecksum;
		nChecksum++;
		m_strLocal += MakeLineByte( nChecksum );
    m_strLocal += _T( "\n" );

		// write it
    pfilData->Write(m_strLocal, m_strLocal.GetLength());
  }

	// output the end of file
	m_strLocal = _T( ":" );
  m_strLocal += MakeLineByte(0);
  m_strLocal += MakeLineWord(0);
	m_strLocal += MakeLineByte( 1 );
	m_strLocal += MakeLineByte( 0xFF );

	// write it
  pfilData->Write(m_strLocal, m_strLocal.GetLength());
}

void CHexFiles::GenBin(CFile* pfilData, DWORD dwStartingAddress)
{
  DWORD     dwByteIdx;
  BYTE      nData;

  // now for this block
  for (dwByteIdx = 0; dwByteIdx < (DWORD)GetCount(); dwByteIdx++)
  {
    // write it
    nData = GetAt(dwByteIdx);
    pfilData->Write(&nData, 1);
  }
}

//////////////////////////////////////////////////////////////////////
// conversion utilities
//////////////////////////////////////////////////////////////////////
DWORD CHexFiles::GetLineAddress( short iOffset, short iLength )
{
	DWORD dwValue = 0;
	while ( --iLength >= 0 )
	{
		dwValue |= ( GetLineByte( iOffset ) << ( iLength * 8 ));
		iOffset += 2;
	}

	return( dwValue );
}

WORD CHexFiles::GetLineWord( short iOffset )
{
	// get the msg
	WORD wValue = GetLineByte( iOffset ) << 8;
	wValue |= GetLineByte( iOffset + 2 );

	// return it
	return( wValue );
}

BYTE CHexFiles::GetLineByte( short iOffset )
{
	// get the most significant nibble
	BYTE nValue = AscHex( iOffset ) << 4;

	// get the least significan tnibble
	nValue |= AscHex( iOffset + 1 );

	// return it
	return( nValue );
}

BYTE CHexFiles::AscHex( short iOffset )
{
	// convert to binary
	BYTE nValue = m_strLocal.GetAt( iOffset ) - '0';

	// test for A-F
	if ( nValue > 9 )
	{
		// adjust it
		nValue -=( 'A' - '9' - 1 );
	}

	// return it
	return( nValue );
}

CString CHexFiles::MakeLineAddress( DWORD dwValue, int iAddrLength )
{
	BYTE	nValue;
	CString strTemp;
	strTemp = _T( "" );
	while( --iAddrLength >= 0 )
	{
		nValue = ( BYTE )( dwValue >> ( iAddrLength * 8 ) & 0xFF );
		strTemp += MakeLineByte( nValue );
	}

	return( strTemp );
}

CString CHexFiles::MakeLineWord( WORD wValue )
{
	CString strTemp;
	strTemp = MakeLineByte( wValue >> 8 );
	strTemp += MakeLineByte( wValue & 0xFF );
	return( strTemp );
}

CString CHexFiles::MakeLineByte( BYTE nValue )
{
	CString strTemp;
	strTemp = HexAsc( nValue >> 4 );
	strTemp += HexAsc( nValue & 0x0F );
	return( strTemp );
}

CString CHexFiles::HexAsc( BYTE nValue )
{
	CString strTemp;
	strTemp.Format( _T( "%1X" ), nValue );
	return( strTemp );
}
