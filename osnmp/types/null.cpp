#include "null.h"

bool osnmp::decode(const RawData &data, Null &value)
{
    Q_UNUSED(value)
    return data.isNull();
}

bool osnmp::encode(Null value, RawData &data)
{
    Q_UNUSED(value)
    data = std::move(RawData(Identifier(UniversalTagNumber::Null)));
    return true;
}

bool osnmp::encode(std::nullptr_t value, RawData &data)
{    
    return encode(Null(value), data);
}


