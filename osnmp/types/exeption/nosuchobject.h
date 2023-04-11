#ifndef NOSUCHOBJECT_H
#define NOSUCHOBJECT_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    class NoSuchObject {
    public:
        constexpr NoSuchObject() {}
    };

    bool decode(const RawData &data, NoSuchObject &value);
    bool encode(NoSuchObject value, RawData &data);
}

#endif // NOSUCHOBJECT_H
