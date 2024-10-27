#include "CmdDebugger.h"
#include "ui_CmdDebugger.h"

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
