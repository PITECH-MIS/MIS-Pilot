#ifndef PAYLOADDEBUGGER_H
#define PAYLOADDEBUGGER_H

#include <QWidget>
#include "../../EtherCAT/ECATWrapper.h"

namespace Ui {
class PayloadDebugger;
}

class PayloadDebugger : public QWidget
{
    Q_OBJECT

public:
    explicit PayloadDebugger(ECATWrapper* w, QWidget *parent = nullptr);
    ~PayloadDebugger();
private slots:
    void receivePayload(uint16_t slave_id, char *payload, uint8_t len);
private:
    Ui::PayloadDebugger *ui;
    ECATWrapper* wrapper;
};

#endif // PAYLOADDEBUGGER_H
