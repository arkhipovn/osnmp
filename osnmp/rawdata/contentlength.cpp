#include "contentlength.h"
#include <math.h>

osnmp::ContentLength &osnmp::ContentLength::operator =(const ContentLength &len)
{
    if(this == &len) return *this;
    form_ = len.form_;
    length_ = len.length_;
    return *this;
}

osnmp::ParseResult osnmp::ContentLength::decode(QDataStream &stream)
{
    length_ = 0;

    if(stream.atEnd()) return { ParseError::WrongLenDecoded, 0 };

    quint8 byte;
    stream >> byte;
    size_t bytesDecoded = 1;

    if(byte == kIndefiniteFormLength) {      // не определенный размер
        form_ = ContentLength::IndefiniteForm;
        return { ParseError::NoError, bytesDecoded };
    }

    form_ = ContentLength::DefiniteForm;

    if((byte & kBit8) == 0) {      // длина занимает 1 байт
        length_ = byte;
        return { ParseError::NoError, bytesDecoded };
    }

    // длина занимает больше 1 байта
    const quint8 numberOfBytes = byte & kLowBits;   // определили сколько байт занимает длина
    if(numberOfBytes == 0) return { ParseError::WrongLenDecoded, bytesDecoded };

    for(int i = 1; i < numberOfBytes + 1; ++i) {
        if(stream.atEnd()) return { ParseError::WrongLenDecoded, bytesDecoded };

        stream >> byte;
        ++bytesDecoded;

        length_ <<= 8;
        length_ |= byte;
    }

    return { ParseError::NoError, bytesDecoded };
}

osnmp::ParseError osnmp::ContentLength::encode(QDataStream &stream) const
{
    if(form_ == IndefiniteForm) {
        // Неопределенная форма
        stream << kIndefiniteFormLength;
        return ParseError::NoError;
    }

    if(length_ < kIndefiniteFormLength) {
        // Короткая форма. Значит 1 байт занимает
        stream << static_cast<quint8>(length_ & 0xFF);
    }
    else {
        // Длинная форма записи

        // Будем искать первое не нулевое число
        int b = std::ceil(std::log2(length_) / 8.);//std::ceil(length_ / static_cast<double>(0xFF));

        // Запишем кол-во байтов и 1 в старшем бите
        stream << static_cast<quint8>(b | kBit8);

        for(int i = b - 1; i >= 0; --i) {
            stream << static_cast<quint8>((length_ >> (i * 8)) & 0xFF);
        }
    }

    return ParseError::NoError;
}

