#include "counter.h"
#include "private/decoder.h"

bool osnmp::decode(const RawData &data, Counter &value)
{
    return decodeUnsigned(data, value);
}

bool osnmp::encode(Counter value, RawData &data)
{
    return encodeUnsigned(osnmp::kCounterId, value, data);
}
