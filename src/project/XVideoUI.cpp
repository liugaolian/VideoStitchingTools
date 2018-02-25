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
#include <algorithm> //����random_shuffle()������Ҫ����


using namespace std;
static bool pressSlider = false;
static bool isExport = false;

XVideoUI::XVideoUI(QWidget *parent) : QWidget(parent)  //���ø��๹�캯��
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint);  //�쳣GUI������İ�ɫ�˵�������С������󻯣����٣�

	//ע�⣡��ͨ���źŲ۴����µ�����������Ҫ��QTע�ᣡ����
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<std::string>("std::string");

	//ԭ��Ƶ��ʾ�ź�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
		);

	//�����Ƶ��ʾ�ź�
	QObject::connect(  XVideoThread::Get(),
									SIGNAL(ViewDes(cv::Mat)),
									ui.des,
									SLOT(SetImage(cv::Mat))
								);
	//һ��������Ƶ�������
	QObject::connect( XVideoThread::Get(),   //������ΪXVideoThread�Ķ���
									SIGNAL(SaveEnd(std::string)),
									this,   //������Ϊ������
									SLOT(ExportEnd(std::string))
									);
	//һ��ƴ����Ƶ�������
	QObject::connect( XVideoThread::Get(),   //������ΪXVideoThread�Ķ���
									SIGNAL(SaveEndSplice(std::string)),
									this,   //������Ϊ������
									SLOT(ExportEndSplice(std::string))
									);

	startTimer(40);
}

void XVideoUI::timerEvent(QTimerEvent *e)
{
	if (pressSlider)return;
	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos*1000);  //ÿ40ms����һ�ν�������ʾ

}

//����ƵԴ�����ļ��У����������ļ��а���������������Ƶ��
void XVideoUI::Open()
{
	QString rootPath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("��ѡ����Ƶ�ļ�����Ŀ¼"));
	if (rootPath.isEmpty()) return;
	QMessageBox::information(0, QString::fromLocal8Bit("����ѡ����ļ���"), rootPath);

	//�����������Ƶ����ļ��У��������ھ͵�������һ��
	videoDirPath = rootPath.toLocal8Bit().data();
	string vedioFullPath = videoDirPath + "/������Ƶ����ļ���/";
	if (!access(vedioFullPath.data(), 0))
		cout << "-------" << vedioFullPath << " EXISIT-------" << endl;
	else
	{
		cout << "-------" << vedioFullPath << " DOESN'T EXISIT-------" << endl;
		if (0 != mkdir(vedioFullPath.data())) return;  //��Դ��Ƶ����Ŀ¼�´���һ���µ��ļ���
		cout << "-------" << vedioFullPath << " Make Sucessfully -------" << endl;
	}

	vedioFullPath = videoDirPath + "/ƴ����Ƶ����ļ���/";
	if (!access(vedioFullPath.data() , 0))
		cout << "-------" << vedioFullPath << " EXISIT-------" << endl;
	else
	{
		cout << "-------" << vedioFullPath << " DOESN'T EXISIT-------" << endl;
		if (0 != mkdir(vedioFullPath.data())) return;  //��Դ��Ƶ����Ŀ¼�´���һ���µ��ļ���
		cout << "-------" << vedioFullPath << " Make Sucessfully -------" << endl;
	}
	//----------------------------------------------------------
	CStatDir statdir;
	statdir.SetInitDir(videoDirPath.c_str());  //system����ѡ�����ļ���Ŀ¼�£�������һ��ִ����ffmpeg����Ч�أ�����
	file_vec = statdir.BeginBrowseFilenames("*.mp4");  //������Ƶ�ļ�����·��������ʱֻ֧��mp4����������ж�֧�ָ��ࣩ
	vector<char*> file_vec2 = statdir.BeginBrowseFilenames("*.flv");
	vector<char*> file_vec3 = statdir.BeginBrowseFilenames("*.avi");

	for (size_t i = 0; i<file_vec2.size(); ++i)  //����vector�������ַ���
	{
		file_vec.push_back(file_vec2[i]);
	}

	for (size_t i = 0; i<file_vec3.size(); ++i)  //����vector�������ַ���
	{
		file_vec.push_back(file_vec3[i]);
	}

	for (size_t i = 0;i<file_vec.size(); ++i)  //����vector�������ַ���
	{
		cout << file_vec[i] << endl;
		string tmp_ = file_vec[i]; //char*תstring����ֱ�Ӹ�ֵ
		for (size_t j = 0; j<tmp_.size(); ++j)//�����ַ����е��ַ�
		{
			if (tmp_[j] == ' ') tmp_[j] = ',';  //ffmpeg�޷�ʶ���пո��ļ�������������Ҫ���ļ����еĿո��滻��������
		}
		rename(file_vec[i], tmp_.c_str()); //ֱ�Ӹ����ļ���
	}
	
	file_vec = statdir.BeginBrowseFilenames("*.mp4");  //���¶�ȡ(���Ĺ�)�ļ���
	file_vec2 = statdir.BeginBrowseFilenames("*.flv");
	file_vec3 = statdir.BeginBrowseFilenames("*.avi");

	for (size_t i = 0; i<file_vec2.size(); ++i)  //����vector�������ַ���
	{
		file_vec.push_back(file_vec2[i]);
	}

	for (size_t i = 0; i<file_vec3.size(); ++i)  //����vector�������ַ���
	{
		file_vec.push_back(file_vec3[i]);
	}

	cout << "-----���Ĺ����ļ���-----" << endl;
	for (size_t i = 0; i<file_vec.size(); ++i)  //����vector�������ַ���
	{		
		cout << file_vec[i] << endl;
	}
	cout << "--------------------------" << endl;

	finishedSpliceCnt = 0;  //��ƴ�������Ƶ��������
	XVideoThread::Get()->videoPath = file_vec;  //�����̶߳����ͬʱҲ��������run()�߳�
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
//�������϶�
void XVideoUI::SetPos(int pos)
{
	XVideoThread::Get()->Seek((double)pos / 1000.);
}

