#include "Widgets/MainWindow/MainWindow.h"

#include <QApplication>

#ifdef ENV_WIN32
#pragma comment( lib, "Winmm" )
#pragma comment(lib, "ws2_32.lib")
#endif

#ifdef ENV_LINUX
#include <sched.h>
#endif

int main(int argc, char *argv[])
{
    // qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    QApplication a(argc, argv);
    QThread::currentThread()->setPriority(QThread::HighestPriority);
#ifdef ENV_LINUX
    struct sched_param param;
    param.sched_priority = 99;
    if(sched_setscheduler(0, SCHED_RR, &param) != 0)
    {
        qDebugMessage("Error on set Round-Robin Scheduler");
    }
#endif
    // setvbuf(stdout, NULL, _IONBF, 0);
    MainWindow w;
    w.show();
    return a.exec();
}
