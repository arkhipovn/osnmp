#ifndef VARIANT_H
#define VARIANT_H

#include "osnmp/rawdata/rawdata.h"
#include "osnmp/types/all.h"

namespace osnmp {
    using Variant = RawData;
    template <typename T> inline T var_cast(const osnmp::Variant &var) {
        return raw_cast<T>(var);
    }
}

#endif // VARIANT_H
