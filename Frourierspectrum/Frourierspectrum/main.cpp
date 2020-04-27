#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	//����ͼ��Ҷ�ͼ
	Mat Img = imread("C:/Users/hebowei2000/Desktop/expe3/rect-45��.bmp", IMREAD_GRAYSCALE);
	//�ж�ͼ���Ƿ���سɹ�
	if (Img.empty())
	{
		cout << "fail to load image" << endl;
		return -1;
	}
	else
		cout << "succeed to load image" << endl;

	//��0�������ͼ�����
	Mat padded;
	int m = getOptimalDFTSize(Img.rows);
	int n = getOptimalDFTSize(Img.cols);

	copyMakeBorder(Img, padded, 0, m - Img.rows, 0, n - Img.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexImg;
	merge(planes, 2, complexImg); //��planes�ںϺϲ���һ����ͨ������complexImg
	dft(complexImg, complexImg); //���и���Ҷ�任

	split(complexImg, planes);
	Mat real;
	planes[0].copyTo(real);
	Mat ima = planes[1];
	magnitude(planes[0], planes[1], planes[0]);
	Mat magImg = planes[0];

	magImg += Scalar::all(1);
	log(magImg, magImg);
	magImg = magImg(Rect(0, 0, magImg.cols-2, magImg.rows - 2));

	//�������и���Ҷͼ���ǵ�ԭ��λ��ͼ������
	int cx = magImg.cols / 2;
	int cy = magImg.rows / 2;
	Mat q0(magImg, Rect(0, 0, cx, cy)); //���Ͻ�ͼ��
	Mat q1(magImg, Rect(cx,0,cx,cy)); //���Ͻ�ͼ��
	Mat q2(magImg, Rect(0, cy, cx, cy)); //���½�ͼ��
	Mat q3(magImg, Rect(cx, cy, cx, cy)); //���½�ͼ��

	//�������ϽǺ����½�ͼ��
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//�������ϽǺ����½�ͼ��
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

    // �ù�һ��������0-1֮��ĸ�����������任Ϊ���ӵ�ͼ���ʽ
	normalize(magImg, magImg, 0, 1, CV_MINMAX);

	imshow("image", Img);
	imshow("frequencyspctrum", magImg);
	imshow("real", real);
	imshow("ima", ima);
	waitKey(0);
	

	return 0;
	





}