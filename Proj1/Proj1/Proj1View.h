
// Proj1View.h : interface of the CProj1View class
//

#pragma once
#include "CDib.h"
#include "MyTracker.h"
#include "ThresDlg.h"
class CProj1View : public CView
{
protected: // create from serialization only
	CProj1View();
	DECLARE_DYNCREATE(CProj1View)

// Attributes
public:
	CProj1Doc* GetDocument() const;
	//my vars
	CDib mybmp1;			//第一幅图像
	CDib mybmp2;			//第二幅图像
	CSize sizeimage1;		//第一幅图像分辨率
	CSize sizeimage2;		//第二幅图像分辨率
	CPoint m_origin1;		//第一幅屏幕显示起始点
	CPoint m_origin2;		//第二幅屏幕显示起始点
	CMyTracker m_tracker;	//Tracker对象
	CPen pen;				//画Tracker用的笔
	CDib tmp1, tmp2;		//tmp1用于存放合成的临时结果
							//tmp2用于存放二值化的掩模图像
	int nThreshold;			//用于二值化的阈值
	CPoint start1, end1, start2, end2;
							//1动态合成的前景部分在第一幅中的起止点
							//2取出的前景部分在第二幅中的起止点

	int state;				//状态：1-二值化前，2-二值化后，3开始定位
	bool sure;

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
	virtual ~CProj1View();
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
	afx_msg void OnFileOpenimage1();
	afx_msg void OnFileOpenimage2();
	afx_msg void OnFileExit();
	afx_msg void OnMapBinaryprocess();
	afx_msg void OnMappingConfirm();
	afx_msg void OnMappingSave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void UpdateImage();
	void ScaleImage(float fXZoomRatio, float fYZoomRatio);
};

#ifndef _DEBUG  // debug version in Proj1View.cpp
inline CProj1Doc* CProj1View::GetDocument() const
   { return reinterpret_cast<CProj1Doc*>(m_pDocument); }
#endif

