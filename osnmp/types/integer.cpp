#include "integer.h"
#include "private/decoder.h"

bool osnmp::decode(const RawData &data, int &value)
{
    return decodeInt(data, value);
}

bool osnmp::encode(int value, RawData &data)
{
    return encodeInt(UniversalTagNumber::Integer, value, data);
}
