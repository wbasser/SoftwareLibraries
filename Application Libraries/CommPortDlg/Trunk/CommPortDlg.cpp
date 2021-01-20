#include "StdAfx.h"
#include ".\commportdlg.h"

// define the setrect macro
#define	SETRECT( rect, l, t, r, b ) \
	rect.left = l; \
	rect.top = t; \
	rect.right = r; \
	rect.bottom = b;

// construction/destruction
CCommPortDlg::CCommPortDlg( BOOL fBaudRate )
	: m_hLocal(NULL)
{
	// copy the baud rate flag
	m_fBaudRate = fBaudRate;

	// clear the dialog items
	m_iNumDlgItems = 0;

	// initialize the dialog items
	CRect rect( 129, 7, 50, 20 );
	m_dlgItems[ m_iNumDlgItems++ ].Initialize( CDialogItem::BUTTON, IDOK,
		                        WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_DEFPUSHBUTTON,
								&rect, _T( "OK" ));
	SETRECT( rect, 129, 30, 50, 20 );
	m_dlgItems[ m_iNumDlgItems++ ].Initialize( CDialogItem::BUTTON, IDCANCEL, 
		                        WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
		                        &rect, _T( "Cancel" ));
	SETRECT( rect, 10, 5, 20, 10 );
	m_dlgItems[ m_iNumDlgItems++ ].Initialize( CDialogItem::STATICTEXT, IDC_STATIC,
		                        WS_CHILD | WS_VISIBLE | SS_LEFT,
								&rect, _T( "Port" ));
	SETRECT( rect, 10, 17, 90, 60 );
	m_dlgItems[ m_iNumDlgItems++ ].Initialize( CDialogItem::COMBOBOX, IDC_CBO_PORTS, 
								WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST ,
		                        &rect, NULL );

	// determine if baud rate is requested
	if ( fBaudRate )
	{
		SETRECT( rect, 10, 40, 20, 10 );
		m_dlgItems[ m_iNumDlgItems++ ].Initialize( CDialogItem::STATICTEXT, IDC_STATIC,
									WS_CHILD | WS_VISIBLE | SS_LEFT,
									&rect, _T( "Baud Rate" ));
		SETRECT( rect, 10, 52, 90, 60 );
		m_dlgItems[ m_iNumDlgItems++ ].Initialize( CDialogItem::COMBOBOX, IDC_CBO_BAUD, 
									WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST ,
									&rect, NULL );
	}	

	// initialize the dialog template structure
	m_dlgTemplate.cx = 186;
	m_dlgTemplate.cy = 90;
	m_dlgTemplate.x = 0;
	m_dlgTemplate.y = 0;
	m_dlgTemplate.style = WS_CAPTION | WS_VISIBLE | WS_DLGFRAME | WS_POPUP | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT;
	m_dlgTemplate.dwExtendedStyle = 0;
	m_dlgTemplate.cdit = m_iNumDlgItems;

	// create the caption/font names
	WCHAR	szBoxCaption[ ] = L"Select communication parameters";
	WCHAR	szFontName[ ]	= L"MS SHELL DLG";

	// allocate memory
	TRY
	{
		// compute size
		int iBufferSize = sizeof( DLGTEMPLATE ) + ( 2 * sizeof( WORD )) + sizeof( szBoxCaption );
		iBufferSize += sizeof( WORD ) + sizeof( szFontName );
		iBufferSize = ( iBufferSize + 3 ) & ~3;

		// for each item
		for ( int i = 0; i < m_iNumDlgItems; i++ )
		{
			// get the item length
			int iItemLength = sizeof( DLGITEMTEMPLATE ) + 3 * sizeof( WORD );
			iItemLength += ( m_dlgItems[ i ].m_strCaption.GetLength( ) + 1 ) * sizeof( WCHAR );

			// last item does not need extra bytes
			if ( i != ( m_iNumDlgItems - 1 ))
				iItemLength = ( iItemLength + 3 ) & ~3;

			// add it to buffer size
			iBufferSize += iItemLength;
		}

		// allocate memory
		m_hLocal = LocalAlloc( LHND, iBufferSize );
		if ( m_hLocal == NULL )
			AfxThrowMemoryException( );

		// lock it
		m_pBuffer = ( BYTE* )LocalLock( m_hLocal );
		if ( m_pBuffer == NULL )
		{
			LocalFree( m_hLocal );
			AfxThrowMemoryException( );
		}

		// copy the data
		BYTE* pDest = m_pBuffer;
		memcpy( pDest, &m_dlgTemplate, sizeof( DLGTEMPLATE ));
		pDest += sizeof( DLGTEMPLATE );

		// no menu
		*( WORD* )pDest = 0;
		*( WORD* )( pDest + sizeof( WORD )) = 0;
		pDest += 2 * sizeof( WORD );

		// copy the caption/font
		memcpy( pDest, szBoxCaption, sizeof( szBoxCaption ));
		pDest += sizeof( szBoxCaption );

		// copy the font/size
		*( WORD* )pDest = 8;
		pDest += sizeof( WORD );
		memcpy( pDest, szFontName, sizeof( szFontName ));
		pDest += sizeof( szFontName );

		// now copy the item info
		for ( int i = 0; i < m_iNumDlgItems; i++ )
		{
			// DWORD align
			pDest = ( BYTE* )((( DWORD_PTR )pDest + 3 ) & ~3 );

			// copy the template
			memcpy( pDest, ( void* )&m_dlgItems[ i ].m_dlgItemTemplate, sizeof ( DLGITEMTEMPLATE ));
			pDest += sizeof( DLGITEMTEMPLATE );

			// indicate an atom value/control type
			*( WORD* )pDest = 0xFFFF;
			pDest += sizeof( WORD );
			*( WORD* )pDest = m_dlgItems[ i ].m_eCtlType;
			pDest += sizeof( WORD );

			// copy the caption 
			WCHAR*	pchCaption;
			int		iChars, iActualChars;
			iChars = m_dlgItems[ i ].m_strCaption.GetLength( ) + 1;
			pchCaption = new WCHAR[ iChars ];
			iActualChars = MultiByteToWideChar( CP_ACP, 0, m_dlgItems[ i ].m_strCaption, -1, pchCaption, iChars );
			ASSERT( iActualChars > 0 );
			memcpy( pDest, pchCaption, iActualChars * sizeof( WCHAR ));
			pDest += iActualChars* sizeof( WCHAR );
			delete [ ] pchCaption;

			// allocate space for byte in data for control
			*( WORD* )pDest = 0;
			pDest += sizeof( WORD );
		}

		// test for overrun
		ASSERT(( pDest - m_pBuffer ) == iBufferSize );
	}

	// catch the memory exception, report it
	CATCH( CMemoryException, e )
	{
		MessageBox( NULL, _T( "Memory allocation for dialog template failed." ), MB_ICONEXCLAMATION | MB_OK );
	}
	END_CATCH
}

