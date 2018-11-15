
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
	ON_COMMAND(ID_PREWITT_DOIT, &Cw07View::OnPrewittDoit)
	ON_COMMAND(ID_SOBEL_DOIT, &Cw07View::OnSobelDoit)
	ON_COMMAND(ID_LAP_DOIT, &Cw07View::OnLapDoit)
	ON_COMMAND(ID_ROBERT_DOIT, &Cw07View::OnRobertDoit)
	ON_COMMAND(ID_CANNY_DOIT, &Cw07View::OnCannyDoit)
	ON_COMMAND(ID_OTSU1_DOIT, &Cw07View::OnOtsu1Doit)
	ON_COMMAND(ID_AVGITER_DOIT, &Cw07View::OnAvgiterDoit)
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

void Cw07View::OnGray()
{

	// ��ͼ�������ֵ���б任
	// ÿ��
	for (int x = 0; x < sizeDibDisplay_1.cx; x++)
	{
		// ÿ��
		for (int y = 0; y < sizeDibDisplay_1.cy; y++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			//RGBͼ��ת�Ҷ�ͼ�� Gray = R*0.299 + G*0.587 + B*0.114
			double gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue = (int)gray;
			color.rgbGreen = (int)gray;
			color.rgbRed = (int)gray;
			mybmp_1.WritePixel(x, y, color);
		}
	}
	Invalidate();
}

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

