
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
	afx_msg void OnAvgDoit();
	afx_msg void OnMeanDoit();
	afx_msg void OnWeightDoit();
	afx_msg int GetMedianNum(int *num,int i);
	afx_msg void OnLadderDoit();
	afx_msg void OnPrewittDoit();
	afx_msg void OnSobelDoit();
	afx_msg void OnLapDoit();
	afx_msg void OnLogDoit();
	afx_msg void Template3X3(CDib& tmpbmp, float * fpArray,float fvalue);
	afx_msg void Template5X5(CDib& tmpbmp, float * fpArray, float fvalue);
	afx_msg void OnDofinalDoit();
};

#ifndef _DEBUG  // debug version in w07View.cpp
inline Cw07Doc* Cw07View::GetDocument() const
   { return reinterpret_cast<Cw07Doc*>(m_pDocument); }
#endif

