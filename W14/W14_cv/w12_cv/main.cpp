#include<iostream>
#include<vector>

#include<opencv2\opencv.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>


using namespace cv;
using namespace std;

Mat src, src_gray;
//Mat dst;

void thinimage(Mat &srcimage)//单通道、二值化后的图像  
{

	vector<Point> deletelist1;
	int Structure[9];
	int nl = srcimage.rows;
	int nc = srcimage.cols;
	while (true)
	{
		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					Structure[0] = 1;
					if (data_last[i] == 255) Structure[1] = 1;
					else  Structure[1] = 0;
					if (data_last[i + 1] == 255) Structure[2] = 1;
					else  Structure[2] = 0;
					if (data[i + 1] == 255) Structure[3] = 1;
					else  Structure[3] = 0;
					if (data_next[i + 1] == 255) Structure[4] = 1;
					else  Structure[4] = 0;
					if (data_next[i] == 255) Structure[5] = 1;
					else  Structure[5] = 0;
					if (data_next[i - 1] == 255) Structure[6] = 1;
					else  Structure[6] = 0;
					if (data[i - 1] == 255) Structure[7] = 1;
					else  Structure[7] = 0;
					if (data_last[i - 1] == 255) Structure[8] = 1;
					else  Structure[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + Structure[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((Structure[1] == 0) && (Structure[2] == 1)) ap++;
						if ((Structure[2] == 0) && (Structure[3] == 1)) ap++;
						if ((Structure[3] == 0) && (Structure[4] == 1)) ap++;
						if ((Structure[4] == 0) && (Structure[5] == 1)) ap++;
						if ((Structure[5] == 0) && (Structure[6] == 1)) ap++;
						if ((Structure[6] == 0) && (Structure[7] == 1)) ap++;
						if ((Structure[7] == 0) && (Structure[8] == 1)) ap++;
						if ((Structure[8] == 0) && (Structure[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((Structure[1] * Structure[7] * Structure[5] == 0) && (Structure[3] * Structure[5] * Structure[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();

		for (int j = 1; j < (nl - 1); j++)
		{
			uchar* data_last = srcimage.ptr<uchar>(j - 1);
			uchar* data = srcimage.ptr<uchar>(j);
			uchar* data_next = srcimage.ptr<uchar>(j + 1);
			for (int i = 1; i < (nc - 1); i++)
			{
				if (data[i] == 255)
				{
					Structure[0] = 1;
					if (data_last[i] == 255) Structure[1] = 1;
					else  Structure[1] = 0;
					if (data_last[i + 1] == 255) Structure[2] = 1;
					else  Structure[2] = 0;
					if (data[i + 1] == 255) Structure[3] = 1;
					else  Structure[3] = 0;
					if (data_next[i + 1] == 255) Structure[4] = 1;
					else  Structure[4] = 0;
					if (data_next[i] == 255) Structure[5] = 1;
					else  Structure[5] = 0;
					if (data_next[i - 1] == 255) Structure[6] = 1;
					else  Structure[6] = 0;
					if (data[i - 1] == 255) Structure[7] = 1;
					else  Structure[7] = 0;
					if (data_last[i - 1] == 255) Structure[8] = 1;
					else  Structure[8] = 0;
					int whitepointtotal = 0;
					for (int k = 1; k < 9; k++)
					{
						whitepointtotal = whitepointtotal + Structure[k];
					}
					if ((whitepointtotal >= 2) && (whitepointtotal <= 6))
					{
						int ap = 0;
						if ((Structure[1] == 0) && (Structure[2] == 1)) ap++;
						if ((Structure[2] == 0) && (Structure[3] == 1)) ap++;
						if ((Structure[3] == 0) && (Structure[4] == 1)) ap++;
						if ((Structure[4] == 0) && (Structure[5] == 1)) ap++;
						if ((Structure[5] == 0) && (Structure[6] == 1)) ap++;
						if ((Structure[6] == 0) && (Structure[7] == 1)) ap++;
						if ((Structure[7] == 0) && (Structure[8] == 1)) ap++;
						if ((Structure[8] == 0) && (Structure[1] == 1)) ap++;
						if (ap == 1)
						{
							if ((Structure[1] * Structure[3] * Structure[5] == 0) && (Structure[3] * Structure[1] * Structure[7] == 0))
							{
								deletelist1.push_back(Point(i, j));
							}
						}
					}
				}
			}
		}
		if (deletelist1.size() == 0) break;
		for (size_t i = 0; i < deletelist1.size(); i++)
		{
			Point tem;
			tem = deletelist1[i];
			uchar* data = srcimage.ptr<uchar>(tem.y);
			data[tem.x] = 0;
		}
		deletelist1.clear();
	}
}

/** @函数 main */
int main(int argc, char** argv)
{
	/// 装载图像
	src = imread("3.bmp");

	if (!src.data)
	{
		return -1;
	}

	/// 原图像转换为灰度图像
	cvtColor(src, src_gray, CV_BGR2GRAY);
	
	//骨架提取
	thinimage(src_gray);

	//显示图像
	imshow("dest", src_gray);

	/// 等待用户反应
	waitKey(0);
	return 0;
}