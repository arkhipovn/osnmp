#include "pdu.h"

//#include "osnmp/snmputilities.h"

osnmp::Pdu::Pdu() : t(PduType::Response),
    reqId(0), errStatus(0), errIndex(0)
{

}

osnmp::Pdu::Pdu(const osnmp::PduType &type) : t(type),
    reqId(0), errStatus(0), errIndex(0)
{

}

osnmp::Pdu::Pdu(const Pdu &other) : t(other.t),
    reqId(other.reqId), errStatus(other.errStatus), errIndex(other.errIndex), variableBindings(other.variableBindings)
{

}

osnmp::PduType osnmp::Pdu::type() const
{
    return t;
}

int osnmp::Pdu::requestId() const
{
    return reqId;
}

void osnmp::Pdu::setRequestId(int value)
{
    reqId = value;
}

void osnmp::Pdu::setError(int index, int status)
{
    errIndex = index;
    errStatus = status;
}

void osnmp::Pdu::setError(int index, ErrorCodes status)
{
    errIndex = index;
    errStatus = static_cast<int>(status);
}

int osnmp::Pdu::errorStatus() const
{
    return errStatus;
}

void osnmp::Pdu::setErrorStatus(int value)
{
    errStatus = value;
}

void osnmp::Pdu::setErrorStatus(ErrorCodes value)
{
    errStatus = static_cast<int>(value);
}

int osnmp::Pdu::errorIndex() const
{
    return errIndex;
}

void osnmp::Pdu::setErrorIndex(int value)
{
    errIndex = value;
}

void osnmp::Pdu::setNonRepeaters(int value)
{
    errStatus = value;
}

void osnmp::Pdu::setMaxRepetitions(int value)
{
    errIndex = value;
}

int osnmp::Pdu::nonRepeaters() const
{
    return errStatus;
}

int osnmp::Pdu::maxRepetitions() const
{
    return errIndex;
}

void osnmp::Pdu::addVariable(const OID &o, const Variant &var)
{
    addVariable(Varbind(o, var));
}

void osnmp::Pdu::addVariables(const VarbindList &variables)
{
    variableBindings.append(variables);
}

void osnmp::Pdu::addVariable(const Varbind &variable)
{
    variableBindings.append(variable);
}

void osnmp::Pdu::addVariable(const OID &o)
{
    addVariable(o, Null());
}

void osnmp::Pdu::addBooleanVar(const OID &o, bool value)
{
    addVariable(o, value ? 1 : 0);
}

void osnmp::Pdu::addIntegerVar(const OID &o, int value)
{
    addVariable(o, value);
}

void osnmp::Pdu::addOctetStringVar(const OID &o, const QString &array)
{
    addVariable(o, array);
}

void osnmp::Pdu::addOctetStringVar(const OID &o, const OctetString &array)
{
    addVariable(o, array);
}

void osnmp::Pdu::addNullVar(const OID &o)
{
    addVariable(o, Null());
}

void osnmp::Pdu::clear()
{
    variableBindings.clear();
}

osnmp::PduType osnmp::Pdu::typeFromId(const osnmp::Identifier &id)
{
    return static_cast <PduType>(id.tag());
}

osnmp::Identifier osnmp::Pdu::idFromType(PduType type)
{
    switch (type) {
        case PduType::Get:      return kPduGetId;
        case PduType::GetNext:  return kPduGetNextId;
        case PduType::Response: return kPduResponseId;
        case PduType::Set:      return kPduSetId;
        case PduType::Trap:     return kPduTrapId;
        case PduType::GetBulk:  return kPduGetBulkId;
        case PduType::Inform:   return kPduInformId;
        case PduType::Trap2:    return kPduTrap2Id;
        case PduType::Report:   return kPduReportId;
        default:
            break;
    }

    return Identifier();
}

osnmp::Pdu &osnmp::Pdu::operator =(const osnmp::Pdu &other)
{
    if(this == &other) return *this;
    t = other.t;
    reqId = other.reqId;
    errStatus = other.errStatus;
    errIndex = other.errIndex;
    variableBindings = other.variableBindings;
    return *this;
}

osnmp::Pdu osnmp::Pdu::response() const
{
    Pdu pdu(PduType::Response);
    pdu.reqId = this->reqId;
    //    Pdu pdu(*this);
    //    pdu.t = PduType::Response;
    return pdu;
}

bool osnmp::decode(const RawData &data, Pdu &value)
{
    value.clear();
    if(data.isEmpty() || data.isPrimitive()) return false;

    auto content = static_cast<ConstructedContent*>(data.content());
    if(content->size() != 4) return false;

    value = Pdu(Pdu::typeFromId(data.identifier()));

    int id;
    if(!decode(content->at(0), id)) return false;
    value.setRequestId(id);

    int errStatus;
    if(!decode(content->at(1), errStatus)) return false;
    value.setErrorStatus(errStatus);

    int errInd;
    if(!decode(content->at(2), errInd)) return false;
    value.setErrorIndex(errInd);

    SequenceOf <Varbind> seq;
    if(!decode(content->at(3), seq)) return false;
    value.addVariables(seq.toVector()); //@N вот тут много времени тратится на преобразование всех элементов в Varbing
    return true;
}

bool osnmp::encode(const Pdu &value, RawData &data)
{
    auto content = new ConstructedContent({
                                              value.requestId(),
                                              value.errorStatus(),
                                              value.errorIndex(),
                                              SequenceOf <Varbind>(value.variables())   //@N вот тут много времени тратится не преобразование всех элементов из Varbing
                                          });
    data = std::move(RawData(Pdu::idFromType(value.type()), content));
    return true;
}
