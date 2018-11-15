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
	ON_COMMAND(ID_FILTER_IDEA, &CImageprocessView::OnFilterIdea)
	ON_COMMAND(ID_FILTER_BATWAS, &CImageprocessView::OnFilterBatwas)
	ON_COMMAND(ID_FILTER_GASSIAN, &CImageprocessView::OnFilterGassian)
	ON_COMMAND(ID_FILTER_EXP, &CImageprocessView::OnFilterExp)
	ON_COMMAND(ID_FILTER_LADDER, &CImageprocessView::OnFilterLadder)
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

	if (flagfft == 1)
	{
		mybmp.Draw(pDC, CPoint(0, 0), mybmp.GetDimensions());
		bmpfft.Draw(pDC,CPoint(mybmp.GetDimensions().cx+10,0),fftsize);
	}

	else if (flagfft == 2)
	{
		mybmp.Draw(pDC, CPoint(0, 0), mybmp.GetDimensions());
		bmpifft.Draw(pDC, CPoint(mybmp.GetDimensions().cx + 10, 0), mybmp.GetDimensions());

	}
	else 
		mybmp.Draw(pDC,CPoint(0,0),sizeimage);
	
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

	// 对图象的象素值进行变换
	// 每列
	for(int x = 0; x < sizeimage.cx; x++)
	{
		// 每行
		for(int y = 0; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x,y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
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
 * \函数名称： 
 *   FFT_2D() 
 * 
 * \输入参数: 
 *   complex<double> * pCTData - 图像数据 
 *   int    nWidth - 数据宽度 
 *   int    nHeight - 数据高度 
 *   complex<double> * pCFData - 傅立叶变换后的结果 
 * 
 * \返回值: 
 *   无 
 * 
 * \说明: 
 *   二维傅立叶变换。 
 * 
 ************************************************************************ 
 */ 
void  CImageprocessView::FFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{ 

	int x,y;
	int nTransWidth =nWidth; 
	int nTransHeight =nHeight; 

	// x，y（行列）方向上的迭代次数 
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
	// 计算x，y（行列）方向上的迭代次数 
    nXLev = (int) ( log(nTransWidth)/log(2) +  0.5 ); 
	nYLev = (int) ( log(nTransHeight)/log(2) + 0.5 ); 

	for(y = 0; y < nTransHeight; y++) 
	{ 
		// y方向进行快速傅立叶变换 
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev); 
	} 

	// pCFData中目前存储了pCTData经过行变换的结果 
	// 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行 
	// 傅立叶行变换（实际上相当于对列进行傅立叶变换） 
	for(y = 0; y < nTransHeight; y++) 
	{ 
		for(x = 0; x < nTransWidth; x++) 
		{ 
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x]; 
		} 
	} 

	for(y = 0; y < nTransWidth; y++) 
	{ 
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的 
		// 傅立叶变换 
		FFT_1D(&pCTData[y* nTransHeight], &pCFData[y * nTransHeight], nYLev); 
	} 

	// pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向 
	// 的傅立叶变换，对其进行了转置，现在把结果转置回来 
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
 * \函数名称： 
 *   IFFT_2D() 
 * 
 * \输入参数: 
 *   complex<double> * pCFData - 频域数据 
 *   complex<double> * pCTData - 时域数据 
 *   int    nWidth - 图像数据宽度 
 *   int    nHeight - 图像数据高度 
 * 
 * \返回值: 
 *   无 
 * 
 * \说明: 
 *   二维傅立叶反变换。 
 * 
 ************************************************************************ 
 */ 
void CImageprocessView::IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{ 
	// 循环控制变量 
	int x; 
	int y; 

	int nTransWidth =nWidth; 
	int nTransHeight =nHeight; 

	// 分配工作需要的内存空间 
	complex<double> *pCWork= new complex<double>[nTransWidth * nTransHeight]; 

	//临时变量 
	complex<double> *pCTmp ; 

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭 
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭 
	for(y = 0; y < nTransHeight; y++) 
	{ 
		for(x = 0; x < nTransWidth; x++) 
		{ 
			pCTmp = &pCFData[nTransWidth * y + x] ; 
			pCWork[nTransWidth * y + x] = complex<double>( pCTmp->real() , -pCTmp->imag() ); 
		} 
	} 

	// 调用傅立叶正变换 
	FFT_2D(pCWork, nWidth, nHeight, pCTData) ; 

	// 求时域点的共轭，求得最终结果 
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据 
	// 相差一个系数 
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
 * \函数名称： 
 *   FFT_1D() 
 * 
 * \输入参数: 
 *   complex<double> * pCTData - 指向时域数据的指针，输入的需要变换的数据 
 *   complex<double> * pCFData - 指向频域数据的指针，输出的经过变换的数据 
 *   nLevel －傅立叶变换蝶形算法的级数，2的幂数， 
 * 
 * \返回值: 
 *   无 
 * 
 * \说明: 
 *   一维快速傅立叶变换。 
 * 
 ************************************************************************* 
 */ 

void CImageprocessView::FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel) 
{ 



	// 循环控制变量 */
	int i; 
	int     j; 
	int     k; 

	//double PI = 3.1415926;  

	// 傅立叶变换点数 
	int nCount =0 ; 

	// 计算傅立叶变换点数 
	nCount =1<<nLevel; 

	// 某一级的长度 
	int nBtFlyLen; 
	nBtFlyLen = 0 ; 

	// 变换系数的角度 ＝2 * PI * i / nCount 
	double dAngle; 

	complex<double> *pCW ; 

	// 分配内存，存储傅立叶变化需要的系数表 
	pCW  = new complex<double>[nCount / 2]; 

	// 计算傅立叶变换的系数 
	for(i = 0; i < nCount / 2; i++) 
	{ 
		dAngle = -2 * 3.141592 * i / nCount; 
		pCW[i] = complex<double> ( cos(dAngle), sin(dAngle) ); 
	} 

	// 变换需要的工作空间 
	complex<double> *pCWork1,*pCWork2;  

	// 分配工作空间 
	pCWork1 = new complex<double>[nCount]; 

	pCWork2 = new complex<double>[nCount]; 


	// 临时变量 
	complex<double> *pCTmp; 

	// 初始化，写入数据 
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount); 

	// 临时变量 
	int nInter;  
	nInter = 0; 

	// 蝶形算法进行快速傅立叶变换 
	for(k = 0; k < nLevel; k++) 
	{ 
		for(j = 0; j < (int)pow(2,k); j++) 
		{ 
			//计算长度 
			nBtFlyLen = (int)pow( 2,(nLevel-k) ); 

			//倒序重排，加权计算 
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

	// 交换 pCWork1和pCWork2的数据 
	pCTmp   = pCWork1 ; 
	pCWork1 = pCWork2 ; 
	pCWork2 = pCTmp ; 
	} 

	// 重新排序 
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

	// 释放内存空间 
	delete pCW; 
	delete pCWork1; 
	delete pCWork2; 
	pCW = NULL ; 
	pCWork1 = NULL ; 
	pCWork2 = NULL ; 
	

} 
/************************************************************************* 
 
 * \函数名称： 
 *    IFFT_1D() 
 * 
 * \输入参数: 
 *   complex<double> * pCTData - 指向时域数据的指针，输入的需要反变换的数据 
 *   complex<double> * pCFData - 指向频域数据的指针，输出的经过反变换的数据 
 *   nLevel －傅立叶变换蝶形算法的级数，2的幂数， 
 * 
 * \返回值: 
 *   无 
 * 
 * \说明: 
 *   一维快速傅立叶反变换。 
 * 
 ************************************************************************ 
 */ 
void CImageprocessView::IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel) 
{ 

	// 循环控制变量 
	int i; 

	// 傅立叶反变换点数 
	int nCount; 

	// 计算傅立叶变换点数 
	//nCount = (int)pow(2,nLevel) ; 
     nCount=1<<nLevel;
	// 变换需要的工作空间 
	complex<double> *pCWork; 

	// 分配工作空间 
	pCWork = new complex<double>[nCount]; 

	// 将需要反变换的数据写入工作空间pCWork 
	memcpy(pCWork, pCFData, sizeof(complex<double>) * nCount); 

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭 
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭 
	for(i = 0; i < nCount; i++) 
	{ 
		pCWork[i] = complex<double> (pCWork[i].real(), -pCWork[i].imag()); 
	} 

	// 调用快速傅立叶变换实现反变换，结果存储在pCTData中 
	FFT_1D(pCWork, pCTData, nLevel); 

	// 求时域点的共轭，求得最终结果 
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据 
	// 相差一个系数nCount 
	for(i = 0; i < nCount; i++) 
	{ 
		pCTData[i] =  
		complex<double> (pCTData[i].real() / nCount, -pCTData[i].imag() / nCount); 
	} 

	// 释放内存 
	delete pCWork; 
	pCWork = NULL; 
} 

