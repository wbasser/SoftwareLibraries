// GlobalFunctions.h: Schnittstelle für die Klasse CGlobalFunctions.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALFUNCTIONS_H__44AC5D38_F5B0_4E8E_B50E_4138D0E91536__INCLUDED_)
#define AFX_GLOBALFUNCTIONS_H__44AC5D38_F5B0_4E8E_B50E_4138D0E91536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlobalFunctions : public CObject  
{
public:
	CGlobalFunctions();
	virtual ~CGlobalFunctions();

public:
	static CString GetFileVersionX();
	static CString GetProductVersionX();
	static CString GetVersionInfo(HMODULE hLib, CString csEntry);
	static CString FormatVersion(CString cs);

private:
	static CString m_csFileVersion;
	static CString m_csProductVersion;

};

#endif // !defined(AFX_GLOBALFUNCTIONS_H__44AC5D38_F5B0_4E8E_B50E_4138D0E91536__INCLUDED_)
