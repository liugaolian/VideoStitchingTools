
#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <XFilter.h>
#include <string>
#include <algorithm> //����random_shuffle()������Ҫ����
#include "XAudio.h"

using namespace std;
using namespace cv;

//һ����ƵԴ
static VideoCapture cap1;

//������Ƶ
static VideoWriter vw;

static bool isexit = false; //GUI�رձ�־

//��ʼ������Ƶ
bool XVideoThread::StartSave(const std::string filename, int width , int height ) 
{

	cout << "��ʼ����" << endl;
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
	this->isWrite = true;  //ִ�е�����־Ϊtrue
	mutex.unlock();
	return true;
}

//ֹͣ������Ƶ��д����Ƶ֡������
void XVideoThread::StopSave()
{
	cout << "ֹͣ����" << endl;
	mutex.lock();
	vw.release(); //�ֶ��ͷţ�Ŀ����Ϊ�˼�ʱд����Ƶ֡������������ʵ�����������лᱻ�Զ�����
	isWrite = false;   //ִ�е�����־Ϊfalse
	mutex.unlock();
}

bool XVideoThread::Seek(double pos)
{
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos *count;
	return Seek(frame);
}
//��ת��Ƶ 
///@para frame int ֡λ��
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
//���ص�ǰ���ŵ�λ��
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
//��һ����ƵԴ�ļ�
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
		//1.����capture
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
		totalMs = (count / (double)fps) * 1000;   //��ȡ��Ƶ��ʱ��
		//����д����		
		//sprintf(dst_str);
		cout << "��ʼ����" << endl;
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
		isWrite = true;  //ִ�е�����־Ϊtrue
		mutex.unlock();

//-----------------------------------------------------------------------------

		while (!isexit)  //GUIһ���رգ�����ֹ��whileѭ��
		{
			//2.�򿪣��󴰿ڲ��ţ��˲��������Ҵ��ڲ���
			mutex.lock();
			if (isexit) { mutex.unlock(); break; }
			if (!cap1.isOpened()){ mutex.unlock(); msleep(5); continue; }
			//��ȡһ֡��Ƶ�����벢��ɫת��
			if (!cap1.read(mat1) || mat1.empty())   //��ȡ�����һ֡���߶�ȡʧ��
			{
				mutex.unlock();
				//�Ѿ�����֡β�ˣ�ֹͣ����
				if (isWrite)
				{
					//ֹͣ�������ͷ�vw����д����Ƶ����ͷ��Ϣ��������ͻȻ�رգ���������Ƶ����������������Ϣ�������ò�������ʱ�Ϳ�����ʱ����
					StopSave();
					//�źź����������źŵ�GUI�Ĳۺ����У�֪ͨGUI����ʾ���ڸ����ı�����ʾ�û�
					SaveEnd(file);
					break;//�˳�ѭ����������һ����Ƶ�ļ�
				}
				msleep(5);
				continue;
			}
			//���ڴӵ�һ֡��ʼ���������������������ԣ���Ƶ��һֱ������GUI�ϲ���
			if (!isWrite) ViewImage1(mat1);   //���δִ�е���������ߴ��ڼ�������ԭ��Ƶ������ԭ��Ƶ�����ٲ��ţ���������һ֡
			//ͨ��������������Ƶ
			Mat des = XFilter::Get()->Filter(mat1, Mat());
			if (!isWrite) ViewDes(des);  //���δִ�е��������ұߴ��ڼ���������Ƶ��������Ƶ�����ٲ��ţ���������һ֡

			//3.����
			int s = 0; 
			s = 1000 / fps;  //����ÿһ֡��ͣ��ʱ��
			if (isWrite)
			{
				s = 1;    //����д�Ļ�����ֹͣ���ţ�����д��(������֡��д�����ǵȵ��ƻ��˶�����)
				vw.write(des); //��vwд�봦����֡������һ֡��
			}
			msleep(s);
			mutex.unlock();
		}
		if ((isexit))break;//�˳�forѭ��
	}
	isSaveEndMosaic = true;
}

void XVideoThread::Open_Splicer_Save(string videoRootPath, vector<char *> file_vec)
{
	vector<char *> spliceVec;
	for (int i = 0; i < file_vec.size(); ++i)  //����������Ƶ��������Ƭͷ����ƴ�ӣ�
	{
		cout << i << "-";
		vector<int> randomSet;
		for (int id = 0; id < file_vec.size(); ++id)  //����һ����Ŵ�0~file_vec.size() ������
		{
			if (id != i)     //ע�⣺�ų���Ƭͷ��ţ��Ӷ���֤�������ָ�Ƭͷ�����ظ�(����randomSet��������file_vec��һ��)
				randomSet.push_back(id);
		}
		//--------------
		random_shuffle(randomSet.begin(), randomSet.end());    //����random_shuffle()����������Ҹ��������ݣ��Ӷ���֤���ֶ����ظ�����
		//--------------
		int id_tmp = XAudio::Get()->spliceNum - 1; //ע���1//2018.1.7
		for (int id = 0; id < id_tmp; ++id)  //���ǵ�����ֻҪ�ĸ�����������Գ�ȡǰ���ĸ����ּ���//2018.1.7
		//for (int id = 0; id < 4; ++id)
		{
			cout << randomSet[id] << " ";
			spliceVec.push_back(file_vec[randomSet[id]]);  //���4�������Ƶ��Ϊ����ƴ��Ƭ��	
		}
		const char* outvideo = strrchr(file_vec[i], '\\') + 1; //����ļ�����Ƭͷ������һ�£� �Ӿ���·���л�ȡ�ļ���
		cout << endl << outvideo << endl;
		string outputPath = videoRootPath + "/ƴ����Ƶ����ļ���/" + outvideo;
		XAudio::Get()->SpliceVideo(videoRootPath, file_vec[i], spliceVec, outputPath);  //Դ��Ƶ��Ŀ¼��Ƭͷ�����Ƭ�������ϣ�����ļ���	
		cout << "==================!!!!!!!!!!! here !!!!!!!!!!!==============\n";
		SaveEndSplice(file_vec[i]);//�źź����������źŵ�GUI�Ĳۺ����У�֪ͨGUI����ʾ���ڸ����ı�����ʾ�û�
		spliceVec.clear(); //���
		randomSet.clear(); //���
	}
	isSaveEndSplice = true;
}

void XVideoThread::run()
{
	while (!isexit)
	{
		if (isSaveEndMosaic == false)
		{
			Open_Filter_Save(videoPath); //�򿪣������˴�������
		}
		if (isSaveEndSplice == false)
		{
			Open_Splicer_Save(videoRootPath, videoPath);  //ƴ����Ƶ
		}
		msleep(100); //�߳�˯�ߣ����������٣��������CPU��Դ�ᱻռ�úܶ࣬��ܿ�������
	}
}






XVideoThread::XVideoThread()
{
	start();   //�����߳�	
}

XVideoThread::~XVideoThread()
{
	
	mutex.lock();
	isexit = true;   //������������ʱ����λisexit��־����ʹ�߳�run()��ѭ���˳�
	mutex.unlock();
	wait();
}
