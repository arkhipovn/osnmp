#ifndef MIBVARIABLE_H
#define MIBVARIABLE_H

#include "osnmp.h"
#include "osnmp/oid.h"

namespace osnmp {
    class MibVariableList;
    class MibVariable {
    public:
        MibVariable(quint16 valueId, Type type, ObjectAccess access, const OID &oid);
        MibVariable(ObjectAccess access, const OID &base, int rows = -1);

        ~MibVariable();

        quint16 valueId() const { return valueId_; }
        Type type() const;
        ObjectAccess access() const;
        OID oid() const { return oid_; }
        OID &oid() { return oid_; }
        MibVariable *parent() const { return parent_; }
        MibVariableList *columns() const { return columns_; }

        OID absoluteOid() const;    // с учетом родителей..

        inline bool isNull() const { return oid_.isNull(); }

        bool operator <(const OID &other) const { return oid_ < other; }
        bool operator >(const OID &other) const { return oid_ > other; }
        bool operator <(const MibVariable &other) const { return oid_ < other.oid_; }

        bool operator <=(const OID &other) const { return !(*this > other); }

        // Работает только если тип Sequence
        void append(MibVariable *variable);
        void append(QVector <MibVariable *> list);
        void clearColumns();

        bool isSequence() const;

        int rows() const;
        void setRows(int rows);

    protected:
        quint16 valueId_;           // Для того, чтобы было проще найти параметр. Вместо того чтобы искать его по OID можно по valueId

        // Описание. Как в mib файле
        Type type_;
        ObjectAccess access_;
        // Добавить граничные значения и тд..

        OID oid_;                   // Полный путь

        MibVariable *parent_;           // Если MibVariable - является колонкой в таблице
        MibVariableList *columns_;      // Колонки, в случае если это таблица..

        // Нужно для таблицы. Допустим если мы изначально знаем, что будет 20 строк с одинарным индексом (int), то можно это указать
        int rows_;
    };
}

#endif // MIBVARIABLE_H
