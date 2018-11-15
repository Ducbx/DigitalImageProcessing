// Dialog5.cpp : implementation file
//

#include "stdafx.h"
#include "Imageprocess.h"
#include "Dialog5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog5 dialog


CDialog5::CDialog5(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog5)
	m_x = 50;
	m_y = 50;
	m_x1 = 50;
	m_y1 = 50;
	//}}AFX_DATA_INIT
}


void CDialog5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog5)
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDX_Text(pDX, IDC_EDIT3, m_x1);
	DDX_Text(pDX, IDC_EDIT4, m_y1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog5, CDialog)
	//{{AFX_MSG_MAP(CDialog5)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog5 message handlers
