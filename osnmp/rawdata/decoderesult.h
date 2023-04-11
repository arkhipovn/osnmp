#ifndef DECODERESULT_H
#define DECODERESULT_H

#include "constants.h"

namespace osnmp {
    enum class ParseError : quint8 {
        NoError = 0,

        WrongLen,      // Что-то не так с длиной объекта
        WrongType,     // Что-то не так с типом

        WrongLenDecoded,    // Не получилось декодировать длину
        WrongTypeDecoded    // Не получилось декодировать тип
    };

    struct ParseResult {
        ParseError error;
        size_t bytes;
        inline bool isValid() const { return error == ParseError::NoError; }
        inline ParseResult& operator += (const ParseResult &other) { bytes += other.bytes; return *this; }
    };
}

#endif // DECODERESULT_H
