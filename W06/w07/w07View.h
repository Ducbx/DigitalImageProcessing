
// w07View.h : interface of the Cw07View class
//

#pragma once
#include "CDib.h"

class Cw07View : public CView
{
protected: // create from serialization only
	Cw07View();
	DECLARE_DYNCREATE(Cw07View)
	
// Attributes
public:
	Cw07Doc* GetDocument() const;
	CDib mybmp_1;
	CSize sizeDibDisplay_1;
	CDib mybmp_gray;
	CSize sizeDibDisplay_gray;
	CDib mybmp_output;
	CSize sizeDibDisplay_output;
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
	virtual ~Cw07View();
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
	afx_msg void OnReadDoit();

	afx_msg void OnDoLinearpoint();
	afx_msg void OnDoUnlinear();
	afx_msg void OnDoGraph();
	afx_msg void OnDoQiang();
	afx_msg void OnDoHuidu();
};

#ifndef _DEBUG  // debug version in w07View.cpp
inline Cw07Doc* Cw07View::GetDocument() const
   { return reinterpret_cast<Cw07Doc*>(m_pDocument); }
#endif

