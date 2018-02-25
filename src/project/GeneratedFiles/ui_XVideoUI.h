/********************************************************************************
** Form generated from reading UI file 'XVideoUI.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XVIDEOUI_H
#define UI_XVIDEOUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "xvideowidget.h"

QT_BEGIN_NAMESPACE

class Ui_XVideoUIClass
{
public:
    QPushButton *closeButton;
    XVideoWidget *src1;
    QPushButton *openButton;
    QSlider *playSlider;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *bright;
    QDoubleSpinBox *contrast;
    QPushButton *setButton;
    XVideoWidget *des;
    QLabel *label_3;
    QComboBox *mosaic;
    QPushButton *beginButton;
    QTextBrowser *textDisplay;
    QLabel *label_4;
    QLabel *label_9;
    QLabel *label_11;
    QLabel *finishVedioCnt;
    QLabel *allVedioCntMosaic;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *finishVedioCnt_2;
    QLabel *allVedioCntSplice;
    QLabel *label_7;
    QLabel *label_10;
    QTextBrowser *textDisplay_2;
    QLabel *label_12;
    QLabel *label_8;
    QSpinBox *spliceNumber;
    QLabel *label_13;

    void setupUi(QWidget *XVideoUIClass)
    {
        if (XVideoUIClass->objectName().isEmpty())
            XVideoUIClass->setObjectName(QStringLiteral("XVideoUIClass"));
        XVideoUIClass->resize(1038, 518);
        XVideoUIClass->setStyleSheet(QString::fromUtf8("QPushButton:hover\n"
" { \n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(120, 120, 120, 255), stop:0.495 rgba(39, 39, 39, 255), stop:0.505 rgba(39,39, 39, 255), stop:1 rgba(120, 120,120, 255));\n"
" border: none;\n"
"border-radius:5px;\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";\n"
" }\n"
"QPushButton:!hover\n"
" { \n"
"	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(39, 39, 39, 255), stop:0.495 rgba(120, 120, 120, 255), stop:0.505 rgba(120,120, 120, 255), stop:1 rgba(39, 39,39, 255));\n"
" border: none;\n"
"border-radius:5px;\n"
"color: rgb(255, 255, 255);\n"
"font: 75 12pt \"\351\273\221\344\275\223\";\n"
" }\n"
"#XVideoUIClass\n"
"{\n"
"	background-color: rgb(53, 53, 53);\n"
"}\n"
"#closeButton\n"
"{\n"
"font: 75 11pt \"\347\255\211\347\272\277\";\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(53, 53, 53);\n"
"}\n"
"\n"
"\n"
"QLabel\n"
"{\n"
"	\n"
"	color: rgb(255, 255, 255);\n"
"	\n"
"	fo"
                        "nt: 75 12pt \"\351\273\221\344\275\223\";\n"
"}\n"
""));
        closeButton = new QPushButton(XVideoUIClass);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        closeButton->setGeometry(QRect(990, 10, 31, 21));
        closeButton->setStyleSheet(QStringLiteral(""));
        closeButton->setFlat(true);
        src1 = new XVideoWidget(XVideoUIClass);
        src1->setObjectName(QStringLiteral("src1"));
        src1->setGeometry(QRect(10, 40, 380, 280));
        openButton = new QPushButton(XVideoUIClass);
        openButton->setObjectName(QStringLiteral("openButton"));
        openButton->setGeometry(QRect(20, 390, 141, 111));
        playSlider = new QSlider(XVideoUIClass);
        playSlider->setObjectName(QStringLiteral("playSlider"));
        playSlider->setGeometry(QRect(10, 330, 381, 22));
        playSlider->setStyleSheet(QStringLiteral("color: rgb(0, 0, 127);"));
        playSlider->setMaximum(999);
        playSlider->setOrientation(Qt::Horizontal);
        label = new QLabel(XVideoUIClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 420, 91, 31));
        label_2 = new QLabel(XVideoUIClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(200, 460, 121, 31));
        bright = new QSpinBox(XVideoUIClass);
        bright->setObjectName(QStringLiteral("bright"));
        bright->setGeometry(QRect(330, 430, 71, 22));
        contrast = new QDoubleSpinBox(XVideoUIClass);
        contrast->setObjectName(QStringLiteral("contrast"));
        contrast->setGeometry(QRect(330, 470, 71, 22));
        contrast->setMinimum(1);
        contrast->setMaximum(3);
        setButton = new QPushButton(XVideoUIClass);
        setButton->setObjectName(QStringLiteral("setButton"));
        setButton->setGeometry(QRect(410, 390, 151, 111));
        des = new XVideoWidget(XVideoUIClass);
        des->setObjectName(QStringLiteral("des"));
        des->setGeometry(QRect(410, 40, 380, 280));
        label_3 = new QLabel(XVideoUIClass);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(250, 380, 51, 41));
        mosaic = new QComboBox(XVideoUIClass);
        mosaic->setObjectName(QStringLiteral("mosaic"));
        mosaic->setGeometry(QRect(330, 390, 69, 22));
        beginButton = new QPushButton(XVideoUIClass);
        beginButton->setObjectName(QStringLiteral("beginButton"));
        beginButton->setGeometry(QRect(620, 390, 151, 111));
        textDisplay = new QTextBrowser(XVideoUIClass);
        textDisplay->setObjectName(QStringLiteral("textDisplay"));
        textDisplay->setGeometry(QRect(800, 150, 221, 81));
        textDisplay->setStyleSheet(QString::fromUtf8("color: rgb(250, 250, 250);\n"
"background-color: rgb(104, 104, 104);\n"
"font: 10pt \"\346\245\267\344\275\223\";"));
        label_4 = new QLabel(XVideoUIClass);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(800, 110, 161, 21));
        label_9 = new QLabel(XVideoUIClass);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(800, 40, 121, 41));
        label_11 = new QLabel(XVideoUIClass);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(800, 70, 121, 41));
        finishVedioCnt = new QLabel(XVideoUIClass);
        finishVedioCnt->setObjectName(QStringLiteral("finishVedioCnt"));
        finishVedioCnt->setGeometry(QRect(930, 40, 51, 41));
        allVedioCntMosaic = new QLabel(XVideoUIClass);
        allVedioCntMosaic->setObjectName(QStringLiteral("allVedioCntMosaic"));
        allVedioCntMosaic->setGeometry(QRect(930, 70, 51, 41));
        label_5 = new QLabel(XVideoUIClass);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 0, 241, 41));
        label_5->setStyleSheet(QString::fromUtf8("font: 16pt \"\346\245\267\344\275\223\";"));
        label_6 = new QLabel(XVideoUIClass);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(290, 10, 251, 31));
        label_6->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"font: 9pt \"\346\245\267\344\275\223\";"));
        finishVedioCnt_2 = new QLabel(XVideoUIClass);
        finishVedioCnt_2->setObjectName(QStringLiteral("finishVedioCnt_2"));
        finishVedioCnt_2->setGeometry(QRect(930, 300, 51, 41));
        allVedioCntSplice = new QLabel(XVideoUIClass);
        allVedioCntSplice->setObjectName(QStringLiteral("allVedioCntSplice"));
        allVedioCntSplice->setGeometry(QRect(930, 330, 51, 41));
        label_7 = new QLabel(XVideoUIClass);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(800, 370, 161, 21));
        label_10 = new QLabel(XVideoUIClass);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(800, 300, 121, 41));
        textDisplay_2 = new QTextBrowser(XVideoUIClass);
        textDisplay_2->setObjectName(QStringLiteral("textDisplay_2"));
        textDisplay_2->setGeometry(QRect(800, 410, 221, 81));
        textDisplay_2->setStyleSheet(QString::fromUtf8("color: rgb(250, 250, 250);\n"
"background-color: rgb(104, 104, 104);\n"
"font: 10pt \"\346\245\267\344\275\223\";"));
        label_12 = new QLabel(XVideoUIClass);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(800, 330, 121, 41));
        label_8 = new QLabel(XVideoUIClass);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(580, 340, 111, 41));
        spliceNumber = new QSpinBox(XVideoUIClass);
        spliceNumber->setObjectName(QStringLiteral("spliceNumber"));
        spliceNumber->setGeometry(QRect(700, 350, 71, 22));
        spliceNumber->setMinimum(2);
        spliceNumber->setMaximum(8);
        spliceNumber->setValue(5);
        label_13 = new QLabel(XVideoUIClass);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(480, 310, 321, 41));
        label_13->setStyleSheet(QString::fromUtf8("font: 12pt \"\346\245\267\344\275\223\";\n"
"color: rgb(255, 0, 0);"));

        retranslateUi(XVideoUIClass);
        QObject::connect(closeButton, SIGNAL(clicked()), XVideoUIClass, SLOT(close()));
        QObject::connect(openButton, SIGNAL(clicked()), XVideoUIClass, SLOT(Open()));
        QObject::connect(playSlider, SIGNAL(sliderPressed()), XVideoUIClass, SLOT(SliderPress()));
        QObject::connect(playSlider, SIGNAL(sliderReleased()), XVideoUIClass, SLOT(SliderRelease()));
        QObject::connect(playSlider, SIGNAL(sliderMoved(int)), XVideoUIClass, SLOT(SetPos(int)));
        QObject::connect(setButton, SIGNAL(clicked()), XVideoUIClass, SLOT(Set()));
        QObject::connect(beginButton, SIGNAL(clicked()), XVideoUIClass, SLOT(BeginProcess()));

        QMetaObject::connectSlotsByName(XVideoUIClass);
    } // setupUi

    void retranslateUi(QWidget *XVideoUIClass)
    {
        XVideoUIClass->setWindowTitle(QApplication::translate("XVideoUIClass", "XVideoUI", Q_NULLPTR));
        closeButton->setText(QApplication::translate("XVideoUIClass", "X", Q_NULLPTR));
        openButton->setText(QApplication::translate("XVideoUIClass", "1.\346\211\223\345\274\200", Q_NULLPTR));
        label->setText(QApplication::translate("XVideoUIClass", "\344\272\256\345\272\246[0-100]", Q_NULLPTR));
        label_2->setText(QApplication::translate("XVideoUIClass", "\345\257\271\346\257\224\345\272\246[1.0-3.0]", Q_NULLPTR));
        setButton->setText(QApplication::translate("XVideoUIClass", "2.\350\256\276\347\275\256", Q_NULLPTR));
        label_3->setText(QApplication::translate("XVideoUIClass", "\351\251\254\350\265\233\345\205\213", Q_NULLPTR));
        mosaic->clear();
        mosaic->insertItems(0, QStringList()
         << QApplication::translate("XVideoUIClass", "\344\270\215\346\267\273\345\212\240", Q_NULLPTR)
         << QApplication::translate("XVideoUIClass", "\346\267\273\345\212\240", Q_NULLPTR)
        );
        beginButton->setText(QApplication::translate("XVideoUIClass", "3.\345\244\204\347\220\206\345\271\266\345\257\274\345\207\272", Q_NULLPTR));
        label_4->setText(QApplication::translate("XVideoUIClass", "\345\275\223\345\211\215\346\211\223\347\240\201\345\256\214\347\232\204\350\247\206\351\242\221\346\226\207\344\273\266", Q_NULLPTR));
        label_9->setText(QApplication::translate("XVideoUIClass", "\345\267\262\346\211\223\347\240\201\345\256\214\350\247\206\351\242\221\346\225\260\357\274\232", Q_NULLPTR));
        label_11->setText(QApplication::translate("XVideoUIClass", "\345\276\205\346\211\223\347\240\201\350\247\206\351\242\221\346\200\273\346\225\260\357\274\232", Q_NULLPTR));
        finishVedioCnt->setText(QApplication::translate("XVideoUIClass", "- -", Q_NULLPTR));
        allVedioCntMosaic->setText(QApplication::translate("XVideoUIClass", "- -", Q_NULLPTR));
        label_5->setText(QApplication::translate("XVideoUIClass", "\347\201\253\345\261\261\345\260\217\350\247\206\351\242\221\346\211\271\351\207\217\346\213\274\346\216\245\345\212\251\346\211\213 V1.0 ", Q_NULLPTR));
        label_6->setText(QApplication::translate("XVideoUIClass", "\344\275\234\350\200\205\357\274\232\350\275\251\350\276\225\345\215\201\345\233\233  \351\202\256\347\256\261\357\274\232liugaolian@163.com", Q_NULLPTR));
        finishVedioCnt_2->setText(QApplication::translate("XVideoUIClass", "- -", Q_NULLPTR));
        allVedioCntSplice->setText(QApplication::translate("XVideoUIClass", "- -", Q_NULLPTR));
        label_7->setText(QApplication::translate("XVideoUIClass", "\345\275\223\345\211\215\346\213\274\346\216\245\345\256\214\347\232\204\350\247\206\351\242\221\346\226\207\344\273\266", Q_NULLPTR));
        label_10->setText(QApplication::translate("XVideoUIClass", "\345\267\262\346\213\274\346\216\245\345\256\214\350\247\206\351\242\221\346\225\260\357\274\232", Q_NULLPTR));
        label_12->setText(QApplication::translate("XVideoUIClass", "\345\276\205\346\213\274\346\216\245\350\247\206\351\242\221\346\200\273\346\225\260\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("XVideoUIClass", "\346\213\274\346\216\245\344\270\252\346\225\260[2-8]", Q_NULLPTR));
        label_13->setText(QApplication::translate("XVideoUIClass", "\350\255\246\345\221\212\357\274\201\350\247\206\351\242\221\346\200\273\346\225\260\350\265\267\347\240\201\350\246\201\346\257\224\346\213\274\346\216\245\344\270\252\346\225\260\345\244\232\344\270\200\344\270\252\357\274\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class XVideoUIClass: public Ui_XVideoUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XVIDEOUI_H
