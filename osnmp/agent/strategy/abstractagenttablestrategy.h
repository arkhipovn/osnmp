#ifndef ABSTRACTAGENTTABLESTRATEGY_H
#define ABSTRACTAGENTTABLESTRATEGY_H

#include "abstractagentstrategy.h"

namespace osnmp {
    // Если стратегия добавляется в том числе и для таблицы..
    class AbstractAgentTableStrategy : public AbstractAgentStrategy {
#ifdef QOBJECT_AGENT_STRATEGY
        Q_OBJECT
    public:
        explicit AbstractAgentTableStrategy(QObject *parent = nullptr);
#else
    public:
        AbstractAgentTableStrategy();
#endif
        virtual ~AbstractAgentTableStrategy() = default;

        /*
            Функция для поиска индекса в таблице. Если индекс таблицы = int и точно известно кол-во строк, то можно при добавлении в mib
            сразу указать это кол-во и тогда в данной функции не нужно описывать таблицу

            @exact - точное совпадение. Если передать false - то должен найти следующий элемент. Если он есть
            @ret - вернуть текущий или следующий индекс. Если ничего не найдено, то Varbind() или OID::Null;
        */
        virtual OID findTableRow(int valueId, const OID &index, bool exact) const = 0;

        /*
            Вернуть последний индекс в таблице
        */
        virtual OID lastTableRow(int valueId) const = 0;

        OID firstTableRow(int valueId) const;
    };
}

#endif // ABSTRACTAGENTTABLESTRATEGY_H
