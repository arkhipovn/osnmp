#include "uinteger.h"
#include "private/decoder.h"

bool osnmp::decode(const RawData &data, uint &value)
{
    return decodeUnsigned(data, value);
}

bool osnmp::encode(uint value, RawData &data)
{
    return encodeUnsigned(osnmp::kUnsignedId, value, data);
}
