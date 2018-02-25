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
		for (int i = 0; i < tasks.size(); i++)  //遍历任务列表
		{
			switch (tasks[i].type)
			{
				case XTASK_GAIN:
					//亮度对比度调整
					p.Gain(tasks[i].para[0], tasks[i].para[1]);
					break;
				case XTASK_MOSAIC:
					//打码
					p.Mosaic();
					break;
				default:
					break;
			}
		}
		cv::Mat re = p.Get();//获取处理后结果
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
	static CXFilter cx;  //多态，基类指针指向派生类对象
	return &cx;
}

XFilter::XFilter()
{
}

XFilter::~XFilter()
{
}
