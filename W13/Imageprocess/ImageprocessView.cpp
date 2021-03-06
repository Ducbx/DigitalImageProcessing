
// ImageprocessView.cpp: CImageprocessView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Imageprocess.h"
#endif

#include "ImageprocessDoc.h"
#include "ImageprocessView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageprocessView

IMPLEMENT_DYNCREATE(CImageprocessView, CView)

BEGIN_MESSAGE_MAP(CImageprocessView, CView)
	// 标准打印命令
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(OPEN1, &CImageprocessView::OnOpen)
	ON_COMMAND(REGIONGROW1, &CImageprocessView::OnRegiongrow)
	ON_COMMAND(SNAKEINIT1, &CImageprocessView::OnSnakeinit)
	ON_COMMAND(ID_1_EVOLUTION, &CImageprocessView::OnEvolution)
	ON_COMMAND(ID_GARY, &CImageprocessView::OnGary)
END_MESSAGE_MAP()




CImageprocessView::CImageprocessView()
{
	downflag = false;
	lastnode = NULL;
	num_snake_points = 0;
	firstnode = new Node;
}

CImageprocessView::~CImageprocessView()
{
	//	KillTimer(1);
	//	KillTimer(2);
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

	mybmp.Draw(pDC, CPoint(0, 0), sizeimage);
	orginnew.x = sizeimage.cx + 10;
	orginnew.y = 0;
	tmpbmp.Draw(pDC, orginnew, sizeimage);

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

void CImageprocessView::OnOpen()
{
	CFileDialog FileDlg(TRUE, _T("*.bmp"), "", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, "image files (*.bmp) |*.bmp|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||", NULL);
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
		if (!mybmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}

	if (mybmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return;
	}
	sizeimage = mybmp.GetDimensions();
	Invalidate();
}


void CImageprocessView::OnGary()
{
	// 对图象的象素值进行变换
	// 每列
	for (int x = 0; x < sizeimage.cx; x++)
	{
		// 每行
		for (int y = 0; y < sizeimage.cy; y++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);
			//RGB图像转灰度图像 Gray = R*0.299 + G*0.587 + B*0.114
			double gray = color.rgbRed*0.299 + color.rgbGreen*0.587 + color.rgbBlue*0.114;
			color.rgbBlue = (int)gray;
			color.rgbGreen = (int)gray;
			color.rgbRed = (int)gray;
			mybmp.WritePixel(x, y, color);
		}
	}
	Invalidate();
}



/**********************************************************************
/*   pUnRegion指针指向的数据区存储了区域生长的结果，其中1（逻辑）表示
*	 对应象素为生长区域，0表示为非生长区域
*   区域生长一般包含三个比较重要的问题：
*		1. 种子点的选取
*		2. 生长准则
*		3. 终止条件
*	 每个问题解决的好坏直接关系到区域生长的结果。
*	 本函数的种子点选取为图像的中心，生长准则是相邻象素的象素值小于
*	 nThreshold, 终止条件是一直进行到再没有满足生长准则需要的象素时为止
*
*************************************************************************/

