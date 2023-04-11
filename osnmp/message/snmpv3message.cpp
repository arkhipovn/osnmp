#include "snmpv3message.h"
#include "osnmp/agent/strategy/abstractagentstrategy.h"
#include "osnmp/mib/mib.h"
#include "osnmp/conf/snmpconf.h"

osnmp::SnmpV3Message::SnmpV3Message() : SnmpMessage(SNMP3)
{

}

osnmp::Pdu &osnmp::SnmpV3Message::pdu()
{
    return scopedPdu_.pdu();
}

osnmp::Pdu osnmp::SnmpV3Message::pdu() const
{
    return scopedPdu_.pdu();
}

void osnmp::SnmpV3Message::setPdu(const Pdu &pdu)
{
    scopedPdu_.setPdu(pdu);
}

osnmp::ScopedPdu &osnmp::SnmpV3Message::scopedPdu()
{
    return scopedPdu_;
}

osnmp::ScopedPdu osnmp::SnmpV3Message::scopedPdu() const
{
    return scopedPdu_;
}

void osnmp::SnmpV3Message::setScopedPdu(const ScopedPdu &scopedPdu)
{
    scopedPdu_ = scopedPdu;
}

osnmp::SecurityParameters &osnmp::SnmpV3Message::secParameters()
{
    return secParameters_;
}

osnmp::SecurityParameters osnmp::SnmpV3Message::secParameters() const
{
    return secParameters_;
}

void osnmp::SnmpV3Message::setSecParameters(const SecurityParameters &secParameters)
{
    secParameters_ = secParameters;
}

osnmp::HeaderData &osnmp::SnmpV3Message::headerData()
{
    return headerData_;
}

osnmp::HeaderData osnmp::SnmpV3Message::headerData() const
{
    return headerData_;
}

void osnmp::SnmpV3Message::setHeaderData(const HeaderData &headerData)
{
    headerData_ = headerData;
}

void osnmp::SnmpV3Message::decode(const osnmp::RawData &data, bool *isok)
{
    bool temp = osnmp::decode(data, *this);
    if(isok != nullptr) *isok = temp;
}

osnmp::RawData osnmp::SnmpV3Message::encode(bool *isok) const
{
    RawData data;
    bool temp = osnmp::encode(*this, data);
    if(isok != nullptr) *isok = temp;
    return data;
}

bool osnmp::SnmpV3Message::makeResponse(osnmp::SnmpConf *conf, osnmp::MibModule *mib)
{
    Q_UNUSED(conf)
    Q_UNUSED(mib)
    return false;
}

// ________________________

bool osnmp::decode(const RawData &data, SnmpV3Message &value)
{
    Q_UNUSED(data)
    Q_UNUSED(value)

    if(data.isEmpty() || data.isPrimitive()) return false;
    auto content = static_cast<ConstructedContent*>(data.content());

    if(content->size() != 4) return false;

    int ver;
    if(!decode(content->at(0), ver)) return false;
    value.setVersion(ver);

    HeaderData headerData;
    if(!decode(content->at(1), headerData)) return false;
    value.setHeaderData(headerData);

    SecurityParameters securityParameters;
    if(!decode(content->at(2), securityParameters)) return false;
    value.setSecParameters(securityParameters);

    ScopedPdu scopedPdu;
    if(!decode(content->at(3), scopedPdu)) return false;
    value.setScopedPdu(scopedPdu);

    return true;
}

bool osnmp::encode(const SnmpV3Message &value, RawData &data)
{
    auto content = new ConstructedContent({
                                              value.version(),
                                              value.headerData(),
                                              value.secParameters(),
                                              value.scopedPdu()
                                          });
    data = std::move(RawData(Identifier(UniversalTagNumber::Sequence, TagType::Constructor), content));
    return true;
}
