#pragma once


// CTwoRefDlg dialog

class CTwoRefDlg : public CDialog
{
	DECLARE_DYNAMIC(CTwoRefDlg)

public:
	CTwoRefDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTwoRefDlg();
	float ref1;
	float ref2;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TWOREFDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit2();
};
