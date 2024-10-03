#include "motordebugger.h"
#include "ui_motordebugger.h"

MotorDebugger::MotorDebugger(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MotorDebugger)
{
    ui->setupUi(this);
}

MotorDebugger::~MotorDebugger()
{
    delete ui;
}
