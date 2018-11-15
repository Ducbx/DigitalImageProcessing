#pragma once


// ThresDlg dialog

class ThresDlg : public CDialog
{
	DECLARE_DYNAMIC(ThresDlg)

public:
	ThresDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ThresDlg();
	int ref1;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THRESDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnStnClickedClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedStatic();
	virtual BOOL OnInitDialog();
};
