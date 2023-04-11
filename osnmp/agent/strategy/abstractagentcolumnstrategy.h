#ifndef AbstractAgentColumnStrategy_H
#define AbstractAgentColumnStrategy_H

#include "abstractagenttablestrategy.h"

namespace osnmp {
    // Если индекс таблицы = int и точно известно кол-во строк, то можно при добавлении в mib
    // использовать данную стратегию и сразу указать кол-во строк

    class AbstractAgentColumnStrategy : public AbstractAgentTableStrategy
    {
#ifdef QOBJECT_AGENT_STRATEGY
        Q_OBJECT
    public:
        AbstractAgentColumnStrategy(int rows, QObject *parent = nullptr);
#else
    public:
        AbstractAgentColumnStrategy(int rows);
#endif

        virtual ~AbstractAgentColumnStrategy() = default;

        /*
            Функция для поиска индекса в таблице. Если индекс таблицы = int и точно известно кол-во строк, то можно при добавлении в mib
            сразу указать это кол-во и тогда в данной функции не нужно описывать таблицу

            @exact - точное совпадение. Если передать false - то должен найти следующий элемент. Если он есть
            @ret - вернуть текущий или следующий индекс. Если ничего не найдено, то Varbind() или OID::Null;
        */
        OID findTableRow(int valueId, const OID &index, bool exact) const override;

        /*
            Вернуть последний индекс в таблице
        */
        OID lastTableRow(int valueId) const override;

    protected:
        int rows;
    };
}

#endif // AbstractAgentColumnStrategy_H
