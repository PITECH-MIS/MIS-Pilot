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

private:
    Ui::PayloadDebugger *ui;
    ECATWrapper* wrapper;
};

#endif // PAYLOADDEBUGGER_H
