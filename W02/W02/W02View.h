
// W02View.h : interface of the CW02View class
//

#pragma once
#include "CDib.h"

class CW02View : public CView
{
protected: // create from serialization only
	CW02View();
	DECLARE_DYNCREATE(CW02View)

// Attributes
public:
	CW02Doc* GetDocument() const;
	CDib mybmp_1;
	CSize sizeDibDisplay_1;
	CDib mybmp_2;
	CSize sizeDibDisplay_2;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CW02View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();
	afx_msg void OnOpen1();
};

#ifndef _DEBUG  // debug version in W02View.cpp
inline CW02Doc* CW02View::GetDocument() const
   { return reinterpret_cast<CW02Doc*>(m_pDocument); }
#endif

