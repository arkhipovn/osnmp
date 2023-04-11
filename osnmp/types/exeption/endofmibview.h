#ifndef ENDOFMIBVIEW_H
#define ENDOFMIBVIEW_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    class EndOfMibView {
    public:
        constexpr EndOfMibView() {}
    };

    bool decode(const RawData &data, EndOfMibView &value);
    bool encode(EndOfMibView value, RawData &data);
}

#endif // ENDOFMIBVIEW_H
