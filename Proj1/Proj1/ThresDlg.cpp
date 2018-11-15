// ThresDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Proj1.h"
#include "ThresDlg.h"
#include "afxdialogex.h"


// ThresDlg dialog

IMPLEMENT_DYNAMIC(ThresDlg, CDialog)

ThresDlg::ThresDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_THRESDLG, pParent)
{
	
}

ThresDlg::~ThresDlg()
{
}

void ThresDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ThresDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &ThresDlg::OnEnChangeEdit1)
	ON_STN_CLICKED(IDCLOSE, &ThresDlg::OnStnClickedClose)
	ON_BN_CLICKED(IDOK, &ThresDlg::OnBnClickedOk)
	ON_STN_CLICKED(ID_STATIC, &ThresDlg::OnStnClickedStatic)
END_MESSAGE_MAP()


// ThresDlg message handlers


void ThresDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void ThresDlg::OnStnClickedClose()
{
	// TODO: Add your control notification handler code here
}


void ThresDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CEdit *pBoxone;
	pBoxone = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString str1;
	pBoxone->GetWindowTextA(str1);

	ref1 = atoi(str1);
	if (ref1 <= 0 || ref1 >= 255)
	{
		MessageBox("Please input a value 0~255");
	}

	CDialog::OnOK();
}


void ThresDlg::OnStnClickedStatic()
{
	// TODO: Add your control notification handler code here
}


BOOL ThresDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(ID_STATIC)->SetWindowText("Threshold:");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
