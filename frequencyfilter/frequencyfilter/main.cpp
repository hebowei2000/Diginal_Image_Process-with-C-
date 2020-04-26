#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat gaussianlbrf(Mat src, float sigma); // ��˹��ͨ�˲�������
Mat ideallbrf(Mat src, float sigma); //�����ͨ�˲���
Mat idealhbrf(Mat src, float sigma); //�����ͨ�˲���
Mat freqfilt(Mat src, Mat blur); //Ƶ�����˲�����
Mat butterworthlbrf(Mat src, float sigma, int n);//������˹��ͨ�˲���
Mat homoblurf(Mat src, float sigma);//̬ͬ�˲���

int main()
{
	Mat padded11,padded12, padded21,padded22, padded31;
	Mat input1 = imread("C:/Users/hebowei2000/Desktop/expe3/grid.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat input2 = imread("C:/Users/hebowei2000/Desktop/expe3/lena.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat input3 = imread("C:/Users/hebowei2000/Desktop/expe3/cave.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	int w1 = getOptimalDFTSize(input1.cols); //��ȡ����dtf�����ųߴ�
	int h1 = getOptimalDFTSize(input1.rows); //��ȡ����dtf�����ųߴ�
	copyMakeBorder(input1, padded11, 0, h1 - input1.rows, 0, w1 - input1.cols, BORDER_CONSTANT, Scalar::all(0));  //�߽����
	padded11.convertTo(padded11, CV_32FC1); //��ͼ��ת��Ϊfloat��
	padded11.copyTo(padded12);
	int w2 = getOptimalDFTSize(input2.cols); //��ȡ����dtf�����ųߴ�
	int h2 = getOptimalDFTSize(input2.rows); //��ȡ����dtf�����ųߴ�
	copyMakeBorder(input2, padded21, 0, h2 - input2.rows, 0, w2 - input2.cols, BORDER_CONSTANT, Scalar::all(0));  //�߽����
	padded21.convertTo(padded21, CV_32FC1); //��ͼ��ת��Ϊfloat��
	padded21.copyTo(padded22);
	int w3 = getOptimalDFTSize(input3.cols); //��ȡ����dtf�����ųߴ�
	int h3 = getOptimalDFTSize(input3.rows); //��ȡ����dtf�����ųߴ�
	copyMakeBorder(input3, padded31, 0, h3 - input3.rows, 0, w3 - input3.cols, BORDER_CONSTANT, Scalar::all(0));  //�߽����
	padded31.convertTo(padded31, CV_32FC1); //��ͼ��ת��Ϊfloat��
	Mat ideallKernel = ideallbrf(padded11, 100); //�����ͨ�˲���
    Mat out11 = freqfilt(padded11, ideallKernel); //Ƶ�����˲�
	Mat idealhKernel = idealhbrf(padded12, 20); //�����ͨ�˲���
	Mat out12 = freqfilt(padded12, idealhKernel); //Ƶ�����˲�
	Mat gaussianKernel = gaussianlbrf(padded21, 18); //��˹��ͨ�˲���
	Mat out21 = freqfilt(padded21, gaussianKernel); //Ƶ�����˲�
	Mat butterworthlKernel = butterworthlbrf(padded22,220,1 ); //������˹��ͨ�˲���1
	Mat out22 = freqfilt(padded22, butterworthlKernel); //Ƶ�����˲�
	Mat homoKernel = homoblurf(padded31, 64); //̬ͬ�˲���
	Mat out31 = freqfilt(padded31, homoKernel); //Ƶ�����˲�
	imshow("grid.bmp", input1);
	imshow("�����ͨ�˲��� sigma=100", out11);
	imshow("�����ͨ�˲��� sigma=20", out12);
	imshow("lena.bmp", input2);
	imshow("��˹��ͨ�˲��� sigma=100", out21);
	imshow("������˹��ͨ�˲��� sigma=100 n=1", out22);
	imshow("cave.jpg", input3);
	imshow("̬ͬ�˲��� sigma=64", out31);
	waitKey(0);
	return 0;

}

//*****************************��˹��ͨ�˲���*************
Mat gaussianlbrf(Mat src, float sigma)
{
	Mat gaussianBlur(src.size(), CV_32FC1);
	float d0 = 2 * sigma*sigma;//����ԽС��Ƶ���˹�˲���Խխ���˳���Ƶ�ɷ�Խ�࣬ͼ��Խƽ��
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

//*******************�����ͨ�˲���***************
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
//*******************�����ͨ�˲���***************
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
//******************************������˹��ͨ�˲���***************
Mat butterworthlbrf(Mat src, float sigma,int n)
{
	Mat butterworthlBlur(src.size(), CV_32FC1);
	float d0 = sigma;//����ԽС��Ƶ���˹�˲���Խխ���˳���Ƶ�ɷ�Խ�࣬ͼ��Խƽ��
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
//*******************̬ͬ�˲���********************************
Mat homoblurf(Mat src, float sigma)
{   
	double gammaH = 1.5;
	double gammaL = 0.5;
	int c = 1;
	Mat homoblur(src.size(), CV_32FC1);
	float d0 = 2 * sigma*sigma;//����ԽС��Ƶ���˹�˲���Խխ���˳���Ƶ�ɷ�Խ�࣬ͼ��Խƽ��
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
//*************************Ƶ�����˲�*****************
Mat freqfilt(Mat src, Mat blur)
{
	//************DFT****************
	Mat plane[] = { src,Mat::zeros(src.size(),CV_32FC1) };
	Mat complexIm;
	merge(plane, 2, complexIm);//��src,zeros����������ϲ�Ϊһ��2ͨ����Mat������
	dft(complexIm, complexIm); //���и���Ҷ�任���������������

	//*********���Ļ�************************
	split(complexIm, plane);//����ʵ�����鲿
	int cx = plane[0].cols / 2; 
	int cy = plane[0].rows / 2;
	Mat part1_r(plane[0], Rect(0, 0, cx, cy));
	Mat part2_r(plane[0], Rect(cx, 0, cx, cy));
	Mat part3_r(plane[0], Rect(0, cy, cx, cy));
	Mat part4_r(plane[0], Rect(cx, cy, cx, cy));

	Mat temp;
	part1_r.copyTo(temp); //���������½���λ�ã�ʵ����
	part4_r.copyTo(part1_r);
	temp.copyTo(part4_r);

	part2_r.copyTo(temp); //���������½���λ�ã�ʵ����
	part3_r.copyTo(part2_r);
	temp.copyTo(part3_r);

	Mat part1_i(plane[1], Rect(0, 0, cx, cy));
	Mat part2_i(plane[1], Rect(cx, 0, cx, cy));
	Mat part3_i(plane[1], Rect(0, cy, cx, cy));
	Mat part4_i(plane[1], Rect(cx, cy, cx, cy));

	part1_i.copyTo(temp); //���������½���λ�ã��鲿��
	part4_i.copyTo(part1_i);
	temp.copyTo(part4_i);

	part2_i.copyTo(temp); //���������½���λ�ã��鲿��
	part3_i.copyTo(part2_i);
	temp.copyTo(part3_i);

	//***************�˲���������DFT����ĳ˻�************
	Mat blur_r, blur_i, BLUR;
	multiply(plane[0], blur, blur_r);
	multiply(plane[1], blur, blur_i);
	Mat plane1[] = { blur_r,blur_i };
	merge(plane1, 2, BLUR);

	/*magnitude(plane[0], plane[1], plane[0]);//��ȡ����ͼ��0ͨ��Ϊʵ��ͨ����1Ϊ�鲿����Ϊ��ά����Ҷ�任����Ǹ���  
	plane[0] += Scalar::all(1);  //����Ҷ��o�����ͼƬ���÷��������ж�����������ȽϺÿ� 
	log(plane[0], plane[0]);    // float�͵ĻҶȿռ�Ϊ[0��1])
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);  //��һ��������ʾ
	imshow("ԭͼ��Ƶ��ͼ", plane[0]);*/
	

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

	idft(BLUR, BLUR); //idft���Ҳ�Ǹ���
	split(BLUR, plane); //����ͨ��
	magnitude(plane[0], plane[1], plane[0]); //��ͨ����ģ
	normalize(plane[0], plane[0], 1, 0, CV_MINMAX);
	return plane[0];
	
}
