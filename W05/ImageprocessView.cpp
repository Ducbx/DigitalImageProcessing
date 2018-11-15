// ImageprocessView.cpp : implementation of the CImageprocessView class
//

#include "stdafx.h"
#include "Imageprocess.h"

#include "ImageprocessDoc.h"
#include "ImageprocessView.h"
#include "Dialog1.h"
#include "math.h"
#include "Dialog2.h"
#include "Dialog3.h"
#include "Dialog4.h"
#include "Dialog5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 3.14159265359
 
/////////////////////////////////////////////////////////////////////////////
// CImageprocessView

IMPLEMENT_DYNCREATE(CImageprocessView, CView)

BEGIN_MESSAGE_MAP(CImageprocessView, CView)
	//{{AFX_MSG_MAP(CImageprocessView)
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_COMMAND(ID_Gray, OnGray)
	ON_COMMAND(ID_FFT, OnFft)
	ON_COMMAND(ID_IFFT, OnIfft)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_ROTATE_ROTATE30, &CImageprocessView::OnRotateRotate30)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView construction/destruction

CImageprocessView::CImageprocessView()
{
	flagfft = 0;

}

CImageprocessView::~CImageprocessView()
{
}

BOOL CImageprocessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView drawing

void CImageprocessView::OnDraw(CDC* pDC)
{
	CImageprocessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if (flagfft == 2)
	{
		mybmp.Draw(pDC, CPoint(0, 0), mybmp.GetDimensions());
		mybmp_raotate.Draw(pDC, CPoint(mybmp.GetDimensions().cx + 10, 0), mybmp_raotate.GetDimensions());
		bmpfft.Draw(pDC,CPoint(mybmp.GetDimensions().cx+ mybmp_raotate.GetDimensions().cx+20,0),fftsize);
	}
	else if (flagfft == 1)
	{
		mybmp.Draw(pDC, CPoint(0, 0), mybmp.GetDimensions());
		mybmp_raotate.Draw(pDC, CPoint(mybmp.GetDimensions().cx + 10, 0), mybmp_raotate.GetDimensions());
	}
	else if (flagfft==3)
		bmpifft.Draw(pDC,CPoint(0,0),fftsize);
	else 
		mybmp.Draw(pDC,CPoint(0,0), mybmp.GetDimensions());
	
}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView printing

BOOL CImageprocessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageprocessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageprocessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView diagnostics

#ifdef _DEBUG
void CImageprocessView::AssertValid() const
{
	CView::AssertValid();
}

void CImageprocessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageprocessDoc* CImageprocessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageprocessDoc)));
	return (CImageprocessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView message handlers

void CImageprocessView::OnOpen() 
{
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp) |*.bmp|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
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
	sizeimage=mybmp.GetDimensions();
	Invalidate();
}

void CImageprocessView::OnGray() 
{

	// ��ͼ�������ֵ���б任
	// ÿ��
	for(int x = 0; x < sizeimage.cx; x++)
	{
		// ÿ��
		for(int y = 0; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x,y);
			//RGBͼ��ת�Ҷ�ͼ�� Gray = R*0.299 + G*0.587 + B*0.114
		    double gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue  = (int)gray;
			color.rgbGreen = (int)gray;
			color.rgbRed   = (int)gray;
			mybmp.WritePixel(x, y,color);
		}
	}
	Invalidate();
}



	
	
	
	
	
	
	


/************************************************************************* 
 * 
 * \�������ƣ� 
 *   FFT_2D() 
 * 
 * \�������: 
 *   complex<double> * pCTData - ͼ������ 
 *   int    nWidth - ���ݿ�� 
 *   int    nHeight - ���ݸ߶� 
 *   complex<double> * pCFData - ����Ҷ�任��Ľ�� 
 * 
 * \����ֵ: 
 *   �� 
 * 
 * \˵��: 
 *   ��ά����Ҷ�任�� 
 * 
 ************************************************************************ 
 */ 
