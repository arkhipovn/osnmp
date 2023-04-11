#ifndef BERCONSTANTS_H
#define BERCONSTANTS_H

#include <QtGlobal>

namespace osnmp { // Basic Encoding Rules
    constexpr quint8 kIndefiniteFormLength   = 0x80; // Неопределнный размер длины
    constexpr quint8 kBit8                   = 0x80; // 1000 0000
    constexpr quint8 kLowBits                = quint8(~kBit8);    // 0x7F; // 0111 1111

    constexpr quint8 kTagNumberMask  = 0x1F; // маска для выделения номера тега

    constexpr quint8 kTagClassMask   = 0xC0; // маска для выделения класса
    constexpr quint8 kTagTypeMask    = 0x20; // для выделения типа
    constexpr quint8 kTagClassOffset = 6;    // смещение
    constexpr quint8 kTagTypeOffset  = 5;
}


#endif // BERCONSTANTS_H
