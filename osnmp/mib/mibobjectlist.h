#ifndef MIBOBJECTLIST_H
#define MIBOBJECTLIST_H

#include "mib.h"

namespace osnmp {
    class MibObjectList
    {
    public:
        using Data = QVector <osnmp::Mib*>;

        MibObjectList();
        MibObjectList(const Data &data);
        ~MibObjectList();

        void append(Mib *var);
        void append(const Data &data);
        void clear();

        MibSearchResult find(const OID &oid, bool exact, const OIDVector &includes) const;

        MibSearchResult find(const OID &oid, bool exact = true) const;
        MibSearchResult find(const OID &oid, const OIDVector &includes) const;

        MibSearchResult firstObject(const OIDVector &includes = {}) const;
        MibSearchResult lastObject(const OIDVector &includes = {}) const;
        // Найти следующий элемент в дереве
        MibSearchResult nextObject(Mib *object, const OIDVector &includes = {}) const;

        int size() const { return data.size(); }
        const Mib* at(int i) const { return data.at(i); }
        Mib *operator[](int i) { return data[i]; }

    protected:
        Data data;

        // бинарный поиск

        MibSearchResult find(const OID &oid, bool exact, int left, int right, const OIDVector &includes) const;
    };
}

#endif // MIBOBJECTLIST_H
