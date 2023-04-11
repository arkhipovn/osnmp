#ifndef ASNUINTEGER_H
#define ASNUINTEGER_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    using Gauge = uint; // == UInteger
    using UInteger = uint;

    bool decode(const RawData &data, uint &value);
    bool encode(uint value, RawData &data);
}

#endif // ASNUINTEGER_H

