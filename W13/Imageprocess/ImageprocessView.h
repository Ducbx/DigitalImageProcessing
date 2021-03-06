// ImageprocessView.h : interface of the CImageprocessView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSVIEW_H__43D5507C_6FD6_4C70_9BDE_A3E0CE0FD658__INCLUDED_)
#define AFX_IMAGEPROCESSVIEW_H__43D5507C_6FD6_4C70_9BDE_A3E0CE0FD658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDib.h"

const int INITIAL = 0;
const int ACTIVE = 1;
const int EXPANDED = 2;
const double SQRT2 = 1.4142135623730950488016887242097;
const double SQINV = 1.0 / SQRT2;
const double PI = 3.1415926;


struct Grad
{
	double Ix, Iy, G;
};
struct Vector
{
	double x;
	double y;
};
struct Node
{
	CPoint point;
	Node *next;
};
class CImageprocessView : public CView
{
protected: // create from serialization only
	CImageprocessView();
	DECLARE_DYNCREATE(CImageprocessView)

	// Attributes
public:
	CImageprocessDoc * GetDocument();

	CDib mybmp;
	CSize sizeimage;
	int nWidth, nHeight;

	CDib tmpbmp;
	CSize sizenewimage;
	CPoint orginnew;

	//snake³õÊ¼»¯´¦Àí
	bool downflag;
	double *ImageData;
	Node *p, *lastnode;
	Node *firstnode;
	CPoint p_OldPos;
	int num_snake_points;
	Grad *fG;
	double *grad_mag;
	CPoint *pos, *Snake_points;


	//**************************************************************
	//Snakes Function
	double alpha, beta, gamma;/* Weights for energies */
	double *curvature;
	bool over;
	double threshold_curvature;

	//Draw
	CPen myPen, myPen2;
	COLORREF color, color2; //»­±ÊÑÕÉ«
	int colorSelect; //»­±ÊÑÕÉ«Ñ¡È¡£¬1ºì£¬2ÂÌ£¬3À¶
	int penWidth;   //»­±Ê¿í¶È


	void DirGrad(double *ImageData, int nWidth, int nHeight, double *pnGradX, double *pnGradY);



	// Operations
public:

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
public:
	void GetElasticEnergy(int i, double In_engery2[3][3]);
	void GetBlendingEnergy(int i, double In_engery1[3][3]);
	void GetExternalEnergy(int i, double Ex_energy[3][3]);
	void Gauss();
	void Gradient();
	void SnakeEvolution();
	double find_distance(int no, CPoint point);
	double find_curvature(int no, CPoint point);
	void Snake_interp();
	double find_continuity(int no, CPoint point, double avg_distance);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageprocessView)
	afx_msg void OnOpen();
	afx_msg void OnGary();
	afx_msg void OnRegiongrow();
	afx_msg void OnSplitcombine();
	afx_msg void OnSnakeinit();
	afx_msg void OnEvolution();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageprocessView.cpp
inline CImageprocessDoc* CImageprocessView::GetDocument()
{
	return (CImageprocessDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSVIEW_H__43D5507C_6FD6_4C70_9BDE_A3E0CE0FD658__INCLUDED_)
