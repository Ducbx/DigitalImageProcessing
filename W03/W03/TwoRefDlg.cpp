// TwoRefDlg.cpp : implementation file
//

#include "stdafx.h"
#include "W03.h"
#include "TwoRefDlg.h"
#include "afxdialogex.h"


// CTwoRefDlg dialog

IMPLEMENT_DYNAMIC(CTwoRefDlg, CDialog)

CTwoRefDlg::CTwoRefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TWOREFDLG, pParent)
{

}

CTwoRefDlg::~CTwoRefDlg()
{
}

void CTwoRefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTwoRefDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CTwoRefDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CTwoRefDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, &CTwoRefDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CTwoRefDlg message handlers


void CTwoRefDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CTwoRefDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CEdit *pBoxone;
	CEdit *pBoxone2;
	pBoxone = (CEdit*)GetDlgItem(IDC_EDIT1);
	pBoxone2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	CString str1;
	CString str2;
	pBoxone->GetWindowTextA(str1);
	pBoxone2->GetWindowTextA(str2);

	ref1 = atoi(str1);
	ref2 = atoi(str2);
	CDialog::OnOK();



}


void CTwoRefDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
