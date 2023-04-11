#include "bitstring.h"
#include <math.h>

bool osnmp::decode(const RawData &data, BitString &value)
{
    //@N переделать функцию..
    value.clear();
    if(data.identifier() != UniversalTagNumber::BitString) return false;
    if(data.isEmpty() || !data.isPrimitive()) return false;

    QByteArray content = *static_cast<PrimitiveContent*>(data.content());

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    value = QBitArray::fromBits(content, content.size() * 8);
#else
    value.resize(content.size() * 8);
    for(int i = 0; i < content.size(); ++i) {
        auto byte = content.at(i);
        for(int j = 0; j < 8; ++j) {
            value.setBit(i * 8 + j, static_cast<bool>((byte >> j) & 1));
        }
    }
#endif
    return true;
}

bool osnmp::encode(const BitString &value, RawData &data)
{
    auto bitArray = value;
    bitArray.resize(std::ceil(value.size() / 8.) * 8);

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    auto byts = QByteArray::fromRawData(bitArray.bits(), bitArray.size() / 8);
#else
    QByteArray byts;
    for(int i = 0; i < bitArray.size() / 8; ++i) {
        uchar byte = 0;
        for(int j = 0; j < 8; ++j) {
            byte |= (bitArray.at(i * 8 + j) << j);
        }
        byts.append(byte);
    }
#endif

    data = std::move(RawData(UniversalTagNumber::BitString, new PrimitiveContent(byts)));
    return true;
}

//@N добавить составной тип
