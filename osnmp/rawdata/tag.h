#ifndef TAG_H
#define TAG_H

#include <QtGlobal>

namespace osnmp { // Basic Encoding Rules
    using Tag = quint64;        // В сташем байте содержится TagClass|TagType в остальных TagNumber
    using TagNumber = quint64;  // Биты за исключением первых трех

    enum class TagClass : quint8 {
        UniversalClass          = 0x0,      // Типы, которые определены только в X.690 и имеют одинаковый смысл во всех приложениях
        ApplicationClass        = 0x1,      // Типы, смысл которых меняется в зависимости от приложения
        ContextSpecificClass    = 0x2,      // Типы, смысл которых зависит от данного составного типа
        PrivateClass            = 0x3       // Типы, смысл которых зависит от конкретной организации
    };

    // Элемент данных. Бит 6 в идентификаторе
    enum class TagType : quint8 {   // P/C
        Primitive        = 0x0,     // Простой (Int..)
        Constructor      = 0x1      // Могут содержать в себе другие различные наборы данных
    };

    // Стандартные Primitive типы класса ApplicationClass
    enum class UniversalTagNumber : TagNumber {
        EOC          = 0x0,      // End-of-Content
        // ...
        Integer      = 0x2,
        BitString    = 0x3,
        OctetString  = 0x4,
        Null         = 0x5,
        ObjectId     = 0x6,
        // ...
        Sequence     = 0x10,
        SequenceOf   = Sequence
        // ...
        // Другие нет смысла сейчас реализовывать. Их в SNMP все равно нет
    };
}

#endif // TAG_H