//�ۺ�������������XVideoThread��run()�����ġ�һ��������Ƶ������ϡ��ź�
void XVideoUI::ExportEnd(std::string file)
{
	static unsigned int finishedCnt = 0;  //�Ѵ�����Ƶ��
	finishedCnt++;
	char str1_[5];
	sprintf(str1_, "%d", finishedCnt);
	ui.finishVedioCnt->setText(QString::fromLocal8Bit(str1_)); //GUI��ʾ����ɵ���Ƶ�ļ���
	cout << "finishedCnt = " << finishedCnt << endl;

	isExport = false;  //ͨ��������ϱ�־����������һ����Ƶ�������µ���
	const char* showStr = file.data();
	const char *q = strrchr(showStr, '\\' ) + 1; //�Ӿ���·���л�ȡ�ļ���
	ui.textDisplay->setText(QString::fromLocal8Bit(showStr)); //GUI��ʾ����ɵ���Ƶ�ļ���
	
	int ss = 0;
	int end = XVideoThread::Get()->totalMs ;   //��ȡ��Ƶʱ������Ƶ��ʱ��һ����
	int t = 0;
	t = end - ss;  //��ȡ��Ƶʱ�� = ����ʱ��-��ʼʱ��

	//������Ƶ
	XAudio::Get()->ExportA(file, file + ".mp3", ss, t);        //��ȡԴ��Ƶ������
	XAudio::Get()->Merge(file + ".avi", file + ".mp3", videoDirPath + "/������Ƶ����ļ���/" + q);  //�ϲ���Ƶ��Ŀ����Ƶ
	
	//ɾ�� �м��ļ�.avi��.mp3������
	string cmd = "del " + file + ".avi";
	system(cmd.c_str());
	cmd = "del " + file + ".mp3";
	system(cmd.c_str());
}

//�ۺ�������������XVideoThread��run()�����ġ�һ��ƴ����Ƶ������ϡ��ź�
void XVideoUI::ExportEndSplice(std::string file)
{
	finishedSpliceCnt++;
	char str1[5];
	sprintf(str1, "%d", finishedSpliceCnt);
	ui.finishVedioCnt_2->setText(QString::fromLocal8Bit(str1)); //GUI��ʾ����ɵ���Ƶ�ļ���
	cout << " finishedCnt = " << finishedSpliceCnt << endl;

	isExport = false;  //ͨ��������ϱ�־����������һ����Ƶ�������µ���
	const char* showStr = file.data();
	const char *q = strrchr(showStr, '\\') + 1; //�Ӿ���·���л�ȡ�ļ���
	ui.textDisplay_2->setText(QString::fromLocal8Bit(showStr)); //GUI��ʾ����ɵ���Ƶ�ļ���
}

//�ۺ�����������Ƶ����
void XVideoUI::BeginProcess()
{
	cout << "------������Ƶ����---------" << endl;
	unsigned int allCnt = file_vec.size();   //��������Ƶ����
	char str0[5];
	sprintf(str0, "%d", allCnt);

	//ui.allVedioCntMosaic->setText(QString::fromLocal8Bit(str0));    //GUI��ʾ�ܵ���Ƶ�ļ���
	//XVideoThread::Get()->isSaveEndMosaic = false;  //������Ƶ�����˴�������

	XAudio::Get()->spliceNum = ui.spliceNumber->value();  //2018.1.7
	XVideoThread::Get()->isSaveEndSplice = false;  //������Ƶƴ�Ӵ�������
	ui.allVedioCntSplice->setText(QString::fromLocal8Bit(str0));    //GUI��ʾ�ܵ���Ƶ�ļ���
}

//�ۺ��������ù�����
void XVideoUI::Set()
{
	XFilter::Get()->Clear();  //��������б�
	//�ԱȶȺ�����
	if (ui.bright->value() > 0 || ui.contrast->value()>1)  //��GUI�Ķ�Ӧ��ֵ�仯ʱ
	{
		XFilter::Get()->Add(XTask{ XTASK_GAIN,{ (double)ui.bright->value(), ui.contrast->value() } }); //C++11���﷨
	}
	//ͼ�����
	if (ui.mosaic->currentIndex() == 1)  //��ǰ����Ϊ1�������������
	{
		XFilter::Get()->Add(XTask{ XTASK_MOSAIC });
	}
}