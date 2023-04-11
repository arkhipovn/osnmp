#ifndef VARBIND_H
#define VARBIND_H

#include "osnmp/variant.h"

namespace osnmp {
    // Sequence из двух параметров
    class Varbind
    {
    public:
        Varbind();
        Varbind(const OID &oid);
        Varbind(const OID &oid, const Variant &data);
        Varbind(const Varbind &other);
        Varbind(Varbind &&other);

        inline OID oid() const { return oid_; }
        inline Variant data() const { return var_; }

        void setOid(const OID &oid) { oid_ = oid; }
        void setData(const Variant &data) { var_ = data; }

        inline bool isEmpty() const { return oid_.isEmpty(); }

        Varbind &operator =(const Varbind &other);
        Varbind &operator =(const Variant &data);

        bool isNull() const;
        bool isExceptionData() const;

        void clear();

    private:
        OID oid_;
        Variant var_;
    };

    using VarbindList = QVector <Varbind>;
    //    using VarbindList = SequenceOf <Varbind>;

    bool decode(const RawData &data, Varbind &value);
    bool encode(const Varbind &value, RawData &data);
}

//    QDebug operator <<(QDebug stream, const Varbind &var);

#endif // VARBIND_H


