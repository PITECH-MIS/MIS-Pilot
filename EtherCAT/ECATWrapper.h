#ifndef ECATWRAPPER_H
#define ECATWRAPPER_H

#include <QThread>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QFile>
#include <QtXml/QDomDocument>

#include "ECATSlave.h"
#include "PDOMasterProtocol.h"
#include "utils.h"

class ECATWrapper : public QObject
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
    QHash<uint16_t, ECATSlave*> slaves;
    // Singleton.
    static ECATWrapper *getInstance();
    PDOMasterProtocol pdoProtocol;
    ECATWrapper(const ECATWrapper &) = delete;
    ECATWrapper &operator=(const ECATWrapper &) = delete;
    int printErrorStack();
signals:
    void debugMessage(QString msg);
    void infoMessage(QString msg);
    void errorMessage(QString msg);
    void onStateChanged();
private slots:
    void pdoWorkerLoop();
    void checkStateLoop(); // watchdog
private:
    static int PO2SOconfigCb(uint16_t slave);
    void run();
    QString ethName;
    QTimer *pdoTimer = nullptr;
    QThread *pdoThread = nullptr;
    QTimer *checkStateTimer = nullptr;
    QThread *checkStateThread = nullptr;
    QTimer *pdoProtocolTimer = nullptr;
    QThread *pdoProtocolThread = nullptr;
    char IOMap[4096] = {0};
    int realWKC = 0;
    int expectedWKC = 0;
    uint16_t expectedState = EC_STATE_NONE;
    uint32_t oloop = 0;
    uint32_t iloop = 0;
};

#endif // ECATWRAPPER_H
