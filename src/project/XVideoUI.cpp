/*******************************************************************************

*******************************************************************************/

#include "XVideoUI.h"
#include <QFileDialog>
#include <string>
#include <QMessageBox>
#include "XVideoThread.h"
#include "XFilter.h"
#include "XAudio.h"
#include "BrowseDir.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib> 
#include <algorithm> //这里random_shuffle()函数需要包含


using namespace std;
static bool pressSlider = false;
static bool isExport = false;

XVideoUI::XVideoUI(QWidget *parent) : QWidget(parent)  //调用父类构造函数
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint);  //异常GUI最上面的白色菜单栏（最小化，最大化，销毁）

	//注意！！通过信号槽传递新的数据类型需要向QT注册！！！
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<std::string>("std::string");

	//原视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
		);

	//输出视频显示信号
	QObject::connect(  XVideoThread::Get(),
									SIGNAL(ViewDes(cv::Mat)),
									ui.des,
									SLOT(SetImage(cv::Mat))
								);
	//一个打码视频导出完毕
	QObject::connect( XVideoThread::Get(),   //发送者为XVideoThread的对象
									SIGNAL(SaveEnd(std::string)),
									this,   //接收者为本对象
									SLOT(ExportEnd(std::string))
									);
	//一个拼接视频导出完毕
	QObject::connect( XVideoThread::Get(),   //发送者为XVideoThread的对象
									SIGNAL(SaveEndSplice(std::string)),
									this,   //接收者为本对象
									SLOT(ExportEndSplice(std::string))
									);

	startTimer(40);
}

void XVideoUI::timerEvent(QTimerEvent *e)
{
	if (pressSlider)return;
	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos*1000);  //每40ms更新一次进度条显示

}

//打开视频源所在文件夹！！！（该文件夹包含大量待打码视频）
void XVideoUI::Open()
{
	QString rootPath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择视频文件所在目录"));
	if (rootPath.isEmpty()) return;
	QMessageBox::information(0, QString::fromLocal8Bit("您所选择的文件夹"), rootPath);

	//创建处理后视频存放文件夹，若不存在就当即创建一个
	videoDirPath = rootPath.toLocal8Bit().data();
	string vedioFullPath = videoDirPath + "/打码视频存放文件夹/";
	if (!access(vedioFullPath.data(), 0))
		cout << "-------" << vedioFullPath << " EXISIT-------" << endl;
	else
	{
		cout << "-------" << vedioFullPath << " DOESN'T EXISIT-------" << endl;
		if (0 != mkdir(vedioFullPath.data())) return;  //在源视频所在目录下创建一个新的文件夹
		cout << "-------" << vedioFullPath << " Make Sucessfully -------" << endl;
	}

	vedioFullPath = videoDirPath + "/拼接视频存放文件夹/";
	if (!access(vedioFullPath.data() , 0))
		cout << "-------" << vedioFullPath << " EXISIT-------" << endl;
	else
	{
		cout << "-------" << vedioFullPath << " DOESN'T EXISIT-------" << endl;
		if (0 != mkdir(vedioFullPath.data())) return;  //在源视频所在目录下创建一个新的文件夹
		cout << "-------" << vedioFullPath << " Make Sucessfully -------" << endl;
	}
	//----------------------------------------------------------
	CStatDir statdir;
	statdir.SetInitDir(videoDirPath.c_str());  //system进入选定的文件夹目录下！！！这一句执行了ffmpeg才生效呢！！！
	file_vec = statdir.BeginBrowseFilenames("*.mp4");  //所有视频文件绝对路径（先暂时只支持mp4，后面加入判断支持更多）
	vector<char*> file_vec2 = statdir.BeginBrowseFilenames("*.flv");
	vector<char*> file_vec3 = statdir.BeginBrowseFilenames("*.avi");

	for (size_t i = 0; i<file_vec2.size(); ++i)  //遍历vector中所有字符串
	{
		file_vec.push_back(file_vec2[i]);
	}

	for (size_t i = 0; i<file_vec3.size(); ++i)  //遍历vector中所有字符串
	{
		file_vec.push_back(file_vec3[i]);
	}

	for (size_t i = 0;i<file_vec.size(); ++i)  //遍历vector中所有字符串
	{
		cout << file_vec[i] << endl;
		string tmp_ = file_vec[i]; //char*转string可以直接赋值
		for (size_t j = 0; j<tmp_.size(); ++j)//遍历字符串中的字符
		{
			if (tmp_[j] == ' ') tmp_[j] = ',';  //ffmpeg无法识别有空格文件名，所以这里要把文件名中的空格替换掉！！！
		}
		rename(file_vec[i], tmp_.c_str()); //直接更改文件名
	}
	
	file_vec = statdir.BeginBrowseFilenames("*.mp4");  //重新读取(更改过)文件名
	file_vec2 = statdir.BeginBrowseFilenames("*.flv");
	file_vec3 = statdir.BeginBrowseFilenames("*.avi");

	for (size_t i = 0; i<file_vec2.size(); ++i)  //遍历vector中所有字符串
	{
		file_vec.push_back(file_vec2[i]);
	}

	for (size_t i = 0; i<file_vec3.size(); ++i)  //遍历vector中所有字符串
	{
		file_vec.push_back(file_vec3[i]);
	}

	cout << "-----更改过的文件名-----" << endl;
	for (size_t i = 0; i<file_vec.size(); ++i)  //遍历vector中所有字符串
	{		
		cout << file_vec[i] << endl;
	}
	cout << "--------------------------" << endl;

	finishedSpliceCnt = 0;  //已拼接完成视频数，清零
	XVideoThread::Get()->videoPath = file_vec;  //创建线程对象的同时也就启动了run()线程
	XVideoThread::Get()->videoRootPath = videoDirPath;
}

