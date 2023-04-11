#ifndef COUNTER64_H
#define COUNTER64_H

#include "private/auto_value.h"
#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    using Counter64 = auto_value <quint64>;

    bool decode(const RawData &data, Counter64 &value);
    bool encode(Counter64 value, RawData &data);
}

#endif // COUNTER64_H
