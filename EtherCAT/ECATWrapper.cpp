#include "ECATWrapper.h"
#include <QSet>
#include <QDebug>

ECATWrapper::ECATWrapper()
{
    memset(IOMap, 0, sizeof(IOMap));
}
ECATWrapper::~ECATWrapper()
{
    qDebugMessage("ECATWrapper destroyed");
    if(pdoThread)
    {
        pdoThread->quit();
        pdoThread->wait();
    }
    if(checkStateThread)
    {
        checkStateThread->quit();
        checkStateThread->wait();
    }
}

ECATWrapper *ECATWrapper::getInstance()
{
    static ECATWrapper wrapper;
    return &wrapper;
}

void ECATWrapper::pdoWorkerLoop()
{
    emit onStateChanged(); // executed on GUI thread with Qt::QueuedConnection
    ec_send_processdata();
    realWKC = ec_receive_processdata(EC_TIMEOUTRET);
}

void ECATWrapper::initEth(QString& name)
{
    ethName = name;
    // start(QThread::TimeCriticalPriority);
    run();
}

/* The easy way of increasing the CoE throughput of SOEM is simply to spawn multiple tasks that each do their own CoE for a single slave.
 * SOEM is multi-tasking safe. But beware of the the buffer limit (MAXBUF) that is default 16. You could increase this when necessary.
 */

int ECATWrapper::PO2SOconfigCb(uint16_t slave)
{
    ECATWrapper* inst = ECATWrapper::getInstance();
    emit inst->infoMessage(QString::asprintf("Slave #%d PO2SOconfig() called", slave));
    int dummy = sizeof(uint32_t);
    ec_SDOread(slave, 0x1018, 0x04, false, &dummy, &inst->serial_number, EC_TIMEOUTRXM);
    emit inst->debugMessage(QString::asprintf("Slave #%d SN: %llu", slave, inst->serial_number));
    dummy = 16;
    char temp[16];
    ec_SDOread(slave, 0x1008, 0x00, false, &dummy, temp, EC_TIMEOUTRXM);
    inst->device_name = filterASCIIVisibleChar(temp, sizeof(temp));
    // emit inst->debugMessage(QString::asprintf("%s", temp));
    emit inst->debugMessage(inst->device_name);
    return 1;
}

