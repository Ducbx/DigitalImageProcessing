
// W03View.cpp : implementation of the CW03View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "W03.h"
#endif

#include "W03Doc.h"
#include "W03View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CW03View

IMPLEMENT_DYNCREATE(CW03View, CView)

BEGIN_MESSAGE_MAP(CW03View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CW03View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_COMMAND(add, &CW03View::Onadd)
	ON_COMMAND(minus, &CW03View::Onminus)
	ON_COMMAND(multi, &CW03View::Onmulti)
	ON_COMMAND(div, &CW03View::Ondiv)
	ON_COMMAND(and, &CW03View::Onand)
	ON_COMMAND(or , &CW03View::Onor)
	ON_COMMAND(not, &CW03View::Onnot)
	ON_COMMAND(move, &CW03View::Onmove)
	ON_COMMAND(rotate, &CW03View::Onrotate)
	ON_COMMAND(zoom, &CW03View::Onzoom)
	ON_COMMAND(horizontalMirror, &CW03View::Onhorizontalmirror)
	ON_COMMAND(verticalMirror, &CW03View::Onverticalmirror)
	ON_COMMAND(Open2, &CW03View::OnOpen2)
	ON_COMMAND(open1, &CW03View::Onopen1)
	ON_COMMAND(ID_SPECIAL_GRAY, &CW03View::OnSpecialGray)
	ON_COMMAND(ID_SPECIAL_TWO, &CW03View::OnSpecialTwo)
END_MESSAGE_MAP()

// CW03View construction/destruction

CW03View::CW03View()
{
	// TODO: add construction code here

}

CW03View::~CW03View()
{
}

BOOL CW03View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CW03View drawing

void CW03View::OnDraw(CDC* pDC)
{
	CW03Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CRect rt;
	GetClientRect(&rt);
	mybmp_1.Draw(pDC, (0, 0), sizeDibDisplay_1);
	mybmp_2.Draw(pDC, (0, sizeDibDisplay_1.cx + 1), sizeDibDisplay_2);
	mybmp_output.Draw(pDC,(0, sizeDibDisplay_1.cx+sizeDibDisplay_2.cx + 2), sizeDibDisplay_output);

}


// CW03View printing
	 

void CW03View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CW03View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CW03View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CW03View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CW03View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CW03View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CW03View diagnostics

#ifdef _DEBUG
void CW03View::AssertValid() const
{
	CView::AssertValid();
}

void CW03View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CW03Doc* CW03View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CW03Doc)));
	return (CW03Doc*)m_pDocument;
}
#endif //_DEBUG

void CW03View::Onadd()
{
	// TODO: Add your command handler code here
	if (mybmp_1.IsEmpty() || mybmp_2.IsEmpty())
	{
		MessageBox("Please read two pictures");
		return;
	}
	int sx = (sizeDibDisplay_1.cx <= sizeDibDisplay_2.cx) ? sizeDibDisplay_1.cx : sizeDibDisplay_2.cx;
	int sy = (sizeDibDisplay_1.cy <= sizeDibDisplay_2.cy) ? sizeDibDisplay_1.cy : sizeDibDisplay_2.cy;
	//mybmp_output.Empty();
	/*
	//Debug
	CString temp;
	temp.Format(_T("x:%d y:%d bitCount:%d\n"), sizeDibDisplay_1.cx, sizeDibDisplay_1.cy, mybmp_1.m_lpBMIH->biBitCount);
	MessageBox(temp);
	printf("x:%d y:%d bitCount:%d\n", sizeDibDisplay_1.cx, sizeDibDisplay_1.cy, mybmp_1.m_lpBMIH->biBitCount);*/
	//mybmp_output.CreateCDib(sizeDibDisplay_1, mybmp_1.m_lpBMIH->biBitCount);
	mybmp_output.CreateCDib(CSize(sx,sy),24);

	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;

			color1 = mybmp_1.GetPixel(x, y);
			color2 = mybmp_2.GetPixel(x, y);

			//合成图像
			RGBQUAD color;
			color.rgbBlue = color1.rgbBlue + color2.rgbBlue /*-180*/;
				if(color.rgbBlue>255)
					color.rgbBlue =color2.rgbBlue;

			color.rgbGreen = color1.rgbGreen + color2.rgbGreen /*- 180*/;
				if(color.rgbGreen>255)
					color.rgbGreen =color2.rgbGreen;

			color.rgbRed = color1.rgbRed + color2.rgbRed /*- 180*/;
				if(color.rgbRed>255)
					color.rgbRed =color2.rgbRed;

			mybmp_output.WritePixel(x, y, color);
			

		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();

}

