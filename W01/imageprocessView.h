
// imageprocessView.h : CimageprocessView 类的接口
//

#pragma once

#include"CDib.h"

class CimageprocessView : public CView
{
public:

	CDib mybmp;           //CDib类型的对象
	CSize sizeDibDisplay; //图像尺度


protected: // 仅从序列化创建
	CimageprocessView();
	DECLARE_DYNCREATE(CimageprocessView)

// 特性
public:
	CimageprocessDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CimageprocessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();


	afx_msg void OnRedblue();
};

#ifndef _DEBUG  // imageprocessView.cpp 中的调试版本
inline CimageprocessDoc* CimageprocessView::GetDocument() const
   { return reinterpret_cast<CimageprocessDoc*>(m_pDocument); }
#endif

