
#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <XFilter.h>
#include <string>
#include <algorithm> //这里random_shuffle()函数需要包含
#include "XAudio.h"

using namespace std;
using namespace cv;

//一号视频源
static VideoCapture cap1;

//保存视频
static VideoWriter vw;

static bool isexit = false; //GUI关闭标志

//开始保存视频
bool XVideoThread::StartSave(const std::string filename, int width , int height ) 
{

	cout << "开始导出" << endl;
	Seek(0);
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	if (width <= 0) 
		width = cap1.get(CAP_PROP_FRAME_WIDTH);
	if (height <= 0)
		height = cap1.get(CAP_PROP_FRAME_HEIGHT);

	vw.open(filename,
		VideoWriter::fourcc('X', '2', '6', '4'),
		this->fps,
		Size(width, height)
		);
	if (!vw.isOpened())
	{
		mutex.unlock();
		cout << "start save failed!" << endl;
		return false;
	}
	this->isWrite = true;  //执行导出标志为true
	mutex.unlock();
	return true;
}

//停止保存视频，写入视频帧的索引
void XVideoThread::StopSave()
{
	cout << "停止导出" << endl;
	mutex.lock();
	vw.release(); //手动释放（目的是为了即时写入视频帧的索引），其实在析构函数中会被自动调用
	isWrite = false;   //执行导出标志为false
	mutex.unlock();
}

bool XVideoThread::Seek(double pos)
{
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos *count;
	return Seek(frame);
}
//跳转视频 
///@para frame int 帧位置
bool XVideoThread::Seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return  false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();
	return re;
}
//返回当前播放的位置
double XVideoThread::GetPos()
{
	double pos = 0;
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return pos;
	}
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(CAP_PROP_POS_FRAMES);
	if (count>0.001)
		pos = cur / count;
	mutex.unlock(); 
	return pos;
}
//打开一号视频源文件
bool XVideoThread::Open(const std::string file)
{

	cout <<"open :"<< file << endl;
	mutex.lock();
	bool re  = cap1.open(file);
	mutex.unlock();
	cout << re << endl;
	if (!re) return re;
	fps = cap1.get(CAP_PROP_FPS);
	if (fps <= 0) fps = 25;
	return true;
}

void XVideoThread::Open_Filter_Save(vector<char *> allFile)
{
	Mat mat1;
	for (int i = 0; i < allFile.size(); ++i)
	{
		//1.创建capture
		string file = allFile[i];
		cout << "open :" << file << endl;
		mutex.lock();
		if (!cap1.open(file))
		{
			cout << "!cap1.open(file)" << endl;
			mutex.unlock();
			return;
		}
		mutex.unlock();
		fps = cap1.get(CAP_PROP_FPS);
		if (fps <= 0) fps = 25;
		double count = cap1.get(CAP_PROP_FRAME_COUNT);
		totalMs = (count / (double)fps) * 1000;   //获取视频总时长
		//创建写容器		
		//sprintf(dst_str);
		cout << "开始导出" << endl;
		Seek(0);
		mutex.lock();
		if (!cap1.isOpened())
		{
			cout << "!cap1.isOpened()" << endl;
			mutex.unlock();
			return;
		}
		int width = cap1.get(CAP_PROP_FRAME_WIDTH);
		int height = cap1.get(CAP_PROP_FRAME_HEIGHT);
		vw.open(file+".avi",VideoWriter::fourcc('X', '2', '6', '4'),fps,Size(width, height));
		if (!vw.isOpened())
		{
			mutex.unlock();
			cout << "start save failed!" << endl;
			return;
		}
		isWrite = true;  //执行导出标志为true
		mutex.unlock();

//-----------------------------------------------------------------------------

		while (!isexit)  //GUI一旦关闭，则终止此while循环
		{
			//2.打开，左窗口播放，滤波器处理，右窗口播放
			mutex.lock();
			if (isexit) { mutex.unlock(); break; }
			if (!cap1.isOpened()){ mutex.unlock(); msleep(5); continue; }
			//读取一帧视频，解码并颜色转换
			if (!cap1.read(mat1) || mat1.empty())   //读取到最后一帧或者读取失败
			{
				mutex.unlock();
				//已经到了帧尾了，停止导出
				if (isWrite)
				{
					//停止导出，释放vw对象，写入视频索引头信息（若程序突然关闭，导出的视频是来不及加索引信息，所以用播放器打开时就看不到时长）
					StopSave();
					//信号函数，发送信号到GUI的槽函数中，通知GUI的显示窗口更新文本，提示用户
					SaveEnd(file);
					break;//退出循环，处理下一个视频文件
				}
				msleep(5);
				continue;
			}
			//由于从第一帧开始就启动导出操作，很明显，视频将一直不会在GUI上播放
			if (!isWrite) ViewImage1(mat1);   //如果未执行导出，则左边窗口继续播放原视频。否则原视频将不再播放，定格在上一帧
			//通过过滤器处理视频
			Mat des = XFilter::Get()->Filter(mat1, Mat());
			if (!isWrite) ViewDes(des);  //如果未执行导出，则右边窗口继续播放视频。否则视频将不再播放，定格在上一帧

			//3.导出
			int s = 0; 
			s = 1000 / fps;  //播放每一帧的停留时间
			if (isWrite)
			{
				s = 1;    //若是写的话，就停止播放，快速写入(否则以帧率写入岂不是等到黄花菜都凉了)
				vw.write(des); //往vw写入处理后的帧（导出一帧）
			}
			msleep(s);
			mutex.unlock();
		}
		if ((isexit))break;//退出for循环
	}
	isSaveEndMosaic = true;
}

