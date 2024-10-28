#ifndef PAYLOADDEBUGGER_H
#define PAYLOADDEBUGGER_H

#include <QWidget>

namespace Ui {
class CmdDebugger;
}

class CmdDebugger : public QWidget
{
    Q_OBJECT

public:
    explicit CmdDebugger(QWidget *parent = nullptr);
    ~CmdDebugger();

private:
    Ui::CmdDebugger *ui;
};

#endif // PAYLOADDEBUGGER_H
