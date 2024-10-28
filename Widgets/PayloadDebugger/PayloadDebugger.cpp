#include "PayloadDebugger.h"
#include "ui_PayloadDebugger.h"

CmdDebugger::CmdDebugger(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CmdDebugger)
{
    ui->setupUi(this);
}

CmdDebugger::~CmdDebugger()
{
    delete ui;
}
