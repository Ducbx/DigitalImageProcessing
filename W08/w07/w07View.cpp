
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
	ON_COMMAND(ID_LADDER_DOIT, &Cw07View::OnLadderDoit)
	ON_COMMAND(ID_PREWITT_DOIT, &Cw07View::OnPrewittDoit)
	ON_COMMAND(ID_SOBEL_DOIT, &Cw07View::OnSobelDoit)
	ON_COMMAND(ID_LAP_DOIT, &Cw07View::OnLapDoit)
	ON_COMMAND(ID_LOG_DOIT, &Cw07View::OnLogDoit)
	ON_COMMAND(ID_DOFINAL_DOIT, &Cw07View::OnDofinalDoit)
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
	for (int i = 0; i < sizeDibDisplay_1.cy - 1; i++)
	{

		for (int j = 0; j < sizeDibDisplay_1.cx - 1; j++)
		{
			RGBQUAD color = mybmp_1.GetPixel(j, i);
			int num=0.299*color.rgbRed+0.587*color.rgbGreen +0.114*color.rgbBlue;
			color.rgbGreen = num;
			color.rgbRed = num;
			color.rgbBlue = num;
			mybmp_1.WritePixel(j, i, color);
		}
	}

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


void Cw07View::OnLadderDoit()
{
	// TODO: 在此添加命令处理程序代码
	
	int thresh =10;

	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	for (int i = 0; i < sizeDibDisplay_1.cy - 1; i++)
	{
		for (int j = 0; j < sizeDibDisplay_1.cx - 1; j++)
		{
			RGBQUAD color1, color2, color3, color;
			// 指向DIB第i行，第j个象素的指针
			color1 = mybmp_1.GetPixel(j, i);

			// 指向DIB第i+1行，第j个象素的指针
			color2 = mybmp_1.GetPixel(j, i + 1);

			// 指向DIB第i行，第j+1个象素的指针
			color3 = mybmp_1.GetPixel(j + 1, i);

			int bTemp = abs(color1.rgbBlue - color2.rgbBlue) + abs(color1.rgbBlue - color3.rgbBlue);

			// 判断是否小于阈值
			// 判断是否大于阈值，对于小于情况，灰度值不变。
			if (bTemp >= thresh)
			{
				if (bTemp < 255)
				{
					color.rgbGreen = bTemp;
					color.rgbRed = bTemp;
					color.rgbBlue = bTemp;
					mybmp_output.WritePixel(j, i, color);
				}
				else {
					color.rgbGreen = 255;
					color.rgbRed = 255;
					color.rgbBlue = 255;
					mybmp_output.WritePixel(j, i, color);
				}
			}
			else {
				
				//mybmp_output.WritePixel(j, i, mybmp_1.GetPixel(j,i));
			}
		}
	}
	Invalidate(true);
}


void Cw07View::OnPrewittDoit()
{
	// TODO: 在此添加命令处理程序代码
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();

	CSize sizeimage = mybmp_1.GetDimensions();
	CDib tmpbmp1;
	tmpbmp1.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp_1);

	CDib tmpbmp2;
	tmpbmp2.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp2.CopyDib(&mybmp_1);

	float aTemplate[9];			// 模板数组

								// 设置Prewitt模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	Template3X3(tmpbmp1, aTemplate,1.0);

	// 设置Prewitt模板参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	Template3X3(tmpbmp2, aTemplate,1.0);


	//求两幅缓存图像的最大值
	for (int j = 0; j < sizeimage.cy; j++)
	{
		for (int i = 0; i < sizeimage.cx; i++)
		{
			RGBQUAD color, color1, color2;

			// 指向缓存图像1第j行，第i个象素的指针	
			color1 = tmpbmp1.GetPixel(i, j);

			// 指向缓存图像2第j行，第i个象素的指针			
			color2 = tmpbmp2.GetPixel(i, j);

			if (color2.rgbRed > color1.rgbRed)
			{
				color = color2;
			}
			else
				color = color1;

			mybmp_output.WritePixel(i, j, color);
		}
	}
	Invalidate(true);
}


