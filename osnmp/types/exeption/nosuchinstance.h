#ifndef NOSUCHINSTANCE_H
#define NOSUCHINSTANCE_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
class NoSuchInstance {
public:
    constexpr NoSuchInstance() {}
};

bool decode(const RawData &data, NoSuchInstance &value);
bool encode(NoSuchInstance value, RawData &data);
}

#endif // NOSUCHINSTANCE_H