void CW03View::Onminus()
{
	// TODO: Add your command handler code here
	if (mybmp_1.IsEmpty() || mybmp_2.IsEmpty())
	{
		MessageBox("Please read two pictures");
		return;
	}
	int sx = (sizeDibDisplay_1.cx <= sizeDibDisplay_2.cx) ? sizeDibDisplay_1.cx : sizeDibDisplay_2.cx;
	int sy = (sizeDibDisplay_1.cy <= sizeDibDisplay_2.cy) ? sizeDibDisplay_1.cy : sizeDibDisplay_2.cy;
	mybmp_output.CreateCDib(CSize(sx, sy), 24);

	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;

			color1 = mybmp_1.GetPixel(x, y);
			color2 = mybmp_2.GetPixel(x, y);

			//合成图像
			RGBQUAD color;
			color.rgbBlue = color1.rgbBlue - color2.rgbBlue ;
			if (color.rgbBlue<0)
				color.rgbBlue = 0;

			color.rgbGreen = color1.rgbGreen - color2.rgbGreen;
			if (color.rgbGreen < 0)
				color.rgbGreen = 0;

			color.rgbRed = color1.rgbRed - color2.rgbRed;
			if (color.rgbRed<0)
				color.rgbRed = 0;

			mybmp_output.WritePixel(x, y, color);


		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();
}

void CW03View::Onmulti()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if (mybmp_1.IsEmpty() || mybmp_2.IsEmpty())
	{
		MessageBox("Please read two pictures");
		return;
	}
	int sx = (sizeDibDisplay_1.cx <= sizeDibDisplay_2.cx) ? sizeDibDisplay_1.cx : sizeDibDisplay_2.cx;
	int sy = (sizeDibDisplay_1.cy <= sizeDibDisplay_2.cy) ? sizeDibDisplay_1.cy : sizeDibDisplay_2.cy;
	mybmp_output.CreateCDib(CSize(sx, sy), 24);

	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;

			color1 = mybmp_1.GetPixel(x, y);
			color2 = mybmp_2.GetPixel(x, y);

			//合成图像
			RGBQUAD color;
			color.rgbBlue = color1.rgbBlue * color2.rgbBlue;

			if (color.rgbBlue>255)
				color.rgbBlue = color1.rgbBlue;

			color.rgbGreen = color1.rgbGreen * color2.rgbGreen;

			if (color.rgbGreen>255)
				color.rgbGreen = color1.rgbGreen;

			color.rgbRed = color1.rgbRed * color2.rgbRed;

			if (color.rgbRed>255)
				color.rgbRed = color1.rgbRed;


			mybmp_output.WritePixel(x, y, color);


		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();
}

void CW03View::Ondiv()
{
	// TODO: Add your command handler code here
	if (mybmp_1.IsEmpty() || mybmp_2.IsEmpty())
	{
		MessageBox("Please read two pictures");
		return;
	}
	int sx = (sizeDibDisplay_1.cx <= sizeDibDisplay_2.cx) ? sizeDibDisplay_1.cx : sizeDibDisplay_2.cx;
	int sy = (sizeDibDisplay_1.cy <= sizeDibDisplay_2.cy) ? sizeDibDisplay_1.cy : sizeDibDisplay_2.cy;
	mybmp_output.CreateCDib(CSize(sx, sy), 24);

	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;

			color1 = mybmp_1.GetPixel(x, y);
			color2 = mybmp_2.GetPixel(x, y);

			//合成图像
			RGBQUAD color;
			if (color2.rgbBlue == 0)
			{
				color.rgbBlue = 0;
			}
			else {
				color.rgbBlue = color1.rgbBlue / color2.rgbBlue;
			}
			if (color2.rgbRed == 0)
			{
				color.rgbRed = 0;
			}
			else {
				color.rgbRed = color1.rgbRed / color2.rgbRed;
			}
			if (color2.rgbGreen == 0)
			{
				color.rgbGreen = 0;
			}
			else {
				color.rgbGreen = color1.rgbGreen / color2.rgbGreen;
			}
			mybmp_output.WritePixel(x, y, color);
		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();
}

