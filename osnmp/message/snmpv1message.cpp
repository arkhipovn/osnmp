#include "snmpv1message.h"
#include "osnmp/agent/strategy/abstractagentstrategy.h"
#include "osnmp/mib/mibobject.h"

osnmp::SnmpV1Message::SnmpV1Message() : SnmpMessage(SNMP1)
{

}

QString osnmp::SnmpV1Message::community() const
{
    return community_;
}

void osnmp::SnmpV1Message::setCommunity(const QString &community)
{
    community_ = community;
}

osnmp::Pdu &osnmp::SnmpV1Message::pdu()
{
    return pdu_;
}

osnmp::Pdu osnmp::SnmpV1Message::pdu() const
{
    return pdu_;
}

void osnmp::SnmpV1Message::setPdu(const Pdu &pdu)
{
    pdu_ = pdu;
}

// ______________________

void osnmp::SnmpV1Message::decode(const osnmp::RawData &data, bool *isok)
{
    bool temp = osnmp::decode(data, *this);
    if(isok != nullptr) *isok = temp;
}

osnmp::RawData osnmp::SnmpV1Message::encode(bool *isok) const
{
    RawData data;
    bool temp = osnmp::encode(*this, data);
    if(isok != nullptr) *isok = temp;
    return data;
}

bool osnmp::SnmpV1Message::makeResponse(osnmp::SnmpConf *conf, osnmp::MibModule *mib)
{
    if(community_.isEmpty()) return false;
    auto communitySettings = conf->communities().constFind(community_);
    if(communitySettings == conf->communities().constEnd()) return false;

    // Формируем ответный PDU
    auto responsePdu = pdu_.response();

    switch (pdu_.type()) {
        case PduType::Get:
            if(communitySettings.value().access() == ObjectAccess::NoAccess) {  // Нет прав
                responsePdu.setError(1, ErrorCodes::NoAccess);
            }
            else {
                responsePdu.addVariables(pdu_.variables());
                int i = 1;
                for(auto it = responsePdu.variables().begin(); it != responsePdu.variables().end(); ++it, ++i) {
                    if(!communitySettings.value().checkAccess(it->oid())) { // Нет доступа
                        responsePdu.setError(i, ErrorCodes::NoSuchName);
                    }
                    else { // Доступ есть
                        // Посмотрим есть ли нужный OID
                        auto searchResponse = mib->find(it->oid(), true);
                        if(searchResponse.isEmpty()) {   // Нет такого OID
                            responsePdu.setError(i, ErrorCodes::NoSuchName);
                            break;
                        }
                        else if(!searchResponse.mib->isVariable()) {   // Не нашли похоже.. Чисто перестраховка. Такого не должно быть!!
                            responsePdu.setError(i, ErrorCodes::NoSuchName);
                            break;
                        }
                        else {
                            auto obj = static_cast<MibObject*>(searchResponse.mib);

                            auto strategy = obj->strategy();
                            if(strategy == nullptr) {
                                responsePdu.setError(i, ErrorCodes::NoSuchName);
                                break;
                            }

                            ErrorCodes errorCode = ErrorCodes::NoError;
                            auto var = strategy->getValue(obj->valueId(), searchResponse.tableIndex, errorCode);
                            if(errorCode != ErrorCodes::NoError) {
                                responsePdu.setError(i, errorCode);
                                break;
                            }
                            it->setData(var);
                        }
                    }
                }
            }
            break;
        case PduType::GetNext:
            if(communitySettings.value().access() == ObjectAccess::NoAccess) {   // Нет прав
                responsePdu.setError(1, ErrorCodes::NoAccess);
            }
            else {
                responsePdu.addVariables(pdu_.variables());
                int i = 1;
                for(auto it = responsePdu.variables().begin(); it != responsePdu.variables().end(); ++it, ++i) {
                    // Посмотрим есть ли нужный OID
                    auto searchResponse = mib->find(it->oid(), communitySettings.value().includes());
                    if(searchResponse.isEmpty()) {    // Нет такого OID и тк exact = false, значит это конец списка
                        responsePdu.setError(i, ErrorCodes::NoSuchName);
                        break;
                    }
                    else {
                        it->setOid(searchResponse.mib->absoluteOid() + searchResponse.tableIndex);

                        if(!searchResponse.mib->isVariable()) { // Не нашли похоже.. Чисто перестраховка. Такого не должно быть!!
                            responsePdu.setError(i, ErrorCodes::NoSuchName);
                            break;
                        }
                        else {
                            auto obj = static_cast<MibObject*>(searchResponse.mib);

                            auto strategy = obj->strategy();
                            if(strategy == nullptr) {
                                responsePdu.setError(i, ErrorCodes::NoSuchName);
                                break;
                            }

                            ErrorCodes errorCode = ErrorCodes::NoError;
                            auto var = strategy->getValue(obj->valueId(), searchResponse.tableIndex, errorCode);
                            if(errorCode != ErrorCodes::NoError) {
                                responsePdu.setError(i, errorCode);
                                break;
                            }

                            if(var.identifier() == static_cast<Tag>(ExceptionType::NoSuchInstance)) {
                                responsePdu.setError(i, ErrorCodes::NoSuchName);    //@N точно не знаю..
                                break;
                            }

                            it->setData(var);
                        }
                    }

                }
            }
            break;
        case PduType::Set:
            if(communitySettings.value().access() != ObjectAccess::ReadWrite) {  // Нет прав
                responsePdu.setError(1, ErrorCodes::NoAccess);
            }
            else {
                responsePdu.addVariables(pdu_.variables());
                int i = 1;
                for(auto it = responsePdu.variables().begin(); it != responsePdu.variables().end(); ++it, ++i) {
                    if(!communitySettings.value().checkAccess(it->oid())) { // Нет доступа
                        // говорим, что нет доступа и выходим
                        responsePdu.setError(i, ErrorCodes::NoAccess); // +1 тк индексация начинается с 1
                        break;                              // выходим, дальше искать не нужно..
                    }
                    else {
                        // Посмотрим есть ли нужный OID
                        auto searchResponse = mib->find(it->oid(), true);
                        if(searchResponse.isEmpty()) {   /// Нет такого OID
                            responsePdu.setError(i, ErrorCodes::NoSuchName);   // Вроде бы эту ошибку отправляют..
                            break;
                        }
                        else if(!searchResponse.mib->isVariable()) {   // Не нашли похоже.. Чисто перестраховка. Такого не должно быть!!
                            responsePdu.setError(i, ErrorCodes::NoSuchName);   // Вроде бы эту ошибку отправляют..
                            break;
                        }

                        auto obj = static_cast<MibObject*>(searchResponse.mib);

                        auto strategy = obj->strategy();
                        if(strategy == nullptr) {
                            responsePdu.setError(i, ErrorCodes::NoSuchName);   // Вроде бы эту ошибку отправляют..
                            break;
                        }

                        // Посмотрим что там с переменной. Переменная предназначена для записи?
                        if(obj->access() != ObjectAccess::ReadWrite) {
                            // Только для чтения
                            responsePdu.setError(i, ErrorCodes::NoSuchName);
                            break;                              // выходим
                        }

                        // Проверим совпадает ли тип?
                        if(static_cast<Tag>(obj->type()) != it->data().identifier()) {
                            responsePdu.setError(i, ErrorCodes::WrongType);
                            break;                              // выходим
                        }

                        //@N Граничные значения если есть

                        ErrorCodes errorCode = ErrorCodes::NoError;
                        auto var = strategy->setValue(obj->valueId(), searchResponse.tableIndex, it->data(), errorCode);
                        if(errorCode != ErrorCodes::NoError) {
                            if(errorCode == ErrorCodes::NotWritale)
                                errorCode = ErrorCodes::NoSuchName;
                            responsePdu.setError(i, errorCode);
                            break;
                        }
                        it->setData(var);
                    }
                }
            }

            //@N Так-то нужно бы сначала проверить можно ли записать.. А потом если что откатить назад..

            break;
        default:
            return false;
    }

    setPdu(responsePdu);

    return true;
}

bool osnmp::encode(const SnmpV1Message &value, RawData &data)
{
    auto content = new ConstructedContent({
                                              value.version(),
                                              value.community(),
                                              value.pdu()
                                          });
    data = std::move(RawData(Identifier(UniversalTagNumber::Sequence, TagType::Constructor), content));
    return true;
}

bool osnmp::decode(const RawData &data, SnmpV1Message &value)
{
    if(data.isEmpty() || data.isPrimitive()) return false;
    auto content = static_cast<ConstructedContent*>(data.content());

    if(content->size() != 3) return false;

    int ver;
    if(!decode(content->at(0), ver)) return false;
    value.setVersion(ver);

    QString community;
    if(!decode(content->at(1), community)) return false;
    value.setCommunity(community);

    Pdu pdu;
    if(!decode(content->at(2), pdu)) return false;
    value.setPdu(pdu);

    return true;
}