CCommPortDlg::~CCommPortDlg(void)
{
	// unlock/free
	if( m_hLocal != NULL )
	{
		LocalUnlock( m_hLocal );
		LocalFree( m_hLocal );
	}
}

void CCommPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBO_PORTS, m_cboPorts);
	if ( m_fBaudRate )
	{
		DDX_Control(pDX, IDC_CBO_BAUD, m_cboBaud );
	}
}

BEGIN_MESSAGE_MAP(CCommPortDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CCommPortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// do we have a baud rate
	if ( m_fBaudRate )
	{
		// set the baud rate
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("1200" )), 1200 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("2400" )), 2400 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("4800" )), 4800 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("9600" )), 9600 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("19200" )), 19200 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("38400" )), 38400 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("57600" )), 57600 );
		m_cboBaud.SetItemData( m_cboBaud.AddString( _T("115200" )), 115200 );

		// select the appropriate baud rate
		CString strBaud;
		strBaud.Format( _T( "%d" ), m_iBaudRate );
		m_cboBaud.SetCurSel( m_cboBaud.FindString( 0, strBaud ));
	}

	// enumerate the serial ports
	EnumerateSerialPorts( );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCommPortDlg::EnumerateSerialPorts( void )
{
	UINT		iCurSel = 0;
	int			iIndex;
	CString		strTemp;

	//Use QueryDosDevice to look for all devices of the form COMx. This is a better
	//solution as it means that no ports have to be opened at all.
	TCHAR szDevices[ 65535 ];
	DWORD dwChars = QueryDosDevice( NULL, szDevices, 65535 );
	if ( dwChars )
	{
		int i = 0;

		for (;;)
		{
			//Get the current device name
			TCHAR* pszCurrentDevice = &szDevices[ i ];

			//If it looks like "COMX" then
			//add it to the array which will be returned
			int nLen = ( int )_tcslen(pszCurrentDevice );
			if ( nLen > 3 && _tcsnicmp(pszCurrentDevice, _T( "COM" ), 3 ) == 0 )
			{
				//Work out the port number
				int nPort = _ttoi( &pszCurrentDevice[ 3 ] );
				if ( nPort != 0 )
				{
					strTemp.Format( _T( "Comm%d" ), nPort );
					iIndex = m_cboPorts.AddString(( LPCTSTR )strTemp );
					m_cboPorts.SetItemData( iIndex, nPort );
					if ( m_iCommPort == nPort )
						iCurSel = iIndex;
				}
			}

			// Go to next NULL character
			while(szDevices[ i ] != _T( '\0' ))
				i++;

			// Bump pointer to the next string
			i++;

			// The list is double-NULL terminated, so if the character is
			// now NULL, we're at the end
			if ( szDevices[ i ] == _T( '\0' ))
				break;
		}

		// set the current selected
		m_cboPorts.SetCurSel( iCurSel );
	}
	else
		TRACE(_T( "Failed in call to QueryDosDevice, GetLastError:%d\n" ), GetLastError( ));
}

