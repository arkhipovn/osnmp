#ifndef DECODER_H
#define DECODER_H

#include "osnmp/rawdata/rawdata.h"

template <typename T>
bool decodeUnsigned(const osnmp::RawData &data, T &value)
{
    value = 0;
    if(data.isEmpty() || !data.isPrimitive()) return false;

    auto content = static_cast<osnmp::PrimitiveContent*>(data.content());

    if(static_cast<size_t>(content->size()) > (sizeof(T) + 1))
        return false;

    if(content->isEmpty()) return false;

    for(int i = 0; i < content->size(); ++i) {
        value <<= 8;
        value |= static_cast <T>(static_cast<quint8>(content->at(i)));
    }

    return true;
}

template <typename T>
bool encodeUnsigned(const osnmp::Identifier &id, T value, osnmp::RawData &data)
{
    QByteArray content;

    content.prepend(quint8(value & 0xFF));
    value >>= 8;

    while(value != 0) {
        content.prepend(quint8(value & 0xFF));
        value >>= 8;
    }

    if(content.at(0) & osnmp::kBit8)
        //        content.prepend(quint8(0x0));
        content.prepend(char(0x0));   //@N 5.5 error

    data = osnmp::RawData(id, new osnmp::PrimitiveContent(content));
    return true;
}


template <typename T>
bool decodeInt(const osnmp::RawData &data, T &value)
{
    value = 0;
    if(data.isEmpty() || !data.isPrimitive()) return false;

    auto content = static_cast<osnmp::PrimitiveContent*>(data.content());

    if(static_cast<size_t>(content->size()) > (sizeof(T) + 1))
        return false;

    if(content->isEmpty()) return false;

    T bigValue = static_cast<quint8>(content->at(0)) & osnmp::kBit8;
    T smallValue = static_cast <T>(static_cast<quint8>(content->at(0)) & osnmp::kLowBits);

    for(int i = 1; i < content->size(); ++i) {
        bigValue <<= 8;
        smallValue <<= 8;
        smallValue |= static_cast <T>(static_cast<quint8>(content->at(i)));
    }

    value = smallValue - bigValue;
    return true;
}

template <typename T>
bool encodeInt(const osnmp::Identifier &id, T value, osnmp::RawData &data)
{
    QByteArray content;
    T testvalue = 0;
    if(value < 0) testvalue = -1;

    content.prepend(quint8(value & 0xFF));
    value >>= 8;

    while(value != testvalue) {
        content.prepend(quint8(value & 0xFF));
        value >>= 8;
    }

    if((content.at(0) & osnmp::kBit8) != (testvalue & osnmp::kBit8))
        content.prepend(quint8(testvalue & 0xFF));

    data = std::move(osnmp::RawData(id, new osnmp::PrimitiveContent(content)));
    return true;
}

#endif // DECODER_H