void CImageprocessView::OnRegiongrow()
{

	//四邻域的生长
	/*	tmpbmp.CreateCDib(sizeimage,24);

	int nWidth			= sizeimage.cx;
	int nHeight			= sizeimage.cy;

	for( int y=0; y<nHeight ; y++ )
	for(int x=0; x<nWidth ; x++ )
	{
	RGBQUAD color;

	color.rgbBlue  = 255;
	color.rgbGreen = 255;
	color.rgbRed =   255;

	tmpbmp.WritePixel(x,y,color);
	}

	static int nDx[]={-1,0,1,0};
	static int nDy[]={0,1,0,-1};

	int *flag = new int[nWidth*nHeight];
	// 初始化
	memset(flag,0,sizeof(int)*nWidth*nHeight);

	// 种子点
	int nSeedX, nSeedY;

	// 设置种子点为图像的中心
	nSeedX = nWidth /2 ;
	nSeedY = nHeight/2 ;

	// 定义堆栈，存储坐标
	int * pnGrowQueX ;
	int * pnGrowQueY ;

	// 分配空间
	pnGrowQueX = new int [nWidth*nHeight];
	pnGrowQueY = new int [nWidth*nHeight];


	// 定义堆栈的起点和终点
	// 当nStart=nEnd, 表示堆栈中只有一个点
	int nStart ;
	int nEnd   ;

	//初始化
	nStart = 0 ;
	nEnd   = 0 ;

	// 把种子点的坐标压入栈
	pnGrowQueX[nEnd] = nSeedX;
	pnGrowQueY[nEnd] = nSeedY;

	// 当前正在处理的象素
	int nCurrX ;
	int nCurrY ;

	// 循环控制变量
	int k ;

	// 图象的横纵坐标,用来对当前象素的4邻域进行遍历
	int xx;
	int yy;
	int nThreshold =50;

	while (nStart<=nEnd)
	{
	// 当前种子点的坐标
	nCurrX = pnGrowQueX[nStart];
	nCurrY = pnGrowQueY[nStart];

	// 对当前点的4邻域进行遍历
	for (k=0; k<4; k++)
	{
	// 4邻域象素的坐标
	xx = nCurrX+nDx[k];
	yy = nCurrY+nDy[k];

	// 判断象素(xx，yy) 是否在图像内部,是否已经处理过,flag[yy*nWidth+xx]==0 表示还没有处理
	// 生长条件：判断象素(xx，yy)和当前象素(nCurrX,nCurrY) 象素值差的绝对值

	if ( (xx < nWidth) && (xx>=0) && (yy<nHeight) && (yy>=0)
	&& (flag[yy*nWidth+xx]==0))
	{
	RGBQUAD color;
	color = mybmp.GetPixel(xx,yy);
	int gray1 = color.rgbRed;

	color = mybmp.GetPixel(nSeedX,nSeedY);//color = mybmp.GetPixel(nCurrX,nCurrY);
	int gray2 = color.rgbRed;



	if (abs(gray1 - gray2)<nThreshold)

	{
	flag[yy*nWidth+xx]=1;
	// 堆栈的尾部指针后移一位
	nEnd++;
	// 象素(xx，yy) 压入栈
	pnGrowQueX[nEnd] = xx;
	pnGrowQueY[nEnd] = yy;

	// 把象素(xx，yy)设置成逻辑1（255）
	// 同时也表明该象素处理过
	color.rgbRed = 0;
	color.rgbGreen = 0;
	color.rgbBlue = 255 ;

	tmpbmp.WritePixel(xx, yy,color);
	}
	}
	}
	nStart++;
	}

	// 释放内存
	delete []pnGrowQueX;
	delete []pnGrowQueY;
	pnGrowQueX = NULL ;
	pnGrowQueY = NULL ;

	Invalidate(); */

	//八邻域的生长
	tmpbmp.CreateCDib(sizeimage, 24);
	//图象的长宽大小
	int nWidth = sizeimage.cx;
	int nHeight = sizeimage.cy;

	for (int y = 0; y<nHeight; y++)
		for (int x = 0; x<nWidth; x++)
		{
			RGBQUAD color;

			color.rgbBlue = 255;
			color.rgbGreen = 255;
			color.rgbRed = 255;

			tmpbmp.WritePixel(x, y, color);
		}


	int i, j;
	int thresh = 50;

	//二维数组direction代表中心像素点8邻域坐标与该点在x和y方向上的偏移,
	//其中第一列为x方向的偏移,第二列为y方向的偏移
	int direction[8][2] = { { 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 },{ -1,0 },{ -1,1 } };

	//栈申请，此处假定进栈的像素最多为图像总像素数
	CPoint *stack = new CPoint[nWidth*nHeight];

	//栈顶指针
	int top;

	//当前正处理的点和弹出的点
	CPoint currentPoint, popPoint;

	//循环变量，遍历array数组的第一维下标
	int k;


	//临时变量
	int temp1, temp2;

	//记录种子像素的灰度值
	RGBQUAD color;

	// 设置种子点为图像的中心
	CPoint seed;
	seed.x = nWidth / 2;
	seed.y = nHeight / 2;

	color = mybmp.GetPixel(seed.x, seed.y);

	temp1 = color.rgbRed;

	//将给定种子点置标记0,入栈

	int *flag = new int[nWidth*nHeight];
	// 初始化
	memset(flag, 0, sizeof(int)*nWidth*nHeight);
	flag[seed.y*nWidth + seed.x] = 0;

	top = 0;
	stack[top].x = seed.x;
	stack[top].y = seed.y;

	//堆栈
	while (top>-1) {
		//弹出栈顶元素,该元素已经生长过
		popPoint.x = stack[top].x;
		popPoint.y = stack[top].y;
		top--;

		//考察弹出像素周围是否有没有生长的像素
		for (k = 0; k<8; k++) {

			//待考察的邻域点
			currentPoint.x = popPoint.x + direction[k][0];
			currentPoint.y = popPoint.y + direction[k][1];

			//如果待考察的点不在图像内，则跳过
			if (currentPoint.x<0 || currentPoint.x>nWidth - 1 ||
				currentPoint.y<0 || currentPoint.y>nHeight - 1)
				continue;


			//弹出的点周围有尚没生长的点
			if (flag[currentPoint.y*nWidth + currentPoint.x] == 0) {
				RGBQUAD color;
				color = mybmp.GetPixel(currentPoint.x, currentPoint.y);
				temp2 = color.rgbRed;
				//如果当前被考察的像素灰度值与种子点灰度值之差小于给定的阈值,
				//则认为相似,将其进栈处理
				if (abs(temp1 - temp2)<thresh) {
					//给该点置生长标记0
					flag[currentPoint.y*nWidth + currentPoint.x] = 1;

					top++;
					stack[top].x = currentPoint.x;
					stack[top].y = currentPoint.y;

					color.rgbRed = 0;
					color.rgbGreen = 0;
					color.rgbBlue = 255;

					tmpbmp.WritePixel(currentPoint.x, currentPoint.y, color);
				}
			}

		}

	}

	//清除缓冲区
	delete[]stack;
	Invalidate();


}


