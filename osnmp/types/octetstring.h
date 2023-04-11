#ifndef OCTETSTRING_H
#define OCTETSTRING_H

#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
    using OctetString = QByteArray;

    bool decode(const RawData &data, OctetString &value);
    bool encode(const OctetString &value, RawData &data);

    bool decode(const RawData &data, QString &value);
    bool encode(const QString &value, RawData &data);

    bool encode(const char *value, RawData &data);
}

#endif // OCTETSTRING_H