void Cw07View::OnSobelDoit()
{
	// TODO: 在此添加命令处理程序代码
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();

	CSize sizeimage = mybmp_1.GetDimensions();
	CDib tmpbmp1;
	tmpbmp1.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp_1);

	CDib tmpbmp2;
	tmpbmp2.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp2.CopyDib(&mybmp_1);

	float aTemplate[9];			// 模板数组

								// 设置Prewitt模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;


	Template3X3(tmpbmp1, aTemplate,1.0);

	// 设置Prewitt模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;


	Template3X3(tmpbmp2, aTemplate,1.0);


	//求两幅缓存图像的最大值
	for (int j = 0; j < sizeimage.cy; j++)
	{
		for (int i = 0; i < sizeimage.cx; i++)
		{
			RGBQUAD color, color1, color2;

			// 指向缓存图像1第j行，第i个象素的指针	
			color1 = tmpbmp1.GetPixel(i, j);

			// 指向缓存图像2第j行，第i个象素的指针			
			color2 = tmpbmp2.GetPixel(i, j);

			if (color2.rgbRed > color1.rgbRed)
			{
				color = color2;
			}
			else
				color = color1;

			mybmp_output.WritePixel(i, j, color);
		}
	}
	Invalidate(true);

}


void Cw07View::OnLapDoit()
{
	// TODO: 在此添加命令处理程序代码
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	mybmp_output.CopyDib(&mybmp_1);
	

	float aTemplate[9];			// 模板数组

								// 设置Prewitt模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 9.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;


	Template3X3(mybmp_output, aTemplate,1.0);
	Invalidate(true);

}


void Cw07View::OnLogDoit()
{
	// TODO: 在此添加命令处理程序代码
	float aTemplate[25];			// 模板数组
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	mybmp_output.CopyDib(&mybmp_1);					// 设置模板参数
	aTemplate[0] = 0;
	aTemplate[1] = 0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0;
	aTemplate[4] = 0;
	aTemplate[5] = 0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -2.0;
	aTemplate[8] = -1.0;
	aTemplate[9] = 0;
	aTemplate[10] = -1.0;
	aTemplate[11] = -2.0;
	aTemplate[12] = 16.0;
	aTemplate[13] = -2.0;
	aTemplate[14] = -1.0;
	aTemplate[15] = 0;
	aTemplate[16] = -1.0;
	aTemplate[17] = -2.0;
	aTemplate[18] = -1.0;
	aTemplate[19] = 0;
	aTemplate[20] = 0;
	aTemplate[21] = 0;
	aTemplate[22] = -1.0;
	aTemplate[23] = 0;
	aTemplate[24] = 0;
	Template5X5(mybmp_output,aTemplate,1.5);
	Invalidate(true);

}

void Cw07View::Template3X3(CDib& tmpbmp, float * fpArray,float fvalue)
{
	CDib newbmp;
	CSize sizeimage = tmpbmp.GetDimensions();
	newbmp.CreateCDib(sizeimage, tmpbmp.m_lpBMIH->biBitCount);
	
	for (int y = 1; y < sizeimage.cy - 1; y++)	// 行(除去边缘几行)
	{
		for (int x = 1; x < sizeimage.cx - 1; x++)// 列(除去边缘几列)
		{
			RGBQUAD color;

			double fResult = 0;

			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					color = tmpbmp.GetPixel(x + i, y + j);

					// 保存象素值
					fResult += color.rgbRed * fpArray[(i + 1) * 3 + (j + 1)];
				}
			}

			fResult = fabs(fResult)*fvalue;	// 取绝对值
			if (fResult > 255)
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
				newbmp.WritePixel(x, y, color);
			}
			else
			{
				color.rgbGreen = (unsigned char)(fResult + 0.5);
				color.rgbRed = (unsigned char)(fResult + 0.5);
				color.rgbBlue = (unsigned char)(fResult + 0.5);
				newbmp.WritePixel(x, y, color);
			}
		}
	}
	tmpbmp.CopyDib(&newbmp);

}