void  CImageprocessView::FFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{ 

	int x,y;
	int nTransWidth =nWidth; 
	int nTransHeight =nHeight; 

	// x��y�����У������ϵĵ������� 
	int nXLev; 
	int nYLev; 

/*	while(nWidth * 2 <= sizeimage.cx)
	{
		nWidth *= 2;
		nYLev++;
	}	
	while(nHeight * 2 <= sizeimage.cy)
	{
		nHeight *= 2;
		nXLev++;
	}*/
	// ����x��y�����У������ϵĵ������� 
    nXLev = (int) ( log(nTransWidth)/log(2) +  0.5 ); 
	nYLev = (int) ( log(nTransHeight)/log(2) + 0.5 ); 

	for(y = 0; y < nTransHeight; y++) 
	{ 
		// y������п��ٸ���Ҷ�任 
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev); 
	} 

	// pCFData��Ŀǰ�洢��pCTData�����б任�Ľ�� 
	// Ϊ��ֱ������FFT_1D����Ҫ��pCFData�Ķ�ά����ת�ã���һ������FFT_1D���� 
	// ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任�� 
	for(y = 0; y < nTransHeight; y++) 
	{ 
		for(x = 0; x < nTransWidth; x++) 
		{ 
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x]; 
		} 
	} 

	for(y = 0; y < nTransWidth; y++) 
	{ 
		// ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з���� 
		// ����Ҷ�任 
		FFT_1D(&pCTData[y* nTransHeight], &pCFData[y * nTransHeight], nYLev); 
	} 

	// pCFData��Ŀǰ�洢��pCTData������ά����Ҷ�任�Ľ��������Ϊ�˷����з��� 
	// �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û��� 
	for(y = 0; y < nTransHeight; y++) 
	{ 
		for(x = 0; x < nTransWidth; x++) 
		{ 
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y]; 
		} 
	} 

	memcpy(pCTData, pCFData, sizeof(complex<double>) * nTransHeight * nTransWidth ); 
} 

/************************************************************************* 
 * 
 * \�������ƣ� 
 *   IFFT_2D() 
 * 
 * \�������: 
 *   complex<double> * pCFData - Ƶ������ 
 *   complex<double> * pCTData - ʱ������ 
 *   int    nWidth - ͼ�����ݿ�� 
 *   int    nHeight - ͼ�����ݸ߶� 
 * 
 * \����ֵ: 
 *   �� 
 * 
 * \˵��: 
 *   ��ά����Ҷ���任�� 
 * 
 ************************************************************************ 
 */ 
void CImageprocessView::IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{ 
	// ѭ�����Ʊ��� 
	int x; 
	int y; 

	int nTransWidth =nWidth; 
	int nTransHeight =nHeight; 

	// ���乤����Ҫ���ڴ�ռ� 
	complex<double> *pCWork= new complex<double>[nTransWidth * nTransHeight]; 

	//��ʱ���� 
	complex<double> *pCTmp ; 

	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ���� 
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ��� 
	for(y = 0; y < nTransHeight; y++) 
	{ 
		for(x = 0; x < nTransWidth; x++) 
		{ 
			pCTmp = &pCFData[nTransWidth * y + x] ; 
			pCWork[nTransWidth * y + x] = complex<double>( pCTmp->real() , -pCTmp->imag() ); 
		} 
	} 

	// ���ø���Ҷ���任 
	FFT_2D(pCWork, nWidth, nHeight, pCTData) ; 

	// ��ʱ���Ĺ��������ս�� 
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������ 
	// ���һ��ϵ�� 
	for(y = 0; y < nTransHeight; y++) 
	{ 
		for(x = 0; x < nTransWidth; x++) 
		{ 
			pCTmp = &pCTData[nTransWidth * y + x] ; 
			pCTData[nTransWidth * y + x] =  
			complex<double>( pCTmp->real()/(nTransWidth*nTransHeight), 
			 -pCTmp->imag()/(nTransWidth*nTransHeight) ); 
		} 
	} 
		delete pCWork ; 
		pCWork = NULL ; 
} 
/************************************************************************* 
 * 
 * \�������ƣ� 
 *   FFT_1D() 
 * 
 * \�������: 
 *   complex<double> * pCTData - ָ��ʱ�����ݵ�ָ�룬�������Ҫ�任������ 
 *   complex<double> * pCFData - ָ��Ƶ�����ݵ�ָ�룬����ľ����任������ 
 *   nLevel ������Ҷ�任�����㷨�ļ�����2�������� 
 * 
 * \����ֵ: 
 *   �� 
 * 
 * \˵��: 
 *   һά���ٸ���Ҷ�任�� 
 * 
 ************************************************************************* 
 */ 

