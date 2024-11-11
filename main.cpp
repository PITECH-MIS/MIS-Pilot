#include "Widgets/MainWindow/MainWindow.h"

#include <QApplication>
#include "RobotArm/RobotArmWrapper.h"

#ifdef ENV_WIN32
#pragma comment( lib, "Winmm" )
#pragma comment(lib, "ws2_32.lib")
#include "processthreadsapi.h"
#endif

#ifdef ENV_LINUX
#include <sched.h>
#endif

int main(int argc, char *argv[])
{
    // qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    QApplication a(argc, argv);
    QThread::currentThread()->setPriority(QThread::TimeCriticalPriority);
#ifdef ENV_WIN32
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#endif
#ifdef ENV_LINUX
    struct sched_param param;
    param.sched_priority = 20;
    if(sched_setscheduler(0, SCHED_RR, &param) != 0) qDebugMessage("Error on set RT Scheduler");
    // // Run Task in dedicated performace CPUs
    // cpu_set_t cpus;
    // CPU_ZERO(&cpus);
    // CPU_SET(0, &cpus);
    // CPU_SET(1, &cpus);
    // CPU_SET(2, &cpus);
    // CPU_SET(3, &cpus);
    // if(sched_setaffinity(0, sizeof(cpu_set_t), &cpus) != 0) qDebugMessage("Error on setaffinity");
#endif
    // setvbuf(stdout, NULL, _IONBF, 0);
    // RobotArmWrapper wrapper;
    // wrapper.init();
    MainWindow w;
    w.show();
    return a.exec();
}
