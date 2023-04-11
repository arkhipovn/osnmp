#include "endofmibview.h"

bool osnmp::decode(const RawData &data, EndOfMibView &value)
{
    Q_UNUSED(value)
    return data.identifier() == osnmp::kEndOfMibViewId;
}

bool osnmp::encode(EndOfMibView value, RawData &data)
{
    Q_UNUSED(value)
    data = std::move(RawData(osnmp::kEndOfMibViewId));
    return true;
}



//bool osnmp::encode(Null value, RawData &data)
//{
//    Q_UNUSED(value)
//    data = std::move(RawData(Identifier(UniversalTagNumber::Null)));
//    return true;
//}
