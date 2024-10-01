/********************************************************************************
** Form generated from reading UI file 'controllerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLLERWINDOW_H
#define UI_CONTROLLERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "JoypadWidget/joypad.h"

QT_BEGIN_NAMESPACE

class Ui_ControllerWindow
{
public:
    QWidget *centralwidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_2;
    QSlider *leftJoyHorizontalSlider;
    QComboBox *leftJoyComboBox;
    QSlider *leftJoyVerticalSlider;
    JoyPad *leftJoyPad;
    QLabel *label;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_3;
    QSlider *rightJoyHorizontalSlider;
    QComboBox *rightJoyComboBox;
    QSlider *rightJoyVerticalSlider;
    JoyPad *rightJoyPad;
    QLabel *label_2;
    QGroupBox *motorDebugGroupBox;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_3;
    QLCDNumber *motorPositionLcd;
    QSlider *motorModeSlider;
    QSlider *motorTrajAngleSlider;
    QCheckBox *motorEnableCheckBox;
    QComboBox *motorSNComboBox;
    QGroupBox *groupBox_2;
    QLCDNumber *motorSpeedLcd;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QLCDNumber *motorStatusLed1;
    QGroupBox *groupBox_8;
    QLCDNumber *motorStatusLed2;
    QGroupBox *groupBox_7;
    QLCDNumber *motorStatusLed3;
    QGroupBox *groupBox_6;
    QLCDNumber *motorStatusLed4;
    QGroupBox *groupBox_5;
    QLCDNumber *motorStatusLed5;
    QGroupBox *groupBox_4;
    QLCDNumber *motorStatusLed6;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ControllerWindow)
    {
        if (ControllerWindow->objectName().isEmpty())
            ControllerWindow->setObjectName("ControllerWindow");
        ControllerWindow->resize(800, 600);
        centralwidget = new QWidget(ControllerWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(40, 270, 222, 299));
        gridLayout_2 = new QGridLayout(gridLayoutWidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setVerticalSpacing(10);
        gridLayout_2->setContentsMargins(0, 0, 0, 5);
        leftJoyHorizontalSlider = new QSlider(gridLayoutWidget);
        leftJoyHorizontalSlider->setObjectName("leftJoyHorizontalSlider");
        leftJoyHorizontalSlider->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        leftJoyHorizontalSlider->setMinimum(-100);
        leftJoyHorizontalSlider->setMaximum(100);
        leftJoyHorizontalSlider->setTracking(false);
        leftJoyHorizontalSlider->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(leftJoyHorizontalSlider, 3, 0, 1, 1);

        leftJoyComboBox = new QComboBox(gridLayoutWidget);
        leftJoyComboBox->setObjectName("leftJoyComboBox");
        leftJoyComboBox->setMinimumSize(QSize(0, 30));
        leftJoyComboBox->setMaximumSize(QSize(16777215, 30));
        leftJoyComboBox->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        gridLayout_2->addWidget(leftJoyComboBox, 1, 0, 1, 1);

        leftJoyVerticalSlider = new QSlider(gridLayoutWidget);
        leftJoyVerticalSlider->setObjectName("leftJoyVerticalSlider");
        leftJoyVerticalSlider->setEnabled(true);
        leftJoyVerticalSlider->setMinimum(-100);
        leftJoyVerticalSlider->setMaximum(100);
        leftJoyVerticalSlider->setTracking(false);
        leftJoyVerticalSlider->setOrientation(Qt::Orientation::Vertical);
        leftJoyVerticalSlider->setTickPosition(QSlider::TickPosition::NoTicks);

        gridLayout_2->addWidget(leftJoyVerticalSlider, 2, 2, 1, 1);

        leftJoyPad = new JoyPad(gridLayoutWidget);
        leftJoyPad->setObjectName("leftJoyPad");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftJoyPad->sizePolicy().hasHeightForWidth());
        leftJoyPad->setSizePolicy(sizePolicy);
        leftJoyPad->setMinimumSize(QSize(200, 200));
        leftJoyPad->setMaximumSize(QSize(200, 200));

        gridLayout_2->addWidget(leftJoyPad, 2, 0, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(540, 270, 221, 299));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setVerticalSpacing(10);
        gridLayout_3->setContentsMargins(0, 0, 0, 5);
        rightJoyHorizontalSlider = new QSlider(gridLayoutWidget_2);
        rightJoyHorizontalSlider->setObjectName("rightJoyHorizontalSlider");
        rightJoyHorizontalSlider->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        rightJoyHorizontalSlider->setMinimum(-100);
        rightJoyHorizontalSlider->setMaximum(100);
        rightJoyHorizontalSlider->setTracking(false);
        rightJoyHorizontalSlider->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_3->addWidget(rightJoyHorizontalSlider, 3, 0, 1, 1);

        rightJoyComboBox = new QComboBox(gridLayoutWidget_2);
        rightJoyComboBox->setObjectName("rightJoyComboBox");
        rightJoyComboBox->setMinimumSize(QSize(0, 30));
        rightJoyComboBox->setMaximumSize(QSize(16777215, 30));
        rightJoyComboBox->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        gridLayout_3->addWidget(rightJoyComboBox, 1, 0, 1, 1);

        rightJoyVerticalSlider = new QSlider(gridLayoutWidget_2);
        rightJoyVerticalSlider->setObjectName("rightJoyVerticalSlider");
        rightJoyVerticalSlider->setMinimum(-100);
        rightJoyVerticalSlider->setMaximum(100);
        rightJoyVerticalSlider->setTracking(false);
        rightJoyVerticalSlider->setOrientation(Qt::Orientation::Vertical);

        gridLayout_3->addWidget(rightJoyVerticalSlider, 2, 2, 1, 1);

        rightJoyPad = new JoyPad(gridLayoutWidget_2);
        rightJoyPad->setObjectName("rightJoyPad");
        sizePolicy.setHeightForWidth(rightJoyPad->sizePolicy().hasHeightForWidth());
        rightJoyPad->setSizePolicy(sizePolicy);
        rightJoyPad->setMinimumSize(QSize(200, 200));
        rightJoyPad->setMaximumSize(QSize(200, 200));

        gridLayout_3->addWidget(rightJoyPad, 2, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget_2);
        label_2->setObjectName("label_2");

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        motorDebugGroupBox = new QGroupBox(centralwidget);
        motorDebugGroupBox->setObjectName("motorDebugGroupBox");
        motorDebugGroupBox->setGeometry(QRect(20, 10, 751, 101));
        gridLayoutWidget_3 = new QWidget(motorDebugGroupBox);
        gridLayoutWidget_3->setObjectName("gridLayoutWidget_3");
        gridLayoutWidget_3->setGeometry(QRect(0, 20, 751, 80));
        gridLayout = new QGridLayout(gridLayoutWidget_3);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(gridLayoutWidget_3);
        groupBox_3->setObjectName("groupBox_3");
        motorPositionLcd = new QLCDNumber(groupBox_3);
        motorPositionLcd->setObjectName("motorPositionLcd");
        motorPositionLcd->setGeometry(QRect(0, 20, 101, 31));

        gridLayout->addWidget(groupBox_3, 1, 2, 1, 1);

        motorModeSlider = new QSlider(gridLayoutWidget_3);
        motorModeSlider->setObjectName("motorModeSlider");
        sizePolicy.setHeightForWidth(motorModeSlider->sizePolicy().hasHeightForWidth());
        motorModeSlider->setSizePolicy(sizePolicy);
        motorModeSlider->setMinimumSize(QSize(100, 0));
        motorModeSlider->setMaximum(3);
        motorModeSlider->setOrientation(Qt::Orientation::Horizontal);
        motorModeSlider->setTickPosition(QSlider::TickPosition::TicksBelow);

        gridLayout->addWidget(motorModeSlider, 0, 2, 1, 1);

        motorTrajAngleSlider = new QSlider(gridLayoutWidget_3);
        motorTrajAngleSlider->setObjectName("motorTrajAngleSlider");
        motorTrajAngleSlider->setMinimum(-80);
        motorTrajAngleSlider->setMaximum(80);
        motorTrajAngleSlider->setSingleStep(1);
        motorTrajAngleSlider->setPageStep(1);
        motorTrajAngleSlider->setOrientation(Qt::Orientation::Horizontal);
        motorTrajAngleSlider->setTickPosition(QSlider::TickPosition::TicksBelow);
        motorTrajAngleSlider->setTickInterval(1);

        gridLayout->addWidget(motorTrajAngleSlider, 0, 3, 1, 1);

        motorEnableCheckBox = new QCheckBox(gridLayoutWidget_3);
        motorEnableCheckBox->setObjectName("motorEnableCheckBox");

        gridLayout->addWidget(motorEnableCheckBox, 0, 1, 1, 1);

        motorSNComboBox = new QComboBox(gridLayoutWidget_3);
        motorSNComboBox->setObjectName("motorSNComboBox");
        sizePolicy.setHeightForWidth(motorSNComboBox->sizePolicy().hasHeightForWidth());
        motorSNComboBox->setSizePolicy(sizePolicy);
        motorSNComboBox->setMinimumSize(QSize(120, 0));
        motorSNComboBox->setMaximumSize(QSize(120, 16777215));

        gridLayout->addWidget(motorSNComboBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(gridLayoutWidget_3);
        groupBox_2->setObjectName("groupBox_2");
        motorSpeedLcd = new QLCDNumber(groupBox_2);
        motorSpeedLcd->setObjectName("motorSpeedLcd");
        motorSpeedLcd->setGeometry(QRect(0, 20, 161, 31));

        gridLayout->addWidget(groupBox_2, 1, 0, 1, 2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        groupBox = new QGroupBox(gridLayoutWidget_3);
        groupBox->setObjectName("groupBox");
        motorStatusLed1 = new QLCDNumber(groupBox);
        motorStatusLed1->setObjectName("motorStatusLed1");
        motorStatusLed1->setGeometry(QRect(0, 20, 71, 31));

        horizontalLayout->addWidget(groupBox);

        groupBox_8 = new QGroupBox(gridLayoutWidget_3);
        groupBox_8->setObjectName("groupBox_8");
        motorStatusLed2 = new QLCDNumber(groupBox_8);
        motorStatusLed2->setObjectName("motorStatusLed2");
        motorStatusLed2->setGeometry(QRect(0, 20, 71, 31));

        horizontalLayout->addWidget(groupBox_8);

        groupBox_7 = new QGroupBox(gridLayoutWidget_3);
        groupBox_7->setObjectName("groupBox_7");
        motorStatusLed3 = new QLCDNumber(groupBox_7);
        motorStatusLed3->setObjectName("motorStatusLed3");
        motorStatusLed3->setGeometry(QRect(0, 20, 61, 31));

        horizontalLayout->addWidget(groupBox_7);

        groupBox_6 = new QGroupBox(gridLayoutWidget_3);
        groupBox_6->setObjectName("groupBox_6");
        motorStatusLed4 = new QLCDNumber(groupBox_6);
        motorStatusLed4->setObjectName("motorStatusLed4");
        motorStatusLed4->setGeometry(QRect(0, 20, 61, 31));

        horizontalLayout->addWidget(groupBox_6);

        groupBox_5 = new QGroupBox(gridLayoutWidget_3);
        groupBox_5->setObjectName("groupBox_5");
        motorStatusLed5 = new QLCDNumber(groupBox_5);
        motorStatusLed5->setObjectName("motorStatusLed5");
        motorStatusLed5->setGeometry(QRect(0, 20, 61, 31));

        horizontalLayout->addWidget(groupBox_5);

        groupBox_4 = new QGroupBox(gridLayoutWidget_3);
        groupBox_4->setObjectName("groupBox_4");
        motorStatusLed6 = new QLCDNumber(groupBox_4);
        motorStatusLed6->setObjectName("motorStatusLed6");
        motorStatusLed6->setGeometry(QRect(0, 20, 61, 31));

        horizontalLayout->addWidget(groupBox_4);


        gridLayout->addLayout(horizontalLayout, 1, 3, 1, 1);

        ControllerWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ControllerWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        ControllerWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ControllerWindow);
        statusbar->setObjectName("statusbar");
        ControllerWindow->setStatusBar(statusbar);

        retranslateUi(ControllerWindow);

        QMetaObject::connectSlotsByName(ControllerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ControllerWindow)
    {
        ControllerWindow->setWindowTitle(QCoreApplication::translate("ControllerWindow", "Controller", nullptr));
        label->setText(QCoreApplication::translate("ControllerWindow", "Left Joystick", nullptr));
        label_2->setText(QCoreApplication::translate("ControllerWindow", "Right Joystick", nullptr));
        motorDebugGroupBox->setTitle(QCoreApplication::translate("ControllerWindow", "Motor Debug", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ControllerWindow", "Position", nullptr));
        motorEnableCheckBox->setText(QCoreApplication::translate("ControllerWindow", "Enable", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ControllerWindow", "Speed", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ControllerWindow", "Iq", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("ControllerWindow", "CurrentLimit", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("ControllerWindow", "LimiterState", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("ControllerWindow", "ErrorCode", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("ControllerWindow", "Enable", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("ControllerWindow", "Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControllerWindow: public Ui_ControllerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLLERWINDOW_H
