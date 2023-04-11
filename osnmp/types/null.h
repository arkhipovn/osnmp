#ifndef ASNNULL_H
#define ASNNULL_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    class Null {
    public:
        constexpr Null() {}
        constexpr Null(std::nullptr_t) {}
        constexpr operator std::nullptr_t() { return nullptr; }
    };

    bool decode(const RawData &data, Null &value);
    bool encode(Null value, RawData &data);
    bool encode(std::nullptr_t value, RawData &data);
}

#endif // ASNNULL_H
