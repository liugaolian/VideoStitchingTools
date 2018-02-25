#include "XImagePro.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;

//设置原图，会清理处理结果
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty())return;
	this->src1 = mat1;						 //谁调用Set()，这个this指针就是谁
	this->src2 = mat2;
	this->src1.copyTo(des);
}

//设置亮度和对比度
///@para bright double 亮度 0~100
///@para contrast int 对比度 1.0 ~ 3.0
void XImagePro::Gain(double bright, double contrast)
{
	if (des.empty())return;
	des.convertTo(des, -1, contrast, bright); //设置亮度和对比度
}

//往某个ROI区域打马赛克
///@para srcImg Mat& 原图 
///@para roi Rect 目标区域 
void XImagePro::Mosaic()
{
	if (des.empty())return;
	//Mat src1_cpy = src1.clone(); //这句程序会把已经调整过对比度和亮度的图片覆盖
	//Mat src1_cpy = des.clone();
	Mat src1_cpy = Get();
	RNG rng;
	int randomNum; //邻域内随机值 
	double src1_cpy_cols = src1_cpy.cols;
	double src1_cpy_rows = src1_cpy.rows;
	Point ptL = Point(src1_cpy_cols / 64 * 48, src1_cpy_rows / 64 * 1);         //打码区域左上角坐标
	Point ptR = Point(src1_cpy_cols / 64 * 62, src1_cpy_rows / 64 * 15);     //打码区域右下角坐标
	//int neightbourHood = 6; //(固定的)马赛克上每个方框的像素大小  
	int neightbourHood = static_cast<int>((src1_cpy_cols / 256) * (src1_cpy_rows / 256)); //(自适应的)马赛克上每个方框的像素大小（太大会越界，程序崩溃）
	//----------------------------------------------------------------------------
	for (int i = 0; i < ptR.y - ptL.y; i += neightbourHood)  //行扫描，注意：这里是以neightbourHood递增
	{
		for (int j = 0; j < ptR.x - ptL.x; j += neightbourHood) //列扫描
		{
			randomNum = rng.uniform(-neightbourHood / 2, neightbourHood / 2);
			//1. 获得单一马赛克方块对应ROI
			Rect rect = Rect(j + neightbourHood + ptL.x, i + neightbourHood + ptL.y, neightbourHood, neightbourHood);
			Mat mosaicRoi = src1_cpy(rect);  //浅拷贝
			//2. 设置该单一马赛克方块ROI的颜色值
			Scalar sca = Scalar(
				src1_cpy.at<Vec3b>(i + randomNum + ptL.y, j + randomNum + ptL.x)[0],   //B通道
				src1_cpy.at<Vec3b>(i + randomNum + ptL.y, j + randomNum + ptL.x)[1],   //G通道
				src1_cpy.at<Vec3b>(i + randomNum + ptL.y, j + randomNum + ptL.x)[2]);  //R通道
			//3. 以颜色值sca填充整个mosaicImg，得到单一马赛克块图片
			Mat mosaicImg = Mat(rect.size(), CV_8UC3, sca);
			//4. 由于mosaicRoi是浅拷贝,所以操作roi区域就会操作到原图对应区域，从而实现单一马赛克图片的叠加
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