void XVideoUI::SliderPress()
{
	pressSlider = true;
}

void XVideoUI::SliderRelease()
{
	pressSlider = false;
}
//滑动条拖动
void XVideoUI::SetPos(int pos)
{
	XVideoThread::Get()->Seek((double)pos / 1000.);
}

//槽函数：接收来自XVideoThread的run()发出的“一个打码视频导出完毕”信号
void XVideoUI::ExportEnd(std::string file)
{
	static unsigned int finishedCnt = 0;  //已处理视频数
	finishedCnt++;
	char str1_[5];
	sprintf(str1_, "%d", finishedCnt);
	ui.finishVedioCnt->setText(QString::fromLocal8Bit(str1_)); //GUI显示已完成的视频文件名
	cout << "finishedCnt = " << finishedCnt << endl;

	isExport = false;  //通过导出完毕标志来重新载入一个视频，并重新导出
	const char* showStr = file.data();
	const char *q = strrchr(showStr, '\\' ) + 1; //从绝对路径中获取文件名
	ui.textDisplay->setText(QString::fromLocal8Bit(showStr)); //GUI显示已完成的视频文件名
	
	int ss = 0;
	int end = XVideoThread::Get()->totalMs ;   //截取音频时长跟视频总时长一样！
	int t = 0;
	t = end - ss;  //截取音频时长 = 结束时间-开始时间

	//处理音频
	XAudio::Get()->ExportA(file, file + ".mp3", ss, t);        //获取源视频的声音
	XAudio::Get()->Merge(file + ".avi", file + ".mp3", videoDirPath + "/打码视频存放文件夹/" + q);  //合并音频和目标视频
	
	//删除 中间文件.avi和.mp3！！！
	string cmd = "del " + file + ".avi";
	system(cmd.c_str());
	cmd = "del " + file + ".mp3";
	system(cmd.c_str());
}

//槽函数：接收来自XVideoThread的run()发出的“一个拼接视频导出完毕”信号
void XVideoUI::ExportEndSplice(std::string file)
{
	finishedSpliceCnt++;
	char str1[5];
	sprintf(str1, "%d", finishedSpliceCnt);
	ui.finishVedioCnt_2->setText(QString::fromLocal8Bit(str1)); //GUI显示已完成的视频文件名
	cout << " finishedCnt = " << finishedSpliceCnt << endl;

	isExport = false;  //通过导出完毕标志来重新载入一个视频，并重新导出
	const char* showStr = file.data();
	const char *q = strrchr(showStr, '\\') + 1; //从绝对路径中获取文件名
	ui.textDisplay_2->setText(QString::fromLocal8Bit(showStr)); //GUI显示已完成的视频文件名
}

//槽函数：启动视频处理
void XVideoUI::BeginProcess()
{
	cout << "------启动视频处理---------" << endl;
	unsigned int allCnt = file_vec.size();   //待处理视频总数
	char str0[5];
	sprintf(str0, "%d", allCnt);

	//ui.allVedioCntMosaic->setText(QString::fromLocal8Bit(str0));    //GUI显示总的视频文件数
	//XVideoThread::Get()->isSaveEndMosaic = false;  //启动视频马赛克处理！！！

	XAudio::Get()->spliceNum = ui.spliceNumber->value();  //2018.1.7
	XVideoThread::Get()->isSaveEndSplice = false;  //启动视频拼接处理！！！
	ui.allVedioCntSplice->setText(QString::fromLocal8Bit(str0));    //GUI显示总的视频文件数
}

//槽函数：设置过滤器
void XVideoUI::Set()
{
	XFilter::Get()->Clear();  //清空任务列表
	//对比度和亮度
	if (ui.bright->value() > 0 || ui.contrast->value()>1)  //当GUI的对应的值变化时
	{
		XFilter::Get()->Add(XTask{ XTASK_GAIN,{ (double)ui.bright->value(), ui.contrast->value() } }); //C++11的语法
	}
	//图像打码
	if (ui.mosaic->currentIndex() == 1)  //当前索引为1，即添加马赛克
	{
		XFilter::Get()->Add(XTask{ XTASK_MOSAIC });
	}
}