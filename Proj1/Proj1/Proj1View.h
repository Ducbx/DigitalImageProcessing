
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
	CDib mybmp1;			//��һ��ͼ��
	CDib mybmp2;			//�ڶ���ͼ��
	CSize sizeimage1;		//��һ��ͼ��ֱ���
	CSize sizeimage2;		//�ڶ���ͼ��ֱ���
	CPoint m_origin1;		//��һ����Ļ��ʾ��ʼ��
	CPoint m_origin2;		//�ڶ�����Ļ��ʾ��ʼ��
	CMyTracker m_tracker;	//Tracker����
	CPen pen;				//��Tracker�õı�
	CDib tmp1, tmp2;		//tmp1���ڴ�źϳɵ���ʱ���
							//tmp2���ڴ�Ŷ�ֵ������ģͼ��
	int nThreshold;			//���ڶ�ֵ������ֵ
	CPoint start1, end1, start2, end2;
							//1��̬�ϳɵ�ǰ�������ڵ�һ���е���ֹ��
							//2ȡ����ǰ�������ڵڶ����е���ֹ��

	int state;				//״̬��1-��ֵ��ǰ��2-��ֵ����3��ʼ��λ
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