void CImageprocessView::OnSnakeinit()
{
	downflag = true;

	nWidth = sizeimage.cx;
	nHeight = sizeimage.cy;
	ImageData = new double[nWidth*nHeight];
	grad_mag = new double[nHeight*nWidth];

	for (int y = 0; y<nHeight; y++)
	{
		for (int x = 0; x<nWidth; x++)
		{
			RGBQUAD color;
			color = mybmp.GetPixel(x, y);
			ImageData[y*nWidth + x] = (double)color.rgbRed;
		}
	}


}

void CImageprocessView::OnEvolution()
{
	SetTimer(1, 200, NULL);
}
double CImageprocessView::find_distance(int no, CPoint point)
{
	int n = num_snake_points;
	point -= Snake_points[(no - 1 + n) % n];
	return(sqrt(point.x*point.x + point.y*point.y));
}

double CImageprocessView::find_curvature(int no, CPoint point)
{

	int n = num_snake_points;

	CPoint lastpoint = Snake_points[(no - 1 + n) % n], nextpoint = Snake_points[(no + 1 + n) % n];

	CPoint tmp1 = lastpoint - Snake_points[no];
	CPoint tmp2 = nextpoint - Snake_points[no];

	double costheta = double(tmp1.x * tmp2.x + tmp1.y * tmp2.y) /
		(sqrt(tmp1.x * tmp1.x + tmp1.y + tmp1.y)*sqrt(tmp2.x * tmp2.x + tmp2.y * tmp2.y));


	return costheta;

}

void CImageprocessView::GetExternalEnergy(int i, double Ex_energy[3][3])
{

	CPoint CentralPoint = Snake_points[i];

	for (int k1 = 0; k1 < 3; k1++)
		for (int k2 = 0; k2 < 3; k2++)
		{
			int tmpx, tmpy;
			tmpx = CentralPoint.x - 1 + k2;
			tmpy = CentralPoint.y - 1 + k1;

			if (tmpx > 0 && tmpx <sizeimage.cx - 1
				&& tmpy > 0 && tmpy <sizeimage.cy - 1)
			{
				int Ix, Iy;

				Iy = ImageData[(tmpy - 1)*sizeimage.cx + tmpx] - ImageData[tmpy*sizeimage.cx + tmpx];
				Ix = ImageData[tmpy*sizeimage.cx + tmpx] - ImageData[tmpy*sizeimage.cx + tmpx - 1];
				Ex_energy[k1][k2] = sqrt(Ix*Ix + Iy * Iy);

			}
		}

}

