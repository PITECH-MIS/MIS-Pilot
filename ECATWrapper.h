#ifndef ECATWRAPPER_H
#define ECATWRAPPER_H

#include <QThread>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QtXml/QDomDocument>

#include <inttypes.h>
#include "ethercat.h"
#include "utypes.h"
#include "utils.h"
#include <QMutex>
#include <QMutexLocker>

class ECATWrapper : public QThread
{
    Q_OBJECT
public:
    ECATWrapper();
    ~ECATWrapper();
    bool parseSlaveXML(QFile &xmlFile);
    void initEth(QString& name);
    void closeConnection();
    size_t getEthInfo(QMap<QString, QString>& dest);
    int getRealWKC();
    int getExpectedWKC();
    uint16_t getExpectedState();
    QString getExpectedStateName();
    int getSlaveCount();
    QVector<slave_inputs_t*> input_vector;
    QVector<slave_outputs_t*> output_vector;
signals:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);
    void onStateChanged();
private slots:
    void pdoWorkerLoop();
    void checkStateLoop(); // watchdog
private:
    void run();
    QString ethName;
    QTimer *pdoTimer = nullptr;
    QTimer *checkStateTimer = nullptr;
    char IOMap[4096] = {0};
    int realWKC = 0;
    int expectedWKC = 0;
    uint16_t expectedState = EC_STATE_NONE;
    uint32_t oloop = 0;
    uint32_t iloop = 0;
};

#endif // ECATWRAPPER_H
