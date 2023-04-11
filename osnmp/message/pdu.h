#ifndef PDU_H
#define PDU_H

#include "varbind.h"

namespace osnmp {
    class Pdu {
    public:
        Pdu();
        Pdu(const osnmp::PduType &type);
        Pdu(const Pdu &other);

        osnmp::PduType type() const;

        int requestId() const;
        void setRequestId(int value);

        // ErrorCodes
        void setError(int index, int status);
        void setError(int index, ErrorCodes status);

        int errorStatus() const;
        void setErrorStatus(int value);
        void setErrorStatus(ErrorCodes value);

        int errorIndex() const;
        void setErrorIndex(int value);

        // GetBulk PDU only
        void setNonRepeaters(int value);
        void setMaxRepetitions(int value);

        inline VarbindList &variables() { return variableBindings; }
        inline VarbindList variables() const { return variableBindings; }

        int nonRepeaters() const;
        int maxRepetitions() const;

        void addVariables(const VarbindList &variables);

        void addVariable(const Varbind &variable);
        void addVariable(const OID &o, const Variant &var);
        void addVariable(const OID &o);

        void addBooleanVar(const OID &o, bool value);
        void addIntegerVar(const OID &o, int value);
        void addOctetStringVar(const OID &o, const QString &array);
        void addOctetStringVar(const OID &o, const OctetString &array);
        void addNullVar(const OID &o);

        void clear();

        static PduType typeFromId(const Identifier &id);    //@N переделать бы логику преобразования..
        static Identifier idFromType(PduType type);    //@N переделать бы логику преобразования..

        Pdu &operator =(const Pdu &other);

        Pdu response() const;    // возвращает копию, только с типом - PduType::response

    protected:
        PduType t;
        int reqId;
        int errStatus;
        int errIndex;
        VarbindList variableBindings;
    };

    bool decode(const RawData &data, Pdu &value);
    bool encode(const Pdu &value, RawData &data);
}

#endif // PDU_H
