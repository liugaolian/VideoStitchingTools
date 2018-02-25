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
	//����ƵԴ�¼�
	void Open();
	void SliderPress();
	void SliderRelease();
	//�������϶�
	void SetPos(int);
	//���ù�����
	void Set();
	//������Ƶ��������
	void BeginProcess();

	//һ��������Ƶ�������
	void ExportEnd(std::string file);
	//һ��ƴ����Ƶ�������
	void ExportEndSplice(std::string file);
private:
    Ui::XVideoUIClass ui;
	std::string videoDirPath = "";
	std::vector<char*> file_vec;
	unsigned int finishedSpliceCnt = 0;
};
