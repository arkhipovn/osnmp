#ifndef MIBSEARCHRESULT_H
#define MIBSEARCHRESULT_H

#include "osnmp/oid.h"

namespace osnmp {
    class Mib;
    struct MibSearchResult {
        MibSearchResult() :  mib(nullptr), isCurrentPath(false), tableIndex("") {}
        MibSearchResult(Mib *mib, bool isCurrentPath, const OID &tableIndex = OID::Null) : mib(mib), isCurrentPath(isCurrentPath), tableIndex(tableIndex) {}

        bool isEmpty() const { return mib == nullptr; }

        Mib *mib;
        bool isCurrentPath; // был ли найден нужный нам путь
        OID tableIndex;
    };
}

#endif // MIBSEARCHRESULT_H
