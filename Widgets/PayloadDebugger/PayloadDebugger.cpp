#include "PayloadDebugger.h"
#include "ui_PayloadDebugger.h"
#include "utils.h"

PayloadDebugger::PayloadDebugger(ECATWrapper *w, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PayloadDebugger), wrapper(w)
{
    ui->setupUi(this);
    if(wrapper->slaves.size() > 0)
    {
        ui->slaveIDComboBox->addItem("All");
        for(const auto &i : wrapper->slaves.keys()) ui->slaveIDComboBox->addItem(QString::number(i));
        ui->sendButton->setEnabled(true);
    }
    else ui->sendButton->setEnabled(false);
    connect(ui->sendButton, &QPushButton::clicked, this, [this](){
        uint16_t slave_id = ui->slaveIDComboBox->currentText() == "All" ? 0 : (uint16_t)ui->slaveIDComboBox->currentText().toInt();
        uint8_t len = ui->payloadEdit->toPlainText().size();
        if(len > sizeof(pdo_protocol_buf_t::payload)) len = sizeof(pdo_protocol_buf_t::payload);
        auto bArray = ui->payloadEdit->toPlainText().toUtf8().left(len);
        char *payload = bArray.data();
        wrapper->pdoProtocol.sendPayload(slave_id, payload, len);
        qDebugMessage(QString::asprintf("Send PDO Payload to slave #%d: ", slave_id) + QString::fromUtf8(payload));
    });
}

PayloadDebugger::~PayloadDebugger()
{
    delete ui;
}