void Cw07View::Template5X5(CDib& tmpbmp, float * fpArray, float fvalue)
{
	CDib newbmp;
	CSize sizeimage = sizeDibDisplay_1;
	newbmp.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);

	for (int y = 2; y < sizeimage.cy - 2; y++)	// 行(除去边缘几行)
	{
		for (int x = 2; x < sizeimage.cx - 2; x++)// 列(除去边缘几列)
		{
			RGBQUAD color;

			double fResult = 0;

			for (int j = -2; j <= 2; j++)
			{
				for (int i = -2; i <= 2; i++)
				{
					color = tmpbmp.GetPixel(x + i, y + j);
					fResult += color.rgbRed * fpArray[(i + 2) * 5 + (j + 2)];
				}
			}

			fResult = fabs(fResult)*fvalue;	// 取绝对值
			if (fResult > 255)
			{
				color.rgbGreen = 255;
				color.rgbRed = 255;
				color.rgbBlue = 255;
				newbmp.WritePixel(x, y, color);

			}
			else {
				color.rgbGreen = (unsigned char)(fResult + 0.5);
				color.rgbRed = (unsigned char)(fResult + 0.5);
				color.rgbBlue = (unsigned char)(fResult + 0.5);
				newbmp.WritePixel(x, y, color);
			}
		}
	}
	tmpbmp.CopyDib(&newbmp);
	
}



void Cw07View::OnDofinalDoit()
{
	// TODO: 在此添加命令处理程序代码
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	CSize sizeimage = sizeDibDisplay_1;

	//Step1 blur
	CDib _bmp1;
	_bmp1.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	_bmp1.CopyDib(&mybmp_1);

	//2Laplace
	CDib _bmp2;
	_bmp2.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	_bmp2.CopyDib(&_bmp1);
	float aTemplate[9];			// 模板数组
	// 设置Prewitt模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 9.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;
	Template3X3(_bmp2, aTemplate, 0.4);
	double  tempC = 1.0 / 9;   //模板系数 
	CDib _bmp2_1;
	_bmp2_1.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
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
					color = _bmp2.GetPixel(x + i, y + j);
					r += color.rgbRed* tempC;
					g += color.rgbGreen* tempC;
					b += color.rgbBlue* tempC;
				}
						
			color.rgbBlue = (int)b;
			color.rgbGreen = (int)g;
			color.rgbRed = (int)r;
			_bmp2_1.WritePixel(x, y, color);
		}
	}
	_bmp2.CopyDib(&_bmp2_1);



	//Step3 sober
	CDib _bmp3;
	_bmp3.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	CDib _bmp3_1;
	_bmp3_1.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	_bmp3_1.CopyDib(&_bmp1);
	CDib _bmp3_2;
	_bmp3_2.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	_bmp3_2.CopyDib(&_bmp1);

	//	float aTemplate[9];			// 模板数组	
	// 设置Prewitt模板参数
//	float aTemplate[9];
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;
	Template3X3(_bmp3_1, aTemplate,0.7);
	
	// 设置Prewitt模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;	
	Template3X3(_bmp3_2, aTemplate,0.7);
	
	//求两幅缓存图像的最大值
	for (int j = 0; j < sizeimage.cy; j++)
	{
		for (int i = 0; i < sizeimage.cx; i++)
		{
			RGBQUAD color, color1, color2;
	
			// 指向缓存图像1第j行，第i个象素的指针	
			color1 = _bmp3_1.GetPixel(i, j);
	
			// 指向缓存图像2第j行，第i个象素的指针			
			color2 = _bmp3_2.GetPixel(i, j);
	
			if (color2.rgbRed > color1.rgbRed)
			{
				color = color2;
			}
			else
				color = color1;
	
			_bmp3.WritePixel(i, j, color);
		}
	}		

	
	//4 combine
	CDib _bmp4;
	_bmp4.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);

	for (int x = 0; x < sizeDibDisplay_output.cx ; x++)
	{
		for (int y = 0; y < sizeDibDisplay_output.cy ; y++)
		{
			RGBQUAD color;

			int c1 = 8*pow(mybmp_1.GetPixel(x,y).rgbBlue,0.7);
			int c2 = _bmp3.GetPixel(x, y).rgbBlue;
			if (c2 < 35)
			{
				c2 = 0;
			}
			
			int r = c1+c2;
			if (r > 255)
				r = 255;
			color.rgbBlue = (int)r;
			color.rgbGreen = (int)r;
			color.rgbRed = (int)r;
			_bmp4.WritePixel(x, y, color);
		}
	}

	
	
	mybmp_1.CopyDib(&_bmp1);
	mybmp_output.CopyDib(&_bmp4);
	Invalidate(true);
	
}

