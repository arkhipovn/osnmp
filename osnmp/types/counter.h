#ifndef COUNTER_H
#define COUNTER_H

#include "private/auto_value.h"
#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    using Counter = auto_value <uint>;

    bool decode(const RawData &data, Counter &value);
    bool encode(Counter value, RawData &data);
}

#endif // COUNTER_H
