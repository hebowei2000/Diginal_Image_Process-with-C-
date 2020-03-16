#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\imgproc\types_c.h>
#include <iostream>

using namespace	cv;
using namespace std;

bool EqualizeHist(Mat gray, Mat result);

int main(int argc, char** argv)
{
	Mat Img, Img_raw;
	Img = imread("C:/Users/hebowei2000/Desktop/landscape.jpg");
	Img_raw = imread("C:/Users/hebowei2000/Desktop/landscape.jpg");
	if (Img.empty())
	{
		printf("could not open image\n");
		return -1;
	}
	Mat imageRGB[3];

	//devide a multi-channel array to several single-channel array
	split(Img, imageRGB);

	for (int i = 0;i < 3;i++)
	{
		EqualizeHist(imageRGB[i], imageRGB[i]);
	}

	//create a multi-channel array out of several single-channel array
	merge(imageRGB, 3, Img);
	imshow("raw_image", Img_raw);
	imshow("EqualizeHisted_Image", Img);

	//delay
	waitKey();

	return 0;

}

bool EqualizeHist(Mat gray, Mat result)
{   
	//calculate gray histogram of original image
	map<int, int>mp;
	for (int i = 0; i < gray.rows;i++)
	{
		uchar* ptr = (uchar*)gray.data + i * gray.cols;
		for (int j = 0; j < gray.cols;j++)
		{
			int value = ptr[j];
			mp[value]++;
		}

	}
	//calculate the cumulative gray histogram of the original image
	map<int, double>valuepro;
	double sumpro = 0.0;
	int sumpixel = gray.cols*gray.rows;
	for (int i = 0; i < 256; i++)
	{
		sumpro += (1.0*mp[i]) / sumpixel;
		valuepro[i] = sumpro;

	}
	// calculate the new gray distribution for result image
	for (int i = 0; i < gray.rows; i++)
	{
		uchar* ptr1= (uchar*)gray.data + i * gray.cols;
		for (int j = 0; j < gray.cols; j++)
		{
			int value = ptr1[j];
			double p = valuepro[value];
			result.at<uchar>(i, j) = 255 * p;

		}
	}
	return true;

}