void Cw07View::OnPrewittDoit()
{
	// TODO: �ڴ���������������
	OnGray();
	Dlg1 dlg;
	dlg.DoModal();
	float u = dlg.ref1;
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();

	CSize sizeimage = mybmp_1.GetDimensions();
	CDib tmpbmp1;
	tmpbmp1.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp_1);

	CDib tmpbmp2;
	tmpbmp2.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp2.CopyDib(&mybmp_1);

	float aTemplate[9];			// ģ������

								// ����Prewittģ�����
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	Template3X3(tmpbmp1, aTemplate, u);

	// ����Prewittģ�����
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	Template3X3(tmpbmp2, aTemplate, u);


	//����������ͼ������ֵ
	for (int j = 0; j < sizeimage.cy; j++)
	{
		for (int i = 0; i < sizeimage.cx; i++)
		{
			RGBQUAD color, color1, color2;

			// ָ�򻺴�ͼ��1��j�У���i�����ص�ָ��	
			color1 = tmpbmp1.GetPixel(i, j);

			// ָ�򻺴�ͼ��2��j�У���i�����ص�ָ��			
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
	// TODO: �ڴ���������������
	OnGray();
	Dlg1 dlg;
	dlg.DoModal();
	float u = dlg.ref1;
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();

	CSize sizeimage = mybmp_1.GetDimensions();
	CDib tmpbmp1;
	tmpbmp1.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp_1);

	CDib tmpbmp2;
	tmpbmp2.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp2.CopyDib(&mybmp_1);

	float aTemplate[9];			// ģ������

								// ����Prewittģ�����
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;


	Template3X3(tmpbmp1, aTemplate,u);

	// ����Prewittģ�����
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;


	Template3X3(tmpbmp2, aTemplate,u);


	//����������ͼ������ֵ
	for (int j = 0; j < sizeimage.cy; j++)
	{
		for (int i = 0; i < sizeimage.cx; i++)
		{
			RGBQUAD color, color1, color2;

			// ָ�򻺴�ͼ��1��j�У���i�����ص�ָ��	
			color1 = tmpbmp1.GetPixel(i, j);

			// ָ�򻺴�ͼ��2��j�У���i�����ص�ָ��			
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
	// TODO: �ڴ���������������
	OnGray();
	Dlg1 dlg;
	dlg.DoModal();
	float u = dlg.ref1;
	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();

	CSize sizeimage = mybmp_1.GetDimensions();
	CDib tmpbmp1;
	tmpbmp1.CreateCDib(sizeimage, mybmp_1.m_lpBMIH->biBitCount);
	tmpbmp1.CopyDib(&mybmp_1);
	

	float aTemplate[9];			// ģ������

								// ����Prewittģ�����
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = -1.0;
	aTemplate[4] = 9.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = -1.0;
	aTemplate[8] = -1.0;


	Template3X3(tmpbmp1, aTemplate,u);
	for (int j = 0; j < sizeimage.cy; j++)
	{
		for (int i = 0; i < sizeimage.cx; i++)
		{
			RGBQUAD color, color1;

			// ָ�򻺴�ͼ��1��j�У���i�����ص�ָ��	
			color1 = tmpbmp1.GetPixel(i, j);

			// ָ�򻺴�ͼ��2��j�У���i�����ص�ָ��			
			color.rgbGreen = 255-color1.rgbBlue;
			color.rgbBlue = 255 - color1.rgbBlue;
			color.rgbRed = 255 - color1.rgbBlue;
			mybmp_output.WritePixel(i, j, color);
		}
	}
	Invalidate(true);

}

void Cw07View::Template3X3(CDib& tmpbmp, float * fpArray,float fvalue)
{
	CDib newbmp;
	CSize sizeimage = tmpbmp.GetDimensions();
	newbmp.CreateCDib(sizeimage, tmpbmp.m_lpBMIH->biBitCount);
	
	for (int y = 1; y < sizeimage.cy - 1; y++)	// ��(��ȥ��Ե����)
	{
		for (int x = 1; x < sizeimage.cx - 1; x++)// ��(��ȥ��Ե����)
		{
			RGBQUAD color;

			double fResult = 0;

			for (int j = -1; j <= 1; j++)
			{
				for (int i = -1; i <= 1; i++)
				{
					color = tmpbmp.GetPixel(x + i, y + j);

					// ��������ֵ
					fResult += color.rgbRed * fpArray[(i + 1) * 3 + (j + 1)];
				}
			}

			fResult = fabs(fResult)*fvalue;	// ȡ����ֵ
			if (fResult > 255)
			{
				color.rgbGreen = (unsigned char)255;
				color.rgbRed = (unsigned char)255;
				color.rgbBlue = (unsigned char)255;
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

	for (int y = 2; y < sizeimage.cy - 2; y++)	// ��(��ȥ��Ե����)
	{
		for (int x = 2; x < sizeimage.cx - 2; x++)// ��(��ȥ��Ե����)
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

			fResult = fabs(fResult)*fvalue;	// ȡ����ֵ
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

void Cw07View::OnRobertDoit()
{
	// TODO: �ڴ���������������
	OnGray();
	Dlg1 dlg;
	dlg.DoModal();
	int thresh =(int) dlg.ref1;

	mybmp_output.CreateCDib(CSize(sizeDibDisplay_1.cx, sizeDibDisplay_1.cy), 24);
	sizeDibDisplay_output = mybmp_output.GetDimensions();
	for (int i = 0; i < sizeDibDisplay_1.cy - 1; i++)
	{
		for (int j = 0; j < sizeDibDisplay_1.cx - 1; j++)
		{
			RGBQUAD color1, color2, color3, color;
			// ָ��DIB��i�У���j�����ص�ָ��
			color1 = mybmp_1.GetPixel(j, i);

			// ָ��DIB��i+1�У���j�����ص�ָ��
			color2 = mybmp_1.GetPixel(j, i + 1);

			// ָ��DIB��i�У���j+1�����ص�ָ��
			color3 = mybmp_1.GetPixel(j + 1, i);

			int bTemp = abs(color1.rgbBlue - color2.rgbBlue) + abs(color1.rgbBlue - color3.rgbBlue);

			// �ж��Ƿ�С����ֵ
			// �ж��Ƿ������ֵ������С��������Ҷ�ֵ���䡣
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


void Cw07View::OnCannyDoit()
{
	// TODO: �ڴ���������������
	//finised in open_cv
}


void Cw07View::OnOtsu1Doit()
{
	// TODO: �ڴ���������������
	int nWidth = sizeDibDisplay_1.cx;
	int nHeight = sizeDibDisplay_1.cy;
	sizeDibDisplay_output = sizeDibDisplay_1;
	mybmp_output.CreateCDib(sizeDibDisplay_1, mybmp_1.m_lpBMIH->biBitCount);

	int thresholdValue = 1;   // ��ֵ
	int ihist[256];       // ͼ��ֱ��ͼ��256����

	int i, j, k; // various counters
	int n, n1, n2, gmin, gmax;

	double m1, m2, sum, csum, fmax, sb;

	// ��ֱ��ͼ����
	memset(ihist, 0, sizeof(ihist));

	gmin = 255; gmax = 0;

	// ����ֱ��ͼ
	for (i = 0; i < nWidth; i++)
		for (j = 0; j < nHeight; j++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(i, j);
			int gray = color.rgbRed;
			ihist[gray]++;
			if (gmax<gray) gmax = gray;
			if (gmin>gray) gmin = gray;
		}

	sum = csum = 0.0;
	n = 0;

	for (i = 0; i <= 255; i++)
	{
		sum += (double)i * (double)ihist[i];
		n += ihist[i];
	}

	if (!n)
	{
		thresholdValue = 160;
		goto L;
	}

	// do the otsu global thresholding method
	fmax = -1.0;
	n1 = 0;
	for (k = 0; k <255; k++)
	{
		n1 += ihist[k];

		if (!n1)  continue;
		n2 = n - n1;

		if (n2 == 0)  break;

		csum += k * ihist[k];
		m1 = csum / n1;
		m2 = (sum - csum) / n2;
		sb = n1 * n2 *(m1 - m2) * (m1 - m2);

		if (sb > fmax)
		{
			fmax = sb;
			thresholdValue = k;
		}
	}

L:
	for (int y = 0; y<nHeight; y++)
	{
		// x����
		for (int x = 0; x<nWidth; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			if (color.rgbBlue < thresholdValue)
			{
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
			}
			else
			{
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
			}
			mybmp_output.WritePixel(x, y, color);
		}
	}

	Invalidate();

}


void Cw07View::OnAvgiterDoit()
{
	// TODO: �ڴ���������������
	Dlg1 dlg;
	dlg.DoModal();
	int subthd = (int)dlg.ref1;

	//ͼ��ĳ����С
	int nWidth = sizeDibDisplay_1.cx;
	int nHeight = sizeDibDisplay_1.cy;

	//1,��ʼ��
	double	avg = 0.0;// ͼ���ƽ��ֵ
	for (int y = 0; y<nHeight; y++)
	{
		for (int x = 0; x<nWidth; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			avg += color.rgbBlue;
		}
	}
	double T = 0;
	T = avg / (nHeight * nWidth);//ѡ��һ����ʼ������ֵT (ͨ��ȡ�Ҷ�ֵ��ƽ��ֵ)


								 //2,ʹ����ֵT ��ͼ������ط�Ϊ������: G1�����Ҷ��������T�� G2�����Ҷ�����С��T
								 //  ����G1���������صľ�ֵ��1���Լ�G2���������صľ�ֵ��2
	double curThd = T;
	double preThd = curThd;
	int y = 0;
	do
	{
		preThd = curThd;
		double u1 = 0, u2 = 0;
		int num_u1 = 0, num_u2 = 0;
		for (y = 0; y<nHeight; y++)
		{
			for (int x = 0; x<nWidth; x++)
			{
				RGBQUAD color;
				color = mybmp_1.GetPixel(x, y);
				if (color.rgbBlue < preThd)
				{
					u1 += color.rgbBlue;
					num_u1++;
				}
				else
				{
					u2 += color.rgbBlue;
					num_u2++;
				}
			}
		}
		curThd = (u1 / num_u1 + u2 / num_u2) / 2;
	} while (fabs(curThd - preThd) > subthd);

	mybmp_output.CreateCDib(sizeDibDisplay_1, mybmp_1.m_lpBMIH->biBitCount);
	sizeDibDisplay_output = sizeDibDisplay_1;
	for (y = 0; y<nHeight; y++)
	{
		// x����
		for (int x = 0; x<nWidth; x++)
		{
			RGBQUAD color;
			color = mybmp_1.GetPixel(x, y);
			if (color.rgbBlue < curThd)
			{
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
			}
			else
			{
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;
			}
			mybmp_output.WritePixel(x, y, color);
		}
	}
	Invalidate();

}