INT_PTR CCommPortDlg::DoIt( void )
{
	// create the dialog
	InitModalIndirect(( DLGTEMPLATE* )m_pBuffer );
	return( DoModal( ));
}

void CCommPortDlg::OnBnClickedOk()
{
	// get the current selected value
	m_iCommPort = ( UINT )m_cboPorts.GetItemData( m_cboPorts.GetCurSel( ));

	// get the baud rate
	if ( m_fBaudRate )
	{
		m_iBaudRate = ( UINT )m_cboBaud.GetItemData( m_cboBaud.GetCurSel( ));
	}

	// call default
	OnOK();
}

// define the dialog item class
CDialogItem::CDialogItem( CDialogItem::CTLTYPE eCtlType )
{
	// copy the control type
	m_eCtlType = eCtlType;
}

// initialization
void CDialogItem::Initialize( CDialogItem::CTLTYPE eCtlType, UINT nID, DWORD dwStyle, CRect* pRect, LPCTSTR lpszCaption )
{
	// fill in the type, location and size of the control
	m_eCtlType = eCtlType;
	if ( pRect != NULL )
	{
		// copy the size
		m_dlgItemTemplate.x		= ( short )pRect->left;
		m_dlgItemTemplate.y		= ( short )pRect->top;
		m_dlgItemTemplate.cx	= ( short )pRect->right;
		m_dlgItemTemplate.cy	= ( short )pRect->bottom;
	}
	else
	{
		// copy the size
		m_dlgItemTemplate.x		= 0;
		m_dlgItemTemplate.y		= 0;
		m_dlgItemTemplate.cx	= 10;
		m_dlgItemTemplate.cy	= 10;
	}

	// set the style and id
	m_dlgItemTemplate.dwExtendedStyle	= 0;
	m_dlgItemTemplate.id				= nID;

	// force the styles
	m_dlgItemTemplate.style = dwStyle;

	// set the caption
	m_strCaption = ( lpszCaption != NULL ) ? lpszCaption : _T( "" );
}
