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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
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
    QMenuBar *menubar;
    QMenu *menuLoadDestFiles;
    QMenu *menuDebugger;
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
        gridLayoutWidget->setGeometry(QRect(40, 270, 223, 299));
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
        gridLayoutWidget_2->setGeometry(QRect(540, 270, 223, 299));
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

        ControllerWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ControllerWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuLoadDestFiles = new QMenu(menubar);
        menuLoadDestFiles->setObjectName("menuLoadDestFiles");
        menuDebugger = new QMenu(menubar);
        menuDebugger->setObjectName("menuDebugger");
        ControllerWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ControllerWindow);
        statusbar->setObjectName("statusbar");
        ControllerWindow->setStatusBar(statusbar);

        menubar->addAction(menuLoadDestFiles->menuAction());
        menubar->addAction(menuDebugger->menuAction());

        retranslateUi(ControllerWindow);

        QMetaObject::connectSlotsByName(ControllerWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ControllerWindow)
    {
        ControllerWindow->setWindowTitle(QCoreApplication::translate("ControllerWindow", "Controller", nullptr));
        label->setText(QCoreApplication::translate("ControllerWindow", "Left Joystick", nullptr));
        label_2->setText(QCoreApplication::translate("ControllerWindow", "Right Joystick", nullptr));
        menuLoadDestFiles->setTitle(QCoreApplication::translate("ControllerWindow", "Load Dest. files", nullptr));
        menuDebugger->setTitle(QCoreApplication::translate("ControllerWindow", "Debugger", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ControllerWindow: public Ui_ControllerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLLERWINDOW_H
