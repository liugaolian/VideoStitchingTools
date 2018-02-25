#include "XImagePro.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

//����ԭͼ������������
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty())return;
	this->src1 = mat1;						 //˭����Set()�����thisָ�����˭
	this->src2 = mat2;
	this->src1.copyTo(des);
}

//�������ȺͶԱȶ�
///@para bright double ���� 0~100
///@para contrast int �Աȶ� 1.0 ~ 3.0
void XImagePro::Gain(double bright, double contrast)
{
	if (des.empty())return;
	des.convertTo(des, -1, contrast, bright); //�������ȺͶԱȶ�
}

//��ĳ��ROI�����������
///@para srcImg Mat& ԭͼ 
///@para roi Rect Ŀ������ 
void XImagePro::Mosaic()
{
	if (des.empty())return;
	//Mat src1_cpy = src1.clone(); //���������Ѿ��������ԱȶȺ����ȵ�ͼƬ����
	//Mat src1_cpy = des.clone();
	Mat src1_cpy = Get();
	RNG rng;
	int randomNum; //���������ֵ 
	double src1_cpy_cols = src1_cpy.cols;
	double src1_cpy_rows = src1_cpy.rows;
	Point ptL = Point(src1_cpy_cols / 64 * 48, src1_cpy_rows / 64 * 1);         //�����������Ͻ�����
	Point ptR = Point(src1_cpy_cols / 64 * 62, src1_cpy_rows / 64 * 15);     //�����������½�����
	//int neightbourHood = 6; //(�̶���)��������ÿ����������ش�С  
	int neightbourHood = static_cast<int>((src1_cpy_cols / 256) * (src1_cpy_rows / 256)); //(����Ӧ��)��������ÿ����������ش�С��̫���Խ�磬���������
	//----------------------------------------------------------------------------
	for (int i = 0; i < ptR.y - ptL.y; i += neightbourHood)  //��ɨ�裬ע�⣺��������neightbourHood����
	{
		for (int j = 0; j < ptR.x - ptL.x; j += neightbourHood) //��ɨ��
		{
			randomNum = rng.uniform(-neightbourHood / 2, neightbourHood / 2);
			//1. ��õ�һ�����˷����ӦROI
			Rect rect = Rect(j + neightbourHood + ptL.x, i + neightbourHood + ptL.y, neightbourHood, neightbourHood);
			Mat mosaicRoi = src1_cpy(rect);  //ǳ����
			//2. ���øõ�һ�����˷���ROI����ɫֵ
			Scalar sca = Scalar(
				src1_cpy.at<Vec3b>(i + randomNum + ptL.y, j + randomNum + ptL.x)[0],   //Bͨ��
				src1_cpy.at<Vec3b>(i + randomNum + ptL.y, j + randomNum + ptL.x)[1],   //Gͨ��
				src1_cpy.at<Vec3b>(i + randomNum + ptL.y, j + randomNum + ptL.x)[2]);  //Rͨ��
			//3. ����ɫֵsca�������mosaicImg���õ���һ�����˿�ͼƬ
			Mat mosaicImg = Mat(rect.size(), CV_8UC3, sca);
			//4. ����mosaicRoi��ǳ����,���Բ���roi����ͻ������ԭͼ��Ӧ���򣬴Ӷ�ʵ�ֵ�һ������ͼƬ�ĵ���
			mosaicImg.copyTo(mosaicRoi);
		}
	}
	des = src1_cpy.clone();
}

XImagePro::XImagePro()
{
}


XImagePro::~XImagePro()
{
}
