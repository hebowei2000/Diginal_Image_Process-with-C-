#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
int main()
{
	Mat image0 = imread("C:/Users/hebowei2000/Desktop/cells.bmp",1);
	Mat image = image0;
	cvtColor(image0, image0, COLOR_RGB2GRAY);
	cvtColor(image, image, COLOR_RGB2GRAY);
	int width = image.cols;
	int height = image.rows;
	int x = 0;
	int y = 0;
	float ip1, ip2, is1, is2, w0, w1, mean1, mean2, mean, deltaTmp, deltaMax;
	int pixelCount[256];
	int i, j;
	int pixelSum = width * height;
	int thres = 0;
	for (i = 0;i < 256;i++)
	{
		pixelCount[i] = 0;
	}
	int pixel = 0;
	//灰度分布直方图
	for (i = 0;i < height; i++)
	{
		for (j = 0;j < width;j++)
		{
			pixel = image.at<uchar>(i, j);
			pixelCount[pixel]++;
		}
	}
	//遍历灰度级【0，255】，计算出类间方差，方差最大对应的灰度值是最佳阈值
	deltaMax = 0;
	for (i = 0;i < 256;i++)
	{
		ip1 = ip2 = is1 = is2 = w0 = w1 = mean1 = mean2 = mean = deltaTmp = 0;
		for (j = 0;j < 256;j++) //遍历像素
		{
			if (j <= i)  //背景部分
			{
				ip1 += pixelCount[j] * j;
				is1 += pixelCount[j];
			}
			else    //前景部分
			{
				ip2 += pixelCount[j] * j;
				is2 += pixelCount[j];
			}
		}
		mean1 = ip1 / is1; //第一类像素平均灰度值
		w0 = (float)is1 / (float)pixelSum; //第一类像素占整个图像的比例
		mean2 = ip2 / is2; //第二类像素平均灰度值
		w1 = 1 - w0; //第二类像素占整个图像的比例
		mean = w0 * mean1 + w1 * mean2;
		deltaTmp = w0 * w1*(mean1 - mean2)*(mean1 - mean2);//灰度值i为阈值时的类间方差
		if(deltaTmp>deltaMax)
		{
			deltaMax = deltaTmp;
			thres = i;

		}

	}
	//threshold(image, image, 0, 255, CV_THRESH_OTSU);
	cout << thres << endl;
	cv::threshold(image, image, thres, 255, cv::THRESH_BINARY);
	/*for (i = 0;i < width; i++)
	{
		for (j = 0;j < height; j++)
		{
			if( image.at<uchar>(i, j) > thres)
				image.at<uchar>(i, j) = 255;
			else
				image.at<uchar>(i, j) = 0;
		}
	}
	*/

	namedWindow("原图");
	imshow("原图", image0);
	namedWindow("OSTU效果图");
	imshow("OSTU效果图", image);
	waitKey();
	return 0;
}