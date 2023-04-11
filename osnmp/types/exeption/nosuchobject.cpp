#include "nosuchobject.h"

bool osnmp::decode(const RawData &data, NoSuchObject &value)
{
    Q_UNUSED(value)
    return data.identifier() == osnmp::kNoSuchObjectId;
}

bool osnmp::encode(NoSuchObject value, RawData &data)
{
    Q_UNUSED(value)
    data = std::move(RawData(osnmp::kNoSuchObjectId));
    return true;
}
