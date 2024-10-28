#ifndef PDOMASTERPROTOCOL_H
#define PDOMASTERPROTOCOL_H

#include "stdint.h"
#include "cstring"
#include "ECATSlave.h"
#include <QSet>
#include <QObject>

#define BUFFER_LEN 48
#define PACKET_HEADER 0x52

class pdo_protocol_buf_t
{
public:
    uint8_t header = 0;
    uint8_t packet_id = 0;
    uint8_t sender_id = 0;
    uint8_t receiver_id = 0;
    char payload[BUFFER_LEN - 8];
    uint8_t crc16_msb = 0;
    uint8_t crc16_lsb = 0;
    bool operator==(const pdo_protocol_buf_t& rhs) const
    {
        if(header != rhs.header || packet_id != rhs.packet_id) return false;
        return memcmp(&sender_id, &rhs.sender_id, sizeof(pdo_protocol_buf_t) - 2) == 0;
    }
    bool operator!=(const pdo_protocol_buf_t& rhs) const
    {
        return !(*this == rhs);
    }
    bool isValid()
    {
        if(header != PACKET_HEADER) return false;
        uint16_t crc16 = (crc16_msb << 8) | crc16_lsb;
        if(crc16 == getCRC16(&header, sizeof(pdo_protocol_buf_t) - 2)) return true;
        return false;
    }
    void prepare()
    {
        header = PACKET_HEADER;
        uint16_t crc16 = getCRC16(&header, sizeof(pdo_protocol_buf_t) - 2);
        crc16_msb = (uint8_t)(crc16 >> 8);
        crc16_lsb = (uint8_t)crc16;
    }
private:
    uint16_t getCRC16(uint8_t *data, uint16_t len);
};

class pdo_slave_t
{
public:
    pdo_slave_t(ECATSlave* ptr) : inst(ptr) {};
    ECATSlave* inst;
    pdo_protocol_buf_t rx_buf; // Slave-to-Master
    pdo_protocol_buf_t tx_buf; // Master-to-Slave
};

class PDOMasterProtocol : public QObject
{
    Q_OBJECT
public:
    PDOMasterProtocol();
    void parseSlaves(QHash<uint16_t, ECATSlave*>& slaves);
public slots:
    void onPDOLoop();
    bool sendPayload(uint16_t slave_id, char *payload, uint8_t len);
signals:
    void receivePayload(uint16_t slave_id, char *payload, uint8_t len);
private:
    QHash<uint16_t, pdo_slave_t*> pdoSlaves;
};

#endif // PDOMASTERPROTOCOL_H
