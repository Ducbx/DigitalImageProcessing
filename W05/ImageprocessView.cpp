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

	bmpfft.CreateCDib(fftsize,mybmp_raotate.m_lpBMIH->biBitCount);

	for(j=0;j<nTransHeight;j++)
	   for(i=0;i<nTransWidth;i++)
		{
		
			int position = j*nTransWidth+i;
				if(i<width &&j<height)
				{
					RGBQUAD color;
					color = mybmp_raotate.GetPixel(i,j);

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


	flagfft =2;
	
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

	bmpfft.CreateCDib(fftsize, mybmp_raotate.m_lpBMIH->biBitCount);

	for(j=0;j<nTransHeight;j++)
	   for(i=0;i<nTransWidth;i++)
		{
		
			int position = j*nTransWidth+i;
				if(i<width &&j<height)
				{
					RGBQUAD color;
					color = mybmp_raotate.GetPixel(i,j);

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
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	long lWidth = mybmp.GetDimensions().cx;			// 获取图像的宽度
	long lHeight = mybmp.GetDimensions().cy;			// 获取图像的高度

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

	//mybmp_output.Empty();
	mybmp_raotate.CreateCDib(CSize(lNewWidth, lNewHeight), mybmp.m_lpBMIH->biBitCount);


	RGBQUAD color;

	// 每行
	for (int y = 0; y < lNewHeight; y++)
	{
		// 每列
		for (int x = 0; x < lNewWidth; x++)
		{

			float x0 = (x - lNewWidth / 2) * fCosa - (y - lNewHeight / 2) * fSina + mybmp.GetDimensions().cx / 2.0;
			float y0 = (x - lNewWidth / 2) * fSina + (y - lNewHeight / 2) * fCosa + mybmp.GetDimensions().cy / 2.0;
			float cx = x0;
			float cy = y0;
			if ((x0 >= 1) && (x0 < mybmp.GetDimensions().cx - 1) && (y0 >= 1) && (y0 < mybmp.GetDimensions().cy - 1))
			{
				// 双线性插值
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
