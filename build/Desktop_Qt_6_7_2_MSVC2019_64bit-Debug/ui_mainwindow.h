/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEEPROM_Update;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QWidget *navigation_widget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *eth_comboBox;
    QPushButton *con_pushButton;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QTextEdit *configXmlPathEdit;
    QToolButton *selectConfigPathButton;
    QPushButton *parseConfigXmlButton;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QTreeView *stateTreeView;
    QPushButton *enterControllerButton;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;
    QMenu *menuTools;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 620);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(800, 620));
        MainWindow->setMaximumSize(QSize(800, 620));
        actionEEPROM_Update = new QAction(MainWindow);
        actionEEPROM_Update->setObjectName("actionEEPROM_Update");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        navigation_widget = new QWidget(centralwidget);
        navigation_widget->setObjectName("navigation_widget");
        navigation_widget->setMinimumSize(QSize(220, 0));
        navigation_widget->setMaximumSize(QSize(220, 16777215));
        verticalLayoutWidget = new QWidget(navigation_widget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 10, 222, 100));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        eth_comboBox = new QComboBox(verticalLayoutWidget);
        eth_comboBox->setObjectName("eth_comboBox");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(eth_comboBox->sizePolicy().hasHeightForWidth());
        eth_comboBox->setSizePolicy(sizePolicy1);
        eth_comboBox->setMinimumSize(QSize(220, 40));
        eth_comboBox->setMaximumSize(QSize(220, 40));

        verticalLayout->addWidget(eth_comboBox);

        con_pushButton = new QPushButton(verticalLayoutWidget);
        con_pushButton->setObjectName("con_pushButton");
        sizePolicy.setHeightForWidth(con_pushButton->sizePolicy().hasHeightForWidth());
        con_pushButton->setSizePolicy(sizePolicy);
        con_pushButton->setMinimumSize(QSize(100, 30));
        con_pushButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        con_pushButton->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        con_pushButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);

        verticalLayout->addWidget(con_pushButton, 0, Qt::AlignmentFlag::AlignRight);

        verticalLayoutWidget_2 = new QWidget(navigation_widget);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(0, 110, 222, 102));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 5);
        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        configXmlPathEdit = new QTextEdit(verticalLayoutWidget_2);
        configXmlPathEdit->setObjectName("configXmlPathEdit");
        sizePolicy.setHeightForWidth(configXmlPathEdit->sizePolicy().hasHeightForWidth());
        configXmlPathEdit->setSizePolicy(sizePolicy);
        configXmlPathEdit->setMinimumSize(QSize(180, 40));
        configXmlPathEdit->setMaximumSize(QSize(220, 40));
        configXmlPathEdit->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        configXmlPathEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        configXmlPathEdit->setUndoRedoEnabled(false);
        configXmlPathEdit->setReadOnly(true);
        configXmlPathEdit->setAcceptRichText(false);
        configXmlPathEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);

        horizontalLayout_2->addWidget(configXmlPathEdit);

        selectConfigPathButton = new QToolButton(verticalLayoutWidget_2);
        selectConfigPathButton->setObjectName("selectConfigPathButton");
        selectConfigPathButton->setMinimumSize(QSize(0, 40));
        selectConfigPathButton->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_2->addWidget(selectConfigPathButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        parseConfigXmlButton = new QPushButton(verticalLayoutWidget_2);
        parseConfigXmlButton->setObjectName("parseConfigXmlButton");
        sizePolicy.setHeightForWidth(parseConfigXmlButton->sizePolicy().hasHeightForWidth());
        parseConfigXmlButton->setSizePolicy(sizePolicy);
        parseConfigXmlButton->setMinimumSize(QSize(220, 30));
        parseConfigXmlButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        verticalLayout_2->addWidget(parseConfigXmlButton);

        verticalLayoutWidget_3 = new QWidget(navigation_widget);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(0, 210, 222, 331));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setSpacing(7);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        stateTreeView = new QTreeView(verticalLayoutWidget_3);
        stateTreeView->setObjectName("stateTreeView");
        stateTreeView->setRootIsDecorated(true);
        stateTreeView->header()->setVisible(false);

        verticalLayout_3->addWidget(stateTreeView);

        enterControllerButton = new QPushButton(verticalLayoutWidget_3);
        enterControllerButton->setObjectName("enterControllerButton");
        enterControllerButton->setEnabled(false);
        sizePolicy.setHeightForWidth(enterControllerButton->sizePolicy().hasHeightForWidth());
        enterControllerButton->setSizePolicy(sizePolicy);
        enterControllerButton->setMinimumSize(QSize(220, 30));
        enterControllerButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);

        verticalLayout_3->addWidget(enterControllerButton);


        horizontalLayout->addWidget(navigation_widget);

        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName("gridLayout_2");
        textBrowser = new QTextBrowser(widget);
        textBrowser->setObjectName("textBrowser");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(textBrowser, 0, 0, 1, 1);


        horizontalLayout->addWidget(widget);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName("menuTools");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuTools->menuAction());
        menuTools->addAction(actionEEPROM_Update);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MIS-Pilot Main", nullptr));
        actionEEPROM_Update->setText(QCoreApplication::translate("MainWindow", "EEPROM Update...", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Ethernet Adapter:", nullptr));
        con_pushButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "EtherCAT Slave Config (.xml):", nullptr));
        selectConfigPathButton->setText(QCoreApplication::translate("MainWindow", "...", nullptr));
        parseConfigXmlButton->setText(QCoreApplication::translate("MainWindow", "Parse ESI XML", nullptr));
        enterControllerButton->setText(QCoreApplication::translate("MainWindow", "Enter Controller...", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