void CImageprocessView::FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel) 
{ 



	// ѭ�����Ʊ��� */
	int i; 
	int     j; 
	int     k; 

	//double PI = 3.1415926;  

	// ����Ҷ�任���� 
	int nCount =0 ; 

	// ���㸵��Ҷ�任���� 
	nCount =1<<nLevel; 

	// ĳһ���ĳ��� 
	int nBtFlyLen; 
	nBtFlyLen = 0 ; 

	// �任ϵ���ĽǶ� ��2 * PI * i / nCount 
	double dAngle; 

	complex<double> *pCW ; 

	// �����ڴ棬�洢����Ҷ�仯��Ҫ��ϵ���� 
	pCW  = new complex<double>[nCount / 2]; 

	// ���㸵��Ҷ�任��ϵ�� 
	for(i = 0; i < nCount / 2; i++) 
	{ 
		dAngle = -2 * 3.141592 * i / nCount; 
		pCW[i] = complex<double> ( cos(dAngle), sin(dAngle) ); 
	} 

	// �任��Ҫ�Ĺ����ռ� 
	complex<double> *pCWork1,*pCWork2;  

	// ���乤���ռ� 
	pCWork1 = new complex<double>[nCount]; 

	pCWork2 = new complex<double>[nCount]; 


	// ��ʱ���� 
	complex<double> *pCTmp; 

	// ��ʼ����д������ 
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount); 

	// ��ʱ���� 
	int nInter;  
	nInter = 0; 

	// �����㷨���п��ٸ���Ҷ�任 
	for(k = 0; k < nLevel; k++) 
	{ 
		for(j = 0; j < (int)pow(2,k); j++) 
		{ 
			//���㳤�� 
			nBtFlyLen = (int)pow( 2,(nLevel-k) ); 

			//�������ţ���Ȩ���� 
			for(i = 0; i < nBtFlyLen/2; i++) 
			{ 
				nInter = j * nBtFlyLen; 
				pCWork2[i + nInter] =  
				pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2]; 
				pCWork2[i + nInter + nBtFlyLen / 2] = 
				(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2])  
				* pCW[(int)(i * pow(2,k))]; 
			} 
	} 

	// ���� pCWork1��pCWork2������ 
	pCTmp   = pCWork1 ; 
	pCWork1 = pCWork2 ; 
	pCWork2 = pCTmp ; 
	} 

	// �������� 
	for(j = 0; j < nCount; j++) 
	{ 
		nInter = 0; 
		for(i = 0; i < nLevel; i++) 
		{ 
			if ( j&(1<<i) ) 
			{ 
				nInter += 1<<(nLevel-i-1); 
			} 
		} 
		pCFData[j]=pCWork1[nInter]; 
	} 

	// �ͷ��ڴ�ռ� 
	delete pCW; 
	delete pCWork1; 
	delete pCWork2; 
	pCW = NULL ; 
	pCWork1 = NULL ; 
	pCWork2 = NULL ; 
	

} 
/************************************************************************* 
 
 * \�������ƣ� 
 *    IFFT_1D() 
 * 
 * \�������: 
 *   complex<double> * pCTData - ָ��ʱ�����ݵ�ָ�룬�������Ҫ���任������ 
 *   complex<double> * pCFData - ָ��Ƶ�����ݵ�ָ�룬����ľ������任������ 
 *   nLevel ������Ҷ�任�����㷨�ļ�����2�������� 
 * 
 * \����ֵ: 
 *   �� 
 * 
 * \˵��: 
 *   һά���ٸ���Ҷ���任�� 
 * 
 ************************************************************************ 
 */ 
