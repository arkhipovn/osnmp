#include "scopedpdu.h"

osnmp::ScopedPdu::ScopedPdu()
{

}

QByteArray osnmp::ScopedPdu::contextEngineID() const
{
    return contextEngineID_;
}

void osnmp::ScopedPdu::setContextEngineID(const QByteArray &contextEngineID)
{
    contextEngineID_ = contextEngineID;
}

QByteArray osnmp::ScopedPdu::contextName() const
{
    return contextName_;
}

void osnmp::ScopedPdu::setContextName(const QByteArray &contextName)
{
    contextName_ = contextName;
}

osnmp::Pdu &osnmp::ScopedPdu::pdu()
{
    return pdu_;
}

osnmp::Pdu osnmp::ScopedPdu::pdu() const
{
    return pdu_;
}

void osnmp::ScopedPdu::setPdu(const Pdu &pdu)
{
    pdu_ = pdu;
}

bool osnmp::decode(const RawData &data, ScopedPdu &value)
{
    if(data.isEmpty() || data.isPrimitive()) return false;

    if(data.identifier() != UniversalTagNumber::Sequence) return false;
    auto content = static_cast<ConstructedContent*>(data.content());
    if(content->size() != 3) return false;

    OctetString contextEngineID;
    if(!decode(content->at(0), contextEngineID)) return false;
    value.setContextEngineID(contextEngineID);

    OctetString contextName;
    if(!decode(content->at(1), contextName)) return false;
    value.setContextName(contextName);

    Pdu pdu;
    if(!decode(content->at(2), pdu)) return false;
    value.setPdu(pdu);

    return true;
}

bool osnmp::encode(const ScopedPdu &value, RawData &data)
{
    auto content = new ConstructedContent({
                                              value.contextEngineID(),
                                              value.contextName(),
                                              value.pdu()
                                          });
    data = std::move(RawData(UniversalTagNumber::Sequence, content));
    return true;
}
