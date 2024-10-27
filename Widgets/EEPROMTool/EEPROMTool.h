#ifndef EEPROMTOOL_H
#define EEPROMTOOL_H

#include <QWidget>

namespace Ui {
class EEPROMTool;
}

class EEPROMTool : public QWidget
{
    Q_OBJECT

public:
    explicit EEPROMTool(QWidget *parent = nullptr);
    ~EEPROMTool();

private:
    Ui::EEPROMTool *ui;
};

#endif // EEPROMTOOL_H
