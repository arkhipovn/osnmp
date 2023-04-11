#ifndef BITSTRING_H
#define BITSTRING_H

#include <QBitArray>
#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    using BitString = QBitArray;

    bool decode(const RawData &data, BitString &value);
    bool encode(const BitString &value, RawData &data);
}

#endif // BITSTRING_H

