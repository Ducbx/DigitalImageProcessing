
// w07View.cpp : implementation of the Cw07View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "w07.h"
#endif

#include "w07Doc.h"
#include "w07View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cw07View

IMPLEMENT_DYNCREATE(Cw07View, CView)

BEGIN_MESSAGE_MAP(Cw07View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cw07View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_READ_DOIT, &Cw07View::OnReadDoit)
	ON_COMMAND(ID_DO_LINEARPOINT, &Cw07View::OnDoLinearpoint)
	ON_COMMAND(ID_DO_UNLINEAR, &Cw07View::OnDoUnlinear)
	ON_COMMAND(ID_DO_GRAPH, &Cw07View::OnDoGraph)
	ON_COMMAND(ID_DO_QIANG, &Cw07View::OnDoQiang)
	ON_COMMAND(ID_DO_HUIDU, &Cw07View::OnDoHuidu)
END_MESSAGE_MAP()

// Cw07View construction/destruction

Cw07View::Cw07View()
{
	// TODO: add construction code here

}

Cw07View::~Cw07View()
{
}

BOOL Cw07View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cw07View drawing

void Cw07View::OnDraw(CDC* pDC)
{
	Cw07Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	mybmp_1.Draw(pDC, (0, 0), sizeDibDisplay_1);
	mybmp_gray.Draw(pDC, (0, sizeDibDisplay_1.cx + 2), sizeDibDisplay_gray);
	mybmp_output.Draw(pDC, (0, sizeDibDisplay_1.cx +sizeDibDisplay_gray.cx + 4), sizeDibDisplay_output);
	// TODO: add draw code for native data here
}


// Cw07View printing


void Cw07View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cw07View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cw07View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cw07View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cw07View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cw07View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cw07View diagnostics

#ifdef _DEBUG
void Cw07View::AssertValid() const
{
	CView::AssertValid();
}

void Cw07View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cw07Doc* Cw07View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cw07Doc)));
	return (Cw07Doc*)m_pDocument;
}
#endif //_DEBUG


// Cw07View message handlers


void Cw07View::OnReadDoit()
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






void Cw07View::OnDoLinearpoint()
{
	// TODO: Add your command handler code here
	CSize sizeimage;
	sizeimage = mybmp_1.GetDimensions();
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();

	// 对图象的象素值进行变换
	// 每行
	for (int y = 0; y < sizeimage.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeimage.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//s = ag + b
			double g = color.rgbRed;

			//线性变换参数
			double a;
			double b;

			if (g >= 110)
			{
				a = 1.2;
				b = 30;
				g = a * g + b;

				if (g >= 255)
					g = 255;
			}
			else if (g<88)
			{
				a = 0.2;
				b = 0;	
				g = a * g + b;
			}

			else if (g>88 && g<100)
			{
				a = 0.5;
				b = 0;
				g = a * g + b;
			}
			else if (g>100 && g<110)
			{
				a = 1.3;
				b = 0;
				g = a * g + b;


			}
			color.rgbBlue = (unsigned char)g;
			color.rgbGreen = (unsigned char)g;
			color.rgbRed = (unsigned char)g;
			mybmp_output
			.WritePixel(x, y, color);
		}
	}
	Invalidate();

}


void Cw07View::OnDoUnlinear()
{
	// TODO: Add your command handler code here
	mybmp_gray.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_gray = mybmp_1.GetDimensions();
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_1.GetDimensions();
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_gray.WritePixel(x, y, color);
		}
	}
	Invalidate();
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_gray.GetPixel(x, y);
			//s = ag + b
			double g = color.rgbRed;
			double c =3.1;
			double a = 1.1;
			g = c * pow(g, a);

			color.rgbBlue = (unsigned char)g;
			color.rgbGreen = (unsigned char)g;
			color.rgbRed = (unsigned char)g;
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate();


}


void Cw07View::OnDoGraph()
{
	// TODO: Add your command handler code here
	mybmp_gray.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_gray = mybmp_1.GetDimensions();
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_1.GetDimensions();
	int count[256] = { 0 };
	int newgray[256] = { 0 };
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_gray.WritePixel(x, y, color);
		}
	}
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每行
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_gray.GetPixel(x, y);
			int gray = color.rgbBlue;

			count[gray]++;
		}
	}
	for (int i = 0; i < 256; i++)
	{
		int lTemp = 0;
		for (int j = 0; j <= i; j++)
		{
			lTemp += count[j];
		}

		// 计算对应的新灰度值
		newgray[i] = (lTemp * 255.0 / sizeDibDisplay_1.cy / sizeDibDisplay_1.cx);
	}

	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每行
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_gray.GetPixel(x, y);
			int gray = newgray[color.rgbBlue];
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate();
}


void Cw07View::OnDoQiang()
{
	// TODO: Add your command handler code here
	mybmp_gray.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_gray = mybmp_1.GetDimensions();
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_1.GetDimensions();
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_gray.WritePixel(x, y, color);
		}
	}
	int graylevel = 5;
	//随机产生颜色
	RGBQUAD* color_tab = new RGBQUAD[graylevel + 1];
	srand((unsigned int)time(NULL));
	for (int i = 0; i <= graylevel; i++)
	{
		color_tab[i].rgbBlue = (unsigned char)rand() % 255;
		color_tab[i].rgbGreen = (unsigned char)rand() % 255;
		color_tab[i].rgbRed = (unsigned char)rand() % 255;
	}

	// 对图象的象素值进行变换
	for (int x = 0; x < sizeDibDisplay_1.cx; x++)
	{
		for (int y = 0; y < sizeDibDisplay_1.cy; y++)
		{
			RGBQUAD color;
			color = mybmp_gray.GetPixel(x, y);

			int level = color.rgbBlue / 25;         //对255个灰度等级的图像，分六种颜色
			mybmp_output.WritePixel(x, y, color_tab[level]);
		}
	}
	Invalidate();
}


void Cw07View::OnDoHuidu()
{
	// TODO: Add your command handler code here
	mybmp_gray.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_gray = mybmp_1.GetDimensions();
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_1.GetDimensions();
	for (int y = 0; y < sizeDibDisplay_1.cy; y++)
	{
		// 每列
		for (int x = 0; x < sizeDibDisplay_1.cx; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			int gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue = (unsigned char)gray;
			color.rgbGreen = (unsigned char)gray;
			color.rgbRed = (unsigned char)gray;
			mybmp_gray.WritePixel(x, y, color);
		}
	}
	for (int x = 0; x < sizeDibDisplay_1.cx; x++)
	{
		for (int y = 0; y < sizeDibDisplay_1.cy; y++)
		{
			RGBQUAD color, color1;
			color1 = mybmp_gray.GetPixel(x, y);

			color.rgbRed = (int)((color1.rgbRed * 2 + 80) / 1 + 200);
			color.rgbGreen = (int)((color1.rgbGreen * 4 + 120) / 3 + 20);
			color.rgbBlue = (int)((color1.rgbBlue + 40) / 2 + 10);
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate();
}
