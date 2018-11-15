// DisplayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "W03.h"
#include "DisplayDialog.h"
#include "afxdialogex.h"


// DisplayDialog dialog

IMPLEMENT_DYNAMIC(DisplayDialog, CDialog)

DisplayDialog::DisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DISPLAYDIALOG, pParent)
{

}

DisplayDialog::~DisplayDialog()
{
}

void DisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void DisplayDialog::OnPaint()
{
	CWnd* p = GetDlgItem(IDD_DISPLAYDIALOG);
	CDC* c = p->GetDC();
	p->Invalidate();
	p->UpdateWindow();
	c->SelectStockObject(BLACK_BRUSH);
	c->Rectangle(0, 0, 10, 10);
	p->ReleaseDC(c);
}


BEGIN_MESSAGE_MAP(DisplayDialog, CDialog)
	ON_BN_CLICKED(IDOK, &DisplayDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_threshold, &DisplayDialog::OnEnChangethreshold)
END_MESSAGE_MAP()


// DisplayDialog message handlers


void DisplayDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//get pointer to edit control
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_threshold);

	//get cString from the pointer
	CString str;
	pBoxOne->GetWindowText(str);

	//use public var to pass it to invoker 
	thres=atoi(str);
	if (thres <= 0 || thres >= 255)
	{
		MessageBox("Please input a value between 0-255");
		return;
	}

	//invoke OnOK() function
	CDialog::OnOK();
}


void DisplayDialog::OnEnChangethreshold()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
