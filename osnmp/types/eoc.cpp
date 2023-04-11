#include "eoc.h"

bool osnmp::decode(const RawData &data, EOC &value)
{
    Q_UNUSED(value)
    return data.isEOC();
}

bool osnmp::encode(EOC value, RawData &data)
{
    Q_UNUSED(value)
    data = std::move(RawData(Identifier(UniversalTagNumber::EOC)));
    return true;
}


