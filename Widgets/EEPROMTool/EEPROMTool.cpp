#include "EEPROMTool.h"
#include "ui_EEPROMTool.h"

EEPROMTool::EEPROMTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EEPROMTool)
{
    ui->setupUi(this);
}

EEPROMTool::~EEPROMTool()
{
    delete ui;
}
