#pragma once
#include <opencv2/core.hpp>
#include <vector>

enum XTaskType
{
	XTASK_NONE,
	XTASK_GAIN,		//亮度对比度调整
	XTASK_MOSAIC, //添加马赛克
};
struct XTask
{
	XTaskType type;
	std::vector<double> para;
};

// 抽象类的规定
//（1）抽象类只能用作其他类的基类，不能建立抽象类对象。
//（2）抽象类不能用作参数类型、函数返回类型或显式转换的类型。
//（3）可以定义指向抽象类的指针和引用，此指针可以指向它的派生类，进而实现多态性！！！

class XFilter  //抽象类（接口类），本身不能定义对象
{
public:
	virtual cv::Mat Filter(cv::Mat mat1,cv::Mat mat2) = 0;  //纯虚函数，子类必须加以实现，自己不用实现
	virtual void Add(XTask task) = 0;  //纯虚函数，子类必须加以实现，自己不用实现
	virtual void Clear() = 0;  //纯虚函数，子类必须加以实现，自己不用实现
	static XFilter *Get();  //单件模式
	virtual ~XFilter();  //虚析构函数，这样才能调用到子类的析构函数
protected:
	XFilter(); //构造函数（受保护）
};

