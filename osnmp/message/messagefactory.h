#ifndef MESSAGEFACTORY_H
#define MESSAGEFACTORY_H

#include "snmpmessage.h"

namespace osnmp {
    class MessageFactory {
    public:
        MessageFactory() = delete;
        ~MessageFactory() = delete;

        // Считывает версию сообщения, выделяет память и декодирует данные
        // Если что-то не так возвращает nullptr
        static SnmpMessage *create(const RawData &data);

        // Выделяет память для пустого сообщения нужной версии
        // Если что-то не так возвращает nullptr
        static SnmpMessage *create(int version);

        // Определяет версию SNMP в сообщении
        // Если что-то не так возвращает -1
        static int getMsgVersion(const RawData &data);
    };
}

#endif // MESSAGEFACTORY_H
