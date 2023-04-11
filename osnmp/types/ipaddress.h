#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <QHostAddress>
#include "osnmp/rawdata/rawdata.h"

namespace osnmp {
using IpAddress = QHostAddress;

bool decode(const RawData &data, QHostAddress &value);
bool encode(const QHostAddress &value, RawData &data);
}

#endif // IPADDRESS_H
