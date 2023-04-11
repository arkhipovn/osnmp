#ifndef MIB_H
#define MIB_H

#include <QSet>
#include "osnmp/agent/strategy/abstractagentstrategy.h"
#include "mibsearchresult.h"

namespace osnmp {
    class Mib {
    public:
        Mib(const OID &oid, AbstractAgentStrategy *strategy = nullptr);
        virtual ~Mib() = default;

        void setOid(const OID &oid);
        OID oid() const;
        OID &oid();
        void setParent(Mib *parent);
        Mib *parent() const;

        OID absoluteOid() const;    // с учетом родителей..
        bool isNull() const;

        bool operator <(const OID &other) const { return oid_ < other; }
        bool operator >(const OID &other) const { return oid_ > other; }
        bool operator <(const Mib &other) const { return oid_ < other.oid_; }
        bool operator >(const Mib &other) const { return oid_ > other.oid_; }
        bool operator <=(const OID &other) const { return !(*this > other); }

        MibSearchResult find(const OID &oid, bool exact = true) const;
        // Поиск для неточного совпадения (exact = false). При этом еще проверяется есть доступ includes
        MibSearchResult find(const OID &oid, const OIDVector &includes) const;

        virtual MibSearchResult findObject(const OID &oid, bool exact, const OIDVector &includes) const = 0;

        // Найти следующий элемент в дереве. Проверяется есть доступ includes. Если нет, то вернется следующий для которого есть доступ
        // Если object == null, то просто первый ищим
        // Если object != null, то посмотрим у потомков есть ли тайкой и вернем следующий за ним
        virtual MibSearchResult nextObject(Mib *object = nullptr, const OIDVector &includes = {}) const = 0;

        // Вернуть первый доступный элемент в дереве. Проверяется есть доступ includes. Если нет, то вернется следующий для которого есть доступ
        virtual MibSearchResult firstObject(const OIDVector &includes = {}) const = 0;
        virtual MibSearchResult lastObject(const OIDVector &includes = {}) const = 0;


        virtual bool isVariable() const = 0;    // является ли переменной
        virtual bool isTable() const = 0;

        AbstractAgentStrategy *strategy() const;
        void setStrategy(AbstractAgentStrategy *strategy);  //@N скрыть бы от пользователя..

        // список стратегий
        virtual QSet <AbstractAgentStrategy*> getStrategySet() const;

    protected:
        OID oid_;
        Mib *parent_;
        AbstractAgentStrategy *strategy_;
    };
}

#endif // MIB_H
