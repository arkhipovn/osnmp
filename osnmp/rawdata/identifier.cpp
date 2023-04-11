#include "identifier.h"
#include <cmath>

osnmp::Identifier &osnmp::Identifier::operator =(const Identifier &id)
{
    if(this == &id) return *this;
    Identifier tmp(id);
    swap(tmp);
    return *this;
}

osnmp::Identifier &osnmp::Identifier::operator =(const UniversalTagNumber &tag)
{
    Identifier tmp(tag);
    swap(tmp);
    return *this;
}

void osnmp::Identifier::swap(Identifier &other)
{
    std::swap(c, other.c);
    std::swap(n, other.n);
    std::swap(t, other.t);
}

osnmp::ParseResult osnmp::Identifier::decode(QDataStream &stream)
{
    n = 0;
    if(stream.atEnd()) return { ParseError::WrongTypeDecoded, 0 };

    quint8 byte;
    stream >> byte;
    size_t bytesDecoded = 1;

    c = static_cast<TagClass>((byte & kTagClassMask) >> kTagClassOffset);
    t = static_cast<TagType>((byte & kTagTypeMask) >> kTagTypeOffset);

    // 5 первых бит посмотрим
    if((byte & kTagNumberMask) == kTagNumberMask) {
        // Если все единицы. Значит идентификатор состоит из нескольких байт

        while(!stream.atEnd()) {
            stream >> byte;
            ++bytesDecoded;

            n += byte & kLowBits;

            if((byte & kBit8) == 0)
                return { ParseError::NoError, bytesDecoded };

            n <<= 7;
        }
    }
    else {
        n = byte & kTagNumberMask;
        return { ParseError::NoError, bytesDecoded };
    }

    return { ParseError::WrongTypeDecoded, bytesDecoded };
}

osnmp::ParseError osnmp::Identifier::encode(QDataStream &stream) const
{
    quint8 tag = (static_cast<quint8>(c) << kTagClassOffset |     // add class
                  static_cast<quint8>(t) << kTagTypeOffset);      // add type

    if(n < kTagNumberMask) {
        // Короткая форма
        stream << static_cast<quint8>(tag | static_cast<quint8>(n));
    }
    else {
        // длинная форма записи
        stream << static_cast<quint8>(tag | kTagNumberMask);  // Заполним всеми единицами первый байт

        // Будем искать первое не нулевое число
        int b = std::ceil(n / static_cast<double>(kLowBits));
        for(int i = b - 1; i >= 0; --i) {
            quint8 byte = static_cast<quint8>((n >> (i * 7)) & kLowBits);
            if(i == 0) stream << byte;      // старший бит = 0
            else stream << static_cast<quint8>(byte | kBit8);   // старший бит = 1
        }
    }

    return ParseError::NoError;
}

QDebug operator <<(QDebug stream, const osnmp::Identifier &id)
{
    return stream << "Id:" << static_cast<quint8>(id.tagClass()) << static_cast<quint8>(id.type()) << static_cast<osnmp::TagNumber>(id.number());
}