//////////////////////////////////////////////////////////////
//该函数用来实现二维傅立叶变换
//参数height、width分别表示图象的高度和宽度，ising表示正反变换
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


	double dTmpOne; // 临时变量 
	double  dTmpTwo; 


	int nTransWidth; 
	int  nTransHeight; 

	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width)/log(2); 
	dTmpTwo = ceil(dTmpOne)    ; 
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransWidth = (int)dTmpTwo    ; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height)/log(2); 
	dTmpTwo = ceil (dTmpOne)    ;
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransHeight = (int)dTmpTwo   ; 
	
	complex<double> *t=new complex<double>[nTransHeight*nTransWidth];
	

	fftsize = CSize(nTransWidth,nTransHeight);

	bmpfft.CreateCDib(fftsize,mybmp.m_lpBMIH->biBitCount);

	for(j=0;j<nTransHeight;j++)
	   for(i=0;i<nTransWidth;i++)
		{
		
			int position = j*nTransWidth+i;
				if(i<width &&j<height)
				{
					RGBQUAD color;
					color = mybmp.GetPixel(i,j);

					t[position]=complex<double>(color.rgbRed,0.0);  //赋予强度值
					
				}
				else
				{
					t[position]=complex<double>(0.0,0.0);
				}
	   }

	//傅里叶变换
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


	flagfft =1;
	
	Invalidate();
	
}