void CW03View::Onand()
{
	// TODO: Add your command handler code here
	DisplayDialog ddlg;
	ddlg.DoModal();
	CString cstr;
	threshold = ddlg.thres;
	cstr.Format("Threshold is %d\n", ddlg.thres);
	MessageBox(cstr);

	CDib tmpBmp_1;
	CDib tmpBmp_2;
	if (mybmp_1.IsEmpty() || mybmp_2.IsEmpty())
	{
		MessageBox("Please read two pictures");
		return;
	}
	int sx = (sizeDibDisplay_1.cx <= sizeDibDisplay_2.cx) ? sizeDibDisplay_1.cx : sizeDibDisplay_2.cx;
	int sy = (sizeDibDisplay_1.cy <= sizeDibDisplay_2.cy) ? sizeDibDisplay_1.cy : sizeDibDisplay_2.cy;
	tmpBmp_1.CreateCDib(sizeDibDisplay_1, 24);
	tmpBmp_2.CreateCDib(sizeDibDisplay_2, 24);
	mybmp_output.CreateCDib(CSize(sx, sy), 24);
	//对图像1灰度变换and二值化处理
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			tmpBmp_1.WritePixel(x, y, color);
		}
	}
	//对图像2灰度变化and二值化处理
	for (int y = 0; y < sizeDibDisplay_2.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_2.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_2.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			tmpBmp_2.WritePixel(x, y, color);
		}
	}
	

	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;
			RGBQUAD tmpcolor;
			color1 = tmpBmp_1.GetPixel(x, y);
			color2 = tmpBmp_2.GetPixel(x, y);
			//合成图像
			if (color1.rgbBlue == 255 && color2.rgbBlue == 255)
			{
				tmpcolor.rgbBlue = (unsigned char)255;
				tmpcolor.rgbGreen = (unsigned char)255;
				tmpcolor.rgbRed = (unsigned char)255;
			}
			else {
				tmpcolor.rgbBlue = (unsigned char)0;
				tmpcolor.rgbGreen = (unsigned char)0;
				tmpcolor.rgbRed = (unsigned char)0;
			}
			mybmp_output.WritePixel(x, y, tmpcolor);
		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();


}

void CW03View::Onor()
{
	// TODO: Add your command handler code here

	DisplayDialog ddlg;
	ddlg.DoModal();
	CString cstr;
	threshold = ddlg.thres;
	cstr.Format("Threshold is %d\n", ddlg.thres);
	MessageBox(cstr);

	CDib tmpBmp_1;
	CDib tmpBmp_2;
	if (mybmp_1.IsEmpty() || mybmp_2.IsEmpty())
	{
		MessageBox("Please read two pictures");
		return;
	}
	int sx = (sizeDibDisplay_1.cx <= sizeDibDisplay_2.cx) ? sizeDibDisplay_1.cx : sizeDibDisplay_2.cx;
	int sy = (sizeDibDisplay_1.cy <= sizeDibDisplay_2.cy) ? sizeDibDisplay_1.cy : sizeDibDisplay_2.cy;
	tmpBmp_1.CreateCDib(sizeDibDisplay_1, 24);
	tmpBmp_2.CreateCDib(sizeDibDisplay_2, 24);
	mybmp_output.CreateCDib(CSize(sx, sy), 24);
	//对图像1灰度变换and二值化处理
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			tmpBmp_1.WritePixel(x, y, color);
		}
	}
	//对图像2灰度变化and二值化处理
	for (int y = 0; y < sizeDibDisplay_2.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_2.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_2.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			tmpBmp_2.WritePixel(x, y, color);
		}
	}


	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD color2;
			RGBQUAD tmpcolor;
			color1 = tmpBmp_1.GetPixel(x, y);
			color2 = tmpBmp_2.GetPixel(x, y);
			//合成图像
			if (color1.rgbBlue == 0 && color2.rgbBlue == 0)
			{
				tmpcolor.rgbBlue = (unsigned char)0;
				tmpcolor.rgbGreen = (unsigned char)0;
				tmpcolor.rgbRed = (unsigned char)0;
			}
			else {
				tmpcolor.rgbBlue = (unsigned char)255;
				tmpcolor.rgbGreen = (unsigned char)255;
				tmpcolor.rgbRed = (unsigned char)255;
			}
			mybmp_output.WritePixel(x, y, tmpcolor);
		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();
}