//弯曲势能的计算
void CImageprocessView::GetBlendingEnergy(int i, double In_engery1[3][3])
{
	CPoint CentralPoint = Snake_points[i];
	int n = num_snake_points;
	CPoint lastpoint = Snake_points[(i - 1 + n) % n], nextpoint = Snake_points[(i + 1 + n) % n];
	CPoint midpoint((lastpoint.x + nextpoint.x) / 2, (lastpoint.y + nextpoint.y) / 2);

	for (int k1 = 0; k1 < 3; k1++)
		for (int k2 = 0; k2 < 3; k2++)
		{
			int tmpx, tmpy;
			tmpx = CentralPoint.x - 1 + k2;
			tmpy = CentralPoint.y - 1 + k1;

			In_engery1[k1][k2] = sqrt((tmpx - midpoint.x)*(tmpx - midpoint.x) + (tmpy - midpoint.y)*(tmpy - midpoint.y));
		}


}

void CImageprocessView::GetElasticEnergy(int i, double In_engery2[3][3])
{
	int n = num_snake_points;

	CPoint lastpoint = Snake_points[(i - 1 + n) % n], nextpoint = Snake_points[(i + 1 + n) % n];
	CPoint midpoint((lastpoint.x + nextpoint.x + 0.5) / 2, (lastpoint.y + nextpoint.y + 0.5) / 2);

	CPoint tmp1 = Snake_points[i] - lastpoint;
	CPoint tmp2 = nextpoint - Snake_points[i];


	double	tx, ty;
	tx = (double)tmp1.x / sqrt(tmp1.x * tmp1.x + tmp1.y * tmp1.y) + (double)tmp2.x / sqrt(tmp2.x * tmp2.x + tmp2.y * tmp2.y);
	ty = (double)tmp1.y / sqrt(tmp1.x * tmp1.x + tmp1.y * tmp1.y) + (double)tmp2.y / sqrt(tmp2.x * tmp2.x + tmp2.y * tmp2.y);



	//计算法线方向
	double npointx, npointy;
	npointx = -ty;
	npointy = tx;

	CPoint CentralPoint = Snake_points[i];

	for (int k1 = 0; k1 < 3; k1++)
		for (int k2 = 0; k2 < 3; k2++)
		{
			int tmpx, tmpy;
			tmpx = CentralPoint.x - 1 + k2;
			tmpy = CentralPoint.y - 1 + k1;

			CPoint tmp;
			tmp.x = CentralPoint.x - tmpx;
			tmp.y = CentralPoint.y - tmpy;

			In_engery2[k1][k2] = tmp.x * npointx + tmp.y * npointy;
		}



}
void CImageprocessView::Snake_interp()
{
	int i, k;
	double dis_ptp;
	int dmin = 1;
	int dmax = 5;
	int n = num_snake_points;

	for (i = 0; i<num_snake_points; i++)
	{
		dis_ptp = find_distance(i, Snake_points[i]);
		//去掉距离小于最小距离的点
		if (dis_ptp<dmin)
		{
			for (k = i - 1; k<num_snake_points - 1; k++)
				Snake_points[k] = Snake_points[k + 1];
			num_snake_points--;

		}
		//在距离较大的点之间插入新的节点
		/*if(dis_ptp>dmax)
		{
		CPoint *tp;//创建新的临时数组
		tp=new CPoint[no_of_snake_points+1];
		//前面数据的导出
		for(k=0;k<i;k++)
		tp[k]=Snake_points[k];
		//插入新的节点
		n=no_of_snake_points;
		tp[i].x=(Snake_points[i].x+Snake_points[(i-1+n)%n].x)/2;
		tp[i].y=(Snake_points[i].y+Snake_points[(i-1+n)%n].y)/2;
		//后面数据的导出
		for(k=i+1;k<no_of_snake_points+1;k++)
		tp[k]=Snake_points[k-1];
		no_of_snake_points++;
		//delete Snake_points;
		Snake_points=new CPoint[no_of_snake_points];
		for(k=0;k<no_of_snake_points;k++)
		Snake_points[k]=tp[k];
		delete tp;

		}*/


	}

}
void CImageprocessView::SnakeEvolution()
{
	alpha = -1;      //外能量参数
	beta = 1;          //弯曲内能系数 二阶导
	gamma = -1;		   //弹性势能系数 一阶导
	Snake_points = pos;

	double Ex_engery[3][3], In_engery1[3][3], In_engery2[3][3];
	CPoint tmppoint;
	double *Iterenergy = new double[num_snake_points + 100];
	double *curvature = new double[num_snake_points + 100];

	int n = num_snake_points;
	//每个控制点的最小能量
	for (int i = 0; i<num_snake_points; i++)
	{
		Iterenergy[i] = 100000;
	}

	for (int IterNum = 0; IterNum<30; IterNum++)
	{
		for (int i = 0; i<num_snake_points; i++)
		{
			GetExternalEnergy(i, Ex_engery);     //计算i的邻域9个像素的外能量

			GetBlendingEnergy(i, In_engery1);    //计算i的邻域9个像素的弯曲内量

			GetElasticEnergy(i, In_engery2);      //计算i的邻域9个像素的弹性内量

			double minenergy = 100000;
			int tmpk1, tmpk2;
			for (int k1 = 0; k1 < 3; k1++)
				for (int k2 = 0; k2 < 3; k2++)
				{
					double energy = alpha * Ex_engery[k1][k2] + beta * In_engery1[k1][k2] + gamma * In_engery2[k1][k2];

					if (energy < minenergy)
					{
						tmpk1 = k1;
						tmpk2 = k2;

						minenergy = energy;
					}
				}


			if (minenergy< Iterenergy[i] && fabs(minenergy)>1e-6)
			{
				Snake_points[i].x = Snake_points[i].x - 1 + tmpk2;
				Snake_points[i].y = Snake_points[i].y - 1 + tmpk1;
				Iterenergy[i] = minenergy;
			}

		}
	}


}

