#ifndef PDOENDPOINTACCESS_H
#define PDOENDPOINTACCESS_H

#include "../EtherCAT/PDOMasterProtocol.h"
#include "endpoints_enum.h"

class PDOEndpointAccess
{
public:
    PDOEndpointAccess() = default;
    void SendEndpoint(const uint32_t SN, const PROTOCOL_ENDPOINT ep, const float value);
    void TriggerIndicator(const uint32_t SN);
    void SetHome(const uint32_t SN);
    // Singleton
    static PDOEndpointAccess *getInstance();
    PDOEndpointAccess(const PDOEndpointAccess &) = delete;
    PDOEndpointAccess &operator=(const PDOEndpointAccess &) = delete;
private:
    PDOMasterProtocol* protocol = PDOMasterProtocol::getInstance();
};

#endif // PDOENDPOINTACCESS_H
