#ifndef ASNINTEGER_H
#define ASNINTEGER_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    using Integer = int;

    bool decode(const RawData &data, int &value);
    bool encode(int value, RawData &data);
}

#endif // ASNINTEGER_H