void CImageprocessView::IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel) 
{ 

	// ѭ�����Ʊ��� 
	int i; 

	// ����Ҷ���任���� 
	int nCount; 

	// ���㸵��Ҷ�任���� 
	//nCount = (int)pow(2,nLevel) ; 
     nCount=1<<nLevel;
	// �任��Ҫ�Ĺ����ռ� 
	complex<double> *pCWork; 

	// ���乤���ռ� 
	pCWork = new complex<double>[nCount]; 

	// ����Ҫ���任������д�빤���ռ�pCWork 
	memcpy(pCWork, pCFData, sizeof(complex<double>) * nCount); 

	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ���� 
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ��� 
	for(i = 0; i < nCount; i++) 
	{ 
		pCWork[i] = complex<double> (pCWork[i].real(), -pCWork[i].imag()); 
	} 

	// ���ÿ��ٸ���Ҷ�任ʵ�ַ��任������洢��pCTData�� 
	FFT_1D(pCWork, pCTData, nLevel); 

	// ��ʱ���Ĺ��������ս�� 
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������ 
	// ���һ��ϵ��nCount 
	for(i = 0; i < nCount; i++) 
	{ 
		pCTData[i] =  
		complex<double> (pCTData[i].real() / nCount, -pCTData[i].imag() / nCount); 
	} 

	// �ͷ��ڴ� 
	delete pCWork; 
	pCWork = NULL; 
} 

//////////////////////////////////////////////////////////////
//�ú�������ʵ�ֶ�ά����Ҷ�任
//����height��width�ֱ��ʾͼ��ĸ߶ȺͿ�ȣ�ising��ʾ�����任
//////////////////////////////////////////////////////////////
void CImageprocessView::fourier( complex<double> * data, int nTransHeight, int nTransWidth, int isign)
{


	complex<double> *pCTData = new complex<double>[nTransHeight*nTransWidth];
	complex<double> *pCFData = new complex<double>[nTransHeight*nTransWidth];

	if(isign == 1)
	{
		for(int j = 0; j < nTransHeight; j++)
			for(int i = 0; i < nTransWidth; i++)
			{
				pCTData[j*nTransWidth+i] = data[j*nTransWidth+i];
			}
	FFT_2D(pCTData, nTransWidth, nTransHeight, pCFData);

		for(int i = 0; i < nTransWidth; i++)
			for(int j = 0; j < nTransHeight; j++)
				data[j*nTransWidth+i]= pCFData[j*nTransWidth+i];
			
	}
	else
		if(isign == -1)
		{
			for(int j = 0; j < nTransHeight; j++)
			for(int i = 0; i < nTransWidth; i++)
			{
				pCFData[j*nTransWidth+i] = data[j*nTransWidth+i];
			}
			IFFT_2D(pCFData, pCTData, nTransWidth, nTransHeight);
			for(int i = 0; i < nTransWidth; i++)
			for(int j = 0; j < nTransHeight; j++)
			{
				data[j*nTransWidth+i] = pCTData[j*nTransWidth+i];
			}
		}

	
}