void CW03View::Onnot()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	DisplayDialog ddlg;
	ddlg.DoModal();
	CString cstr;
	threshold = ddlg.thres;
	cstr.Format("Threshold is %d\n", ddlg.thres);
	MessageBox(cstr);

	CDib tmpBmp_1;
	if (mybmp_1.IsEmpty())
	{
		MessageBox("Please read the first picture");
		return;
	}
	int sx = sizeDibDisplay_1.cx ;
	int sy = sizeDibDisplay_1.cy;
	tmpBmp_1.CreateCDib(sizeDibDisplay_1, 24);
	mybmp_output.CreateCDib(CSize(sx, sy), 24);
	//对图像1灰度变换and二值化处理
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			tmpBmp_1.WritePixel(x, y, color);
		}
	}

	for (int y = 0; y < sy; y++)
	{
		// 每列
		for (int x = 0; x < sx; x++)
		{
			RGBQUAD color1;
			RGBQUAD tmpcolor;
			color1 = tmpBmp_1.GetPixel(x, y);
			//合成图像
			if (color1.rgbBlue == 255 )
			{
				tmpcolor.rgbBlue = (unsigned char)0;
				tmpcolor.rgbGreen = (unsigned char)0;
				tmpcolor.rgbRed = (unsigned char)0;
			}
			else {
				tmpcolor.rgbBlue = (unsigned char)255;
				tmpcolor.rgbGreen = (unsigned char)255;
				tmpcolor.rgbRed = (unsigned char)255;
			}
			mybmp_output.WritePixel(x, y, tmpcolor);
		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();

}

void CW03View::OnSpecialGray()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	/*DisplayDialog ddlg;
	ddlg.DoModal();
	CString cstr;
	threshold = ddlg.thres;
	cstr.Format("%d\n", ddlg.thres);
	MessageBox(cstr);*/

	if (mybmp_1.IsEmpty())
	{
		MessageBox("Please read the first picture");
		return;
	}
	int sx = sizeDibDisplay_1.cx;
	int sy = sizeDibDisplay_1.cy;
	mybmp_output.CreateCDib(CSize(sx, sy), 24);
	//对图像1灰度变换and二值化处理
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			//gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_output.WritePixel(x, y, color);
		}
	}


	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();
}

void CW03View::OnSpecialTwo()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	DisplayDialog ddlg;
	ddlg.DoModal();
	CString cstr;
	threshold = ddlg.thres;
	cstr.Format("Threshold is %d\n", ddlg.thres);
	MessageBox(cstr);

	if (mybmp_1.IsEmpty())
	{
		MessageBox("Please read the first picture");
		return;
	}
	int sx = sizeDibDisplay_1.cx;
	int sy = sizeDibDisplay_1.cy;
	mybmp_output.CreateCDib(CSize(sx, sy), 24);
	//对图像1灰度变换and二值化处理
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = onGray(color);
			gray = (gray >= threshold) ? 255 : 0;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_output.WritePixel(x, y, color);
		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();
}


void CW03View::Onmove()
{
	CTwoRefDlg dlg;
	dlg.DoModal();
	int off_x = dlg.ref1;
	int off_y = dlg.ref2;
	CString cstr;
	cstr.Format("x_off:%d y_off:%d\n", dlg.ref1,dlg.ref2);
	MessageBox(cstr);
	// TODO: Add your command handler code here

	if (mybmp_1.IsEmpty())
	{
		MessageBox("Please read the first picture");
		return;
	}

	mybmp_output.CreateCDib(sizeDibDisplay_1, 24);


	

	// 每行
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);

			// 计算该象素在源DIB中的坐标
			int x0 = x + off_x;
			int y0 = y + off_y;


			// 判断是否在源图范围内
			if (x0 >= sizeDibDisplay_1.cx || x0<0 || y0 >= sizeDibDisplay_1.cy || y0<0)
			{
				/*color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;*/
			}
			else {
				RGBQUAD _color;
				_color.rgbBlue = color.rgbBlue;
				_color.rgbGreen = color.rgbGreen;
				_color.rgbRed = color.rgbRed;
				mybmp_output.WritePixel(x0, y0, _color);
			}
		}
	}
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	Invalidate();

}



