#pragma once
#include <opencv2/core.hpp>
#include <vector>

enum XTaskType
{
	XTASK_NONE,
	XTASK_GAIN,		//���ȶԱȶȵ���
	XTASK_MOSAIC, //���������
};
struct XTask
{
	XTaskType type;
	std::vector<double> para;
};

// ������Ĺ涨
//��1��������ֻ������������Ļ��࣬���ܽ������������
//��2�������಻�������������͡������������ͻ���ʽת�������͡�
//��3�����Զ���ָ��������ָ������ã���ָ�����ָ�����������࣬����ʵ�ֶ�̬�ԣ�����

class XFilter  //�����ࣨ�ӿ��ࣩ�������ܶ������
{
public:
	virtual cv::Mat Filter(cv::Mat mat1,cv::Mat mat2) = 0;  //���麯��������������ʵ�֣��Լ�����ʵ��
	virtual void Add(XTask task) = 0;  //���麯��������������ʵ�֣��Լ�����ʵ��
	virtual void Clear() = 0;  //���麯��������������ʵ�֣��Լ�����ʵ��
	static XFilter *Get();  //����ģʽ
	virtual ~XFilter();  //�������������������ܵ��õ��������������
protected:
	XFilter(); //���캯�����ܱ�����
};