void XVideoThread::Open_Splicer_Save(string videoRootPath, vector<char *> file_vec)
{
	vector<char *> spliceVec;
	for (int i = 0; i < file_vec.size(); ++i)  //遍历所有视频，轮流做片头进行拼接！
	{
		cout << i << "-";
		vector<int> randomSet;
		for (int id = 0; id < file_vec.size(); ++id)  //创建一个存放从0~file_vec.size() 的向量
		{
			if (id != i)     //注意：排除掉片头序号，从而保证后面数字跟片头都不重复(所以randomSet的数量比file_vec少一个)
				randomSet.push_back(id);
		}
		//--------------
		random_shuffle(randomSet.begin(), randomSet.end());    //调用random_shuffle()函数随机打乱该向量内容（从而保证数字都不重复）！
		//--------------
		int id_tmp = XAudio::Get()->spliceNum - 1; //注意减1//2018.1.7
		for (int id = 0; id < id_tmp; ++id)  //我们的任务只要四个随机数，所以抽取前面四个数字即可//2018.1.7
		//for (int id = 0; id < 4; ++id)
		{
			cout << randomSet[id] << " ";
			spliceVec.push_back(file_vec[randomSet[id]]);  //存放4个随机视频作为后向拼接片段	
		}
		const char* outvideo = strrchr(file_vec[i], '\\') + 1; //输出文件名与片头名保持一致： 从绝对路径中获取文件名
		cout << endl << outvideo << endl;
		string outputPath = videoRootPath + "/拼接视频存放文件夹/" + outvideo;
		XAudio::Get()->SpliceVideo(videoRootPath, file_vec[i], spliceVec, outputPath);  //源视频根目录，片头，后接片段名集合，输出文件名	
		cout << "==================!!!!!!!!!!! here !!!!!!!!!!!==============\n";
		SaveEndSplice(file_vec[i]);//信号函数，发送信号到GUI的槽函数中，通知GUI的显示窗口更新文本，提示用户
		spliceVec.clear(); //清空
		randomSet.clear(); //清空
	}
	isSaveEndSplice = true;
}

void XVideoThread::run()
{
	while (!isexit)
	{
		if (isSaveEndMosaic == false)
		{
			Open_Filter_Save(videoPath); //打开，马赛克处理，导出
		}
		if (isSaveEndSplice == false)
		{
			Open_Splicer_Save(videoRootPath, videoPath);  //拼接视频
		}
		msleep(100); //线程睡眠！这句程序不能少，否则电脑CPU资源会被占用很多，会很卡！！！
	}
}






XVideoThread::XVideoThread()
{
	start();   //启动线程	
}

XVideoThread::~XVideoThread()
{
	
	mutex.lock();
	isexit = true;   //调用析构函数时，置位isexit标志，促使线程run()的循环退出
	mutex.unlock();
	wait();
}