void CImageprocessView::OnIfft() 
{


		int width = sizeimage.cx;
	int height = sizeimage.cy;   
	int i,j;


	double dTmpOne; // 临时变量 
	double  dTmpTwo; 


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width)/log(2); 
	dTmpTwo = ceil(dTmpOne)    ; 
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransWidth = (int)dTmpTwo    ; // 

	// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height)/log(2); 
	dTmpTwo = ceil (dTmpOne)    ;
	dTmpTwo = pow(2,dTmpTwo)    ; 
	nTransHeight = (int)dTmpTwo   ; 
	
	complex<double> *t=new complex<double>[nTransHeight*nTransWidth];
	

	fftsize = CSize(nTransWidth,nTransHeight);

	bmpfft.CreateCDib(fftsize,mybmp.m_lpBMIH->biBitCount);

	for(j=0;j<nTransHeight;j++)
	   for(i=0;i<nTransWidth;i++)
		{
		
			int position = j*nTransWidth+i;
				if(i<width &&j<height)
				{
					RGBQUAD color;
					color = mybmp.GetPixel(i,j);

					t[position]=complex<double>(color.rgbRed,0.0);  //赋予强度值
					
				}
				else
				{
					t[position]=complex<double>(0.0,0.0);
				}
	   }

	//傅里叶变换
    fourier(t,nTransHeight,nTransWidth,1);

		//傅里叶反变换
	fourier(t,nTransHeight,nTransWidth,-1);
	

	bmpifft.CreateCDib(fftsize,mybmp.m_lpBMIH->biBitCount);

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


	flagfft =2;	
	Invalidate();
	
}

