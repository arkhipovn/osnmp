#ifndef MIBOBJECT_H
#define MIBOBJECT_H

#include "mib.h"
#include "osnmp.h"

namespace osnmp {
    class MibObject : public Mib {
    public:
        MibObject(quint16 valueId, Type type, ObjectAccess access, const OID &oid, AbstractAgentStrategy *strategy = nullptr);
        virtual ~MibObject();

        quint16 valueId() const;
        Type type() const;
        ObjectAccess access() const;

        MibSearchResult findObject(const OID &oid, bool exact, const OIDVector &includes) const override;

        MibSearchResult firstObject(const OIDVector &includes) const override;
        MibSearchResult lastObject(const OIDVector &includes) const override;
        MibSearchResult nextObject(Mib *object = nullptr, const OIDVector &includes = {}) const override;

        bool isVariable() const override { return true; }
        bool isTable() const override { return false; }

    protected:
        quint16 valueId_;       // Для того, чтобы было проще найти параметр. Вместо того чтобы искать его по OID можно по valueId

        // Описание. Как в mib файле
        Type type_;
        ObjectAccess access_;

        // Добавить граничные значения и тд..
    };
}

#endif // MIBOBJECT_H

