#include "trapv2.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#else
int bounded(int lowest, int highest) {
    return (qrand() / static_cast<double>(RAND_MAX)) * (highest - lowest) + lowest;
}
#endif




osnmp::TrapV2::TrapV2(const QString &community, osnmp::TimeTicks timeTicks, const OID &snmpTrapOID) : SnmpV2Message(SNMP2c)
{
    community_ = community;
    pdu_ = Pdu(PduType::Trap2);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    pdu_.setRequestId(QRandomGenerator::global()->bounded(10, 65535));
#else
    pdu_.setRequestId(bounded(10, 65535));
#endif

    pdu_.addVariable(OID::SysUpTime, timeTicks);
    pdu_.addVariable(OID(OID::Trap), snmpTrapOID);
}

osnmp::TrapV2::TrapV2(const QString &community, TrapType type) : SnmpV2Message(SNMP2c)
{
    community_ = community;
    pdu_ = Pdu(PduType::Trap2);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    pdu_.setRequestId(QRandomGenerator::global()->bounded(10, 65535));
#else
    pdu_.setRequestId(bounded(10, 65535));
#endif

    pdu_.addVariable(OID::SysUpTime, TimeTicks::upTime());
    pdu_.addVariable(OID(OID::Trap), oidFromTrapType(type));
}

osnmp::TrapV2::TrapV2(const QString &community, const OID &snmpTrapOID) : SnmpV2Message(SNMP2c)
{
    community_ = community;
    pdu_ = Pdu(PduType::Trap2);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    pdu_.setRequestId(QRandomGenerator::global()->bounded(10, 65535));
#else
    pdu_.setRequestId(bounded(10, 65535));
#endif

    pdu_.addVariable(OID::SysUpTime, TimeTicks::upTime());
    pdu_.addVariable(OID(OID::Trap), snmpTrapOID);
}

osnmp::TrapV2::TrapV2(const QString &community, osnmp::TimeTicks timeTicks, osnmp::TrapType type) : SnmpV2Message(SNMP2c)
{
    community_ = community;
    pdu_ = Pdu(PduType::Trap2);

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    pdu_.setRequestId(QRandomGenerator::global()->bounded(10, 65535));
#else
    pdu_.setRequestId(bounded(10, 65535));
#endif

    pdu_.addVariable(OID::SysUpTime, timeTicks);
    pdu_.addVariable(OID(OID::Trap), oidFromTrapType(type));
}

OID osnmp::TrapV2::oidFromTrapType(osnmp::TrapType type)
{
    switch (type) {
        case TrapType::ColdStart:
            return OID::ColdStart;
        case TrapType::WarmStart:
            return OID::WarmStart;
        case TrapType::LinkDown:
            return OID::LinkDown;
        case TrapType::LinkUp:
            return OID::LinkUp;
        case TrapType::AuthenticationFailure:
            return OID::AuthenticationFailure;
        case TrapType::EGPneighborLoss:
            return OID::EGPneighborLoss;
        case TrapType::EntrpriseSpecific:
        default:
            break;
    }
    return OID::Null;
}

void osnmp::TrapV2::addVariables(const osnmp::VarbindList &variables)
{
    pdu_.addVariables(variables);
}

void osnmp::TrapV2::addVariable(const osnmp::Varbind &variable)
{
    pdu_.addVariable(variable);
}

void osnmp::TrapV2::addVariable(const OID &o, const osnmp::Variant &var)
{
    pdu_.addVariable(o, var);
}

void osnmp::TrapV2::addVariable(const OID &o)
{
    pdu_.addVariable(o);
}

void osnmp::TrapV2::addBooleanVar(const OID &o, bool value)
{
    pdu_.addBooleanVar(o, value);
}

void osnmp::TrapV2::addIntegerVar(const OID &o, int value)
{
    pdu_.addIntegerVar(o, value);
}

void osnmp::TrapV2::addOctetStringVar(const OID &o, const QString &array)
{
    pdu_.addOctetStringVar(o, array);
}

void osnmp::TrapV2::addOctetStringVar(const OID &o, const QByteArray &array)
{
    pdu_.addOctetStringVar(o, array);
}

void osnmp::TrapV2::addNullVar(const OID &o)
{
    pdu_.addNullVar(o);
}


