#include "MainWindow.h"

#include <QApplication>

#pragma comment( lib, "Winmm" )
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[])
{
    // qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
    QApplication a(argc, argv);
    // setvbuf(stdout, NULL, _IONBF, 0);
    MainWindow w;
    w.show();
    return a.exec();
}
