#ifndef TRAPV2_H
#define TRAPV2_H

#include "osnmp/message/snmpv2message.h"

namespace osnmp {
    class TrapV2 : public SnmpV2Message {
    public:
        TrapV2(const QString &community, TrapType type);
        TrapV2(const QString &community, const OID &snmpTrapOID);   // TrapType = EntrpriseSpecific

        TrapV2(const QString &community, TimeTicks timeTicks, TrapType type);
        TrapV2(const QString &community, TimeTicks timeTicks, const OID &snmpTrapOID);  // TrapType = EntrpriseSpecific

        void addVariables(const VarbindList &variables);

        void addVariable(const Varbind &variable);
        void addVariable(const OID &o, const Variant &var);
        void addVariable(const OID &o);

        void addBooleanVar(const OID &o, bool value);
        void addIntegerVar(const OID &o, int value);
        void addOctetStringVar(const OID &o, const QString &array);
        void addOctetStringVar(const OID &o, const QByteArray &array);
        void addNullVar(const OID &o);

    private:
        OID oidFromTrapType(TrapType type);
    };
}


#endif // TRAPV2_H

