#include "messagefactory.h"

#include "snmpv1message.h"
#include "snmpv2message.h"
#include "snmpv3message.h"

osnmp::SnmpMessage *osnmp::MessageFactory::create(const osnmp::RawData &data)
{
    if(data.isNull()) return nullptr;

    int ver = getMsgVersion(data);
    auto *msg = MessageFactory::create(ver);

    if(msg == nullptr) return nullptr;

    bool isok;
    msg->decode(data, &isok);

    if(!isok) {
        delete msg;
        return nullptr;
    }

    return msg;
}

osnmp::SnmpMessage *osnmp::MessageFactory::create(int version)
{
    switch (version) {
        case Version::SNMP1:
            return new SnmpV1Message;
        case Version::SNMP2c:
        case Version::SNMP2u:
            return new SnmpV2Message(version);
        case Version::SNMP3:
            return new SnmpV3Message;
        default:
            break;
    }
    return nullptr;
}

int osnmp::MessageFactory::getMsgVersion(const osnmp::RawData &data)
{
    // Нужно понять что за версия..
    if(data.isEmpty() || data.isPrimitive()) return false;
    auto content = static_cast<ConstructedContent*>(data.content());
    if(content->isEmpty()) return -1;
    int ver;
    if(!decode(content->at(0), ver)) return -1;
    return ver;
}
