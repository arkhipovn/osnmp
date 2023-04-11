#include "nosuchinstance.h"

bool osnmp::decode(const RawData &data, NoSuchInstance &value)
{
    Q_UNUSED(value)
    return data.identifier() == osnmp::kNoSuchInstanceId;
}

bool osnmp::encode(NoSuchInstance value, RawData &data)
{
    Q_UNUSED(value)
    data = std::move(RawData(osnmp::kNoSuchInstanceId));
    return true;
}
