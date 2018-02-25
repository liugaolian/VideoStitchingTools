/*******************************************************************************

**
*******************************************************************************/
#pragma once
#include <QThread>
#include "opencv2/core.hpp"
#include <vector>
#include <string>


class XVideoThread:public QThread
{

	Q_OBJECT

public:
	int fps = 0;
	int totalMs = 0;  //视频总时长（ms）
	//单件模式 获取对象
	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	//打开一号视频源文件
	bool Open(const std::string file);
	bool isSaveEndMosaic = true;
	bool isSaveEndSplice = true;
	std::vector<char *> videoPath;
	std::string videoRootPath = "";
	void Open_Filter_Save(std::vector<char *> videoPath);
	void XVideoThread::Open_Splicer_Save(std::string videoRootPath, std::vector<char *> allFile);
	//返回当前播放的位置
	double GetPos();

	//跳转视频 
	///@para frame int 帧位置
	bool Seek(int frame);
	bool Seek(double pos);

	//开始保存视频
	bool StartSave(const std::string filename, int width=0, int height=0);

	//停止保存视频，写入视频帧的索引
	void StopSave();


	~XVideoThread();

	//线程入口函数
	void run();

signals:
	//显示原视频1图像
	void ViewImage1(cv::Mat mat);
	//显示生成后图像
	void ViewDes(cv::Mat mat);
	//通知GUI一个打码视频导出完毕
	void SaveEnd(std::string file); 
	//通知GUI一个拼接视频导出完毕
	void SaveEndSplice(std::string file);
protected:
	QMutex mutex;
	
	//是否开始写视频
	bool isWrite = false;
	XVideoThread();
	
};

