// Dialog4.cpp : implementation file
//

#include "stdafx.h"
#include "Imageprocess.h"
#include "Dialog4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog4 dialog


CDialog4::CDialog4(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog4)
	m_x = 50;
	m_y = 50;
	m_n = 5;
	//}}AFX_DATA_INIT
}


void CDialog4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog4)
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDX_Text(pDX, IDC_EDIT3, m_n);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog4, CDialog)
	//{{AFX_MSG_MAP(CDialog4)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog4 message handlers
