/********************************************************************************
** Form generated from reading UI file 'motordebugger.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOTORDEBUGGER_H
#define UI_MOTORDEBUGGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MotorDebugger
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_3;
    QWidget *formLayoutWidget_4;
    QFormLayout *formLayout_4;
    QLabel *label_8;
    QLineEdit *motorStateSNEdit;
    QLabel *label_6;
    QLineEdit *motorStateErrorCodeEdit;
    QRadioButton *motorStateIsAliveButton;
    QFormLayout *formLayout_14;
    QRadioButton *motorStateEnabledButton;
    QSlider *motorStateModeSlider;
    QLabel *label_18;
    QLabel *label_19;
    QLineEdit *motorStateLimiterEdit;
    QLabel *label_20;
    QLineEdit *motorStateVBusEdit;
    QLineEdit *motorStateIBusEdit;
    QWidget *formLayoutWidget_5;
    QFormLayout *formLayout_13;
    QLabel *label_21;
    QLineEdit *motorStateIqEdit;
    QLabel *label_22;
    QLineEdit *motorStateIdEdit;
    QLabel *label_23;
    QLineEdit *motorStateEstSpeedEdit;
    QLabel *label_24;
    QLineEdit *motorStateEstAngleEdit;
    QLabel *label_25;
    QLineEdit *motorStateCurrLimitEdit;
    QLabel *label_26;
    QLineEdit *motorStateMCUTempEdit;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *motorSNComboBox;
    QFormLayout *formLayout_3;
    QCheckBox *motorEnabledCheckBox;
    QComboBox *motorModeComboBox;
    QGroupBox *groupBox_2;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *motorTargetSpeedSpinBox;
    QLabel *label_5;
    QSpinBox *motorTargetPosSpinBox;
    QDoubleSpinBox *motorTargetTorqueDoubleSpinBox;
    QDoubleSpinBox *motorTargetCurrLimitDoubleSpinBox;

    void setupUi(QWidget *MotorDebugger)
    {
        if (MotorDebugger->objectName().isEmpty())
            MotorDebugger->setObjectName("MotorDebugger");
        MotorDebugger->resize(399, 388);
        gridLayoutWidget = new QWidget(MotorDebugger);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(0, 0, 401, 391));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(gridLayoutWidget);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setMaximumSize(QSize(16777215, 230));
        formLayoutWidget_4 = new QWidget(groupBox_3);
        formLayoutWidget_4->setObjectName("formLayoutWidget_4");
        formLayoutWidget_4->setGeometry(QRect(0, 20, 201, 176));
        formLayout_4 = new QFormLayout(formLayoutWidget_4);
        formLayout_4->setObjectName("formLayout_4");
        formLayout_4->setVerticalSpacing(4);
        formLayout_4->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(formLayoutWidget_4);
        label_8->setObjectName("label_8");

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_8);

        motorStateSNEdit = new QLineEdit(formLayoutWidget_4);
        motorStateSNEdit->setObjectName("motorStateSNEdit");

        formLayout_4->setWidget(1, QFormLayout::FieldRole, motorStateSNEdit);

        label_6 = new QLabel(formLayoutWidget_4);
        label_6->setObjectName("label_6");

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_6);

        motorStateErrorCodeEdit = new QLineEdit(formLayoutWidget_4);
        motorStateErrorCodeEdit->setObjectName("motorStateErrorCodeEdit");

        formLayout_4->setWidget(2, QFormLayout::FieldRole, motorStateErrorCodeEdit);

        motorStateIsAliveButton = new QRadioButton(formLayoutWidget_4);
        motorStateIsAliveButton->setObjectName("motorStateIsAliveButton");
        motorStateIsAliveButton->setMouseTracking(false);
        motorStateIsAliveButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        motorStateIsAliveButton->setCheckable(true);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, motorStateIsAliveButton);

        formLayout_14 = new QFormLayout();
        formLayout_14->setObjectName("formLayout_14");
        motorStateEnabledButton = new QRadioButton(formLayoutWidget_4);
        motorStateEnabledButton->setObjectName("motorStateEnabledButton");

        formLayout_14->setWidget(0, QFormLayout::LabelRole, motorStateEnabledButton);

        motorStateModeSlider = new QSlider(formLayoutWidget_4);
        motorStateModeSlider->setObjectName("motorStateModeSlider");
        motorStateModeSlider->setMaximum(3);
        motorStateModeSlider->setPageStep(1);
        motorStateModeSlider->setTracking(false);
        motorStateModeSlider->setOrientation(Qt::Orientation::Horizontal);
        motorStateModeSlider->setTickPosition(QSlider::TickPosition::TicksBelow);
        motorStateModeSlider->setTickInterval(1);

        formLayout_14->setWidget(0, QFormLayout::FieldRole, motorStateModeSlider);


        formLayout_4->setLayout(0, QFormLayout::FieldRole, formLayout_14);

        label_18 = new QLabel(formLayoutWidget_4);
        label_18->setObjectName("label_18");

        formLayout_4->setWidget(3, QFormLayout::LabelRole, label_18);

        label_19 = new QLabel(formLayoutWidget_4);
        label_19->setObjectName("label_19");

        formLayout_4->setWidget(4, QFormLayout::LabelRole, label_19);

        motorStateLimiterEdit = new QLineEdit(formLayoutWidget_4);
        motorStateLimiterEdit->setObjectName("motorStateLimiterEdit");

        formLayout_4->setWidget(3, QFormLayout::FieldRole, motorStateLimiterEdit);

        label_20 = new QLabel(formLayoutWidget_4);
        label_20->setObjectName("label_20");

        formLayout_4->setWidget(5, QFormLayout::LabelRole, label_20);

        motorStateVBusEdit = new QLineEdit(formLayoutWidget_4);
        motorStateVBusEdit->setObjectName("motorStateVBusEdit");

        formLayout_4->setWidget(4, QFormLayout::FieldRole, motorStateVBusEdit);

        motorStateIBusEdit = new QLineEdit(formLayoutWidget_4);
        motorStateIBusEdit->setObjectName("motorStateIBusEdit");

        formLayout_4->setWidget(5, QFormLayout::FieldRole, motorStateIBusEdit);

        formLayoutWidget_5 = new QWidget(groupBox_3);
        formLayoutWidget_5->setObjectName("formLayoutWidget_5");
        formLayoutWidget_5->setGeometry(QRect(200, 20, 191, 171));
        formLayout_13 = new QFormLayout(formLayoutWidget_5);
        formLayout_13->setObjectName("formLayout_13");
        formLayout_13->setVerticalSpacing(4);
        formLayout_13->setContentsMargins(0, 0, 0, 0);
        label_21 = new QLabel(formLayoutWidget_5);
        label_21->setObjectName("label_21");

        formLayout_13->setWidget(0, QFormLayout::LabelRole, label_21);

        motorStateIqEdit = new QLineEdit(formLayoutWidget_5);
        motorStateIqEdit->setObjectName("motorStateIqEdit");

        formLayout_13->setWidget(0, QFormLayout::FieldRole, motorStateIqEdit);

        label_22 = new QLabel(formLayoutWidget_5);
        label_22->setObjectName("label_22");

        formLayout_13->setWidget(1, QFormLayout::LabelRole, label_22);

        motorStateIdEdit = new QLineEdit(formLayoutWidget_5);
        motorStateIdEdit->setObjectName("motorStateIdEdit");

        formLayout_13->setWidget(1, QFormLayout::FieldRole, motorStateIdEdit);

        label_23 = new QLabel(formLayoutWidget_5);
        label_23->setObjectName("label_23");

        formLayout_13->setWidget(2, QFormLayout::LabelRole, label_23);

        motorStateEstSpeedEdit = new QLineEdit(formLayoutWidget_5);
        motorStateEstSpeedEdit->setObjectName("motorStateEstSpeedEdit");

        formLayout_13->setWidget(2, QFormLayout::FieldRole, motorStateEstSpeedEdit);

        label_24 = new QLabel(formLayoutWidget_5);
        label_24->setObjectName("label_24");

        formLayout_13->setWidget(3, QFormLayout::LabelRole, label_24);

        motorStateEstAngleEdit = new QLineEdit(formLayoutWidget_5);
        motorStateEstAngleEdit->setObjectName("motorStateEstAngleEdit");

        formLayout_13->setWidget(3, QFormLayout::FieldRole, motorStateEstAngleEdit);

        label_25 = new QLabel(formLayoutWidget_5);
        label_25->setObjectName("label_25");

        formLayout_13->setWidget(4, QFormLayout::LabelRole, label_25);

        motorStateCurrLimitEdit = new QLineEdit(formLayoutWidget_5);
        motorStateCurrLimitEdit->setObjectName("motorStateCurrLimitEdit");

        formLayout_13->setWidget(4, QFormLayout::FieldRole, motorStateCurrLimitEdit);

        label_26 = new QLabel(formLayoutWidget_5);
        label_26->setObjectName("label_26");

        formLayout_13->setWidget(5, QFormLayout::LabelRole, label_26);

        motorStateMCUTempEdit = new QLineEdit(formLayoutWidget_5);
        motorStateMCUTempEdit->setObjectName("motorStateMCUTempEdit");

        formLayout_13->setWidget(5, QFormLayout::FieldRole, motorStateMCUTempEdit);


        gridLayout->addWidget(groupBox_3, 2, 0, 1, 1);

        groupBox = new QGroupBox(gridLayoutWidget);
        groupBox->setObjectName("groupBox");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMaximumSize(QSize(390, 50));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(-1, 20, 391, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        motorSNComboBox = new QComboBox(horizontalLayoutWidget);
        motorSNComboBox->setObjectName("motorSNComboBox");

        formLayout->setWidget(0, QFormLayout::FieldRole, motorSNComboBox);


        horizontalLayout->addLayout(formLayout);

        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName("formLayout_3");
        motorEnabledCheckBox = new QCheckBox(horizontalLayoutWidget);
        motorEnabledCheckBox->setObjectName("motorEnabledCheckBox");

        formLayout_3->setWidget(0, QFormLayout::LabelRole, motorEnabledCheckBox);

        motorModeComboBox = new QComboBox(horizontalLayoutWidget);
        motorModeComboBox->setObjectName("motorModeComboBox");

        formLayout_3->setWidget(0, QFormLayout::FieldRole, motorModeComboBox);


        horizontalLayout->addLayout(formLayout_3);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(gridLayoutWidget);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setMaximumSize(QSize(390, 130));
        formLayoutWidget_2 = new QWidget(groupBox_2);
        formLayoutWidget_2->setObjectName("formLayoutWidget_2");
        formLayoutWidget_2->setGeometry(QRect(-1, 19, 391, 111));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setObjectName("formLayout_2");
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(formLayoutWidget_2);
        label_2->setObjectName("label_2");

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(formLayoutWidget_2);
        label_3->setObjectName("label_3");

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(formLayoutWidget_2);
        label_4->setObjectName("label_4");

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_4);

        motorTargetSpeedSpinBox = new QSpinBox(formLayoutWidget_2);
        motorTargetSpeedSpinBox->setObjectName("motorTargetSpeedSpinBox");
        motorTargetSpeedSpinBox->setSingleStep(5);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, motorTargetSpeedSpinBox);

        label_5 = new QLabel(formLayoutWidget_2);
        label_5->setObjectName("label_5");

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_5);

        motorTargetPosSpinBox = new QSpinBox(formLayoutWidget_2);
        motorTargetPosSpinBox->setObjectName("motorTargetPosSpinBox");
        motorTargetPosSpinBox->setSingleStep(15);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, motorTargetPosSpinBox);

        motorTargetTorqueDoubleSpinBox = new QDoubleSpinBox(formLayoutWidget_2);
        motorTargetTorqueDoubleSpinBox->setObjectName("motorTargetTorqueDoubleSpinBox");
        motorTargetTorqueDoubleSpinBox->setSingleStep(0.050000000000000);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, motorTargetTorqueDoubleSpinBox);

        motorTargetCurrLimitDoubleSpinBox = new QDoubleSpinBox(formLayoutWidget_2);
        motorTargetCurrLimitDoubleSpinBox->setObjectName("motorTargetCurrLimitDoubleSpinBox");
        motorTargetCurrLimitDoubleSpinBox->setSingleStep(0.100000000000000);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, motorTargetCurrLimitDoubleSpinBox);


        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);

        gridLayout->setRowStretch(0, 1);

        retranslateUi(MotorDebugger);

        QMetaObject::connectSlotsByName(MotorDebugger);
    } // setupUi

    void retranslateUi(QWidget *MotorDebugger)
    {
        MotorDebugger->setWindowTitle(QCoreApplication::translate("MotorDebugger", "Form", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MotorDebugger", "Motor State", nullptr));
        label_8->setText(QCoreApplication::translate("MotorDebugger", "SN", nullptr));
        label_6->setText(QCoreApplication::translate("MotorDebugger", "ErrorCode", nullptr));
        motorStateIsAliveButton->setText(QCoreApplication::translate("MotorDebugger", "IsAlive", nullptr));
        motorStateEnabledButton->setText(QCoreApplication::translate("MotorDebugger", "Enabled", nullptr));
        label_18->setText(QCoreApplication::translate("MotorDebugger", "Limiter", nullptr));
        label_19->setText(QCoreApplication::translate("MotorDebugger", "Vbus (V)", nullptr));
        label_20->setText(QCoreApplication::translate("MotorDebugger", "Ibus (A)", nullptr));
        label_21->setText(QCoreApplication::translate("MotorDebugger", "Real Iq (A)", nullptr));
        label_22->setText(QCoreApplication::translate("MotorDebugger", "Real Id (A)", nullptr));
        label_23->setText(QCoreApplication::translate("MotorDebugger", "Est. V (RPM)", nullptr));
        label_24->setText(QCoreApplication::translate("MotorDebugger", "Est.Angle (deg)", nullptr));
        label_25->setText(QCoreApplication::translate("MotorDebugger", "CurrLimit (A)", nullptr));
        label_26->setText(QCoreApplication::translate("MotorDebugger", "MCUTemp (\302\260C)", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MotorDebugger", "Motor Control", nullptr));
        label->setText(QCoreApplication::translate("MotorDebugger", "SN:", nullptr));
        motorEnabledCheckBox->setText(QCoreApplication::translate("MotorDebugger", "Enabled", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MotorDebugger", "Motor Target", nullptr));
        label_2->setText(QCoreApplication::translate("MotorDebugger", "Current Limit", nullptr));
        label_3->setText(QCoreApplication::translate("MotorDebugger", "Set Torque (Iq, A)", nullptr));
        label_4->setText(QCoreApplication::translate("MotorDebugger", "Set Speed (RPM)", nullptr));
        label_5->setText(QCoreApplication::translate("MotorDebugger", "Set Absolute Position (deg)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MotorDebugger: public Ui_MotorDebugger {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOTORDEBUGGER_H