////Step2 add
//CDib _bmp2;
//_bmp2.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
//for (int j = 0; j < sizeimage.cy; j++)
//{
//	for (int i = 0; i < sizeimage.cx; i++)
//	{
//		RGBQUAD color, color1, color2;
//
//		// 指向缓存图像1第j行，第i个象素的指针	
//		color1 = _bmp1.GetPixel(i, j);
//
//		// 指向缓存图像2第j行，第i个象素的指针			
//		color2 = mybmp_1.GetPixel(i, j);
//
//		color.rgbBlue = color.rgbGreen = color.rgbRed = color1.rgbBlue + color2.rgbBlue;
//
//		_bmp2.WritePixel(i, j, color);
//	}
//}
//
////Step3 Sobel
//CDib _bmp3;
//_bmp3.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
//
//CDib _bmp3_1;
//_bmp3_1.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
//_bmp3_1.CopyDib(&_bmp2);
//CDib _bmp3_2;
//_bmp3_2.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
//_bmp3_2.CopyDib(&_bmp2);
//
////	float aTemplate[9];			// 模板数组
//
//// 设置Prewitt模板参数
//aTemplate[0] = -1.0;
//aTemplate[1] = -2.0;
//aTemplate[2] = -1.0;
//aTemplate[3] = 0.0;
//aTemplate[4] = 0.0;
//aTemplate[5] = 0.0;
//aTemplate[6] = 1.0;
//aTemplate[7] = 2.0;
//aTemplate[8] = 1.0;
//
//
//Template3X3(_bmp3_1, aTemplate);
//
//// 设置Prewitt模板参数
//aTemplate[0] = -1.0;
//aTemplate[1] = 0.0;
//aTemplate[2] = 1.0;
//aTemplate[3] = -2.0;
//aTemplate[4] = 0.0;
//aTemplate[5] = 2.0;
//aTemplate[6] = -1.0;
//aTemplate[7] = 0.0;
//aTemplate[8] = 1.0;
//
//
//Template3X3(_bmp3_2, aTemplate);
//
//
////求两幅缓存图像的最大值
//for (int j = 0; j < sizeimage.cy; j++)
//{
//	for (int i = 0; i < sizeimage.cx; i++)
//	{
//		RGBQUAD color, color1, color2;
//
//		// 指向缓存图像1第j行，第i个象素的指针	
//		color1 = _bmp3_1.GetPixel(i, j);
//
//		// 指向缓存图像2第j行，第i个象素的指针			
//		color2 = _bmp3_2.GetPixel(i, j);
//
//		if (color2.rgbRed > color1.rgbRed)
//		{
//			color = color2;
//		}
//		else
//			color = color1;
//
//		_bmp3.WritePixel(i, j, color);
//	}
//}
//
////Step4 5*5模糊
//CDib _bmp4;
//_bmp4.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
//
//int	temp[5][5] = { 1 };	  // 3*3模板
//double  tempC = 1.0 / 25;   //模板系数 
//
//for (int x = 0 + 2; x < sizeDibDisplay_output.cx - 2; x++)
//{
//	for (int y = 0 + 2; y < sizeDibDisplay_output.cy - 2; y++)
//	{
//		RGBQUAD color;
//
//		double r = 0;
//		double g = 0;
//		double b = 0;
//
//		for (int i = -2; i <= 2; i++)
//			for (int j = -2; j <= 2; j++)
//			{
//				color = mybmp_1.GetPixel(x + i, y + j);
//				r += color.rgbRed* tempC;
//				g += color.rgbGreen* tempC;
//				b += color.rgbBlue* tempC;
//			}
//
//		color.rgbBlue = (int)b;
//		color.rgbGreen = (int)g;
//		color.rgbRed = (int)r;
//		_bmp4.WritePixel(x, y, color);
//	}
//}
//
//CDib _bmp5;
//_bmp5.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
//for (int x = 0; x < sizeDibDisplay_output.cx; x++)
//{
//	for (int y = 0; y < sizeDibDisplay_output.cy; y++)
//	{
//		RGBQUAD color;
//		double r;
//		if (_bmp2.GetPixel(x, y).rgbBlue > 125)
//			r = _bmp4.GetPixel(x, y).rgbBlue;
//		else {
//			r = 0;
//		}
//
//
//		color.rgbBlue = (int)r;
//		color.rgbGreen = (int)r;
//		color.rgbRed = (int)r;
//		_bmp5.WritePixel(x, y, color);
//	}
//}