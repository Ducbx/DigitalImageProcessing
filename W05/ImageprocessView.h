// ImageprocessView.h : interface of the CImageprocessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSVIEW_H__43D5507C_6FD6_4C70_9BDE_A3E0CE0FD658__INCLUDED_)
#define AFX_IMAGEPROCESSVIEW_H__43D5507C_6FD6_4C70_9BDE_A3E0CE0FD658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDib.h"
#include <math.h> 
#include <direct.h> 
#include <complex> 
using namespace std; 

class CImageprocessView : public CView
{
protected: // create from serialization only
	CImageprocessView();
	DECLARE_DYNCREATE(CImageprocessView)

// Attributes
public:
	CImageprocessDoc* GetDocument();
	
	
	CDib mybmp;
	CDib mybmp_raotate;
	CDib bmpfft,bmpifft;
	CSize sizeimage;
	int flagfft ;
	CSize  fftsize;
	
	int nTransWidth; 
	int  nTransHeight; 
// Operations
public:
	void fourier(complex<double> * data, int height, int width, int isign);
	void FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel);
	void IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel);
	void FFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData);
	void IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageprocessView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageprocessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageprocessView)
	afx_msg void OnOpen();
	afx_msg void OnGray();
	afx_msg void OnTl();
	afx_msg void OnFft();
	afx_msg void OnIfft();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRotateRotate30();
};

#ifndef _DEBUG  // debug version in ImageprocessView.cpp
inline CImageprocessDoc* CImageprocessView::GetDocument()
   { return (CImageprocessDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSVIEW_H__43D5507C_6FD6_4C70_9BDE_A3E0CE0FD658__INCLUDED_)
