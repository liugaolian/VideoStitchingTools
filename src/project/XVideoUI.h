/*******************************************************************************

**
*******************************************************************************/
#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUI.h"
#include <string>
class XVideoUI : public QWidget
{
    Q_OBJECT

public:
    XVideoUI(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent *e);
public slots:
	//打开视频源事件
	void Open();
	void SliderPress();
	void SliderRelease();
	//滑动条拖动
	void SetPos(int);
	//设置过滤器
	void Set();
	//启动视频处理并导出
	void BeginProcess();

	//一个打码视频导出完毕
	void ExportEnd(std::string file);
	//一个拼接视频导出完毕
	void ExportEndSplice(std::string file);
private:
    Ui::XVideoUIClass ui;
	std::string videoDirPath = "";
	std::vector<char*> file_vec;
	unsigned int finishedSpliceCnt = 0;
};
