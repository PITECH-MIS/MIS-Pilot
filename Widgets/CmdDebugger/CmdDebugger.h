#ifndef CMDDEBUGGER_H
#define CMDDEBUGGER_H

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

#endif // CMDDEBUGGER_H
