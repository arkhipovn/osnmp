#include "counter64.h"
#include "private/decoder.h"

bool osnmp::decode(const RawData &data, Counter64 &value)
{
    return decodeUnsigned(data, value);
}

bool osnmp::encode(Counter64 value, RawData &data)
{
    return encodeUnsigned(osnmp::kCounter64Id, value, data);
}

