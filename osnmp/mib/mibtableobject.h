#ifndef MIBTABLEOBJECT_H
#define MIBTABLEOBJECT_H

#include "mibobject.h"
#include "osnmp/agent/strategy/abstractagenttablestrategy.h"

namespace osnmp {
    class MibObjectList;

    class MibTableObject : public MibObject {
    public:
        MibTableObject(ObjectAccess access, const OID &base, AbstractAgentTableStrategy *strategy = nullptr);
        ~MibTableObject();

        void append(MibObject *object);
        void append(QVector <MibObject *> list);
        void clear();

        MibSearchResult findObject(const OID &oid, bool exact, const OIDVector &includes) const override;

        MibSearchResult firstObject(const OIDVector &includes = {}) const override;
        MibSearchResult lastObject(const OIDVector &includes = {}) const override;
        MibSearchResult nextObject(Mib *object = nullptr, const OIDVector &includes = {}) const override;

        bool isVariable() const override { return false; }
        bool isTable() const override { return true; }

        QSet <AbstractAgentStrategy*> getStrategySet() const override;

    protected:
        MibObjectList *columns_;
    };
}

#endif // MIBTABLEOBJECT_H