void CImageprocessView::OnFft() 
{

	int width = sizeimage.cx;
	int height = sizeimage.cy;   
	int i,j;


	double dTmpOne; // ��ʱ���� 
	double  dTmpTwo; 


	int nTransWidth; 
	int  nTransHeight; 

	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width)/log(2); 
	dTmpTwo = ceil(dTmpOne)    ; 
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransWidth = (int)dTmpTwo    ; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height)/log(2); 
	dTmpTwo = ceil (dTmpOne)    ;
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransHeight = (int)dTmpTwo   ; 
	
	complex<double> *t=new complex<double>[nTransHeight*nTransWidth];
	

	fftsize = CSize(nTransWidth,nTransHeight);

	bmpfft.CreateCDib(fftsize,mybmp_raotate.m_lpBMIH->biBitCount);

	for(j=0;j<nTransHeight;j++)
	   for(i=0;i<nTransWidth;i++)
		{
		
			int position = j*nTransWidth+i;
				if(i<width &&j<height)
				{
					RGBQUAD color;
					color = mybmp_raotate.GetPixel(i,j);

					t[position]=complex<double>(color.rgbRed,0.0);  //����ǿ��ֵ
					
				}
				else
				{
					t[position]=complex<double>(0.0,0.0);
				}
	   }

	//����Ҷ�任
    fourier(t,nTransHeight,nTransWidth,1);


	double *tmp = new double[nTransWidth * nTransHeight];


	for(int x=0;x<nTransWidth;x++)
	for(int y=0;y<nTransHeight;y++)
	{
		RGBQUAD color;
		int pos = y*nTransWidth +x;

		double dTemp = sqrt(t[pos].real() * t[pos].real() + 
				         t[pos].imag() * t[pos].imag()) / 100;
	 
		int gray ;	

		if (dTemp > 255)
		{
			gray = 255;
		}
		else
		   gray = (int)dTemp;
					

		color.rgbGreen = gray;
		color.rgbRed  = gray;
		color.rgbBlue = gray; 

		bmpfft.WritePixel(x<nTransWidth/2 ? x+nTransWidth/2 : x-nTransWidth/2,y<nTransHeight/2 ? y+nTransHeight/2 : y-nTransHeight/2, color);
		
	}


	flagfft =2;
	
	Invalidate();
	
}

void CImageprocessView::OnIfft() 
{


		int width = sizeimage.cx;
	int height = sizeimage.cy;   
	int i,j;


	double dTmpOne; // ��ʱ���� 
	double  dTmpTwo; 


	// ������и���Ҷ�任�Ŀ�� ��2���������ݣ� 
	dTmpOne = log(width)/log(2); 
	dTmpTwo = ceil(dTmpOne)    ; 
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransWidth = (int)dTmpTwo    ; // 

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ� 
	dTmpOne = log(height)/log(2); 
	dTmpTwo = ceil (dTmpOne)    ;
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransHeight = (int)dTmpTwo   ; 
	
	complex<double> *t=new complex<double>[nTransHeight*nTransWidth];
	

	fftsize = CSize(nTransWidth,nTransHeight);

	bmpfft.CreateCDib(fftsize, mybmp_raotate.m_lpBMIH->biBitCount);

	for(j=0;j<nTransHeight;j++)
	   for(i=0;i<nTransWidth;i++)
		{
		
			int position = j*nTransWidth+i;
				if(i<width &&j<height)
				{
					RGBQUAD color;
					color = mybmp_raotate.GetPixel(i,j);

					t[position]=complex<double>(color.rgbRed,0.0);  //����ǿ��ֵ
					
				}
				else
				{
					t[position]=complex<double>(0.0,0.0);
				}
	   }

	//����Ҷ�任
    fourier(t,nTransHeight,nTransWidth,1);

		//����Ҷ���任
	fourier(t,nTransHeight,nTransWidth,-1);
	

	bmpifft.CreateCDib(fftsize, mybmp_raotate.m_lpBMIH->biBitCount);

	double max=0.0;
		for(j=0;j<nTransHeight;j++)
	  for(i=0;i<nTransWidth;i++)
		{
		  if(max<t[nTransWidth*j+i].real())
				max=t[nTransWidth*j+i].real();
		}


	for(j=0;j<nTransHeight;j++)
	{
		for(i=0;i<nTransWidth;i++)
		{
			if(i<width &&j<height)
			{

				int gray=sqrt(t[nTransWidth*j+i].real()*t[nTransWidth*j+i].real()+t[nTransWidth*j+i].imag()*t[nTransWidth*j+i].imag());
				RGBQUAD color;
				color.rgbBlue  = gray*255/max;
				color.rgbGreen = gray*255/max;
				color.rgbRed   = gray*255/max;
				bmpifft.WritePixel(i, j,color);
			}
		}
	}


	flagfft =3;	
	Invalidate();
	
}





