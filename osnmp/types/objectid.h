#ifndef ASNOBJECTID_H
#define ASNOBJECTID_H

#include "osnmp/rawdata/rawdata.h"
#include "osnmp/oid.h"

namespace osnmp {
    using ObjectId = OID;
    bool decode(const RawData &data, ObjectId &value);
    bool encode(const ObjectId &value, RawData &data);
}

#endif // ASNOBJECTID_H
