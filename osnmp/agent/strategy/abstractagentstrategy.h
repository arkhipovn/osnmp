#ifndef ABSTRACTAGENTSTRATEGY_H
#define ABSTRACTAGENTSTRATEGY_H

#include "osnmp/message/varbind.h"
#include <QDebug>
#include <QObject>

namespace osnmp {
#ifdef QOBJECT_AGENT_STRATEGY
    class AbstractAgentStrategy : public QObject {
        Q_OBJECT
    public:
        explicit AbstractAgentStrategy(QObject *parent = nullptr);
#else
    class AbstractAgentStrategy {
    public:
        AbstractAgentStrategy() = default;
#endif
        virtual ~AbstractAgentStrategy() = default;

        /*
             @errorCode - вернуть ошибку, если что-то не так
             @index - индекс в таблице. То если если OID таблицы 1.2.3,а идет запрос 1.2.3.4.5.6, то @index будет равен 4.5.6
        */
        virtual Variant getValue(int valueId, const OID &index, ErrorCodes &errorCode) = 0;

        /*
             @var - значение переменной для valueId
             @errorCode - вернуть ошибку, если что-то не так
             @index - индекс в таблице. То если если OID таблицы 1.2.3,а идет запрос 1.2.3.4.5.6, то @index будет равен 4.5.6
         */
        virtual Variant setValue(int valueId, const OID &index, const Variant &var, ErrorCodes &errorCode) = 0;
    };
}

#endif // ABSTRACTAGENTSTRATEGY_H
