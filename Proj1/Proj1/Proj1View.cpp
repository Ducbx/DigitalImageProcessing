
// Proj1View.cpp : implementation of the CProj1View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Proj1.h"
#endif

#include "Proj1Doc.h"
#include "Proj1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProj1View

IMPLEMENT_DYNCREATE(CProj1View, CView)

BEGIN_MESSAGE_MAP(CProj1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CProj1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPENIMAGE1, &CProj1View::OnFileOpenimage1)
	ON_COMMAND(ID_FILE_OPENIMAGE2, &CProj1View::OnFileOpenimage2)
	ON_COMMAND(ID_FILE_EXIT, &CProj1View::OnFileExit)
	ON_COMMAND(ID_MAP_BINARYPROCESS, &CProj1View::OnMapBinaryprocess)
	ON_COMMAND(ID_MAPPING_CONFIRM, &CProj1View::OnMappingConfirm)
	ON_COMMAND(ID_MAPPING_SAVE, &CProj1View::OnMappingSave)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CProj1View construction/destruction

CProj1View::CProj1View()
{
	// TODO: add construction code here
	m_tracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeInside | CRectTracker::hatchedBorder;
	m_tracker.m_nHandleSize = 6;
	pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_tracker.m_rect.SetRect(0, 0, 0, 0);
	state = 1;
	sure = false;
}

CProj1View::~CProj1View()
{
}

BOOL CProj1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CProj1View drawing

void CProj1View::OnDraw(CDC* pDC)
{
	CProj1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_origin2 = m_origin1.x + sizeimage1.cx + 10;
	m_origin2.y = 0;

	if (state == 1)
	{
		mybmp1.Draw(pDC, m_origin1, sizeimage1);
		mybmp2.Draw(pDC, m_origin2, sizeimage2);
	}
	else {
		tmp1.Draw(pDC, m_origin1, sizeimage1);
		tmp2.Draw(pDC, m_origin2, sizeimage2);
	}

	if (sure == false&&state!=1)
	{
		CClientDC dc(this);
		m_tracker.Draw(&dc, &pen);
	}

	// TODO: add draw code for native data here
}


// CProj1View printing


void CProj1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CProj1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CProj1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CProj1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CProj1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CProj1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CProj1View diagnostics

#ifdef _DEBUG
void CProj1View::AssertValid() const
{
	CView::AssertValid();
}

void CProj1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProj1Doc* CProj1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProj1Doc)));
	return (CProj1Doc*)m_pDocument;
}
#endif //_DEBUG


// CProj1View message handlers


