#ifndef ECATSLAVE_H
#define ECATSLAVE_H

#ifdef ENV_WIN32
#include <inttypes.h>
#else
#include "stdint.h"
#endif
#include "ethercat.h"
#include "utypes.h"
#include <QString>

class ECATSlave
{
public:
    ECATSlave(uint16_t id);
    /* EtherCAT Protocol-defined Area */
    const uint16_t slave_id = 0;
    ec_slavet *slave_t = nullptr;
    uint32_t serial_number = 0;
    QString name;
    /* PDO Mapped Area */
    slave_inputs_t* input = nullptr;
    slave_outputs_t* output = nullptr;
    /* User-defined Area */
    int last_motor_count = 0;
};

#endif // ECATSLAVE_H
