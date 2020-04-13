#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//Roberts算子函数
void Roberts(Mat &src, Mat &dst)
{
	dst = src.clone();
	int width = dst.cols;
	int height = dst.rows;
	uchar pixel[4];
	for (int j = 0;j < height - 1;j++)
	{
		uchar*Lup = dst.ptr<uchar>(j);
		uchar*Ldown = dst.ptr<uchar>(j+1);
		for (int i = 0;i < width - 1;i++)
		{
			//Roberts 算子
			pixel[0] = Lup[i];
			pixel[1] = Lup[i+1];
			pixel[2] = Ldown[i];
			pixel[3] = Ldown[i + 1];
			//生成梯度值
			Lup[i] = sqrt(double((pixel[0] - pixel[3])*(pixel[0] - pixel[3]) + (pixel[1] - pixel[2])*(pixel[1] - pixel[2])));
			
		}
	}
}
//Sobel算子函数
void Sobel(Mat &src, Mat &dst)
{
	dst = src.clone();
	int width = dst.cols;
	int height = dst.rows;
	uchar pixel[9];
	for (int j = 1;j < height - 1;j++)
	{
		uchar*Lup = src.ptr<uchar>(j - 1);
		uchar*L = src.ptr<uchar>(j);
		uchar*Ldown = src.ptr<uchar>(j + 1);
		uchar*Lwrite = dst.ptr<uchar>(j);
		for (int i = 1;i < width - 1;i++)
		{
			//Sobel 算子
			pixel[0] = Lup[i - 1];
			pixel[1] = Lup[i];
			pixel[2] = Lup[i + 1];
			pixel[3] = L[i - 1];
			pixel[4] = L[i];
			pixel[5] = L[i + 1];
			pixel[6] = Ldown[i - 1];
			pixel[7] = Ldown[i];
			pixel[8] = Ldown[i + 1];
			//生成梯度值
			Lwrite[i] = sqrt(double((-pixel[0]+pixel[2]-2*pixel[3]+2*pixel[5]-pixel[6]+pixel[8])*(-pixel[0] + pixel[2] - 2 * pixel[3] + 2 * pixel[5] - pixel[6] + pixel[8]) + (-pixel[0] - 2*pixel[1] - pixel[2] + pixel[6] +2*pixel[7]+ pixel[8])*(-pixel[0] - 2 * pixel[1] - pixel[2] + pixel[6] + 2 * pixel[7] + pixel[8])));

		}
	}
}
//Laplace算子函数
void Laplace(Mat &src, Mat &dst)
{
	dst = src.clone();
	int width = dst.cols;
	int height = dst.rows;
	uchar pixel[9];
	for (int j = 1;j < height - 1;j++)
	{
		uchar*Lup = src.ptr<uchar>(j - 1);
		uchar*L = src.ptr<uchar>(j);
		uchar*Ldown = src.ptr<uchar>(j + 1);
		uchar*Lwrite = dst.ptr<uchar>(j);
		for (int i = 1;i < width - 1;i++)
		{
			//Sobel 算子
			pixel[0] = Lup[i - 1];
			pixel[1] = Lup[i];
			pixel[2] = Lup[i + 1];
			pixel[3] = L[i - 1];
			pixel[4] = L[i];
			pixel[5] = L[i + 1];
			pixel[6] = Ldown[i - 1];
			pixel[7] = Ldown[i];
			pixel[8] = Ldown[i + 1];
			//生成梯度值
			Lwrite[i] = sqrt(double(( pixel[1] + pixel[3] - 4 * pixel[4] + pixel[5] + pixel[7])*(pixel[1] + pixel[3] - 4 * pixel[4] + pixel[5] + pixel[7])));

		}
	}
}
int main()
{
	Mat image0 = imread("C:/Users/hebowei2000/Desktop/lena.bmp", 1);
	cvtColor(image0, image0, COLOR_RGB2GRAY);
	Mat image1(image0.size(), image0.type());
	Mat image2(image0.size(), image0.type());
	Mat image3(image0.size(), image0.type());
	Roberts(image0, image1);
	Sobel(image0, image2);
	Laplace(image0, image3);
	namedWindow("原图");
	imshow("原图", image0);
	namedWindow("Roberts算子图");
	imshow("Roberts算子图", image1);
	namedWindow("Sobel算子图");
	imshow("Sobel算子图", image2);
	namedWindow("Laplace算子图");
	imshow("Laplace算子图", image3);
	waitKey();
	return 0;
}