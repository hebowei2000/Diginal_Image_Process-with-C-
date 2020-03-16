#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\imgproc\types_c.h>
#include <iostream>

using namespace	cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat Img, Img_raw;
	Img = imread("C:/Users/hebowei2000/Desktop/girl.jpg", 0);
	Img_raw = imread("C:/Users/hebowei2000/Desktop/girl.jpg", 0);
	if (Img.empty())
	{
		printf("could not open image\n");
		return -1;
	}
	int row = Img.rows;
	int col = Img.cols;
	int seg_size = 3;
	Scalar mean;
	Scalar stddev;
	Mat Seg = Mat::zeros(seg_size, seg_size, CV_8UC1);
	meanStdDev(Img, mean, stddev);
	double mean_total = mean.val[0];
	double std_total = stddev.val[0];
	double E = 4;
	double k0 = 0.4;
	double k1 = 0.02;
	double k2 = 0.4;

	for (int i = 1; i < row - 1; i++)
	{
		for (int j = 1; j < col - 1; j++)
		{
			Seg = Img_raw(Rect(j - 1, i - 1, seg_size, seg_size));
			meanStdDev(Seg, mean, stddev);
			if (mean.val[0] <= k0 * mean_total)
			{
				if (stddev.val[0] <= k2 * std_total&& stddev.val[0] >= k1 * std_total) {
					Img.at<uchar>(i, j) = Img.at<uchar>(i, j) * E;
					//equalizeHist(parts[i][j], parts[i][j]);
				}
			}
		}
	}
	imshow("original", Img_raw);
	imshow("new", Img);
	waitKey(0);
	return 0;
}