void CImageprocessView::OnRotateRotate30()
{
	// TODO: Add your command handler code here
	if (mybmp.IsEmpty())
	{
		MessageBox("Please read the first picture");
		return;
	}

	int m_rotateangle = -30;



	//////////////////
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	long lWidth = mybmp.GetDimensions().cx;			// ��ȡͼ��Ŀ��
	long lHeight = mybmp.GetDimensions().cy;			// ��ȡͼ��ĸ߶�

												// ����ת�ǶȴӶ�ת��������
	float	fRotateAngle = m_rotateangle*3.1415926535 / 180.0;		// ��ת�Ƕȣ����ȣ�	
	float	fSina, fCosa;		// ��ת�Ƕȵ����Һ�����	
	fSina = (float)sin((double)fRotateAngle);	// ������ת�Ƕȵ�����
	fCosa = (float)cos((double)fRotateAngle);	// ������ת�Ƕȵ�����

												// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float)(-lWidth / 2);
	fSrcY1 = (float)(lHeight / 2);
	fSrcX2 = (float)(lWidth / 2);
	fSrcY2 = (float)(lHeight / 2);
	fSrcX3 = (float)(-lWidth / 2);
	fSrcY3 = (float)(-lHeight / 2);
	fSrcX4 = (float)(lWidth / 2);
	fSrcY4 = (float)(-lHeight / 2);



	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	//ͼ������


	// ������ת���ͼ��ʵ�ʿ��
	long lNewWidth = (long)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);
	// ������ת���ͼ��߶�
	long lNewHeight = (long)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	//mybmp_output.Empty();
	mybmp_raotate.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp.m_lpBMIH->biBitCount);


	RGBQUAD color;

	// ÿ��
	for (int y = 0; y < lNewHeight; y++)
	{
		// ÿ��
		for (int x = 0; x < lNewWidth; x++)
		{

			float x0 = (x - lNewWidth / 2) * fCosa - (y - lNewHeight / 2) * fSina + mybmp.GetDimensions().cx / 2.0;
			float y0 = (x - lNewWidth / 2) * fSina + (y - lNewHeight / 2) * fCosa + mybmp.GetDimensions().cy / 2.0;
			float cx = x0;
			float cy = y0;
			if ((x0 >= 1) && (x0 < mybmp.GetDimensions().cx - 1) && (y0 >= 1) && (y0 < mybmp.GetDimensions().cy - 1))
			{
				// ˫���Բ�ֵ
				//f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)

				float u = cx - (int)cx;
				float v = cy - (int)cy;
				int i = (int)cx;
				int j = (int)cy;

				int red;
				red = (1 - u)*(1 - v)*mybmp.GetPixel(i, j).rgbRed + (1 - u)*v*mybmp.GetPixel(i, j + 1).rgbRed
					+ u*(1 - v)*mybmp.GetPixel(i + 1, j).rgbRed + u*v*mybmp.GetPixel(i + 1, j + 1).rgbRed;
				int green;
				green = (1 - u)*(1 - v)*mybmp.GetPixel(i, j).rgbGreen + (1 - u)*v*mybmp.GetPixel(i, j + 1).rgbGreen
					+ u*(1 - v)*mybmp.GetPixel(i + 1, j).rgbGreen + u*v*mybmp.GetPixel(i + 1, j + 1).rgbGreen;
				int blue;
				blue = (1 - u)*(1 - v)*mybmp.GetPixel(i, j).rgbBlue + (1 - u)*v*mybmp.GetPixel(i, j + 1).rgbBlue
					+ u*(1 - v)*mybmp.GetPixel(i + 1, j).rgbBlue + u*v*mybmp.GetPixel(i + 1, j + 1).rgbBlue;

				color.rgbGreen = green;
				color.rgbRed = red;
				color.rgbBlue = blue;
			}
			else
			{
				color.rgbGreen = 0;
				color.rgbRed = 0;
				color.rgbBlue = 0;
			}
			mybmp_raotate.WritePixel(x, y, color);


		}
	}
	flagfft = 1;
	sizeimage.cx = mybmp_raotate.GetDimensions().cx;
	sizeimage.cy = mybmp_raotate.GetDimensions().cy;
	Invalidate();
}
