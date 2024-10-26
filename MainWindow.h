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

#include "ECATWrapper.h"
#include "RobotArm/RobotArmWrapper.h"
#include "ControllerWindow.h"
#include "TrayIcon.h"

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
    ECATWrapper* wrapper;
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
    void onTextBrowserCustomContextMenu(const QPoint &pos);
private:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
    void ParseStateViewModel();
    void AppendDescToItem(QString desc, int row, QStandardItem *parent);
    TrayIcon *tray;
    QStandardItemModel *stateViewModel;
    Ui::MainWindow *ui;
    ControllerWindow *controllerWindow = nullptr;
    RobotArmWrapper *robotArm;
    QLabel *statusBarStateLabel;
    QLabel *statusBarWkcLabel;
    QLabel *statusBarSlaveCountLabel;
    QMap<QString, QString> ethInfo;
    QMap<QString, QJoystickDevice*> joysticks;
    QString currentPath;
    QString configXMLPath;
};
#endif // MAINWINDOW_H
