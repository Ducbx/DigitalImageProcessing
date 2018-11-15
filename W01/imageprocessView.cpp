// imageprocessView.cpp : CimageprocessView 类的实现
//

#include "stdafx.h"
#include <cmath>
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "imageprocess.h"
#endif

#include "imageprocessDoc.h"
#include "imageprocessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CimageprocessView

IMPLEMENT_DYNCREATE(CimageprocessView, CView)

BEGIN_MESSAGE_MAP(CimageprocessView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CimageprocessView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN, &CimageprocessView::OnOpen)
	ON_COMMAND(ID_REDBLUE, &CimageprocessView::OnRedblue)
END_MESSAGE_MAP()

// CimageprocessView 构造/析构

CimageprocessView::CimageprocessView()
{
	// TODO: 在此处添加构造代码

}

CimageprocessView::~CimageprocessView()
{
}

BOOL CimageprocessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CimageprocessView 绘制

void CimageprocessView::OnDraw(CDC* pDC)
{
	CimageprocessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	mybmp.Draw(pDC,CPoint(0,0),sizeDibDisplay);
}


// CimageprocessView 打印


void CimageprocessView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CimageprocessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CimageprocessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CimageprocessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CimageprocessView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CimageprocessView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CimageprocessView 诊断

#ifdef _DEBUG
void CimageprocessView::AssertValid() const
{
	CView::AssertValid();
}

void CimageprocessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CimageprocessDoc* CimageprocessView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimageprocessDoc)));
	return (CimageprocessDoc*)m_pDocument;
}
#endif //_DEBUG


// CimageprocessView 消息处理程序


void CimageprocessView::OnOpen()
{
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Image"};
	FileDlg.m_ofn.lpstrTitle= title;

	CFile file ;
	if( FileDlg.DoModal() == IDOK ) 
	{
		if(!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if(!mybmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
		
	if (mybmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return ;
	}
	sizeDibDisplay=mybmp.GetDimensions();
	
	Invalidate(TRUE);
	
}


void CimageprocessView::OnRedblue()
{
	CSize ImageSize;
	ImageSize = mybmp.GetDimensions();

	int cx = ImageSize.cx;
	int cy = ImageSize.cy;
	int nx = 200;
	int ny = 150;
	//for (int y = 0; y < ImageSize.cy; y ++)
	//{
	//	for (int x = 0 ; x < ImageSize.cx ; x ++)
	//	{
	//			RGBQUAD RedColor;
	//			RedColor = mybmp.GetPixel(x,y); 				
	//			/*if (y <70  && y>50)
	//			{
	//				RedColor.rgbRed = 255;
	//				RedColor.rgbGreen = 0;
	//				RedColor.rgbBlue = 0;
	//				mybmp.WritePixel(x,y,RedColor);
	//			}	

	//			if (x <60  && x>30)
	//			{

	//				RedColor.rgbRed = 0;
	//				RedColor.rgbGreen = 0;
	//				RedColor.rgbBlue = 255;
	//				mybmp.WritePixel(x,y,RedColor);
	//			}	*/
	//	}
	//}

	for (int x = -ImageSize.cx; x < ImageSize.cx; x++)
	{
		RGBQUAD p1;
		RGBQUAD p2;
		double transX= ((double)x)/nx/* - (double)cx / 2*/;
		if (transX <= 1 && transX >= -1)
		{
			
			//1st
			double transY1 = -(sqrt(1 - pow(transX, 2)) + pow(transX*transX,1.0/3))*ny+(double)cy/2;
			p1 = mybmp.GetPixel(x, transY1+0.5);
			p1.rgbRed = 255;
			p1.rgbGreen = 0;
			p1.rgbBlue = 0;
			mybmp.WritePixel(x+cx/2,transY1+0.5,p1);

			//2nd
			double transY2 = -(-sqrt(1 - pow(transX, 2)) + pow(transX*transX, 1.0 / 3))*ny+(double)cy / 2;
			p2 = mybmp.GetPixel(x, transY2+0.5);
			p2.rgbRed = 255;
			p2.rgbGreen = 0;
			p2.rgbBlue = 0;
			mybmp.WritePixel(x+cx/2, transY2+0.5, p2);
		}
	}


		 mybmp.Save("1111.bmp");
		 Invalidate(TRUE);
	
}


//void CMFCCircleDialogDemoView::OnDraw(CDC* pDC)
//{
//	CMFCCircleDialogDemoDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	// TODO: 在此处为本机数据添加绘制代码
//	/*确定圆心，画出直角坐标系*/
//	CRect rect;
//	GetWindowRect(rect);
//	int center_x = rect.Width() / 2;
//	int center_y = rect.Height() / 2;
//
//	CPen myPen; //创建画笔
//	myPen.CreateStockObject(BLACK_PEN); //设置画笔属性
//	pDC->SelectObject(&myPen); //绑定画笔句柄
//
//	pDC->MoveTo(center_x, 0);
//	pDC->LineTo(center_x, rect.Height());
//	pDC->MoveTo(0, center_y);
//	pDC->LineTo(rect.Width(), center_y);
//
//	/*********************绘制圆****************/
//	//逻辑圆心坐标
//	/*
//	int m_vradius = 50;
//	int m_vcirclex = -70;
//	int m_vcircley = 100;
//	*/
//	//转化为画布中圆心坐标
//	center_x = center_x + m_vcirclex;
//	center_y = center_y - m_vcircley;
//	pDC->SetPixel(center_x, center_y + m_vradius, RGB(125, 125, 125));//绘制圆心的点
//	pDC->Ellipse(center_x - 3, center_y - 3, center_x + 3, center_y + 3); //画个小圆，确定圆心位置
//	int p = 5 / 4 - m_vradius;                                      //键值的初始值
//	int x = 0;
//	int y = m_vradius;
//	//使用中点圆算法画圆  
//	while (x<y) {
//		x++;
//		if (p<0) {
//			p = p + 2 * x + 3;
//			pDC->SetPixel(center_x + x, center_y + y, RGB(125, 125, 125));
//			pDC->SetPixel(center_x + y, center_y + x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x + y, center_y - x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x + x, center_y - y, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - x, center_y - y, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - y, center_y - x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - y, center_y + x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - x, center_y + y, RGB(125, 125, 125));          //以对称的方式完成圆的绘制
//		}
//		else {
//			y--;
//			p = p + 2 * x + 3 - 2 * y - 2;
//			pDC->SetPixel(center_x + x, center_y + y, RGB(125, 125, 125));
//			pDC->SetPixel(center_x + y, center_y + x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x + y, center_y - x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x + x, center_y - y, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - x, center_y - y, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - y, center_y - x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - y, center_y + x, RGB(125, 125, 125));
//			pDC->SetPixel(center_x - x, center_y + y, RGB(125, 125, 125));             //以对称的方式完成圆的绘制  
//		}
//		Sleep(20);
//	}
//}