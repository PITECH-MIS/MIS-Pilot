#include "RobotArmController.h"
#include "ui_RobotArmController.h"

RobotArmController::RobotArmController(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RobotArmController)
{
    ui->setupUi(this);
}

RobotArmController::~RobotArmController()
{
    delete ui;
}