void ECATWrapper::run()
{
    memset(IOMap, 0, sizeof(IOMap));
    if(pdoTimer == nullptr)
    {
        if(pdoThread == nullptr) pdoThread = new QThread(this);
        pdoTimer = new QTimer();
        pdoTimer->moveToThread(pdoThread);
        pdoTimer->setTimerType(Qt::PreciseTimer);
        pdoTimer->setInterval(1);
        connect(pdoTimer, &QTimer::timeout, this, &ECATWrapper::pdoWorkerLoop, Qt::DirectConnection);
        connect(pdoThread, &QThread::finished, pdoTimer, &QTimer::stop);
        connect(pdoThread, &QThread::started, pdoTimer, QOverload<>::of(&QTimer::start));
    }
    if(checkStateTimer == nullptr)
    {
        if(checkStateThread == nullptr) checkStateThread = new QThread(this);
        checkStateTimer = new QTimer();
        checkStateTimer->moveToThread(checkStateThread);
        checkStateTimer->setTimerType(Qt::PreciseTimer);
        checkStateTimer->setInterval(10);
        connect(checkStateTimer, &QTimer::timeout, this, &ECATWrapper::checkStateLoop, Qt::DirectConnection);
        connect(checkStateThread, &QThread::finished, checkStateTimer, &QTimer::stop);
        connect(checkStateThread, &QThread::started, checkStateTimer, QOverload<>::of(&QTimer::start));
    }
    QByteArray bArray = ethName.toLatin1();
    char *eth = bArray.data();
    if(ec_init(eth))
    {
        emit infoMessage("ec_init() succeeded");
        emit debugMessage(QString::asprintf("ifname desc is %s", eth));
        if(ec_config_init(FALSE) > 0)
        {
            emit infoMessage(QString::asprintf("Found %d EtherCAT slave(s)", ec_slavecount));
            emit debugMessage("Mapping slaves I/O");
            for(int i = 1; i <= ec_slavecount; i++) ec_slave[i].PO2SOconfig = PO2SOconfigCb;
            int used_mem = ec_config_map(&IOMap);
            emit debugMessage(QString::asprintf("IOMap used/total: %d/%d (%.0f%%)", used_mem, sizeof(IOMap), 100.0f * (float)(used_mem) / (float)(sizeof(IOMap))));
            ec_configdc();

            emit infoMessage("Slaves I/O mapped, changing state to SAFE_OP");
            expectedState = EC_STATE_SAFE_OP;
            ec_statecheck(0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);
            emit onStateChanged();
            for(int i = 1; i <= ec_slavecount; i++)
            {
                emit debugMessage(QString::asprintf("Slave #%d state: %d", i, ec_slave[i].state));
            }
            oloop = ec_slave[0].Obytes;
            if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
            iloop = ec_slave[0].Ibytes;
            if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
            emit debugMessage(QString::asprintf("oloop: %d, iloop: %d", oloop, iloop));
            emit debugMessage(QString::asprintf("Segments(%d): %d %d %d %d", ec_group[0].nsegments,
                                                                            ec_group[0].IOsegment[0],
                                                                            ec_group[0].IOsegment[1],
                                                                            ec_group[0].IOsegment[2],
                                                                            ec_group[0].IOsegment[3]));
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            emit infoMessage(QString::asprintf("Calculated workcounter(WKC): %d", expectedWKC));
            emit infoMessage("Requesting OP state");
            ec_slave[0].state = EC_STATE_OPERATIONAL;
            expectedState = EC_STATE_OPERATIONAL;
            // emit onStateChanged();
            // pdoWorkerLoop();
            pdoThread->start();
            ec_writestate(0);
            int timeout = 500;
            do
            {
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            }
            while(timeout-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            if(ec_slave[0].state != EC_STATE_OPERATIONAL)
            {
                emit errorMessage("Not all slaves are operational");
                ec_readstate();
                for(int i = 1; i<=ec_slavecount; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL) emit errorMessage(QString::asprintf("Slave %d not in OP mode", i));
                }
                closeConnection();
            }
            else
            {
                emit infoMessage("All slaves reached operational state");

                if(sizeof(slave_inputs_t) != iloop && (iloop % sizeof(slave_inputs_t) != 0))
                {
                    emit errorMessage(QString::asprintf("Size of inputs_t is not equal or common factor to iloop: %d/%d", sizeof(slave_inputs_t), iloop));
                }
                else
                {
                    for(int i = 1; i <= ec_slavecount; i++)
                    {
                        input_vector.append((slave_inputs_t*)ec_slave[i].inputs);
                    }
                    emit infoMessage("Slave inputs mapped successfully");
                }

                if(sizeof(slave_outputs_t) != oloop && (oloop % sizeof(slave_outputs_t) != 0))
                {
                    emit errorMessage(QString::asprintf("Size of outputs_t is not equal or common factor to oloop: %d/%d", sizeof(slave_outputs_t), oloop));
                }
                else
                {
                    for(int i = 1; i <= ec_slavecount; i++)
                    {
                        output_vector.append((slave_outputs_t*)ec_slave[i].outputs);
                    }
                    emit infoMessage("Slave outputs mapped successfully");
                }

                // checkStateTimer->start(10);
                checkStateThread->start();
                emit onStateChanged();
            }
        }
        else
        {
            emit errorMessage("No EtherCAT slave found\n");
            // closeConnection();
            ec_close();
            // quit();
        }
    }
    else
    {
        emit errorMessage("Error in ec_init(eth)\n");
        // closeConnection();
        ec_close();
        // quit();
    }
    // exec();
}

void ECATWrapper::closeConnection()
{
    emit infoMessage("Closing connection, request INIT state for all slaves\n");
    ec_slave[0].state = EC_STATE_INIT;
    ec_writestate(0);
    expectedState = EC_STATE_INIT;
    ec_statecheck(0, EC_STATE_INIT, EC_TIMEOUTRET3);
    realWKC = 0;
    emit onStateChanged();
    input_vector.clear();
    output_vector.clear();
    if(pdoThread)
    {
        pdoThread->quit();
        pdoThread->wait();
    }
    if(checkStateThread)
    {
        checkStateThread->quit();
        checkStateThread->wait();
    }
    ec_close();
}

