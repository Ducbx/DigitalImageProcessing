#if !defined(AFX_DIALOG4_H__C7E988DC_C0E9_4E7B_9F11_8E8105A196EE__INCLUDED_)
#define AFX_DIALOG4_H__C7E988DC_C0E9_4E7B_9F11_8E8105A196EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog4 dialog

class CDialog4 : public CDialog
{
// Construction
public:
	CDialog4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog4)
	enum { IDD = IDD_DIALOG4 };
	int		m_x;
	int		m_y;
	int		m_n;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog4)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG4_H__C7E988DC_C0E9_4E7B_9F11_8E8105A196EE__INCLUDED_)
