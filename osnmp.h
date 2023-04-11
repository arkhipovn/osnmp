#ifndef OSNMP_H
#define OSNMP_H

#include "osnmp/rawdata/identifier.h"
#include "osnmp/oid.h"

namespace osnmp {
    enum class ObjectAccess {
        NoAccess    = 0,
        ReadOnly    = 1,
        ReadWrite   = 2
    };

    enum Version {
        SNMP1   = 0,
        SNMP2c  = 1,
        SNMP2u  = 2,
        //        SNMP2p  = 129
        SNMP3   = 3
    };

    enum RequestStatus {
        RequestSuccess  = 0,
        RequestError    = 1,
        RequestTimeout  = 2
    };

    // ErrorStatus
    enum class ErrorCodes {
        NoError              = 0,    // Все в порядке
        TooBig               = 1,    // Объект не может уложить отклик в одно сообщение
        NoSuchName           = 2,    // В операции указана неизвестная переменная
        BadValue             = 3,    // В команде set использована недопустимая величина или неправильный синтаксис
        ReadOnly             = 4,    // Попытка изменить константу
        GenErr               = 5,    // Прочие ошибки

        NoAccess             = 6,
        WrongType            = 7,
        WrongLength          = 8,
        WrongEncoding        = 9,
        WrongValue           = 10,
        NoCreation           = 11,
        InconsistentValue    = 12,
        ReourceUnavailable   = 13,
        CommitFailed         = 14,
        UndoFailed           = 15,
        ErrAuthorization     = 16,
        NotWritale           = 17,

        InconsistentName     = 18
    };

    // External types id
    constexpr Identifier kIpAddressId       = Identifier(TagClass::ApplicationClass, TagType::Primitive, 0x0);         // 0x40
    constexpr Identifier kCounterId         = Identifier(TagClass::ApplicationClass, TagType::Primitive, 0x1);         // 0x41
    constexpr Identifier kUnsignedId        = Identifier(TagClass::ApplicationClass, TagType::Primitive, 0x2);         // 0x42
    constexpr Identifier kTimeTicksId       = Identifier(TagClass::ApplicationClass, TagType::Primitive, 0x3);         // 0x43
    constexpr Identifier kCounter64Id       = Identifier(TagClass::ApplicationClass, TagType::Primitive, 0x6);         // 0x46

    // ExceptionType types id
    constexpr Identifier kNoSuchObjectId    = Identifier(TagClass::ContextSpecificClass, TagType::Primitive, 0x0);     // 0x80
    constexpr Identifier kNoSuchInstanceId  = Identifier(TagClass::ContextSpecificClass, TagType::Primitive, 0x1);     // 0x81
    constexpr Identifier kEndOfMibViewId    = Identifier(TagClass::ContextSpecificClass, TagType::Primitive, 0x2);     // 0x82

    // PDU types id
    constexpr Identifier kPduGetId          = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x0);   // 0xa0
    constexpr Identifier kPduGetNextId      = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x1);   // 0xa1
    constexpr Identifier kPduResponseId     = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x2);   // 0xa2
    constexpr Identifier kPduSetId          = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x3);   // 0xa3

    constexpr Identifier kPduTrapId         = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x4);   // 0xa4
    constexpr Identifier kPduGetBulkId      = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x5);   // 0xa5
    constexpr Identifier kPduInformId       = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x6);   // 0xa6
    constexpr Identifier kPduTrap2Id        = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x7);   // 0xa7
    constexpr Identifier kPduReportId       = Identifier(TagClass::ContextSpecificClass, TagType::Constructor, 0x8);   // 0xa8


    enum class Type : Tag {     // В 5.5 он не мог автоматически преобразовать к Tag, поэтому явно преобразовываем..
        // Стандартные типы
        Integer             = static_cast<Tag>(Identifier(UniversalTagNumber::Integer)),
        BitString           = static_cast<Tag>(Identifier(UniversalTagNumber::BitString)),
        OctetString         = static_cast<Tag>(Identifier(UniversalTagNumber::OctetString)),
        ObjectId            = static_cast<Tag>(Identifier(UniversalTagNumber::ObjectId)),

        // SNMP типы
        UInteger            = static_cast<Tag>(kUnsignedId),
        Gauge               = static_cast<Tag>(kUnsignedId),
        Counter64           = static_cast<Tag>(kCounter64Id),
        IpAddress           = static_cast<Tag>(kIpAddressId),
        Counter             = static_cast<Tag>(kCounterId),
        TimeTicks           = static_cast<Tag>(kTimeTicksId),

        Null                = static_cast<Tag>(Identifier(UniversalTagNumber::Null)),
        Sequence            = static_cast<Tag>(Identifier(UniversalTagNumber::Sequence, TagType::Constructor))    //@N del ?
    };

    enum class ExceptionType : Tag {        // В 5.5 он не мог автоматически преобразовать к Tag, поэтому явно преобразовываем..
        NoSuchObject        = static_cast<Tag>(kNoSuchObjectId),      // 0x80
        NoSuchInstance      = static_cast<Tag>(kNoSuchInstanceId),    // 0x81
        EndOfMibView        = static_cast<Tag>(kEndOfMibViewId),      // 0x82
    };

    enum class PduType : Tag {      // В 5.5 он не мог автоматически преобразовать к Tag, поэтому явно преобразовываем..
        Get         = static_cast<Tag>(kPduGetId),         // 0xA0,
        GetNext     = static_cast<Tag>(kPduGetNextId),     // 0xA1,
        Response    = static_cast<Tag>(kPduResponseId),    // 0xA2,
        Set         = static_cast<Tag>(kPduSetId),         // 0xA3,
        Trap        = static_cast<Tag>(kPduTrapId),        // 0xA4,
        GetBulk     = static_cast<Tag>(kPduGetBulkId),     // 0xA5,
        Inform      = static_cast<Tag>(kPduInformId),      // 0xA6,
        Trap2       = static_cast<Tag>(kPduTrap2Id),       // 0xA7,
        Report      = static_cast<Tag>(kPduReportId),      // 0xA8
    };

    enum class TrapType {
        ColdStart = 0,              // Установка начального состояния объекта
        WarmStart = 1,              // Восстановление начального состояния объекта
        LinkDown = 2,               // Интерфейс выключился. Первая переменная в сообщении идентифицирует интерфейс
        LinkUp = 3,                 // Интерфейс включился. Первая переменная в сообщении идентифицирует интерфейс
        AuthenticationFailure = 4,  // От менеджера получено snmp-сообщение с неверным паролем (community)
        EGPneighborLoss = 5,        // EGP-партнер отключился. Первая переменная в сообщении определяет IP-адрес партнера.
        EntrpriseSpecific = 6       // Информация о TRAP содержится в поле специальный код
    };

}

#endif // OSNMP_H
