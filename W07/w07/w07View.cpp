
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
	ON_COMMAND(ID_AVG_DOIT, &Cw07View::OnAvgDoit)
	ON_COMMAND(ID_MEAN_DOIT, &Cw07View::OnMeanDoit)
	ON_COMMAND(ID_WEIGHT_DOIT, &Cw07View::OnWeightDoit)
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
	mybmp_output.Draw(pDC, (0, sizeDibDisplay_1.cx  + 2), sizeDibDisplay_output);
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


void Cw07View::OnAvgDoit()
{
	// TODO: Add your command handler code here
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	int	temp[3][3] = { 1,1,1,1,1,1,1,1,1 };	  // 3*3模板
	double  tempC = 1.0 / 9;   //模板系数 

	for (int x = 0 + 1; x < sizeDibDisplay_output.cx - 1; x++)
	{
		for (int y = 0 + 1; y < sizeDibDisplay_output.cy - 1; y++)
		{
			RGBQUAD color;

			double r = 0;
			double g = 0;
			double b = 0;

			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					color = mybmp_1.GetPixel(x + i, y + j);
					r += color.rgbRed*temp[i + 1][j + 1] * tempC;
					g += color.rgbGreen*temp[i + 1][j + 1] * tempC;
					b += color.rgbBlue*temp[i + 1][j + 1] * tempC;
				}

			color.rgbBlue = (int)b;
			color.rgbGreen = (int)g;
			color.rgbRed = (int)r;
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate(true);

}


void Cw07View::OnMeanDoit()
{
	// TODO: Add your command handler code here
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	int	temp[3][3] = { 1,1,1,1,1,1,1,1,1 };	  // 3*3模板
	double  tempC = 1.0 / 9;   //模板系数 

	for (int x = 0 + 1; x < sizeDibDisplay_output.cx - 1; x++)
	{
		for (int y = 0 + 1; y < sizeDibDisplay_output.cy - 1; y++)
		{
			RGBQUAD color;


			int	r[9];
			int	g[9] = { 0 };
			int	b[9] = { 0 };
			int k = 0;
			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					color = mybmp_1.GetPixel(x + i, y + j);
					r[k] = color.rgbRed;
					g[k] = color.rgbGreen;
					b[k] = color.rgbBlue;
					k++;
				}
			int _r = GetMedianNum(r, 9);
			int _g = GetMedianNum(g, 9);
			int _b = GetMedianNum(b, 9);


			color.rgbBlue = (int)_b;
			color.rgbGreen = (int)_g;
			color.rgbRed = (int)_r;
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate(true);
}


void Cw07View::OnWeightDoit()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	double	temp[3][3] = { 1.0 / 16,2.0 / 16,1.0 / 16,2.0 / 16,4.0 / 16,2.0 / 16,1.0 / 16,2.0 / 16,1.0 / 16 };

	for (int x = 0 + 1; x < sizeDibDisplay_output.cx - 1; x++)
	{
		for (int y = 0 + 1; y < sizeDibDisplay_output.cy - 1; y++)
		{
			RGBQUAD color;

			double r = 0;
			double g = 0;
			double b = 0;

			for (int i = -1; i <= 1; i++)
				for (int j = -1; j <= 1; j++)
				{
					color = mybmp_1.GetPixel(x + i, y + j);
					r += color.rgbRed* temp[i + 1][j + 1];
					g += color.rgbGreen*temp[i + 1][j + 1];
					b += color.rgbBlue* temp[i + 1][j + 1];
				}

			color.rgbBlue = (int)b;
			color.rgbGreen = (int)g;
			color.rgbRed = (int)r;
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate(true);
}

int cmp(const void * a,const void *b)
{
	int* _a = (int*)a;
	int* _b = (int*)b;
	return *_a < *_b;
}

int Cw07View::GetMedianNum(int * num, int i)
{
	qsort(num, i, sizeof(int), cmp);
	return num[(i + 1) / 2-1];
}
