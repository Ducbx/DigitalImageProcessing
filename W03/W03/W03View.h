
// W03View.h : interface of the CW03View class
//

#pragma once
#include "CDib.h"
#include "DisplayDialog.h"
#include "TwoRefDlg.h"
class CW03View : public CView
{
protected: // create from serialization only
	CW03View();
	DECLARE_DYNCREATE(CW03View)

// Attributes
public:
	CW03Doc* GetDocument() const;
	CDib mybmp_1;
	CSize sizeDibDisplay_1;
	CDib mybmp_2;
	CSize sizeDibDisplay_2;
	CDib mybmp_output;
	CSize sizeDibDisplay_output;
	int threshold = 125;
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
	virtual ~CW03View();
	int onGray(RGBQUAD o_color);
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
	afx_msg void Onadd();
	afx_msg void Onminus();
	afx_msg void Onmulti();
	afx_msg void Ondiv();
	afx_msg void Onand();
	afx_msg void Onor();
	afx_msg void Onnot();
	afx_msg void Onmove();
	afx_msg void Onrotate();
	afx_msg void Onzoom();
	afx_msg void Onhorizontalmirror();
	afx_msg void Onverticalmirror();
	afx_msg void OnOpen2();
	afx_msg void Onopen1();
	afx_msg void OnSpecialGray();
	afx_msg void OnSpecialTwo();
};

#ifndef _DEBUG  // debug version in W03View.cpp
inline CW03Doc* CW03View::GetDocument() const
   { return reinterpret_cast<CW03Doc*>(m_pDocument); }
#endif

