#ifndef UTILS_H
#define UTILS_H

#include "qobject.h"
#include <QFuture>
#include <QtConcurrent>
#include <QApplication>
#include <QFont>

bool isUsableEth(QString& origin);

void qDebugMessage(QString msg);

void centerOnCursorScreen(QWidget* widget);

QString filterASCIIVisibleChar(char* origin, size_t max_len);

QString secondsToHHmmss(int time_sec);

QString getElidedString(const QString& origin, const QFont& font, int width);

template <class Function, class ...Args>
void spawnTask(Function &&f, Args &&...args)
{
    QFuture<void> future = QtConcurrent::run(std::forward<Function>(f),
                                             std::forward<Args>(args)...);
    while(!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 100);
}
#endif // UTILS_H