void CProj1View::OnFileOpenimage1()
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
		if (!mybmp1.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	if (mybmp1.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeimage1 = mybmp1.GetDimensions();
	m_origin1.x = 0;
	m_origin1.y = 0;
	tmp1.CreateCDib(sizeimage1, mybmp1.m_lpBMIH->biBitCount);

	m_tracker.m_rect.SetRect(sizeimage1.cx / 4, sizeimage1.cy / 4, sizeimage1.cx * 3 / 4, sizeimage1.cy * 3 / 4);
	start1.x = sizeimage1.cx / 4;
	start1.y = sizeimage1.cy / 4;
	end1.x = sizeimage1.cx * 3 / 4;
	end1.y = sizeimage1.cy * 3 / 4;

	CClientDC dc(this);
	m_tracker.Draw(&dc, &pen);

	Invalidate();
}


void CProj1View::OnFileOpenimage2()
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
		if (!mybmp2.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
	if (mybmp2.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeimage2 = mybmp2.GetDimensions();
	tmp2.CreateCDib(sizeimage2, mybmp2.m_lpBMIH->biBitCount);
	Invalidate();
}


void CProj1View::OnFileExit()
{
	// TODO: Add your command handler code here
	PostQuitMessage(0);
}


void CProj1View::OnMapBinaryprocess()
{
	// TODO: Add your command handler code here
	int Xmin = 1000,Ymin = 1000;
	int Xmax = 0, Ymax = 0;
	nThreshold = 100;

	//��dlg��ȡ��threshold
	ThresDlg ddlg;
	ddlg.DoModal();
	CString cstr;
	nThreshold = ddlg.ref1;
	cstr.Format("Threshold is %d\n", ddlg.ref1);
	MessageBox(cstr);

	//��ֵ��������ģ
	tmp2.CreateCDib(sizeimage2, mybmp2.m_lpBMIH->biBitCount);
	for (int x = 0; x < sizeimage2.cx; x++)
	{
		for (int y = 0; y < sizeimage2.cy; y++)
		{
			RGBQUAD color;
			color = mybmp2.GetPixel(x, y);
			double gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*.114;

			if (gray < nThreshold)
			{
				color.rgbBlue = 0;
				color.rgbGreen = 0;
				color.rgbRed = 0;
			}
			else {
				color.rgbBlue = 255;
				color.rgbGreen = 255;
				color.rgbRed = 255;

				if (x < Xmin) Xmin = x;
				if (x > Xmax) Xmax = x;
				if (y < Ymin) Ymin = y;
				if (y > Ymax) Ymax = y;
			}
			tmp2.WritePixel(x, y, color);
		}

		start2.x = Xmin;
		start2.y = Ymin;
		end2.x = Xmax;
		end2.y = Ymax;
		tmp1.CopyDib(&mybmp1);
		state = 2;
		Invalidate();
	}
}


void CProj1View::OnMappingConfirm()
{
	// TODO: Add your command handler code here
	sure = true;
	Invalidate(false);
}


void CProj1View::OnMappingSave()
{
	// TODO: Add your command handler code here
	tmp1.Save("result.bmp");
}


void CProj1View::OnLButtonDown(UINT nFlags, CPoint point)
{

	if (sure)
		return;

	//int ret=m_RectTracker.HitTest(point);
	int nIn;                         //����һ�����ĵ��ֵ��
	nIn = m_tracker.HitTest(point);       //�����㵽������

	CRect Prect1;        //����ͼƬ�ľ���
	CRect Trect;          //������Ƥ���ľ���
	CRect Prect;         //ͼƬ���ο�

	Prect1.left = m_origin1.x;  //�õ�ͼƬ�ľ��δ�С//   GetDlgItem(IDC_PICTURE)->GetWindowRect(&Prect1);  
	Prect1.top = m_origin1.y;
	Prect1.right = sizeimage1.cx;
	Prect1.bottom = sizeimage1.cy;

	ScreenToClient(&Prect1);   //��ͼƬ��ľ��Ծ��δ�С

	if (point.x<Prect1.left || point.x>Prect1.right || point.y<Prect1.top || point.y>Prect1.bottom)
	{
		//UpdateImage();
		//	Invalidate(false);
		return;	//�ж��Ƿ���ͼƬ���ڣ���������ͼƬ���ڵĵ��
	}

	m_tracker.SetCursor(this, nFlags); //�ı�������״
	if (nIn<0)                 //�����ı��������ڣ�
	{
		Invalidate(true);
		m_tracker.TrackRubberBand(this, point, false);   //���ھ��ο�������Ƥ���    

		Prect.left = m_origin1.x;//  GetDlgItem(IDC_PICTURE)->GetWindowRect(&Prect); //�õ�ͼƬ��ľ���
		Prect.top = m_origin1.y;
		Prect.right = sizeimage1.cx;
		Prect.bottom = sizeimage1.cy;

		ScreenToClient(&Prect);
		Trect = m_tracker.m_rect;   //�õ����õ���Ƥ���

									//������С
		Trect.top = (Trect.top<Prect.top ? Prect.top : Trect.top);
		Trect.left = (Trect.left<Prect.left ? Prect.left : Trect.left);
		Trect.bottom = (Trect.bottom>Prect.bottom ? Prect.bottom : Trect.bottom);
		Trect.right = (Trect.right>Prect.right ? Prect.right : Trect.right);

		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom); //���������õľ��ο�

		CClientDC dc(this);

		UpdateImage();
		m_tracker.Draw(&dc, &pen);

		Invalidate(false);
	}
	else //���ı��������ڣ�       
	{
		Invalidate(); //�ػ�����
		CClientDC dc(this);
		Prect.left = m_origin1.x; //  GetDlgItem(IDC_PICTURE)->GetWindowRect(&Prect); 
		Prect.top = m_origin1.y;
		Prect.right = sizeimage1.cx;
		Prect.bottom = sizeimage1.cy;

		ClipCursor(&Prect); //api�����������������ͼƬ����     

		UpdateImage();
		m_tracker.Draw(&dc, &pen);
		Invalidate(false);
		m_tracker.Track(this, point, false);

		ScreenToClient(&Prect);
		Trect = m_tracker.m_rect;   //�õ����õ���Ƥ���


		if (Trect.top<Prect.top)   //�������ο��λ��
		{
			//����ͼƬ�򶥲���λ��
			Trect.bottom = Prect.top - Trect.top + Trect.bottom;
			Trect.top = Prect.top;
		}
		if (Trect.bottom>Prect.bottom)
		{
			//�����ײ���λ��
			Trect.top = Prect.bottom - Trect.bottom + Trect.top;
			Trect.bottom = Prect.bottom;
		}
		if (Trect.right>Prect.right)
		{
			//�����ұ�
			Trect.left = Prect.right - Trect.right + Trect.left;
			Trect.right = Prect.right;
		}
		if (Trect.left<Prect.left)
		{
			//�������
			Trect.right = Prect.left - Trect.left + Trect.right;
			Trect.left = Prect.left;
		}
		//���þ��ο��С
		m_tracker.m_rect.SetRect(Trect.left, Trect.top, Trect.right, Trect.bottom);
		UpdateImage();
		m_tracker.Draw(&dc, &pen);
		Invalidate(false);
		ClipCursor(NULL);   //�ͷŶ���������
	}
	CView::OnLButtonDown(nFlags, point);


}


BOOL CProj1View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	//�������ͼ��
	if (pWnd == this && m_tracker.SetCursor(this, nHitTest))
		return TRUE;
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CProj1View::UpdateImage()
{
	//Initial
	
	start1.x = m_tracker.m_rect.left;
	start1.y = m_tracker.m_rect.top;
	end1.x = m_tracker.m_rect.right;
	end1.y = m_tracker.m_rect.top + m_tracker.m_rect.Height();
	tmp1.CopyDib(&mybmp1);
	//�������
	float xScale = (end1.x - start1.x + 1)*1.0 / (end2.x - start2.x + 1);
	float yScale= (end1.y - start1.y + 1)*1.0 / (end2.y - start2.y + 1);
	//����tracker��ͼ��ӳ�䵽tmp1��
	ScaleImage(xScale, yScale);
	state = 3;
}

void CProj1View::ScaleImage(float fXZoomRatio, float fYZoomRatio)
{
	CSize NewSize;
	NewSize.cx = (end2.x - start2.x + 1)*fXZoomRatio;
	NewSize.cy = (end2.y - start2.y + 1)*fYZoomRatio;
	RGBQUAD color;
	//˫���Բ�ֵ
	for (int y = 0; y < NewSize.cy; y++)
	{
		for (int x = 0; x < NewSize.cx; x++)
		{
			float cx = x / fXZoomRatio;
			float cy = y / fYZoomRatio;
			//Խ�紦��
			if (((int)(cx)-1) >= 0 && ((int)(cx)+1) < end2.x-start2.x && ((int)(cy)-1) >= 0 && ((int)(cy)+1) < end2.y - start2.y)
			{
				//f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)
				float u = cx - (int)cx;
				float v = cy - (int)cy;
				int i = (int)cx+start2.x;
				int j = (int)cy+start2.y;
				RGBQUAD _color;
				_color = tmp2.GetPixel(cx + 0.5 + start2.x, cy + 0.5 + start2.y);
				//��ģ�ж�
				if (_color.rgbRed == 255 && _color.rgbGreen == 255 && _color.rgbBlue == 255)
				{
					int red;
					red = (1 - u)*(1 - v)*mybmp2.GetPixel(i, j).rgbRed + (1 - u)*v*mybmp2.GetPixel(i, j + 1).rgbRed
						+ u*(1 - v)*mybmp2.GetPixel(i + 1, j).rgbRed + u*v*mybmp2.GetPixel(i + 1, j + 1).rgbRed;
					int green;
					green = (1 - u)*(1 - v)*mybmp2.GetPixel(i, j).rgbGreen + (1 - u)*v*mybmp2.GetPixel(i, j + 1).rgbGreen
						+ u*(1 - v)*mybmp2.GetPixel(i + 1, j).rgbGreen + u*v*mybmp2.GetPixel(i + 1, j + 1).rgbGreen;
					int blue;
					blue = (1 - u)*(1 - v)*mybmp2.GetPixel(i, j).rgbBlue + (1 - u)*v*mybmp2.GetPixel(i, j + 1).rgbBlue
						+ u*(1 - v)*mybmp2.GetPixel(i + 1, j).rgbBlue + u*v*mybmp2.GetPixel(i + 1, j + 1).rgbBlue;

					color.rgbGreen = green;
					color.rgbRed = red;
					color.rgbBlue = blue;
					tmp1.WritePixel(x+start1.x, y+start1.y, color);
				}
			}
		}
	}


	Invalidate();
}