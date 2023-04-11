#ifndef MIBMODULE_H
#define MIBMODULE_H

#include "mib.h"

namespace osnmp {
    class MibObjectList;

    class MibModule : public Mib {
    public:
        MibModule(const OID &oid, AbstractAgentStrategy *strategy = nullptr);
        virtual ~MibModule();

        void append(Mib *object);
        void append(QVector <Mib *> list);
        void clear();

        MibSearchResult findObject(const OID &oid, bool exact, const OIDVector &includes) const override;

        MibSearchResult firstObject(const OIDVector &includes = {}) const override;
        MibSearchResult lastObject(const OIDVector &includes = {}) const override;
        MibSearchResult nextObject(Mib *object = nullptr, const OIDVector &includes = {}) const override;

        bool isVariable() const override { return false; }
        bool isTable() const override { return false; }

        QSet <AbstractAgentStrategy*> getStrategySet() const override;

    protected:
        MibObjectList *tree_;
    };

    using MibObjectIdentifier = MibModule;
}

#endif // MIBMODULE_H
