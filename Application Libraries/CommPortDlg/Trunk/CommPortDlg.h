#pragma once

// define the number of controls in this dialog
#define	NUM_DLG_ITEMS	6
#define	IDC_CBO_PORTS	1000
#define	IDC_CBO_BAUD	1001

// define the dialog item class
class CDialogItem
{
public:
// enum the control types
	typedef enum
	{
		BUTTON = 0x0080,
		EDITCONTROL,
		STATICTEXT,
		LISTBOX,
		SCROLLBAR,
		COMBOBOX
	} CTLTYPE;

// construction/destruction
public:
	CDialogItem( CTLTYPE eType );
	CDialogItem( ) { };

// attributes
public:
	DLGITEMTEMPLATE	m_dlgItemTemplate;
	CTLTYPE			m_eCtlType;
	CString			m_strCaption;
	HLOCAL			m_hLocal;

// implementation
public:
	void	Initialize( CTLTYPE eType, UINT nID, DWORD dwStyle, CRect* pRect = NULL, LPCTSTR pszcaption = NULL );
};

class CCommPortDlg : CDialog
{
// construction/destruction
public:
	CCommPortDlg( BOOL fBaudRate = TRUE );
	~CCommPortDlg( void );

// attributes
public:
	int				m_iCommPort;
	int				m_iBaudRate;
	DLGTEMPLATE		m_dlgTemplate;
	CDialogItem		m_dlgItems[ NUM_DLG_ITEMS ];

protected:
	HLOCAL			m_hLocal;
	BYTE*			m_pBuffer;
	CComboBox		m_cboPorts;
	CComboBox		m_cboBaud;
	int				m_iNumDlgItems;
	bool			m_fBaudRate;

// operations
public:
	INT_PTR			DoIt( void );

// implementation
protected:
	void	EnumerateSerialPorts( void );

// overrides
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// messages
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
};