double CImageprocessView::find_continuity(int no, CPoint point, double avg_distance)
{
	return(pow(avg_distance - find_distance(no, point), 2));
}
void CImageprocessView::Gauss()
{
	float aTemplate[25];			// 模板数组

	aTemplate[0] = 1.0;
	aTemplate[1] = 1.0;
	aTemplate[2] = 2.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 1.0;
	aTemplate[5] = 1.0;
	aTemplate[6] = 2.0;
	aTemplate[7] = 4.0;
	aTemplate[8] = 2.0;
	aTemplate[9] = 1.0;
	aTemplate[10] = 2.0;
	aTemplate[11] = 4.0;
	aTemplate[12] = 8.0;
	aTemplate[13] = 4.0;
	aTemplate[14] = 2.0;
	aTemplate[15] = 1.0;
	aTemplate[16] = 2.0;
	aTemplate[17] = 4.0;
	aTemplate[18] = 2.0;
	aTemplate[19] = 1.0;
	aTemplate[20] = 1.0;
	aTemplate[21] = 1.0;
	aTemplate[22] = 2.0;
	aTemplate[23] = 1.0;
	aTemplate[24] = 1.0;

	for (int y = 2; y < sizeimage.cy - 2; y++)	// 行(除去边缘几行)
		for (int x = 2; x < sizeimage.cx - 2; x++)// 列(除去边缘几列)
		{
			RGBQUAD color;
			double fResult = 0;
			for (int j = -2; j <= 2; j++)
				for (int i = -2; i <= 2; i++)
				{
					color = mybmp.GetPixel(x + i, y + j);
					fResult += color.rgbRed * aTemplate[(i + 2) * 5 + (j + 2)];
				}

			ImageData[y*sizeimage.cx + x] = fResult / 52;  //？？
			if (fResult / 52 > 255)
				ImageData[y*sizeimage.cx + x] = 255;
			if (fResult / 52 < 0)
				ImageData[y*sizeimage.cx + x] = 0;


		}


}
//梯度变换
void CImageprocessView::Gradient()
{
	// 指向DIB象素的指针
	/*	double lpSrc[9];

	int	i,j,index,k,x,y,n;
	// 每行
	double Ix,Iy;
	double G,MaxG;
	MaxG=0;*/

	//为fG申请内存
	fG = new Grad[nHeight*nWidth];
	grad_mag = new double[nHeight*nWidth];

	for (int j = 0; j<nHeight; j++)
		for (int i = 0; i<nWidth; i++)
		{
			int tmp = j * nWidth + i;
			fG[tmp].Ix = 0;
			fG[tmp].Iy = 0;
			fG[tmp].G = 0;
		}


	float Template1[9] = { -1.0,-2.0,-1.0,
		0.0, 0.0, 0.0,
		1.0, 2.0, 1.0 };
	float Template2[9] = { -1.0, 0.0, 1.0,
		-2.0, 0.0, 2.0,
		-1.0, 2.0, 1.0 };

	for (int y = 1; y < sizeimage.cy - 1; y++)	// 行(除去边缘几行)
		for (int x = 1; x < sizeimage.cx - 1; x++)// 列(除去边缘几列)
		{
			RGBQUAD color;
			double Ix = 0;
			for (int j = -1; j <= 1; j++)
				for (int i = -1; i <= 1; i++)
				{
					color = mybmp.GetPixel(x + i, y + j);
					Ix += color.rgbRed * Template1[(i + 2) * 5 + (j + 2)];
				}

			double Iy = 0;
			for (int j = -1; j <= 1; j++)
				for (int i = -1; i <= 1; i++)
				{
					color = mybmp.GetPixel(x + i, y + j);
					Iy += color.rgbRed * Template2[(i + 2) * 5 + (j + 2)];
				}
			double G = sqrt(Ix*Ix + Iy * Iy);

			int index = y * sizeimage.cx + x;
			fG[index].G = G;
			fG[index].Ix = Ix;
			fG[index].Iy = Iy;
			grad_mag[index] = fG[index].G;
		}




}
void CImageprocessView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (num_snake_points == 0)
	{
		static int i = 0;
		COLORREF color = RGB(255, 0, 0);
		penWidth = 3;
		if (i == 0)
		{
			myPen.CreatePen(PS_SOLID, penWidth, color);
			i = 1;
		}
	}
	if (downflag)
	{


		CClientDC dc(this);

		//画出当前点
		dc.SelectObject(&myPen);
		dc.MoveTo(point);
		dc.LineTo(point);

		p = new(Node);
		p->point = point;
		p->next = lastnode;
		lastnode = p;


		if (num_snake_points == 0)
		{
			firstnode->point = point;
			p_OldPos = point;
		}

		if (num_snake_points >0)
		{
			dc.MoveTo(p_OldPos);
			dc.LineTo(point.x, point.y);
			CPoint firstpoint, thepoint;
			firstpoint = firstnode->point;
			thepoint = point;
			p_OldPos = point;
			//结束初始化控制点
			if (sqrt((firstpoint.x - point.x)*(firstpoint.x - point.x) + (firstpoint.y - point.y)*(firstpoint.y - point.y))<8)
			{
				pos = new CPoint[num_snake_points];
				long num = 0;

				Node *tmpnod;
				tmpnod = p;

				while (tmpnod != NULL)
				{
					p = tmpnod;
					pos[num] = p->point;
					num++;
					tmpnod = p->next;
					delete(p);
				}
				dc.MoveTo(p_OldPos);
				dc.LineTo(firstpoint.x, firstpoint.y);

				Gauss();
				Gradient();
				downflag = false;


			}

		}
		num_snake_points++;


	}

	CView::OnLButtonDown(nFlags, point);
}