void ECATWrapper::checkStateLoop()
{
    static QSet<int> lostSlaveSet;
    int currentgroup = 0; // TODO: for later use
    expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
    if(expectedState == EC_STATE_OPERATIONAL && ((realWKC < expectedWKC) || ec_group[currentgroup].docheckstate))
    {
        ec_group[currentgroup].docheckstate = FALSE;
        ec_readstate();
        for(int slave = 1; slave <= ec_slavecount; slave++)
        {
            if((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
            {
                ec_group[currentgroup].docheckstate = TRUE;
                if(ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                {
                    emit errorMessage(QString::asprintf("Slave #%d is in SAFEOP + ERROR state, attempting ACK", slave));
                    ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                    ec_writestate(slave);
                }
                else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                {
                    emit infoMessage(QString::asprintf("Slave #%d is in SAFEOP state, returning to OP", slave));
                    ec_slave[slave].state = EC_STATE_OPERATIONAL;
                    ec_writestate(slave);
                }
                else if(ec_slave[slave].state > EC_STATE_NONE)
                {
                    if(ec_reconfig_slave(slave, EC_TIMEOUTMON))
                    {
                        ec_slave[slave].islost = FALSE;
                        emit infoMessage(QString::asprintf("Slave #%d reconfigured", slave));
                    }
                }
                else if(!ec_slave[slave].islost)
                {
                    ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                    if(ec_slave[slave].state == EC_STATE_NONE)
                    {
                        ec_slave[slave].islost = TRUE;
                        lostSlaveSet.insert(slave);
                        emit errorMessage(QString::asprintf("Slave #%d lost (lost:%d/%d)", slave, lostSlaveSet.size(), ec_slavecount));
                        // if(lostSlaveSet.size() >= ec_slavecount)
                        // {
                        //     emit errorMessage("All slave(s) lost, resetting state to INIT");
                        //     lostSlaveSet.clear();
                        //     closeConnection();
                        //     break;
                        // }
                    }
                    else lostSlaveSet.clear();
                }
            }
            if(ec_slave[slave].islost)
            {
                if(ec_slave[slave].state == EC_STATE_NONE || ec_slave[slave].state == EC_STATE_INIT)
                {
                    if(ec_recover_slave(slave, EC_TIMEOUTMON))
                    {
                        ec_slave[slave].islost = FALSE;
                        emit infoMessage(QString::asprintf("Slave #%d recovered", slave));
                        if(lostSlaveSet.contains(slave)) lostSlaveSet.remove(slave);
                    }
                    // else
                    // {
                    //     emit debugMessage(QString::asprintf("Trying to recover slave #%d", slave));
                    // }
                }
                else
                {
                    ec_slave[slave].islost = FALSE;
                    emit infoMessage(QString::asprintf("Slave #%d found with state: %d", slave, ec_slave[slave].state));
                }
            }
        }
        if(!ec_group[currentgroup].docheckstate) emit infoMessage("All slaves resumed OP state");
    }
    else lostSlaveSet.clear();
}

bool ECATWrapper::parseSlaveXML(QFile &xmlFile)
{
    if(!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        emit errorMessage("Failed to open XML file: " + xmlFile.fileName());
        return false;
    }
    QDomDocument doc;
    if(!doc.setContent(&xmlFile))
    {
        emit errorMessage("Failed to parse XML file: "+xmlFile.fileName());
        xmlFile.close();
        return false;
    }
    QDomElement root = doc.documentElement();
    QDomNodeList lst = root.childNodes();
    for(int i = 0; i < lst.size(); i++)
    {
        QDomNode node = lst.at(i);
        if(node.isElement())
        {
            QDomElement e = node.toElement();
            emit infoMessage(e.nodeName());
        }
    }
    xmlFile.close();
    return true;
}

int ECATWrapper::getRealWKC()
{
    return realWKC;
}

int ECATWrapper::getExpectedWKC()
{
    return expectedWKC;
}

uint16_t ECATWrapper::getExpectedState()
{
    return expectedState;
}

QString ECATWrapper::getExpectedStateName()
{
    QString ret = "NONE";
    switch(expectedState)
    {
    case EC_STATE_INIT:
        ret = "INIT";
        break;
    case EC_STATE_PRE_OP:
        ret = "PREOP";
        break;
    case EC_STATE_BOOT:
        ret = "BOOT";
        break;
    case EC_STATE_SAFE_OP:
        ret = "SAFEOP";
        break;
    case EC_STATE_OPERATIONAL:
        ret = "OP";
        break;
    case EC_STATE_ERROR:
        ret = "ERROR";
        break;
    default: break;
    }
    return ret;
}

int ECATWrapper::getSlaveCount()
{
    return ec_slavecount;
}

size_t ECATWrapper::getEthInfo(QMap<QString, QString>& dest)
{
    dest.clear();
    ec_adaptert *adapter = ec_find_adapters();
    while(adapter != NULL)
    {
        QString desc = QString::fromLocal8Bit(adapter->desc, -1);
        if(isUsableEth(desc))
        {
            dest.insert(desc, adapter->name);
        }
        adapter = adapter->next;
    }
    delete adapter;
    return dest.size();
}
