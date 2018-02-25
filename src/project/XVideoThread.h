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
	int totalMs = 0;  //��Ƶ��ʱ����ms��
	//����ģʽ ��ȡ����
	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file);
	bool isSaveEndMosaic = true;
	bool isSaveEndSplice = true;
	std::vector<char *> videoPath;
	std::string videoRootPath = "";
	void Open_Filter_Save(std::vector<char *> videoPath);
	void XVideoThread::Open_Splicer_Save(std::string videoRootPath, std::vector<char *> allFile);
	//���ص�ǰ���ŵ�λ��
	double GetPos();

	//��ת��Ƶ 
	///@para frame int ֡λ��
	bool Seek(int frame);
	bool Seek(double pos);

	//��ʼ������Ƶ
	bool StartSave(const std::string filename, int width=0, int height=0);

	//ֹͣ������Ƶ��д����Ƶ֡������
	void StopSave();


	~XVideoThread();

	//�߳���ں���
	void run();

signals:
	//��ʾԭ��Ƶ1ͼ��
	void ViewImage1(cv::Mat mat);
	//��ʾ���ɺ�ͼ��
	void ViewDes(cv::Mat mat);
	//֪ͨGUIһ��������Ƶ�������
	void SaveEnd(std::string file); 
	//֪ͨGUIһ��ƴ����Ƶ�������
	void SaveEndSplice(std::string file);
protected:
	QMutex mutex;
	
	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;
	XVideoThread();
	
};

