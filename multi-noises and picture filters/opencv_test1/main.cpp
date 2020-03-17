#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <time.h>

using namespace	cv;
using namespace std;

Mat MeanImage, MedianImage;
Mat Img, Img_raw;
Mat img_out1;
Mat img_out2;
Mat img_out3;
Mat img_out4;
int g_nMeanBlurvalue = 5; //Mean filter kernel value
int g_nMedianBlurvalue = 5; // Median filter kernel value

Mat addGuassianNoise_withRNG(Mat &Img);
double generateGuassianNoise(double m, double sigma);
Mat addGuassianNoise_withscanning(Mat &Img);
Mat addSaltAndPepperNoise(Mat &Img, int n);
Mat addImpulseNoise(Mat &Img, int n);
static void on_MeanBlur(int, void*);
static void on_MedianBlur(int, void*);


int main(int argc, char** argv)
{
	//read the raw image from Desktop
	Img = imread("C:/Users/hebowei2000/Desktop/lena.bmp");
	Img_raw = imread("C:/Users/hebowei2000/Desktop/lena.bmp");
	if (Img.empty())
	{
		printf("could not open image\n");
		return -1;
	}
	namedWindow("original");
	imshow("original", Img);

	
	//add the Guassian noise with class RNG
	  img_out1 = addGuassianNoise_withRNG(Img);
	namedWindow("img_out1");
	imshow("img_out1", img_out1);

	//add the Guassian noise with pixel-level scanning
	  img_out2 = addGuassianNoise_withscanning(Img);
	namedWindow("img_out2");
	imshow("img_out2", img_out2);

	//add the salt and pepper noise 
	 img_out3 = addSaltAndPepperNoise(Img, 3000);
	namedWindow("img_out3");
	imshow("img_out3", img_out3);

	//add the Impulse noise
	 img_out4 = addImpulseNoise(Img, 6000);
	namedWindow("img_out4");
	imshow("img_out4", img_out4);

	//Mean filter
	namedWindow("Mean filter");
	createTrackbar("MeanBlurvalue:", "Mean filter", &g_nMeanBlurvalue, 50, on_MeanBlur);
	on_MeanBlur(g_nMeanBlurvalue, 0);
	imshow("Mean filter", MeanImage);

	//Median filter
	namedWindow("Median filter");
	createTrackbar("MedianBlurvalue:", "Median filter", &g_nMedianBlurvalue, 50, on_MedianBlur);
	on_MedianBlur(g_nMedianBlurvalue, 0);
	imshow("Median filter", MedianImage);

	//delay
	waitKey();


	return 0;

}

Mat addGuassianNoise_withRNG(Mat &Img)
{
	//add the Guassian noise with class RNG
	Mat img_out(Img.size(), Img.type());
	Mat noise(Img.size(), Img.type());
	RNG rng(time(NULL));
	rng.fill(noise, RNG::NORMAL, 10, 36);
	cv::add(Img, noise, img_out);
	return img_out;
}

double generateGuassianNoise(double mu, double sigma)
{
	static double z_0;
	double u_1, u_2;
	u_1 = rand() * (1.0 / RAND_MAX);
	u_2 = rand() * (1.0 / RAND_MAX);
	z_0 = sqrt(-2.0*log(u_1))*cos(2 * CV_PI*u_2);
	return z_0 * sigma + mu;

}

Mat addGuassianNoise_withscanning(Mat &Img)
{
	Mat dstImage = Img.clone();
	int channels = dstImage.channels();
	int rowsNumber = dstImage.rows;
	int colsNumber = dstImage.cols*channels;
	for (int i = 0; i < rowsNumber;i++)
	{
		for (int j = 0; j < colsNumber; j++)
		{
			//add Guassian Noise
			int val = dstImage.ptr<uchar>(i)[j] + generateGuassianNoise(2, 0.8) * 32;
			if (val < 0)
				val = 0;
			if (val > 255)
				val = 255;
			dstImage.ptr<uchar>(i)[j] = (uchar)val;
		}
	}
	return dstImage;
}

Mat addSaltAndPepperNoise(Mat &Img, int n)
{
	Mat dstImage = Img.clone();
	for (int k = 0; k < n; k++)
	{
		//choose the row and column randomly
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		// judge the number of channels
		if (dstImage.channels() == 1)
		{
			// add the salt noise
			dstImage.at<uchar>(i, j) = 255;

		}
		else
		{
			dstImage.at<Vec3b>(i, j)[0] = 255;
			dstImage.at<Vec3b>(i, j)[1] = 255;
			dstImage.at<Vec3b>(i, j)[2] = 255;
		}
	}
	for (int k = 0; k < n; k++)
	{
		//choose the row and column randomly
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		// judge the number of channels
		if (dstImage.channels() == 1)
		{
			// add the pepper noise
			dstImage.at<uchar>(i, j) = 0;

		}
		else
		{
			dstImage.at<Vec3b>(i, j)[0] = 0;
			dstImage.at<Vec3b>(i, j)[1] = 0;
			dstImage.at<Vec3b>(i, j)[2] = 0;
		}
	}
	return dstImage;
}

Mat addImpulseNoise(Mat &Img, int n)
{
	Mat dstImage = Img.clone();
	for (int k = 0; k < n; k++)
	{
		//choose the row and column randomly
		int i = rand() % dstImage.rows;
		int j = rand() % dstImage.cols;
		// judge the number of channels
		if (dstImage.channels() == 1)
		{
			// add the salt noise
			dstImage.at<uchar>(i, j) = 255;

		}
		else
		{
			dstImage.at<Vec3b>(i, j)[0] = 255;
			dstImage.at<Vec3b>(i, j)[1] = 255;
			dstImage.at<Vec3b>(i, j)[2] = 255;
		}
	}
	return dstImage;
}
static void on_MeanBlur(int, void*)
{
	blur(img_out3, MeanImage, Size(g_nMeanBlurvalue + 1, g_nMeanBlurvalue + 1));

}
static void on_MedianBlur(int, void*)
{
	medianBlur(img_out3, MedianImage, g_nMedianBlurvalue * 2 + 1);
}
