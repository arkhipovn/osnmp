#ifndef ASNEOC_H
#define ASNEOC_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    class EOC {
    public:
        constexpr EOC() {}

        static constexpr int length = 2;

        // Проверяет является ли строка bytes - EOC
        static bool isEOC(const char *bytes) {
            static const char eoc[2] = { 0x0, 0x0 };
            return strcmp(bytes, eoc) == 0;
        }
    };

    bool decode(const RawData &data, EOC &value);
    bool encode(EOC value, RawData &data);
}

#endif // ASNEOC_H

