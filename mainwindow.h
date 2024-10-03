#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDateTime>
#include <QDebug>
#include <QCloseEvent>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QStandardItemModel>

#include "ecatwrapper.h"
#include "controllerwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *e);
    ECATWrapper wrapper;
private slots:
    void onClickConnect(void);
    void updateEthList(void);
    void onErrorMsg(QString s);
    void onInfoMsg(QString s);
    void onDebugMsg(QString s);
    void onECATStateChanged();
    void onSelectXMLPath();
    void onParseXML();
    void onEnableController();
    void onDisableController();
private:
    void ParseStateViewModel();
    void AppendDescToItem(QString desc, int row, QStandardItem *parent);
    QStandardItemModel *stateViewModel;
    Ui::MainWindow *ui;
    ControllerWindow *controllerWindow = nullptr;
    QLabel *statusBarStateLabel;
    QLabel *statusBarWkcLabel;
    QLabel *statusBarSlaveCountLabel;
    QMap<QString, QString> ethInfo;
    QMap<QString, QJoystickDevice*> joysticks;
    QString currentPath;
    QString configXMLPath;
};
#endif // MAINWINDOW_H