void CImageprocessView::OnFilterIdea()
{
	OnGray();
	CDialog1 dlg;
	dlg.DoModal();
	int u = dlg.m_x;
	int v = dlg.m_y;

	int width = sizeimage.cx;//原图象的宽度和高度    
	int height = sizeimage.cy;
	int i, j;
	double d0;//中间变量




	double dTmpOne; // 临时变量 
	double  dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

								// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;


	////////////////////////////////////////

	complex<double> *t = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *H = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *tmp = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 



	d0 = sqrt(u*u + v * v);//计算截止频率d0

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;
			if (i<width &&j<height)
			{
				RGBQUAD color;
				color = mybmp.GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0);  //赋予强度值


				if (sqrt((i - nTransWidth / 2)*(i - nTransWidth / 2) + (j - nTransHeight / 2)*(j - nTransHeight / 2)) <= d0)
					H[position] = complex<double>(0.0, 0.0);
				else
					H[position] = complex<double>(1.0, 0.0);
			}
			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			//构造理想滤波器
			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}



	double max = 0.0;

	for (j = 0; j<nTransHeight*nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real()*H[j].real() - tmp[j].imag()*H[j].imag(), tmp[j].real()*H[j].imag() + tmp[j].imag()*H[j].real());


	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			//构造理想滤波器
			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{
			if (max<tmp[nTransWidth*j + i].real())
				max = tmp[nTransWidth*j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j<nTransHeight; j++)
	{
		for (i = 0; i<nTransWidth; i++)
		{
			if (i<width &&j<height)
			{

				int gray = sqrt(tmp[nTransWidth*j + i].real()*tmp[nTransWidth*j + i].real() + tmp[nTransWidth*j + i].imag()*tmp[nTransWidth*j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				mybmp.WritePixel(i, j, color);
			}
		}
	}
	delete t;
	delete H;


	flagfft = 3;
	Invalidate();
}


void CImageprocessView::OnFilterBatwas()
{
	OnGray();
	CDialog2 dlg;
	dlg.DoModal();
	int u = dlg.m_x;
	int v = dlg.m_y;
	int n = dlg.m_n;

	int width = sizeimage.cx;//原图象的宽度和高度    
	int height = sizeimage.cy;
	int i, j;
	double d0, max = 0.0;//中间变量


	double dTmpOne; // 临时变量 
	double  dTmpTwo;

	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

								// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;


	////////////////////////////////////////

	complex<double> *t = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *H = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *tmp = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 



	d0 = sqrt(u*u + v * v);//计算截止频率d0

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;
			if (i<width &&j<height)
			{
				RGBQUAD color;
				color = mybmp.GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0);  //赋予强度值



																   //构造Butterworth高通滤波器
				double d = sqrt((i - nTransWidth / 2)*(i - nTransWidth / 2) + (j - nTransHeight / 2)*(j - nTransHeight / 2));
				H[position] = 1 / (1 + (sqrt(2) - 1)*pow((d0 / d), (2 * n)));
			}
			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//滤波
	for (j = 0; j<nTransHeight*nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real()*H[j].real() - tmp[j].imag()*H[j].imag(), tmp[j].real()*H[j].imag() + tmp[j].imag()*H[j].real());


	//非中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{
			if (max<tmp[nTransWidth*j + i].real())
				max = tmp[nTransWidth*j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j<nTransHeight; j++)
	{
		for (i = 0; i<nTransWidth; i++)
		{
			if (i<width &&j<height)
			{

				int gray = sqrt(tmp[nTransWidth*j + i].real()*tmp[nTransWidth*j + i].real() + tmp[nTransWidth*j + i].imag()*tmp[nTransWidth*j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				mybmp.WritePixel(i, j, color);
			}
		}
	}
	delete t;
	delete H;
	delete tmp;

	Invalidate();
	flagfft = 3;


}


void CImageprocessView::OnFilterGassian()
{
	OnGray();
	CDialog3 dlg;
	dlg.DoModal();
	int u = dlg.m_x;
	int v = dlg.m_y;

	int width = sizeimage.cx;//原图象的宽度和高度    
	int height = sizeimage.cy;
	int i, j;
	double d0, max = 0.0;//中间变量


	double dTmpOne; // 临时变量 
	double  dTmpTwo;

	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

								// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;


	////////////////////////////////////////

	complex<double> *t = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *H = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *tmp = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 



	d0 = u * u + v * v;//计算截止频率d0

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;
			if (i<width &&j<height)
			{
				RGBQUAD color;
				color = mybmp.GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0);  //赋予强度值



																   //构造高斯高通滤波器
				double d2 = (i - nTransWidth / 2)*(i - nTransWidth / 2) + (j - nTransHeight / 2)*(j - nTransHeight / 2);
				H[position] = 1 - exp(-d2 / (2 * d0));

			}
			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//滤波
	for (j = 0; j<nTransHeight*nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real()*H[j].real() - tmp[j].imag()*H[j].imag(), tmp[j].real()*H[j].imag() + tmp[j].imag()*H[j].real());


	//非中心化

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{
			if (max<tmp[nTransWidth*j + i].real())
				max = tmp[nTransWidth*j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j<nTransHeight; j++)
	{
		for (i = 0; i<nTransWidth; i++)
		{
			if (i<width &&j<height)
			{

				int gray = sqrt(tmp[nTransWidth*j + i].real()*tmp[nTransWidth*j + i].real() + tmp[nTransWidth*j + i].imag()*tmp[nTransWidth*j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				mybmp.WritePixel(i, j, color);
			}
		}
	}
	delete t;
	delete H;
	delete tmp;

	Invalidate();
	flagfft = 3;



}


void CImageprocessView::OnFilterExp()
{
	OnGray();
	CDialog4 dlg;
	dlg.DoModal();
	int u = dlg.m_x;
	int v = dlg.m_y;
	int n = dlg.m_n;


	double d0 = sqrt(u*u + v * v);//计算截止频率d0


	int width = sizeimage.cx;//原图象的宽度和高度    
	int height = sizeimage.cy;
	int i, j;

	double max = 0.0;

	double dTmpOne; // 临时变量 
	double  dTmpTwo;

	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

								// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;


	////////////////////////////////////////

	complex<double> *t = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *H = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *tmp = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 



	d0 = sqrt(u*u + v * v);//计算截止频率d0

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;
			if (i<width &&j<height)
			{
				RGBQUAD color;
				color = mybmp.GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0);  //赋予强度值

																   //构造指数高通滤波器
				double d = sqrt((i - nTransWidth / 2)*(i - nTransWidth / 2) + (j - nTransHeight / 2)*(j - nTransHeight / 2));
				H[position] = 1 - exp(-0.347*pow((d / d0), n));
			}


			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}

	//滤波
	for (j = 0; j<nTransHeight*nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real()*H[j].real() - tmp[j].imag()*H[j].imag(), tmp[j].real()*H[j].imag() + tmp[j].imag()*H[j].real());


	//非中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{
			if (max<tmp[nTransWidth*j + i].real())
				max = tmp[nTransWidth*j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j<nTransHeight; j++)
	{
		for (i = 0; i<nTransWidth; i++)
		{
			if (i<width &&j<height)
			{

				int gray = sqrt(tmp[nTransWidth*j + i].real()*tmp[nTransWidth*j + i].real() + tmp[nTransWidth*j + i].imag()*tmp[nTransWidth*j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				mybmp.WritePixel(i, j, color);
			}
		}
	}
	delete t;
	delete H;
	delete tmp;

	Invalidate();
	flagfft = 3;


}


void CImageprocessView::OnFilterLadder()
{
	// TODO: 在此添加命令处理程序代码
	OnGray();
	CDialog5 dlg;
	dlg.DoModal();
	int u = dlg.m_x;
	int v = dlg.m_y;
	int u1 = dlg.m_x1;
	int v1 = dlg.m_y1;

	double d0 = sqrt(u*u + v * v);//计算截止频率d0
	double d1 = sqrt(u1*u1 + v1 * v1);


	double max = 0.0;

	int width = sizeimage.cx;//原图象的宽度和高度    
	int height = sizeimage.cy;
	int i, j;

	////////////////////////////////////////
	//获取变换尺度	
	double dTmpOne; // 临时变量 
	double  dTmpTwo;


	// 计算进行傅立叶变换的宽度 （2的整数次幂） 
	dTmpOne = log(width) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo; // 

								// 计算进行傅立叶变换的高度 （2的整数次幂） 
	dTmpOne = log(height) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	complex<double> *t = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *H = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 
	complex<double> *tmp = new complex<double>[nTransHeight*nTransWidth]; // 分配工作空间 

	d0 = sqrt(u*u + v * v);//计算截止频率d0

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;
			if (i<width &&j<height)
			{
				RGBQUAD color;
				color = mybmp.GetPixel(i, j);

				t[position] = complex<double>(color.rgbRed, 0.0);  //赋予强度值

																   //构造梯形低通滤波器
				double tmp = sqrt((i - nTransWidth / 2)*(i - nTransWidth / 2) + (j - nTransHeight / 2)*(j - nTransHeight / 2));

				if (tmp <= d0)
					H[position] = complex<double>(0.0, 0.0);

				else if (tmp > d1)
					H[position] = complex<double>(1.0, 0.0);
				else
				{
					double d = tmp;
					H[position] = 1-(d - d1) / (d0 - d1);
				}


			}
			else
			{
				t[position] = complex<double>(0.0, 0.0);
				H[position] = complex<double>(0.0, 0.0);
			}
		}

	//傅里叶变换
	fourier(t, nTransHeight, nTransWidth, 1);

	//频谱中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}



	//滤波
	for (j = 0; j<nTransHeight*nTransWidth; j++)
		t[j] = complex<double>(tmp[j].real()*H[j].real() - tmp[j].imag()*H[j].imag(), tmp[j].real()*H[j].imag() + tmp[j].imag()*H[j].real());



	//非中心化
	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{

			int position = j * nTransWidth + i;

			int x = i<nTransWidth / 2 ? i + nTransWidth / 2 : i - nTransWidth / 2;
			int y = j<nTransHeight / 2 ? j + nTransHeight / 2 : j - nTransHeight / 2;

			int position1 = y * nTransWidth + x;

			tmp[position1] = t[position];
		}


	//傅里叶反变换
	fourier(tmp, nTransHeight, nTransWidth, -1);

	for (j = 0; j<nTransHeight; j++)
		for (i = 0; i<nTransWidth; i++)
		{
			if (max<tmp[nTransWidth*j + i].real())
				max = tmp[nTransWidth*j + i].real();
		}

	//////////////////////////////////////////

	for (j = 0; j<nTransHeight; j++)
	{
		for (i = 0; i<nTransWidth; i++)
		{
			if (i<width &&j<height)
			{

				int gray = sqrt(tmp[nTransWidth*j + i].real()*tmp[nTransWidth*j + i].real() + tmp[nTransWidth*j + i].imag()*tmp[nTransWidth*j + i].imag());
				RGBQUAD color;
				color.rgbBlue = gray * 255 / max;
				color.rgbGreen = gray * 255 / max;
				color.rgbRed = gray * 255 / max;
				mybmp.WritePixel(i, j, color);
			}
		}
	}
	delete t;
	delete H;
	delete tmp;

	flagfft = 3;

	Invalidate();

}
