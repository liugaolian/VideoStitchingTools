#include "XFilter.h"
#include <QThread>
#include "XImagePro.h"

class CXFilter :public XFilter
{
public:
	std::vector<XTask> tasks;
	QMutex mutex;
	cv::Mat Filter(cv::Mat mat1, cv::Mat mat2)
	{
		mutex.lock();
		XImagePro p;
		p.Set(mat1, mat2);
		for (int i = 0; i < tasks.size(); i++)  //���������б�
		{
			switch (tasks[i].type)
			{
				case XTASK_GAIN:
					//���ȶԱȶȵ���
					p.Gain(tasks[i].para[0], tasks[i].para[1]);
					break;
				case XTASK_MOSAIC:
					//����
					p.Mosaic();
					break;
				default:
					break;
			}
		}
		cv::Mat re = p.Get();//��ȡ��������
		mutex.unlock();
		return re;
	}
	void Add(XTask task)
	{
		mutex.lock();
		tasks.push_back(task);
		mutex.unlock();
	}
	void Clear()
	{
		mutex.lock();
		tasks.clear();
		mutex.unlock();
	}
};

XFilter * XFilter::Get()
{
	static CXFilter cx;  //��̬������ָ��ָ�����������
	return &cx;
}

XFilter::XFilter()
{
}

XFilter::~XFilter()
{
}