#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	//读入图像灰度图
	Mat Img = imread("C:/Users/hebowei2000/Desktop/expe3/rect-45度.bmp", IMREAD_GRAYSCALE);
	//判断图像是否加载成功
	if (Img.empty())
	{
		cout << "fail to load image" << endl;
		return -1;
	}
	else
		cout << "succeed to load image" << endl;

	//以0填充输入图像矩阵
	Mat padded;
	int m = getOptimalDFTSize(Img.rows);
	int n = getOptimalDFTSize(Img.cols);

	copyMakeBorder(Img, padded, 0, m - Img.rows, 0, n - Img.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg); //将planes融合合并成一个多通道数组complexImg
	dft(complexImg, complexImg); //进行傅里叶变换

	split(complexImg, planes);
	Mat real;
	planes[0].copyTo(real);
	Mat ima = planes[1];
	magnitude(planes[0], planes[1], planes[0]);
	Mat magImg = planes[0];

	magImg += Scalar::all(1);
	log(magImg, magImg);
	magImg = magImg(Rect(0, 0, magImg.cols-2, magImg.rows - 2));

	//重新排列傅里叶图像，是的原点位于图像中心
	int cx = magImg.cols / 2;
	int cy = magImg.rows / 2;
	Mat q0(magImg, Rect(0, 0, cx, cy)); //左上角图像
	Mat q1(magImg, Rect(cx,0,cx,cy)); //右上角图像
	Mat q2(magImg, Rect(0, cy, cx, cy)); //左下角图像
	Mat q3(magImg, Rect(cx, cy, cx, cy)); //右下脚图像

	//交换左上角和右下角图像
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//交换右上角和左下角图像
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

    // 用归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
	normalize(magImg, magImg, 0, 1, CV_MINMAX);

	imshow("image", Img);
	imshow("frequencyspctrum", magImg);
	imshow("real", real);
	imshow("ima", ima);
	waitKey(0);
	

	return 0;
	





}