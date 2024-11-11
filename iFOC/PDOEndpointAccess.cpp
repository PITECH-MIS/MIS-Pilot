#include "PDOEndpointAccess.h"

uint16_t getTrimmedSN(uint32_t SN)
{
    return (uint16_t)(SN % 10000);
}

PDOEndpointAccess* PDOEndpointAccess::getInstance()
{
    static PDOEndpointAccess access;
    return &access;
}

void PDOEndpointAccess::SendEndpoint(const uint32_t SN, const PROTOCOL_ENDPOINT ep, const float value)
{
    auto sn = getTrimmedSN(SN);
    protocol->print(0, "slave %d %d %.3f", sn, (uint16_t)ep, value);
}

void PDOEndpointAccess::TriggerIndicator(const uint32_t SN)
{
    SendEndpoint(SN, INDICATOR_TOGGLE, 1.0f);
}

void PDOEndpointAccess::SetHome(const uint32_t SN)
{
    SendEndpoint(SN, SET_HOME, 1.0f);
}
