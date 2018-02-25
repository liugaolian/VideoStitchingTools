/*******************************************************************************

**
*******************************************************************************/
#include "XVideoWidget.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>
using namespace cv;
void XVideoWidget::SetImage(cv::Mat mat)
{
	if (img.isNull())
	{
		uchar *buf = new uchar[width()*height() * 3];
		img = QImage(buf, width(), height(), QImage::Format_RGB888);
	}
	Mat des;
	//设置图像大小
	cv::resize(mat, des, Size(img.size().width(), img.size().height()));
	
	//设置图像颜色格式
	cv::cvtColor(des, des, COLOR_BGR2RGB);
	
	//复制内存空间
	memcpy(img.bits(), des.data, des.cols*des.rows*des.elemSize());
	update();
}

XVideoWidget::XVideoWidget(QWidget *p) :QOpenGLWidget(p)
{

}
void XVideoWidget::paintEvent(QPaintEvent*e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0, 0), img);
	p.end(); 
}

XVideoWidget::~XVideoWidget()
{
}