void CW03View::Onrotate()
{
	// TODO: Add your command handler code here
	if (mybmp_1.IsEmpty())
	{
		MessageBox("Please read the first picture");
		return;
	}

	CTwoRefDlg dlg;
	dlg.DoModal();
	int m_rotateangle = dlg.ref1;
	CString cstr;
	cstr.Format("rotateangle:%d\n", dlg.ref1);
	

	//////////////////
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	long lWidth = sizeDibDisplay_1.cx;			// 获取图像的宽度
	long lHeight = sizeDibDisplay_1.cy;			// 获取图像的高度

											// 将旋转角度从度转换到弧度
	float	fRotateAngle = m_rotateangle*3.1415926535 / 180.0;		// 旋转角度（弧度）	
	float	fSina, fCosa;		// 旋转角度的正弦和余弦	
	fSina = (float)sin((double)fRotateAngle);	// 计算旋转角度的正弦
	fCosa = (float)cos((double)fRotateAngle);	// 计算旋转角度的余弦

												// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float)(-lWidth / 2);
	fSrcY1 = (float)(lHeight / 2);
	fSrcX2 = (float)(lWidth / 2);
	fSrcY2 = (float)(lHeight / 2);
	fSrcX3 = (float)(-lWidth / 2);
	fSrcY3 = (float)(-lHeight / 2);
	fSrcX4 = (float)(lWidth / 2);
	fSrcY4 = (float)(-lHeight / 2);



	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	//图像中心


	// 计算旋转后的图像实际宽度
	long lNewWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	// 计算旋转后的图像高度
	long lNewHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	mybmp_output.Empty();
	mybmp_output.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp_1.m_lpBMIH->biBitCount);

	sizeDibDisplay_output = mybmp_output.GetDimensions();

	RGBQUAD color;

	// 每行
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			color = mybmp_1.GetPixel(x, y);

			// 计算该象素在源DIB中的坐标
			float x0 = fCosa * (x - sizeDibDisplay_1.cx / 2.0) + fSina * (y - sizeDibDisplay_1.cy / 2.0)+0.5;
			float y0 = -fSina * (x - sizeDibDisplay_1.cx / 2.0) + fCosa * (y - sizeDibDisplay_1.cy / 2.0)+0.5;

			x0 = x0 + lNewWidth / 2+0.5;
			y0 = y0 + lNewHeight / 2+0.5;


			mybmp_output.WritePixel((int)x0, (int)y0, color);
		}
	}

	Invalidate();

}


void CW03View::Onzoom()
{
	// TODO: Add your command handler code here
}


void CW03View::Onhorizontalmirror()
{
	// TODO: Add your command handler code here
}


void CW03View::Onverticalmirror()
{
	// TODO: Add your command handler code here
}

void CW03View::Onopen1()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = { "Open Image" };
	FileDlg.m_ofn.lpstrTitle = title;

	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp_1.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp_1.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeDibDisplay_1 = mybmp_1.GetDimensions();

	Invalidate(TRUE);
}

void CW03View::OnOpen2()
{
	// TODO: Add your command handler code here
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
	char title[] = { "Open Image" };
	FileDlg.m_ofn.lpstrTitle = title;

	CFile file;
	if (FileDlg.DoModal() == IDOK)
	{
		if (!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if (!mybmp_2.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp_2.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeDibDisplay_2 = mybmp_2.GetDimensions();

	Invalidate(TRUE);
}

int CW03View::onGray(RGBQUAD o_color)
{
	return  o_color.rgbRed*0.299 + o_color.rgbGreen*0.587 + o_color.rgbBlue*0.114;
}




