#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat gaussianlbrf(Mat src, float sigma); // 高斯低通滤波器函数
Mat ideallbrf(Mat src, float sigma); //理想低通滤波器
Mat idealhbrf(Mat src, float sigma); //理想高通滤波器
Mat freqfilt(Mat src, Mat blur); //频率域滤波函数
Mat butterworthlbrf(Mat src, float sigma, int n);//巴特沃斯低通滤波器
Mat homoblurf(Mat src, float sigma);//同态滤波器

int main()
{
	Mat padded11,padded12, padded21,padded22, padded31;
	Mat input1 = imread("C:/Users/hebowei2000/Desktop/expe3/grid.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat input2 = imread("C:/Users/hebowei2000/Desktop/expe3/lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat input3 = imread("C:/Users/hebowei2000/Desktop/expe3/cave.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	int w1 = getOptimalDFTSize(input1.cols); //获取进行dtf的最优尺寸
	int h1 = getOptimalDFTSize(input1.rows); //获取进行dtf的最优尺寸
	copyMakeBorder(input1, padded11, 0, h1 - input1.rows, 0, w1 - input1.cols, BORDER_CONSTANT, Scalar::all(0));  //边界填充
	padded11.convertTo(padded11, CV_32FC1); //将图像转换为float型
	padded11.copyTo(padded12);
	int w2 = getOptimalDFTSize(input2.cols); //获取进行dtf的最优尺寸
	int h2 = getOptimalDFTSize(input2.rows); //获取进行dtf的最优尺寸
	copyMakeBorder(input2, padded21, 0, h2 - input2.rows, 0, w2 - input2.cols, BORDER_CONSTANT, Scalar::all(0));  //边界填充
	padded21.convertTo(padded21, CV_32FC1); //将图像转换为float型
	padded21.copyTo(padded22);
	int w3 = getOptimalDFTSize(input3.cols); //获取进行dtf的最优尺寸
	int h3 = getOptimalDFTSize(input3.rows); //获取进行dtf的最优尺寸
	copyMakeBorder(input3, padded31, 0, h3 - input3.rows, 0, w3 - input3.cols, BORDER_CONSTANT, Scalar::all(0));  //边界填充
	padded31.convertTo(padded31, CV_32FC1); //将图像转换为float型
	Mat ideallKernel = ideallbrf(padded11, 100); //理想低通滤波器
    Mat out11 = freqfilt(padded11, ideallKernel); //频率域滤波
	Mat idealhKernel = idealhbrf(padded12, 20); //理想高通滤波器
	Mat out12 = freqfilt(padded12, idealhKernel); //频率域滤波
	Mat gaussianKernel = gaussianlbrf(padded21, 18); //高斯低通滤波器
	Mat out21 = freqfilt(padded21, gaussianKernel); //频率域滤波
	Mat butterworthlKernel = butterworthlbrf(padded22,220,1 ); //巴特沃斯低通滤波器1
	Mat out22 = freqfilt(padded22, butterworthlKernel); //频率域滤波
	Mat homoKernel = homoblurf(padded31, 64); //同态滤波器
	Mat out31 = freqfilt(padded31, homoKernel); //频率域滤波
	imshow("grid.bmp", input1);
	imshow("理想低通滤波器 sigma=100", out11);
	imshow("理想高通滤波器 sigma=20", out12);
	imshow("lena.bmp", input2);
	imshow("高斯低通滤波器 sigma=100", out21);
	imshow("巴特沃斯低通滤波器 sigma=100 n=1", out22);
	imshow("cave.jpg", input3);
	imshow("同态滤波器 sigma=64", out31);
	waitKey(0);
	return 0;

}

//*****************************高斯低通滤波器*************
Mat gaussianlbrf(Mat src, float sigma)
{
	Mat gaussianBlur(src.size(), CV_32FC1);
	float d0 = 2 * sigma*sigma;//方差越小，频域高斯滤波器越窄，滤除高频成分越多，图像越平滑
	for (int i = 0;i < src.rows;i++)
	{
		for (int j = 0;j < src.cols;j++)
		{
			float d = pow(float(i - src.rows / 2), 2) + pow(float(j - src.cols / 2), 2);
			gaussianBlur.at<float>(i, j) = expf(-d / d0);
		}
	}
	return gaussianBlur;

}

//*******************理想低通滤波器***************
Mat ideallbrf(Mat src, float sigma)
{
	Mat ideallBlur(src.size(), CV_32FC1);
	float d0 = sigma;
	for (int i = 0;i < src.rows;i++)
	{
		for (int j = 0;j < src.cols;j++)
		{
			float d = sqrt(pow(float(i - src.rows / 2), 2) + pow(float(j - src.cols / 2), 2));
			if (d <= d0)
			{
				ideallBlur.at<float>(i, j) = 1;

			}
			else
			{
				ideallBlur.at<float>(i, j) = 0;
			}
		}
		
	}
	return ideallBlur;
}
//*******************理想高通滤波器***************
Mat idealhbrf(Mat src, float sigma)
{
	Mat idealhBlur(src.size(), CV_32FC1);
	float d0 = sigma;
	for (int i = 0;i < src.rows;i++)
	{
		for (int j = 0;j < src.cols;j++)
		{
			float d = sqrt(pow(float(i - src.rows / 2), 2) + pow(float(j - src.cols / 2), 2));
			if (d <= d0)
			{
				idealhBlur.at<float>(i, j) = 0;

			}
			else
			{
				idealhBlur.at<float>(i, j) = 1;
			}
		}

	}
	return idealhBlur;
}
//******************************巴特沃斯低通滤波器***************
Mat butterworthlbrf(Mat src, float sigma,int n)
{
	Mat butterworthlBlur(src.size(), CV_32FC1);
	float d0 = sigma;//方差越小，频域高斯滤波器越窄，滤除高频成分越多，图像越平滑
	for (int i = 0;i < src.rows;i++)
	{
		for (int j = 0;j < src.cols;j++)
		{
			float d = pow(float(i - src.rows / 2), 2) + pow(float(j - src.cols / 2), 2);
			butterworthlBlur.at<float>(i, j) = 1.0/(1+pow(d/d0,2*n));
		}
	}
	return butterworthlBlur;

}
//*******************同态滤波器********************************
Mat homoblurf(Mat src, float sigma)
{   
	double gammaH = 1.5;
	double gammaL = 0.5;
	int c = 1;
	Mat homoblur(src.size(), CV_32FC1);
	float d0 = 2 * sigma*sigma;//方差越小，频域高斯滤波器越窄，滤除高频成分越多，图像越平滑
	for (int i = 0;i < src.rows;i++)
	{
		for (int j = 0;j < src.cols;j++)
		{
			float d = pow(float(i - src.rows / 2), 2) + pow(float(j - src.cols / 2), 2);
			homoblur.at<float>(i, j) = (gammaH-gammaL)*(1-expf(-c*d / d0))+gammaL;
		}
	}
	return homoblur;

}
//*************************频率域滤波*****************
Mat freqfilt(Mat src, Mat blur)
{
	//************DFT****************
	Mat plane[] = { src,Mat::zeros(src.size(),CV_32FC1) };
	Mat complexIm;
	merge(plane, 2, complexIm);//将src,zeros这两个矩阵合并为一个2通道的Mat类容器
	dft(complexIm, complexIm); //进行傅里叶变换，结果保存在自身

	//*********中心化************************
	split(complexIm, plane);//分类实部与虚部
	int cx = plane[0].cols / 2; 
	int cy = plane[0].rows / 2;
	Mat part1_r(plane[0], Rect(0, 0, cx, cy));
	Mat part2_r(plane[0], Rect(cx, 0, cx, cy));
	Mat part3_r(plane[0], Rect(0, cy, cx, cy));
	Mat part4_r(plane[0], Rect(cx, cy, cx, cy));

	Mat temp;
	part1_r.copyTo(temp); //左上与右下交换位置（实部）
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp); //右上与左下交换位置（实部）
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	Mat part1_i(plane[1], Rect(0, 0, cx, cy));
	Mat part2_i(plane[1], Rect(cx, 0, cx, cy));
	Mat part3_i(plane[1], Rect(0, cy, cx, cy));
	Mat part4_i(plane[1], Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp); //左上与右下交换位置（虚部）
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp); //右上与左下交换位置（虚部）
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);

	//***************滤波器函数与DFT结果的乘积************
	Mat blur_r, blur_i, BLUR;
	multiply(plane[0], blur, blur_r);
	multiply(plane[1], blur, blur_i);
	Mat plane1[] = { blur_r,blur_i };
	merge(plane1, 2, BLUR);

	/*magnitude(plane[0], plane[1], plane[0]);//获取幅度图像，0通道为实部通道，1为虚部，因为二维傅立叶变换结果是复数  
	plane[0] += Scalar::all(1);  //傅立叶变o换后的图片不好分析，进行对数处理，结果比较好看 
	log(plane[0], plane[0]);    // float型的灰度空间为[0，1])
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);  //归一化便于显示
	imshow("原图像频谱图", plane[0]);*/
	

	//**************IDFT**************************
	Mat part111(BLUR, Rect(0, 0, cx, cy));
	Mat part222(BLUR, Rect(cx, 0, cx, cy));
	Mat part333(BLUR, Rect(0, cy, cx, cy));
	Mat part444(BLUR, Rect(cx, cy, cx, cy));

	part111.copyTo(temp);
	part444.copyTo(part111);
	temp.copyTo(part444);

	part222.copyTo(temp);
	part333.copyTo(part222);
	temp.copyTo(part333);

	idft(BLUR, BLUR); //idft结果也是复数
	split(BLUR, plane); //分离通道
	magnitude(plane[0], plane[1], plane[0]); //求通道的模
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);
	return plane[0];
	
}
