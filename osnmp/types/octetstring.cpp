#include "octetstring.h"

osnmp::OctetString encodeByteArray(const osnmp::RawData &raw)
{
    using namespace osnmp;
    OctetString value = "";
    if(raw.isPrimitive())
        value = *static_cast<PrimitiveContent*>(raw.content());
    else {
        auto content = static_cast<ConstructedContent*>(raw.content());
        for(int i = 0; i < content->size(); ++i)
            value.append(encodeByteArray(content->at(i)));
    }
    return value;
}

bool osnmp::decode(const RawData &data, OctetString &value)
{
    value.clear();
    if(data.identifier() != UniversalTagNumber::OctetString) return false;
    if(data.isEmpty()) return false;
    value = encodeByteArray(data);
    return true;
}

bool osnmp::encode(const OctetString &value, RawData &data)
{
    data = std::move(RawData(UniversalTagNumber::OctetString, new PrimitiveContent(value)));
    return true;
}

bool osnmp::decode(const RawData &data, QString &value)
{
    value.clear();
    if(data.identifier() != UniversalTagNumber::OctetString) return false;
    if(data.isEmpty()) return false;
    value = encodeByteArray(data);
    return true;
}

bool osnmp::encode(const QString &value, RawData &data)
{
    data = std::move(RawData(UniversalTagNumber::OctetString, new PrimitiveContent(value.toLatin1())));
    return true;
}

bool osnmp::encode(const char *value, RawData &data)
{
    data = std::move(RawData(UniversalTagNumber::OctetString, new PrimitiveContent(QByteArray(value))));
    return true;
}