void CImageprocessView::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1)
	{
		SnakeEvolution();
		CDC *pDC;
		pDC = GetDC();
		RedrawWindow();
		CPen pen1(PS_SOLID, 2, RGB(255, 30, 0)), pen2(PS_SOLID, 3, RGB(255, 0, 0));
		CPen* pOldPen = pDC->SelectObject(&pen1);
		int i = 0;
		pDC->MoveTo(Snake_points[0]);
		for (i = 0; i<num_snake_points; i++)
		{
			pDC->LineTo(Snake_points[i]);
			int x = Snake_points[i].x;
			int y = Snake_points[i].y;
			pOldPen = pDC->SelectObject(&pen2);
			pDC->Ellipse(x - 2, y - 2, x + 2, y + 2);
		}
		pDC->LineTo(Snake_points[0]);
	}
	else
	{
		//OnLevelsetevolution();
		Invalidate();

	}



	CView::OnTimer(nIDEvent);
}

//double * CImageprocessView::del22(double *u)
//{
//	double *K;
//	K = new double[nWidth*nHeight];
//
//	double *nxx;
//	nxx = new double[nWidth*nHeight];
//	double *nx;
//	nx = new double[nWidth*nHeight];
//
//	double *junk;
//	junk = new double[nWidth*nHeight];
//
//	double *nyy;
//	nyy = new double[nWidth*nHeight];
//	double *ny;
//	ny = new double[nWidth*nHeight];
//
//	DirGrad(u, nWidth, nHeight, nx, junk);
//	DirGrad(u, nWidth, nHeight, junk, ny);
//
//	DirGrad(nx, nWidth, nHeight, nxx, junk);
//	DirGrad(ny, nWidth, nHeight, junk, nyy);
//
//	for (int i = 0; i<nHeight; ++i)
//		for (int j = 0; j<nWidth; ++j)
//		{
//			K[i*nWidth + j] = nxx[i*nWidth + j] + nyy[i*nWidth + j];
//		}
//	delete nxx; nxx = NULL;
//	delete junk; junk = NULL;
//	delete nyy; nyy = NULL;
//	delete nx; nx = NULL;
//	delete ny; ny = NULL;
//
//	return K;
//
//}
//double * CImageprocessView::curvature_central(double *nx, double *ny)
//{
//
//	double *K;
//	K = new double[nWidth*nHeight];
//
//	double *nxx;
//	nxx = new double[nWidth*nHeight];
//
//	double *junk;
//	junk = new double[nWidth*nHeight];
//
//	double *nyy;
//	nyy = new double[nWidth*nHeight];
//
//	DirGrad(nx, nWidth, nHeight, nxx, junk);
//	DirGrad(ny, nWidth, nHeight, junk, nyy);
//
//	for (int i = 0; i<nHeight; ++i)
//		for (int j = 0; j<nWidth; ++j)
//		{
//			K[i*nWidth + j] = nxx[i*nWidth + j] + nyy[i*nWidth + j];
//		}
//	delete nxx; nxx = NULL;
//	delete junk; junk = NULL;
//	delete nyy; nyy = NULL;
//
//	return K;
//}

void CImageprocessView::DirGrad(double *ImageData, int nWidth, int nHeight,
	double *pnGradX, double *pnGradY)
{
	int y;
	int x;

	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			fG[y*nWidth + x].Ix = ImageData[y*nWidth + min(nWidth - 1, x + 1)]
				- ImageData[y*nWidth + max(0, x - 1)];
			pnGradX[y*nWidth + x] = ImageData[y*nWidth + min(nWidth - 1, x + 1)]
				- ImageData[y*nWidth + max(0, x - 1)];
		}
	}

	for (x = 0; x<nWidth; x++)
	{
		for (y = 0; y<nHeight; y++)
		{
			fG[y*nWidth + x].Iy = ImageData[min(nHeight - 1, y + 1)*nWidth + x]
				- ImageData[max(0, y - 1)*nWidth + x];
			pnGradY[y*nWidth + x] = ImageData[y*nWidth + min(nWidth - 1, x + 1)]
				- ImageData[y*nWidth + max(0, x - 1)];
		}
	}


}


